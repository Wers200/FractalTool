#include "framework.h"
#include "compute.h"
#include "vertex.h"
#include "pixel.h"
#include "renderer.h"

#pragma region Shader helper functions
void Renderer::UpdateConstantBuffer() {
    // Write Info variable to the constant buffer
    D3D11_MAPPED_SUBRESOURCE cBufferWrite;
    pDeviceContext->Map(cBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &cBufferWrite);
    memcpy(cBufferWrite.pData, &Info, sizeof(CBUFFER));
    pDeviceContext->Unmap(cBuffer.Get(), 0);

    // Update shader constant buffers
    pDeviceContext->CSSetConstantBuffers(0, 1, cBuffer.GetAddressOf());
    pDeviceContext->PSSetConstantBuffers(0, 1, cBuffer.GetAddressOf());
}

UINT* Renderer::RunComputeShader(bool updateTexture) {
    // Set the unordered access view
    pDeviceContext->CSSetUnorderedAccessViews(0, 1, csUAV.GetAddressOf(), nullptr);

    // Run the compute shader
    pDeviceContext->Dispatch(1, 1, 1);

    // Read the output
    pDeviceContext->CopyResource(csOutputResultBuffer.Get(), csOutputBuffer.Get());

    D3D11_MAPPED_SUBRESOURCE outputRead;
    pDeviceContext->Map(csOutputResultBuffer.Get(), 0, D3D11_MAP_READ, 0, &outputRead);

    UINT* data = (UINT*)outputRead.pData;

    pDeviceContext->Unmap(csOutputResultBuffer.Get(), 0);

    if (updateTexture) {
        // Re-create the texture (I am not sure why, but using the Map-Unmap method didn't work well and messed everything up)
        D3D11_TEXTURE2D_DESC outputTextureDesc;
        csOutputTexture->GetDesc(&outputTextureDesc);

        D3D11_SUBRESOURCE_DATA outputTextureData;
        outputTextureData.pSysMem = data;
        outputTextureData.SysMemPitch = sizeof(UINT) * Info.Size.x;
        outputTextureData.SysMemSlicePitch = 0;

        pDevice->CreateTexture2D(&outputTextureDesc, &outputTextureData, csOutputTexture.GetAddressOf());

        // Re-create and set the shader resource view
        D3D11_SHADER_RESOURCE_VIEW_DESC psSRVdesc;
        psInputSRV->GetDesc(&psSRVdesc);

        pDevice->CreateShaderResourceView(csOutputTexture.Get(), &psSRVdesc, psInputSRV.GetAddressOf());

        pDeviceContext->PSSetShaderResources(0, 1, psInputSRV.GetAddressOf());
    }

    return data;
}
#pragma endregion

void Renderer::CreateDeviceResources() {
    if (!pRTV) {
        RECT rc;
        GetClientRect(hWnd, &rc);

        XMUINT2 size = XMUINT2(
            rc.right - rc.left,
            rc.bottom - rc.top
        );
        
        #pragma region Constant buffer
        Info.C = XMFLOAT2(0, 1);
        Info.Zoom = XMFLOAT4(-2, 2, 2, -2);
        Info.PreviewZoom = XMFLOAT4(-1, -1, -1, -1);
        Info.Size = size;
        Info.MaxIter = 50;

        D3D11_BUFFER_DESC cBufferDesc;
        cBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        cBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cBufferDesc.MiscFlags = 0;
        cBufferDesc.StructureByteStride = 0;
        cBufferDesc.ByteWidth = sizeof(CBUFFER);

        D3D11_SUBRESOURCE_DATA cBufferInitData;
        cBufferInitData.pSysMem = &Info;
        cBufferInitData.SysMemPitch = 0;
        cBufferInitData.SysMemSlicePitch = 0;

        pDevice->CreateBuffer(&cBufferDesc, &cBufferInitData, cBuffer.GetAddressOf());
        #pragma endregion

        #pragma region Compute shader
        // Compile and create the shader
        ComPtr<ID3DBlob> cs_compiled;
        std::vector<char> cs_bytes = readBytes("compute.hlsl");
        D3DCompile(cs_bytes.data(), cs_bytes.size(), NULL, NULL, NULL, "main", "cs_5_0", NULL, NULL, cs_compiled.GetAddressOf(), NULL);

        pDevice->CreateComputeShader(cs_compiled->GetBufferPointer(), cs_compiled->GetBufferSize(), nullptr, cShader.GetAddressOf());
        pDeviceContext->CSSetShader(cShader.Get(), nullptr, 0);

        // Create output buffer (bound to unordered access view)
        D3D11_BUFFER_DESC outputDesc;
        outputDesc.Usage = D3D11_USAGE_DEFAULT;
        outputDesc.CPUAccessFlags = 0;
        outputDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
        outputDesc.ByteWidth = sizeof(UINT) * size.x * size.y;
        outputDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        outputDesc.StructureByteStride = sizeof(UINT);

        pDevice->CreateBuffer(&outputDesc, nullptr, csOutputBuffer.GetAddressOf());

        // Create buffer for extracting data from the bound output buffer
        outputDesc.Usage = D3D11_USAGE_STAGING;
        outputDesc.BindFlags = 0;
        outputDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

        pDevice->CreateBuffer(&outputDesc, 0, csOutputResultBuffer.GetAddressOf());

        // Create the unordered access view
        D3D11_UNORDERED_ACCESS_VIEW_DESC csUAVdesc;
        csUAVdesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
        csUAVdesc.Buffer.FirstElement = 0;
        csUAVdesc.Buffer.Flags = 0;
        csUAVdesc.Format = DXGI_FORMAT_UNKNOWN;
        csUAVdesc.Buffer.NumElements = size.x * size.y;

        pDevice->CreateUnorderedAccessView(csOutputBuffer.Get(), &csUAVdesc, csUAV.GetAddressOf());

        // Set constant buffer and unordered access view
        pDeviceContext->CSSetConstantBuffers(0, 1, cBuffer.GetAddressOf());
        pDeviceContext->CSSetUnorderedAccessViews(0, 1, csUAV.GetAddressOf(), nullptr);

        // Run the compute shader
        UINT* data = RunComputeShader(false);
        
        // Create the texture and put the fresh data into it
        D3D11_TEXTURE2D_DESC outputTextureDesc;
        outputTextureDesc.Usage = D3D11_USAGE_DYNAMIC;
        outputTextureDesc.Width = size.x;
        outputTextureDesc.Height = size.y;
        outputTextureDesc.MipLevels = 1;
        outputTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        outputTextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        outputTextureDesc.Format = DXGI_FORMAT_R32_UINT;
        outputTextureDesc.ArraySize = 1;
        outputTextureDesc.MiscFlags = 0;
        outputTextureDesc.SampleDesc.Count = 1;
        outputTextureDesc.SampleDesc.Quality = 0;

        D3D11_SUBRESOURCE_DATA outputTextureData;
        outputTextureData.pSysMem = data;
        outputTextureData.SysMemPitch = sizeof(UINT) * size.x;
        outputTextureData.SysMemSlicePitch = 0;

        pDevice->CreateTexture2D(&outputTextureDesc, &outputTextureData, csOutputTexture.GetAddressOf());
        #pragma endregion

        #pragma region Vertex shader
        // Compile and create the shader
        ComPtr<ID3DBlob> vs_compiled;
        std::vector<char> vs_bytes = readBytes("vertex.hlsl");
        D3DCompile(vs_bytes.data(), vs_bytes.size(), NULL, NULL, NULL, "main", "vs_5_0", NULL, NULL, vs_compiled.GetAddressOf(), NULL);

        pDevice->CreateVertexShader(vs_compiled->GetBufferPointer(), vs_compiled->GetBufferSize(), nullptr, vShader.GetAddressOf());
        pDeviceContext->VSSetShader(vShader.Get(), nullptr, 0);

        // Create the full-window vertex buffer
        XMFLOAT3 vertices[] = {
            XMFLOAT3(-1, -1, 0),
            XMFLOAT3(-1, 1, 0),
            XMFLOAT3(1, -1, 0),
            XMFLOAT3(1, 1, 0)
        };

        D3D11_BUFFER_DESC vertexBufferDesc;
        vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
        vertexBufferDesc.ByteWidth = (UINT)(sizeof(XMFLOAT3) * 4);
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.CPUAccessFlags = 0;
        vertexBufferDesc.MiscFlags = 0;
        vertexBufferDesc.StructureByteStride = 0;

        D3D11_SUBRESOURCE_DATA vertexBufferInitData;
        vertexBufferInitData.pSysMem = vertices;
        vertexBufferInitData.SysMemPitch = 0;
        vertexBufferInitData.SysMemSlicePitch = 0;

        pDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferInitData, vsVertexBuffer.GetAddressOf());

        // Create the index buffer
        UINT indices[] = {0, 1, 2, 1, 3, 2};

        D3D11_BUFFER_DESC indexBufferDesc;
        indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
        indexBufferDesc.ByteWidth = (UINT)(sizeof(UINT) * 6);
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexBufferDesc.CPUAccessFlags = 0;
        indexBufferDesc.MiscFlags = 0;
        indexBufferDesc.StructureByteStride = 0;

        D3D11_SUBRESOURCE_DATA indexBufferInitData;
        indexBufferInitData.pSysMem = indices;
        indexBufferInitData.SysMemPitch = 0;
        indexBufferInitData.SysMemSlicePitch = 0;

        pDevice->CreateBuffer(&indexBufferDesc, &indexBufferInitData, vsIndexBuffer.GetAddressOf());

        // Set the index and vertex buffers
        UINT stride = sizeof(XMFLOAT3);
        UINT offset = 0;
        pDeviceContext->IASetVertexBuffers(0, 1, vsVertexBuffer.GetAddressOf(), &stride, &offset);
        pDeviceContext->IASetIndexBuffer(vsIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // Create and set the input layout
        D3D11_INPUT_ELEMENT_DESC iaDesc[] = {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
        };
        pDevice->CreateInputLayout(iaDesc, 1, vs_compiled->GetBufferPointer(), vs_compiled->GetBufferSize(), vsInputLayout.GetAddressOf());
        pDeviceContext->IASetInputLayout(vsInputLayout.Get());
        #pragma endregion

        #pragma region Render target view
        // Get back buffer and create render target view
        pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)pBackBuffer.GetAddressOf());
        pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, pRTV.GetAddressOf());
        // Create and set the viewport
        ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
        viewport.Width = (float)size.x;
        viewport.Height = (float)size.y;
        viewport.MinDepth = 0;
        viewport.MaxDepth = 1;
        pDeviceContext->RSSetViewports(1, &viewport);
        #pragma endregion

        #pragma region Pixel shader
        // Compile and create the shader
        ComPtr<ID3DBlob> ps_compiled;
        std::vector<char> ps_bytes = readBytes("pixel.hlsl");
        D3DCompile(ps_bytes.data(), ps_bytes.size(), NULL, NULL, NULL, "main", "ps_5_0", NULL, NULL, ps_compiled.GetAddressOf(), NULL);

        pDevice->CreatePixelShader(ps_compiled->GetBufferPointer(), ps_compiled->GetBufferSize(), nullptr, pShader.GetAddressOf());
        pDeviceContext->PSSetShader(pShader.Get(), nullptr, 0);

        // Set the pixel shader constant buffer too
        pDeviceContext->PSSetConstantBuffers(0, 1, cBuffer.GetAddressOf());
        
        // Create and set the input texture shader resource view
        D3D11_SHADER_RESOURCE_VIEW_DESC psSRVdesc;
        psSRVdesc.Format = DXGI_FORMAT_R32_UINT;
        psSRVdesc.Texture2D.MipLevels = 1;
        psSRVdesc.Texture2D.MostDetailedMip = 0;
        psSRVdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

        pDevice->CreateShaderResourceView(csOutputTexture.Get(), &psSRVdesc, psInputSRV.GetAddressOf());

        pDeviceContext->PSSetShaderResources(0, 1, psInputSRV.GetAddressOf());        
        #pragma endregion
    }
}

void Renderer::CreateDeviceIndependentResources() {
    D3D_FEATURE_LEVEL levels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    }; 

    UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

    #if defined(DEBUG) || defined(_DEBUG)
    deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    #endif

    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    swapChainDesc.OutputWindow = hWnd;

    D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, deviceFlags, levels, 4, D3D11_SDK_VERSION, &swapChainDesc, &pSwapChain, pDevice.GetAddressOf(), nullptr, pDeviceContext.GetAddressOf());
}

Renderer::Renderer(HWND hWnd_) :
hWnd(hWnd_)
{
    CreateDeviceIndependentResources();

    OnRender();
}

Renderer::~Renderer() {}

void Renderer::OnRender(bool updateCBuffer) {
    CreateDeviceResources();

    // In case of updated PreviewZoom update constant buffer
    if (updateCBuffer) UpdateConstantBuffer();

    // Clear and set render target view
    float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    pDeviceContext->ClearRenderTargetView(pRTV.Get(), color);
    pDeviceContext->OMSetRenderTargets(1, pRTV.GetAddressOf(), nullptr);

    // Draw
    pDeviceContext->DrawIndexed(6, 0, 0);

    pSwapChain->Present(1, 0);
}

void Renderer::OnResize() {
    if (pRTV) {
        RECT rc;
        GetClientRect(hWnd, &rc);

        XMUINT2 size = XMUINT2(
            rc.right - rc.left,
            rc.bottom - rc.top
        );

        // Make the size non-zero (or it will die)
        if (size.x == 0) size.x++;
        if (size.y == 0) size.y++;

        // Update the viewport
        viewport.Width = (float)size.x;
        viewport.Height = (float)size.y;
        pDeviceContext->RSSetViewports(1, &viewport);

        // Reset, resize, and set the back buffer and its render target view
        pRTV.Reset(); pBackBuffer.Reset();
        pSwapChain->ResizeBuffers(2, size.x, size.y, DXGI_FORMAT_UNKNOWN, 0);
        pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)pBackBuffer.GetAddressOf());
        pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, pRTV.GetAddressOf());

        #pragma region Update constant buffer and compute shader output texture, UAV and output buffer
        // Update the size in constant buffer
        Info.Size = size;
        UpdateConstantBuffer();

        // Resize the output texture buffers
        D3D11_BUFFER_DESC outputDesc;
        csOutputBuffer->GetDesc(&outputDesc);
        outputDesc.ByteWidth = sizeof(UINT) * size.x * size.y;

        pDevice->CreateBuffer(&outputDesc, nullptr, csOutputBuffer.GetAddressOf());

        outputDesc.Usage = D3D11_USAGE_STAGING;
        outputDesc.BindFlags = 0;
        outputDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

        pDevice->CreateBuffer(&outputDesc, 0, csOutputResultBuffer.GetAddressOf());

        // Resize the unordered access view
        D3D11_UNORDERED_ACCESS_VIEW_DESC csUAVdesc;
        csUAV->GetDesc(&csUAVdesc);
        csUAVdesc.Buffer.NumElements = size.x * size.y;

        pDevice->CreateUnorderedAccessView(csOutputBuffer.Get(), &csUAVdesc, csUAV.GetAddressOf());
                
        // Resize the texture, as well as recall the compute shader because of the new size
        D3D11_TEXTURE2D_DESC outputTextureDesc;
        csOutputTexture->GetDesc(&outputTextureDesc);
        outputTextureDesc.Width = Info.Size.x;
        outputTextureDesc.Height = Info.Size.y;

        D3D11_SUBRESOURCE_DATA outputTextureData;
        outputTextureData.pSysMem = RunComputeShader(false);
        outputTextureData.SysMemPitch = sizeof(UINT) * Info.Size.x;
        outputTextureData.SysMemSlicePitch = 0;

        pDevice->CreateTexture2D(&outputTextureDesc, &outputTextureData, csOutputTexture.GetAddressOf());

        // Re-create and set the texture shader resource view
        D3D11_SHADER_RESOURCE_VIEW_DESC psSRVdesc;
        psInputSRV->GetDesc(&psSRVdesc);

        pDevice->CreateShaderResourceView(csOutputTexture.Get(), &psSRVdesc, psInputSRV.GetAddressOf());

        pDeviceContext->PSSetShaderResources(0, 1, psInputSRV.GetAddressOf());

        // Re-render
        OnRender();
        #pragma endregion
    }
}

void Renderer::Calculate() {
    if (!Reloading) {
        UpdateConstantBuffer();
        RunComputeShader(true);
        OnRender();
    }
}

void Renderer::OnZoom() {
    #pragma region Re-calculate fractal bounds
    if (Info.PreviewZoom.x != -1 && Info.PreviewZoom.x != Info.PreviewZoom.z && Info.PreviewZoom.y != Info.PreviewZoom.w) {
        const float zl = Info.Zoom.x;
        Info.Zoom.x = (Info.PreviewZoom.x < Info.PreviewZoom.z ? Info.PreviewZoom.x : Info.PreviewZoom.z) * (Info.Zoom.z - zl) + zl;
        Info.Zoom.z = (Info.PreviewZoom.x > Info.PreviewZoom.z ? Info.PreviewZoom.x : Info.PreviewZoom.z) * (Info.Zoom.z - zl) + zl;
        const float zt = Info.Zoom.y;
        Info.Zoom.y = (Info.PreviewZoom.y < Info.PreviewZoom.w ? Info.PreviewZoom.y : Info.PreviewZoom.w) * (Info.Zoom.w - zt) + zt;
        Info.Zoom.w = (Info.PreviewZoom.y > Info.PreviewZoom.w ? Info.PreviewZoom.y : Info.PreviewZoom.w) * (Info.Zoom.w - zt) + zt;
        // Basically, I will split up the last line of code (can be any out of 4) into two parts: (...) | * (...) + ...
        // The first part is just getting the real left/right/top/bottom, and the second part is conversion to the complex plane
    }
    #pragma endregion

    // Invalidate PreviewZoom
    Info.PreviewZoom = XMFLOAT4(-1, -1, -1, -1);

    Calculate();
}

void Renderer::OnC_Change() {
    #pragma region Re-calculate fractal C
    Info.C.x = (Info.C.x / Info.Size.x) * (Info.Zoom.z - Info.Zoom.x) + Info.Zoom.x;
    Info.C.y = (Info.C.y / Info.Size.y) * (Info.Zoom.w - Info.Zoom.y) + Info.Zoom.y;
    #pragma endregion 

    Calculate();
}

void Renderer::OnHLSL_Change(SHADER_TYPE shaderType) {
    Reloading = true;

    // Compile the shader (depending on the shaderType variable)
    ComPtr<ID3DBlob> s_compiled;
    std::vector<char> s_bytes = readBytes(shaderType == SHADER_TYPE_COMPUTE ? "compute.hlsl" : shaderType == SHADER_TYPE_VERTEX ? "vertex.hlsl" : "pixel.hlsl");
    D3DCompile(s_bytes.data(), s_bytes.size(), NULL, NULL, NULL, "main", shaderType == SHADER_TYPE_COMPUTE ? "cs_5_0" : shaderType == SHADER_TYPE_VERTEX ? "vs_5_0" : "ps_5_0", NULL, NULL, s_compiled.GetAddressOf(), NULL);

    // Assign the shader to the corresponding variable
    switch (shaderType) {
        case SHADER_TYPE_COMPUTE: 
            pDevice->CreateComputeShader(s_compiled->GetBufferPointer(), s_compiled->GetBufferSize(), nullptr, cShader.GetAddressOf());
            pDeviceContext->CSSetShader(cShader.Get(), nullptr, 0);
            break;
        case SHADER_TYPE_VERTEX: 
            pDevice->CreateVertexShader(s_compiled->GetBufferPointer(), s_compiled->GetBufferSize(), nullptr, vShader.GetAddressOf()); 
            pDeviceContext->VSSetShader(vShader.Get(), nullptr, 0);
            break;
        case SHADER_TYPE_PIXEL: 
            pDevice->CreatePixelShader(s_compiled->GetBufferPointer(), s_compiled->GetBufferSize(), nullptr, pShader.GetAddressOf()); 
            pDeviceContext->PSSetShader(pShader.Get(), nullptr, 0);
            break;
    }

    // Re-calculate and re-render
    Calculate();

    // Set cursor back from loading
    Reloading = false;
}
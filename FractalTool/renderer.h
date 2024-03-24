#pragma once

#define MAX_LOG_LENGTH 1024

enum SHADER_TYPE {
	SHADER_TYPE_COMPUTE = 0,
	SHADER_TYPE_VERTEX = 1,
	SHADER_TYPE_PIXEL = 2
};

struct CBUFFER {
	// Bounds of the camera in the complex plane
	XMFLOAT4 Zoom; // Left is x, Top is y, Right is z and Bottom is w
	// Zoom selection rectangle (when none just -1 everywhere)
	XMFLOAT4 PreviewZoom; // Same scheme as Zoom' one
	// Complex fractal argument
	XMFLOAT2 C;
	// Size of the window
	XMUINT2 Size;
	// Max iterations to perform in compute shader
	UINT MaxIter;
	// Time passed since the app start
	FLOAT Time;

	// Unused variable, added to satisfy the D3D requirement of sizeof(CBUFFER) being divisible by 16
	XMFLOAT2 Unused;
};

class Renderer {
public:
	Renderer(HWND hWnd_);
	~Renderer();

	CBUFFER Info;
	bool Reloading = false;

	void OnRender(float delta);
	void OnResize();
	void OnZoom();
	void OnQuit();
	void OnC_Change();
	void OnHLSL_Change(SHADER_TYPE shader);
private:
	ComPtr<ID3D11Device> pDevice;
	ComPtr<ID3D11DeviceContext> pDeviceContext;
	ComPtr<ID3D11Debug> pDebug;
	ComPtr<IDXGISwapChain> pSwapChain;
	ComPtr<ID3D11Buffer> pBackBuffer;
	ComPtr<ID3D11RenderTargetView> pRTV; // Back buffer render target view
	D3D11_VIEWPORT viewport;

	ComPtr<ID3D11ComputeShader> cShader;
	ComPtr<ID3D11Buffer> csOutputBuffer; // Compute shader output (bound to UAV)
	ComPtr<ID3D11Buffer> csOutputResultBuffer; // Compute shader output (in buffer)
	ComPtr<ID3D11UnorderedAccessView> csUAV; // Compute shader UAV
	ComPtr<ID3D11Buffer> cBuffer; // Constant buffer (also used in pixel shader)
	ComPtr<ID3D11Texture2D> csOutputTexture; // Compute shader output (in texture)

	ComPtr<ID3D11VertexShader> vShader;
	ComPtr<ID3D11Buffer> vsVertexBuffer;
	ComPtr<ID3D11Buffer> vsIndexBuffer;
	ComPtr<ID3D11InputLayout> vsInputLayout;

	ComPtr<ID3D11PixelShader> pShader;
	ComPtr<ID3D11ShaderResourceView> psInputSRV; // Pixel shader input texture SRV (uses csOutputTexture)

	HWND hWnd;

	void UpdateConstantBuffer();
	UINT* RunComputeShader(bool updateTexture);

	void CreateDeviceResources();
	void CreateDeviceIndependentResources();
	void ReleaseResources();
};
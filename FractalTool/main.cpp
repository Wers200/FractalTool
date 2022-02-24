#include "framework.h"
#include "renderer.h"

Renderer* renderer;

// Required for shift-key feature: press shift key and turn on/off the square zoom selection mode
bool shift = false;
float zAtStart = 0;
float wAtStart = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_SIZE:
		renderer->OnResize();
		return 0;
	case WM_RBUTTONDOWN:
	{
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		renderer->Info.PreviewZoom.x = (float)xPos / renderer->Info.Size.x;
		renderer->Info.PreviewZoom.y = (float)yPos / renderer->Info.Size.y;
		renderer->Info.PreviewZoom.z = (float)xPos / renderer->Info.Size.x;
		renderer->Info.PreviewZoom.w = (float)yPos / renderer->Info.Size.y;

		zAtStart = renderer->Info.PreviewZoom.z;
		wAtStart = renderer->Info.PreviewZoom.w;
	}
	return 0;
	case WM_RBUTTONUP:
		renderer->OnZoom();
		return 0;
	case WM_MOUSEMOVE:
		SetCursor(LoadCursor(NULL, IDC_ARROW)); // Sometimes it gets stuck on some cursor :shrug:
		if (renderer->Info.PreviewZoom.x != -1) {
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);
			renderer->Info.PreviewZoom.z = (float)xPos / renderer->Info.Size.x;
			// In case of square mode being turned on force the bottom of zoom make the selection a square
			renderer->Info.PreviewZoom.w = !shift ? (float)yPos / renderer->Info.Size.y : wAtStart - zAtStart + renderer->Info.PreviewZoom.z;
			if(!renderer->Reloading) renderer->OnRender(true);
		}
		return 0;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			// Set zoom to default and invalidate zoom selection
			renderer->Info.Zoom = XMFLOAT4(-2, 2, 2, -2);
			renderer->Info.PreviewZoom = XMFLOAT4(-1, -1, -1, -1);
			renderer->OnZoom();
		}
		else if (wParam == VK_RIGHT) {
			renderer->Info.MaxIter += 25;
			renderer->Calculate();
		}
		else if (wParam == VK_LEFT && renderer->Info.MaxIter > 25) {
			renderer->Info.MaxIter -= 25;
			renderer->Calculate();
		}
		else if (wParam == VK_UP) {
			renderer->Info.MaxIter += 250;
			renderer->Calculate();
		}
		else if (wParam == VK_DOWN && renderer->Info.MaxIter > 250) {
			renderer->Info.MaxIter -= 250;
			renderer->Calculate();
		}
		else if (wParam == VK_SHIFT) shift = true;
		return 0;
	case WM_KEYUP:
		if (wParam == VK_SHIFT) shift = false;
		return 0;
	case WM_LBUTTONDOWN:
		if (wParam == MK_LBUTTON) {
			renderer->Info.C.x = (float)GET_X_LPARAM(lParam);
			renderer->Info.C.y = (float)GET_Y_LPARAM(lParam);
			renderer->OnC_Change();
		}
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, int) {
	// Get screen sizes
	HDC hDC = GetDC(0);
	int screenWidth = GetDeviceCaps(hDC, HORZRES);
	int screenHeight = GetDeviceCaps(hDC, VERTRES);

	// Get the icon
	HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));

	// Create window class
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = L"FractalToolWindow";
	wc.lpfnWndProc = WndProc;
	wc.hIcon = hIcon;
	wc.hInstance = hInstance;

	DeleteObject(hIcon);
	DeleteDC(hDC);

	RegisterClassEx(&wc);

	// Create window & renderer
	HWND hWnd = CreateWindowEx(
		NULL,
		L"FractalToolWindow",
		L"Fractal Tool",
		WS_OVERLAPPEDWINDOW,
		screenWidth / 2 - screenWidth / 5, // Center the window
		screenHeight / 2 - screenWidth / 5,
		(int)(screenWidth / 2.5),
		(int)(screenWidth / 2.5),
		NULL,
		NULL,
		hInstance,
		NULL
	);

	renderer = new Renderer(hWnd);
		
	// Show window
	UpdateWindow(hWnd);
	ShowWindow(hWnd, SW_SHOW);

	std::thread([=] {
		// Get current path
		WCHAR path[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, path);

		// Create path handle
		HANDLE hDir = CreateFile(
			path,
			FILE_LIST_DIRECTORY,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
			NULL
		);
		// Create other file-watching required stuff
		FILE_NOTIFY_INFORMATION fNI[1024];
		DWORD bytesReturned = 0;
		OVERLAPPED overlapped;
		ZeroMemory(&overlapped, sizeof(OVERLAPPED));
		// Create event on the overlapped object
		overlapped.hEvent = CreateEvent(NULL, true, false, NULL);

		// Bind overlapped event to directory changes
		ReadDirectoryChangesW(hDir, fNI, sizeof(fNI), false, FILE_NOTIFY_CHANGE_LAST_WRITE, &bytesReturned, &overlapped, NULL);

		// Start the directory changes reading loop
		MSG msg = {};
		bool quit = false;
		DWORD waitStatus;

		while (!quit) {
			// Wait for directory changes
			waitStatus = WaitForSingleObject(overlapped.hEvent, 0);

			switch (waitStatus) {
			case WAIT_OBJECT_0:
			{
				GetOverlappedResult(hDir, &overlapped, &bytesReturned, false);

				std::wstring fileName(fNI->FileName, fNI->FileNameLength);
				// Using .find function because file name usually contains some unnecessary (typically Chinese) symbols along with the filename
				// Also I know that this kinda looks like Matvie code, but I was too lazy to think of something else
				// As well, for some reason VS on edit gives weird name instead of the filename, so it doesn't work :shrug:
				if (fileName.find(L"compute.hlsl") != fileName.npos) {
					renderer->OnHLSL_Change(SHADER_TYPE_COMPUTE);
				}
				else if (fileName.find(L"vertex.hlsl") != fileName.npos) {
					renderer->OnHLSL_Change(SHADER_TYPE_VERTEX);
				}
				else if (fileName.find(L"pixel.hlsl") != fileName.npos) {
					renderer->OnHLSL_Change(SHADER_TYPE_PIXEL);
				}

				// Reset the overlapped event and wait for directory changes again
				ResetEvent(overlapped.hEvent);

				ReadDirectoryChangesW(hDir, fNI, sizeof(fNI), false, FILE_NOTIFY_CHANGE_LAST_WRITE, &bytesReturned, &overlapped, NULL);
			}
				break;
			case WAIT_TIMEOUT:
				// Check if it there is a quit message
				while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
					if (msg.message == WM_QUIT) {
						quit = true;
						break;
					}
				}
				break;
			}
		}
	}).detach();

	// Start the windows message loop
	MSG msg = {};
	bool quit = false;

	while (!quit) {
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				quit = true;
				break;
			}
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	return 0;
}
#pragma once

#include "resource.h"

#include <string>
#include <vector>
#include <thread>

#include <Windows.h>
#include <wrl.h>
using namespace Microsoft::WRL;

#include <d3d11_4.h>
#include <d3d11sdklayers.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
using namespace DirectX;

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <windowsx.h>
#include <cstdio>
#include "debug.h"
#include "file.h"

#include <filesystem>
#include <fstream>

#if defined(DEBUG) || defined(_DEBUG)
#undef new
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif
#pragma once

#include "resource.h"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOWED true
#define WIREFRAME false
#define VSYNC 1 //1 == ON, 0 = OFF
#define DEBUG 2 /* 2 = debug,  or : D3D11_CREATE_DEVICE_DEBUG */
#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }

#pragma once
#define DIRECTINPUT_VERSION 0x0800

#include <algorithm>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <vector>
#define D3D_DEBUG_INFO

#include <d3d11sdklayers.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#define WORLDBUFFER_SLOT    0
#define CAMERABUFFER_SLOT   1
#define MATERIALBUFFER_SLOT 2
#define LIGHTBUFFER_SLOT    3
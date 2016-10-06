// SharedMem3DRenderer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "SharedMem3DRenderer.h"
#include "Engine.h"
#define CRTDBG_MAP_ALLOC

#include <stdlib.h>
#include <crtdbg.h>


HWND InitWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);



int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
		MSG msg = { 0 };
		{
		HWND wndHandle = InitWindow(hInstance);
	
		Engine engine;
	
		if (!engine.Init(wndHandle))
		{
			MessageBox(wndHandle, TEXT("Could not init engine"), TEXT("ERROR"), MB_OK);
			return -1;
		}
	
	
	
		ShowWindow(wndHandle, nCmdShow);
	
		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
	
				engine.Run();
			}
		}
	
		DestroyWindow(wndHandle);
	}

	_CrtCheckMemory();
	_CrtDumpMemoryLeaks();
	return (int)msg.wParam;
}

HWND InitWindow(HINSTANCE hInstance)
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = L"BTH_D3D_DEMO";
	if (!RegisterClassEx(&wcex))
		return false;

	RECT rc = { 0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	HWND handle = CreateWindow(
		L"BTH_D3D_DEMO",
		L"BTH Direct3D Demo",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	return handle;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_DESTROY:
	{

		PostQuitMessage(0);
		break;
	}

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}

	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
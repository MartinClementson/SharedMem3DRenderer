#pragma once

#include "SharedMem3DRenderer.h"
#include "Graphics.h"
#include "BufferHandler.h"
#include "MayaCommunicator.h"
#include "ResourceManager.h"
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
inline bool CreateCommunicatorThread(MayaCommunicator& communicator)
{
	communicator.Execute();
	return true;
};


class Engine
{
private:
	bool CreateDirect3DContext(HWND* handle);



private:
	
	HANDLE comThread;
	DWORD threadID;
	MayaCommunicator mayaCommunicator;
	Graphics graphics;

	ID3D11Debug* debug; //Debug COM
	HRESULT hr; //Result variable


	ID3D11Device *gDevice						= nullptr;
	ID3D11DeviceContext* gDeviceContext			= nullptr;

	IDXGISwapChain* gSwapChain					= nullptr;
	ID3D11RenderTargetView* gBackbufferRTV		= nullptr;
	ID3D11ShaderResourceView *BackBufferTexture = nullptr;

	//Depth stencil
	ID3D11DepthStencilState* depthState			= nullptr;
	ID3D11DepthStencilView* depthStencilView	= nullptr;
	ID3D11Texture2D *depthBuffer				= nullptr;
	D3D11_VIEWPORT vp; //Viewport
public:
	Engine();
	bool Init(HWND& windowHandle);
	void Run();
	~Engine();
};


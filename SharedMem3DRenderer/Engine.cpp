#include "stdafx.h"
#include "Engine.h"


bool Engine::CreateDirect3DContext(HWND * handle)
{

	//Swap chain description
	DXGI_SWAP_CHAIN_DESC scd;

	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferDesc.Width	 = WINDOW_WIDTH;
	scd.BufferDesc.Height	 = WINDOW_HEIGHT;
	scd.BufferCount			 = 1;
	scd.BufferDesc.Format	 = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage			 = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	scd.OutputWindow		 = *handle;
	scd.SampleDesc.Count	 = 1;
	scd.Windowed		     = WINDOWED;
	scd.BufferDesc.RefreshRate.Numerator = 60; //fps cap
	scd.BufferDesc.RefreshRate.Denominator = 1;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&this->gSwapChain,
		&this->gDevice,
		NULL,
		&this->gDeviceContext);

	if (FAILED(hr))
		return false;

	if (DEBUG == 2)
	{
		hr = gDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)&debug);
		if (FAILED(hr))
			return false;

	}



	//Here goes depth buffer
	D3D11_TEXTURE2D_DESC desc;

	desc.Width				= WINDOW_WIDTH;
	desc.Height				= WINDOW_HEIGHT;
	desc.MipLevels			= 1;
	desc.ArraySize			= 1;
	desc.Format				= DXGI_FORMAT_D32_FLOAT;
	desc.SampleDesc.Count   = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage				= D3D11_USAGE_DEFAULT;
	desc.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags		= 0;
	desc.MiscFlags		    = 0;

	hr = gDevice->CreateTexture2D(&desc, 0, &depthBuffer);

	hr = gDevice->CreateDepthStencilView(depthBuffer, 0, &depthStencilView);

	//Create depth state
	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable		 = true;
	dssDesc.DepthWriteMask   = D3D11_DEPTH_WRITE_MASK_ALL; //Default
	dssDesc.DepthFunc        = D3D11_COMPARISON_LESS;      //Default

	gDevice->CreateDepthStencilState(&dssDesc, &depthState);




	if (SUCCEEDED(hr))
	{
		ID3D11Texture2D* pBackBuffer = nullptr;

		//D3D11_TEXTURE2D_DESC texDesc;
		//ZeroMemory(&texDesc, sizeof(texDesc));
		//texDesc.Width = WINDOW_WIDTH;
		//texDesc.Height = WINDOW_HEIGHT;
		//texDesc.MipLevels = 0;
		//texDesc.ArraySize = 1;
		//texDesc.SampleDesc.Count = 1;
		//texDesc.SampleDesc.Quality = 0;
		//texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		//texDesc.Usage = D3D11_USAGE_DEFAULT;
		//texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		//texDesc.CPUAccessFlags = 0;
		//texDesc.MiscFlags = 0;

		this->gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		//hr = gDevice->CreateTexture2D(&texDesc, 0, &pBackBuffer);
		//if (FAILED(hr))
		//	errorMsg("failed to init backbuffer rtv");


		hr = this->gDevice->CreateRenderTargetView(pBackBuffer, NULL, &this->gBackbufferRTV);
		if (FAILED(hr))
			return false;





		hr = this->gDevice->CreateShaderResourceView(pBackBuffer, nullptr, &BackBufferTexture);

		if (FAILED(hr))
			return false;



		pBackBuffer->Release();

		this->gDeviceContext->OMSetRenderTargets(1, &this->gBackbufferRTV, depthStencilView);

	}

	return true;
}

Engine::Engine()
{
}

bool Engine::Init(HWND& windowHandle)
{

	if (!this->CreateDirect3DContext(&windowHandle))
	{
		MessageBox(windowHandle, TEXT("Could not create direct3dContext"), TEXT("Error"), MB_OK);
		return false;
	}

	vp.Width = (float)WINDOW_WIDTH;
	vp.Height = (float)WINDOW_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	this->gDeviceContext->RSSetViewports(1, &vp);
		


	if (!BufferHandler::GetInstance()->Init(gDevice, gDeviceContext))
		return false;
	if (!this->graphics.Init(this->gDevice, this->gDeviceContext))
		return false;


	//Create communicator and start thread

	comThread = CreateThread(
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)CreateCommunicatorThread,
		&mayaCommunicator,
		0,
		&threadID);
	if (threadID == NULL)
		return false;


	return true;
}

void Engine::Run()
{
	float clearColor[] = { 0, 0, 0, 1 };

	gDeviceContext->OMSetRenderTargets(1, &this->gBackbufferRTV, this->depthStencilView);
	this->gDeviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1, 0);
	this->gDeviceContext->ClearRenderTargetView(gBackbufferRTV, clearColor);
	graphics.Render();
	this->gSwapChain->Present(VSYNC, 0); //Change front and back buffer after rendering
}


Engine::~Engine()
{

	//inter thread communication here
	mayaCommunicator.Stop();
	////

	WaitForSingleObject(comThread, INFINITE);
	CloseHandle(comThread);

	//ResourceManager::GetInstance()->~ResourceManager();
	BufferHandler::GetInstance()->~BufferHandler(); //Delete the monster you created!
	SAFE_RELEASE(gBackbufferRTV);
	
	SAFE_RELEASE(gSwapChain);




	SAFE_RELEASE(depthBuffer);
	SAFE_RELEASE(depthState);
	SAFE_RELEASE(depthStencilView);

	
	SAFE_RELEASE(BackBufferTexture);

	gDeviceContext->ClearState();
	SAFE_RELEASE(gDeviceContext);


	SAFE_RELEASE(gDevice);
	if (DEBUG == 2)
		debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	SAFE_RELEASE(debug);



}

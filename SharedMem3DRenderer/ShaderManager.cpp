#include "stdafx.h"
#include "ShaderManager.h"


ShaderManager::ShaderManager()
{
}


ShaderManager::~ShaderManager()
{
	this->Release();
}
bool ShaderManager::createShaders()
{
	if (!createColorShader())
		return false;
	if (!createTextureShader())
		return false;
	return true;
}

bool ShaderManager::createColorShader()
{
	ID3DBlob* pVS = nullptr;

	D3DCompileFromFile(
		L"Shaders/VertexShaderColor.hlsl",
		nullptr,
		nullptr,
		"VS_main",
		"vs_4_0",
		0,
		0,
		&pVS,
		nullptr);

	hr = this->gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &gVertexShaderColor);
	if (FAILED(hr))
		return false;
	//Create input layout (every vertex)
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA,0 }

	};

	hr = this->gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gVertexLayoutColor);
	pVS->Release();
	if (FAILED(hr))
		return false;

	ID3DBlob *pPs = nullptr;
	D3DCompileFromFile(
		L"Shaders/PixelShaderColor.hlsl",
		nullptr,
		nullptr,
		"PS_main",
		"ps_4_0",
		0,
		0,
		&pPs,
		nullptr);

	hr = this->gDevice->CreatePixelShader(pPs->GetBufferPointer(), pPs->GetBufferSize(), nullptr, &gPixelShaderColor);
	pPs->Release();

	if (FAILED(hr))
		return false;

	//Geometry shader
	ID3DBlob* pGS = nullptr;
	D3DCompileFromFile(
		L"Shaders/GeometryShaderColor.hlsl",
		nullptr,
		nullptr,
		"GS_main",
		"gs_4_0",
		0,
		0,
		&pGS,
		nullptr);

	hr = this->gDevice->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), nullptr, &gGeometryShaderColor);
	pGS->Release();
	if (FAILED(hr))
		return false;

	return true;
}


bool ShaderManager::createTextureShader()
{
	//Create a sample state first



	D3D11_SAMPLER_DESC samplerDesc;
	// use linear interpolation for minification, magnification, and mip-level sampling (quite expensive)
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
	//for all filters: https://msdn.microsoft.com/en-us/library/windows/desktop/ff476132(v=vs.85).aspx

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP; //wrap, (repeat) for use of tiling texutures
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f; //mipmap offset level
	samplerDesc.MaxAnisotropy = 1; //Clamping value used if D3D11_FILTER_ANISOTROPIC or D3D11_FILTER_COMPARISON_ANISOTROPIC is specified in Filter. Valid values are between 1 and 16.
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0; //0 most detailed mipmap level, higher number == less detail
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;


	hr = gDevice->CreateSamplerState(&samplerDesc, &gSampleState);


	if (FAILED(hr))
	{
		return false;


	}
	else
	{
		//Set sampler to pixel shader and the compute shader
		gDeviceContext->PSSetSamplers(0, 1, &this->gSampleState);
		gDeviceContext->CSSetSamplers(0, 1, &this->gSampleState);
	}

	//Load the shaders



	ID3DBlob* pVS = nullptr;

	D3DCompileFromFile(
		L"Shaders/VertexShaderTexture.hlsl",
		nullptr,
		nullptr,
		"VS_main",
		"vs_4_0",
		0,
		0,
		&pVS,
		nullptr);

	hr = this->gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &gVertexShaderTexture);

	if (FAILED(hr))
		return false;
	//Create input layout (every vertex)
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0,    0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMALS",0,   DXGI_FORMAT_R32G32B32_FLOAT , 0,   12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",0,  DXGI_FORMAT_R32G32_FLOAT,	   0,   24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		
	};

	this->gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gVertexLayoutTexture);
	pVS->Release();


	ID3DBlob *pPs = nullptr;
	D3DCompileFromFile(
		L"Shaders/PixelShaderTexture.hlsl",
		nullptr,
		nullptr,
		"PS_main",
		"ps_4_0",
		0,
		0,
		&pPs,
		nullptr);

	hr = this->gDevice->CreatePixelShader(pPs->GetBufferPointer(), pPs->GetBufferSize(), nullptr, &gPixelShaderTexture);
	pPs->Release();

	if (FAILED(hr))
		return false;


	//Geometry shader
	ID3DBlob* pGS = nullptr;
	D3DCompileFromFile(
		L"Shaders/GeometryShaderTexture.hlsl",
		nullptr,
		nullptr,
		"GS_main",
		"gs_4_0",
		0,
		0,
		&pGS,
		nullptr);

	hr = this->gDevice->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), nullptr, &gGeometryShaderTexture);
	pGS->Release();

	if (FAILED(hr))
		return false;


	setActiveShaders(ShaderTypes::TEXTURESHADER);
	return true;

}

void ShaderManager::Release()
{
	//Release Color shaders
	SAFE_RELEASE(gVertexLayoutColor	  );
	SAFE_RELEASE(gVertexShaderColor	  );
	SAFE_RELEASE(gPixelShaderColor	  );
	SAFE_RELEASE(gGeometryShaderColor );

	//Release Texture shaders

	SAFE_RELEASE(gVertexLayoutTexture); 

	SAFE_RELEASE(gVertexShaderTexture	);
	SAFE_RELEASE(gPixelShaderTexture	);
	SAFE_RELEASE(gGeometryShaderTexture	);


	SAFE_RELEASE(gSampleState);




}
void ShaderManager::setActiveShaders(ShaderTypes shader) {

	switch (shader)
	{
	case TEXTURESHADER:
		this->gDeviceContext->VSSetShader(gVertexShaderTexture, nullptr, 0);
		this->gDeviceContext->HSSetShader(nullptr, nullptr, 0);
		this->gDeviceContext->DSSetShader(nullptr, nullptr, 0);
		this->gDeviceContext->GSSetShader(gGeometryShaderTexture, nullptr, 0);
		this->gDeviceContext->PSSetShader(gPixelShaderTexture, nullptr, 0);
		this->gDeviceContext->IASetInputLayout(gVertexLayoutTexture);
		break;

	case COLORSHADER:
		this->gDeviceContext->VSSetShader(gVertexShaderColor, nullptr, 0);
		this->gDeviceContext->HSSetShader(nullptr, nullptr, 0);
		this->gDeviceContext->DSSetShader(nullptr, nullptr, 0);
		this->gDeviceContext->GSSetShader(gGeometryShaderColor, nullptr, 0);
		this->gDeviceContext->PSSetShader(gPixelShaderColor, nullptr, 0);
		this->gDeviceContext->IASetInputLayout(gVertexLayoutColor);
		break;
	}
}


bool ShaderManager::Init(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext)
{
	this->gDevice		 = gDevice;
	this->gDeviceContext = gDeviceContext;

	if (!createShaders())
		return false;

	return true;
}

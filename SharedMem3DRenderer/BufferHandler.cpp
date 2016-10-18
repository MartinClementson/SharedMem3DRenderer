#include "stdafx.h"
#include "BufferHandler.h"


BufferHandler::BufferHandler()
{
}


bool BufferHandler::Init(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext)
{
	this->gDevice = gDevice;
	this->gDeviceContext = gDeviceContext;
	HRESULT hr;


	//Creating world constant buffer																 
	CD3D11_BUFFER_DESC bufferDescWorld;
	ZeroMemory(&bufferDescWorld, sizeof(bufferDescWorld));
	bufferDescWorld.ByteWidth = sizeof(WorldBuffer);
	bufferDescWorld.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescWorld.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescWorld.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDescWorld.MiscFlags = 0;
	bufferDescWorld.StructureByteStride = 0;

	hr = this->gDevice->CreateBuffer(&bufferDescWorld, nullptr, &buffers.bWorldBuffer);
	if (SUCCEEDED(hr))
		this->gDeviceContext->GSSetConstantBuffers(WORLDBUFFER_SLOT, 1, &buffers.bWorldBuffer);
	else
		return false;


	//Creating the Camera constant buffer
	CD3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.ByteWidth			= sizeof(CameraBuffer);
	bufferDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.Usage				= D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags			= 0;
	bufferDesc.StructureByteStride  = 0;

	hr = this->gDevice->CreateBuffer(&bufferDesc, nullptr, &buffers.bCameraBuffer);
	if (SUCCEEDED(hr))
		this->gDeviceContext->GSSetConstantBuffers(CAMERABUFFER_SLOT, 1, &buffers.bCameraBuffer);
	else
		return false;



	//Creating the Material constant buffer
	CD3D11_BUFFER_DESC bufferDescMat;
	ZeroMemory(&bufferDescMat, sizeof(bufferDescMat));
	bufferDescMat.ByteWidth		   = sizeof(MaterialBuffer);
	bufferDescMat.BindFlags		   = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescMat.Usage			   = D3D11_USAGE_DYNAMIC;
	bufferDescMat.CPUAccessFlags   = D3D11_CPU_ACCESS_WRITE;
	bufferDescMat.MiscFlags		   = 0;
	bufferDescMat.StructureByteStride = 0;

	hr = this->gDevice->CreateBuffer(&bufferDescMat, nullptr, &buffers.bMaterialBuffer);
	if (SUCCEEDED(hr))
		this->gDeviceContext->PSSetConstantBuffers(MATERIALBUFFER_SLOT, 1, &buffers.bMaterialBuffer);
	else
		return false;


	CD3D11_BUFFER_DESC bufferDescLight;
	ZeroMemory(&bufferDescLight, sizeof(bufferDescLight));
	bufferDescLight.ByteWidth			 = sizeof(LightBuffer);
	bufferDescLight.BindFlags			 = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescLight.Usage				 = D3D11_USAGE_DYNAMIC;
	bufferDescLight.CPUAccessFlags		 = D3D11_CPU_ACCESS_WRITE;
	bufferDescLight.MiscFlags			 = 0;
	bufferDescLight.StructureByteStride  = 0;

	hr = this->gDevice->CreateBuffer(&bufferDescLight, nullptr, &buffers.bLightBuffer);
	if (SUCCEEDED(hr))
		this->gDeviceContext->PSSetConstantBuffers(LIGHTBUFFER_SLOT, 1, &buffers.bLightBuffer);
	else
		return false;








	return true;

}

BufferHandler::~BufferHandler()
{

	

}

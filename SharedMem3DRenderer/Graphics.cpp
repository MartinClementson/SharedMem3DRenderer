#include "stdafx.h"
#include "Graphics.h"


Graphics::Graphics()
{
	this->resManager = ResourceManager::GetInstance();
}


Graphics::~Graphics()
{
}

bool Graphics::Init(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext)
{

	if (!shaderManager.Init(gDevice, gDeviceContext))
		return false;
	if (!resManager->Init(gDevice, gDeviceContext))
		return false;

}

void Graphics::Render()
{
	
	resManager->camera.UpdateIfDirty();
	shaderManager.setActiveShaders(ShaderTypes::TEXTURESHADER);
	//resManager->testModel->Render();
	resManager->UpdateIfDirty();
	for (size_t i = 0; i < resManager->Models()->size(); i++)
	{
		resManager->Models()->at(i)->Render();

	}
	
}

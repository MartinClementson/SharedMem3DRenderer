#include "stdafx.h"
#include "MaterialNode.h"
inline DirectX::XMFLOAT4 Float3ToXMFLOAT4 (const Float3& other) { 
	DirectX::XMFLOAT4 result; 

	
	result.x = other.x;
	result.y = other.y;
	result.z = other.z;
	result.w = 1.0f;


	return result; 
} 


bool MaterialNode::UpdateMaterial()
{
	if (texturesChanged)
	{
		for (size_t i = 0; i < textureFiles.size(); i++)
		{
			if (!AddTexture(string(textureFiles[i].texturePath), textureFiles[i].type))
					MessageBox(NULL, TEXT("Could not add texture file"), TEXT("ERROR"), MB_OK);	
		}	
		texturesChanged = false;
	}

	BufferHandler::GetInstance()->Buffers()->bMaterialBuffer.UpdateBuffer(&this->materialData, &this->materialName);

	// update const buffer

	//ID3D11Buffer* matBuffer = BufferHandler::GetInstance()->Buffers()->bMaterialBuffer;
	//D3D11_MAPPED_SUBRESOURCE mappedResource;
	//ZeroMemory(&mappedResource, sizeof(mappedResource));
	//
	//this->gDeviceContext->Map(matBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	//
	//MaterialBuffer* temporaryMaterial = (MaterialBuffer*)mappedResource.pData;
	//
	//*temporaryMaterial = this->materialData;
	//
	//this->gDeviceContext->Unmap(matBuffer, 0);


	return true;
}

bool MaterialNode::Init(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext)
{
	this->gDevice = gDevice;
	this->gDeviceContext = gDeviceContext;
	textureFiles.reserve(MAX_TEXTURES);
	textureFiles.push_back(TextureFile());
	textureFiles.push_back(TextureFile());
	//HRESULT result = CoInitialize((LPVOID)0); //initialize Renderer
	//if (FAILED(result))
	//	return false;

	//if (!this->AddTexture(string("C:/Users/BTH/Desktop/rock-texture-with-black-pebbles-photoshop-textures.jpg"), TextureTypes::DIFFUSE))
	//	MessageBox(NULL, TEXT("ERROR loading texture"), TEXT("ERROR"), MB_OK);
	//texturesChanged = true;
	isDirty = true;
	return true;
}

bool MaterialNode::CreateFromMessage(MaterialMessage* material, TextureFile* textures )
{
	this->materialName					= string(material->matName);
	this->materialData.diffuse			= Float3ToXMFLOAT4(material->diffuse);
	this->materialData.ambient			= Float3ToXMFLOAT4(material->ambient);
	this->materialData.specularRGB		= Float3ToXMFLOAT4(material->specularRGB);
	this->materialData.specularValue	= material->specularVal;
	
	for (size_t i = 0; i < material->numTextures; i++) //switch textures
	{
		textureFiles.at(textures[i].type) = textures[i];
		texturesChanged = true;
	}

	isDirty = true;
	return true;
}



bool MaterialNode::AddTexture(std::string & path, TextureTypes type)
{
	if (type == NONE)
		return true;
	if (this->textureMaps[type] != nullptr)
	{
		SAFE_RELEASE(textureMaps[type]);

	}
	else
		this->usedTextures += 1;
	
		

	
	std::wstring widestr = std::wstring(path.begin(), path.end());
	const wchar_t* fileName = widestr.c_str(); 
	
	
	HRESULT hr = DirectX::CreateWICTextureFromFile(gDevice, fileName, nullptr, &textureMaps[type]);
	
	if (FAILED(hr))
		return false;

	switch (type)
	{
	case DIFFUSE:
		this->materialData.usingDiffuseTex = TRUE;
		break;
	case NORMAL:
		this->materialData.usingNormalTex = TRUE;
		break;

	}

	return true;
}

MaterialNode::MaterialNode()
{
}

MaterialNode::MaterialNode(char * name)
{
	this->materialName = string(name);
}


bool MaterialNode::SetActive()
{
	if (isDirty)
	{
		UpdateMaterial();
		isDirty = false;
	}
	if(textureMaps[DIFFUSE] != nullptr)
		this->gDeviceContext->PSSetShaderResources(DIFFUSE, 1, &textureMaps[DIFFUSE]);
	if (textureMaps[NORMAL] != nullptr)
		this->gDeviceContext->PSSetShaderResources(NORMAL, 1, &textureMaps[NORMAL]);


	BufferHandler::GetInstance()->Buffers()->bMaterialBuffer.UpdateBuffer(&this->materialData, &this->materialName);
	
	//ID3D11Buffer* buffer = BufferHandler::GetInstance()->Buffers()->bMaterialBuffer;
	//D3D11_MAPPED_SUBRESOURCE mappedResource;
	//ZeroMemory(&mappedResource, sizeof(mappedResource));
	//
	//this->gDeviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	//
	//MaterialBuffer* tempBufferData = (MaterialBuffer*)mappedResource.pData;
	//
	//*tempBufferData = this->materialData;
	//
	//this->gDeviceContext->Unmap(buffer, 0);


return true;
}

MaterialNode::~MaterialNode()
{
	for (size_t i = 0; i < MAX_TEXTURES; i++)
	{
		if (textureMaps[i] != nullptr)
			SAFE_RELEASE(textureMaps[i]);
	}
}

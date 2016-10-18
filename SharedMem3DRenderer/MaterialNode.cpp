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
	for (size_t i = 0; i < textureFiles.size(); i++)
	{

		if (!AddTexture(string(textureFiles[i].texturePath), textureFiles[i].type))
				MessageBox(NULL, TEXT("Could not add texture file"), TEXT("ERROR"), MB_OK);
		
	}	
	//The texture file has been processed. So remove it to avoid unnecessary add
	textureFiles.clear();

	// update const buffer

	ID3D11Buffer* matBuffer = BufferHandler::GetInstance()->Buffers()->bMaterialBuffer;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(mappedResource));

	this->gDeviceContext->Map(matBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	MaterialBuffer* temporaryMaterial = (MaterialBuffer*)mappedResource.pData;

	*temporaryMaterial = this->materialData;

	this->gDeviceContext->Unmap(matBuffer, 0);


	return true;
}

bool MaterialNode::Init(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext)
{
	this->gDevice = gDevice;
	this->gDeviceContext = gDeviceContext;
	textureFiles.reserve(MAX_TEXTURES);

	return true;
}

bool MaterialNode::CreateFromMessage(MaterialMessage* material, TextureFile* textures )
{
	this->materialName  = string(material->matName);
	this->materialData.diffuse			= Float3ToXMFLOAT4(material->diffuse);
	this->materialData.ambient			= Float3ToXMFLOAT4(material->ambient);
	this->materialData.specularRGB		= Float3ToXMFLOAT4(material->specularRGB);
	this->materialData.specularValue	= material->specularVal;
	
	
	for (size_t i = 0; i < material->numTextures; i++)
	{
		if (textureFiles.size() == MAX_TEXTURES)
		{	// if the max amount of textures exists, find the one we are to replace
			for (size_t i = 0; i < MAX_TEXTURES; i++)
			{
				if (textureFiles.at(DIFFUSE).type == textures[i].type)
				{
					textureFiles.at(DIFFUSE) = textures[i]; //Replace with new textures
					continue;
				}
				if (textureFiles.at(NORMAL).type == textures[i].type)
				{
					textureFiles.at(NORMAL) = textures[i]; //Replace with new textures
					continue;
				}
			}
		}
		else
			textureFiles.at(textures[i].type) = textures[i];
		
	}
	

	return true;
}



bool MaterialNode::AddTexture(std::string & path, TextureTypes type)
{

	if (this->textureMaps[type] != nullptr)
		SAFE_RELEASE(textureMaps[type]);

	
	std::wstring widestr = std::wstring(path.begin(), path.end());
	const wchar_t* fileName = widestr.c_str(); 
	
	
	HRESULT hr = DirectX::CreateWICTextureFromFile(gDevice, fileName, nullptr, &textureMaps[type]);
	


	return false;
}

MaterialNode::MaterialNode()
{
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

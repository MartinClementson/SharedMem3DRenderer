#pragma once
#include "Structures.h"
#include "SharedMem3DRenderer.h"
#include "SharedMemHeaders.h"
#include "WICTextureLoader.h"
#include "BufferHandler.h"

class MaterialNode
{
#define MAX_TEXTURES 2
	
private:

	unsigned int usedTextures = 0;
	MaterialBuffer materialData;
	ID3D11Device *gDevice;
	ID3D11DeviceContext* gDeviceContext;
	std::string materialName;
	std::vector<TextureFile> textureFiles;
	Nodes::NodeType thisType = Nodes::MATERIAL;
	ID3D11ShaderResourceView *  textureMaps[MAX_TEXTURES] = { nullptr };
	
	bool isDirty = false;

	bool UpdateMaterial();
public:
	bool Init(ID3D11Device *gDevice, ID3D11DeviceContext* gDeviceContext);
	bool CreateFromMessage(MaterialMessage* material,TextureFile* textures = nullptr);
	bool AddTexture(std::string&  path, TextureTypes type);
	bool IsType(Nodes::NodeType type) { return (type == thisType ? true : false); };
	MaterialNode();
	std::string Name() { return this->materialName; };
	bool SetActive();
	virtual ~MaterialNode();

};


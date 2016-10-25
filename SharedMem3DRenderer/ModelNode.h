#pragma once
#include "TransformNode.h"
#include "Structures.h"
#include "BufferHandler.h"
#include "MaterialNode.h"
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
using namespace DirectX;
class ModelNode : public TransformNode
{
private:

	ID3D11Device *gDevice			    = nullptr;
	ID3D11DeviceContext* gDeviceContext = nullptr;
	ID3D11Buffer* vertexBuffer			= nullptr;
	ID3D11Buffer* indexBuffer			= nullptr;
	MaterialNode* materialRef		    = nullptr;
	UINT			vertCount   = 0;
	UINT			indexCount  = 0;
	
public:
	ModelNode();
	bool Init(ID3D11Device *gDevice, ID3D11DeviceContext* gDeviceContext, string id);
	void Render();
	void SetWorldMatrix(XMFLOAT4X4 &matrix);
	void SetWorldMatrix(XMMATRIX&matrix);

	void CreateVertexBuffer(Vertex* vertices, unsigned int amount);

	void CreateIndexBuffer(UINT * indices, unsigned int amount);

	void SetMaterial(MaterialNode* mat) { this->materialRef = mat;  };
	MaterialNode* GetMaterial()			{ return this->materialRef; };
	~ModelNode();
	
	///void UpdateModel();
};


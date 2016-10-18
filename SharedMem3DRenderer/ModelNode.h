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
	UINT			vertCount  = 0;
	UINT			indexCount = 0;

	ID3D11Buffer* vertexBuffer	= nullptr;
	ID3D11Buffer* indexBuffer	= nullptr;
	
	MaterialNode* materialRef = nullptr;
	
public:
	ModelNode();
	bool Init(ID3D11Device *gDevice, ID3D11DeviceContext* gDeviceContext);
	void Render();
	void SetWorldMatrix(XMFLOAT4X4 &matrix);
	void SetWorldMatrix(XMMATRIX&matrix);

	void CreateVertexBuffer(Vertex* vertices, unsigned int amount);

	void CreateIndexBuffer(UINT * indices, unsigned int amount);

	MaterialNode* GetMaterial() { return this->materialRef; };
	~ModelNode();
	
	///void UpdateModel();
};


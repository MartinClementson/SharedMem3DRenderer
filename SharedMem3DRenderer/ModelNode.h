#pragma once
#include "TransformNode.h"
#include "Structures.h"
#include "BufferHandler.h"
#include <memory>
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
	std::shared_ptr<Vertex> vertexData		 = nullptr;
	std::shared_ptr<UINT> indexData			 = nullptr;
	std::shared_ptr<LogicalIndex> logicalIDs = nullptr;
	bool newModelData = false;
	
public:
	ModelNode();
	bool Init(ID3D11Device *gDevice, ID3D11DeviceContext* gDeviceContext);
	void Render();
	void SetWorldMatrix(XMFLOAT4X4 &matrix);
	void SetWorldMatrix(XMMATRIX&matrix);
	void UpdateModelData(Vertex* newVertData, UINT newIndData, Float3* normalList);
	void UpdateAllModelData(Vertex* vertices, UINT* indices, UINT numVerts, UINT numIndices);
	void CreateVertexBuffer(Vertex* vertices, unsigned int amount);
	void CreateLogicalIDTracker();
	void Dirtify();
	void CreateIndexBuffer(UINT * indices, unsigned int amount);
	~ModelNode();
	
	///void UpdateModel();
};


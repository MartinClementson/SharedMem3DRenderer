#pragma once
#include "SharedMem3DRenderer.h"
#include "Structures.h"
class TransformNode
{
	
protected:
	std::string id;
	Float3 translation;
	Float3 rotation;
	WorldBuffer worldbuffer;
	
	Nodes::NodeType thisType = Nodes::NodeType::TRANSFORM;

	bool isDirty = false;
public:
	virtual void SetWorldMatrix(DirectX::XMFLOAT4X4 *newMatrix);
	virtual DirectX::XMFLOAT4X4* GetWorldMatrix() { return &this->worldbuffer.worldMatrix; };
	virtual bool Init(ID3D11Device *gDevice, ID3D11DeviceContext* gDeviceContext) { return true; };
	bool IsType(Nodes::NodeType type) { return (type == thisType ? true : false); };
	TransformNode();
	virtual ~TransformNode();
};


#include "stdafx.h"
#include "TransformNode.h"


void TransformNode::SetWorldMatrix(DirectX::XMFLOAT4X4* newMatrix)
{

	
	this->worldbuffer.worldMatrix = *newMatrix;
	isDirty = true;
}

TransformNode::TransformNode()
{
	XMStoreFloat4x4(&this->worldbuffer.worldMatrix, DirectX::XMMatrixIdentity());
}


TransformNode::~TransformNode()
{
}

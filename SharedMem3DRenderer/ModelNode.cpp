#include "stdafx.h"
#include "ModelNode.h"


void ModelNode::CreateVertexBuffer(Vertex * vertices, unsigned int amount)
{
	if (this->vertexBuffer != nullptr)
		SAFE_RELEASE(vertexBuffer);




	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Vertex)* amount;


	D3D11_SUBRESOURCE_DATA data;
	//Send the array of vertices in to pSysMem
	data.pSysMem = vertices;
	HRESULT hr;
	hr = gDevice->CreateBuffer(&bufferDesc, &data, &vertexBuffer);

	if (FAILED(hr))
		MessageBox(NULL, L"Error creating vertexbuffer", L"Error in mesh class", MB_ICONERROR | MB_OK);

	this->vertCount = (UINT)amount;




}


void ModelNode::CreateIndexBuffer(UINT * indices, unsigned int amount)
{

	if (this->indexBuffer != nullptr)
		SAFE_RELEASE(indexBuffer);



	D3D11_BUFFER_DESC ibd;

	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * amount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA ibdData;
	ibdData.pSysMem = indices;

	HRESULT hr;
	hr = gDevice->CreateBuffer(&ibd, &ibdData, &indexBuffer);

	if (FAILED(hr))
		MessageBox(NULL, L"Error creating indexbuffer", L"Error in mesh class", MB_ICONERROR | MB_OK);

	this->indexCount = (UINT)amount;

}


ModelNode::ModelNode(): TransformNode()
{
	thisType = Nodes::NodeType::MESH;
}



bool ModelNode::Init(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext)
{
	this->gDevice = gDevice;
	this->gDeviceContext = gDeviceContext;


	isDirty = true;

	return true;
}

void ModelNode::Render()
{
	if (isDirty)
	{
		XMVECTOR worldDet = XMMatrixDeterminant(XMLoadFloat4x4(&worldbuffer.worldMatrix));
		XMMATRIX normalWorld = XMMatrixInverse(&worldDet, XMLoadFloat4x4(&worldbuffer.worldMatrix));
		XMStoreFloat4x4(&worldbuffer.normalWorldMatrix, XMMatrixTranspose(normalWorld));


		isDirty = false;
	}

	assert(materialRef != nullptr);
	this->materialRef->SetActive();


	ID3D11Buffer* world = BufferHandler::GetInstance()->Buffers()->bWorldBuffer;
	D3D11_MAPPED_SUBRESOURCE mappedResourceWorld;
	ZeroMemory(&mappedResourceWorld, sizeof(mappedResourceWorld));

	this->gDeviceContext->Map(world, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResourceWorld);

	WorldBuffer* temporaryWorld = (WorldBuffer*)mappedResourceWorld.pData;

	*temporaryWorld = this->worldbuffer;

	this->gDeviceContext->Unmap(world, 0);

	UINT32 vertexSize = sizeof(Vertex);
	UINT32 offset = 0;
	this->gDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexSize, &offset);
	this->gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	this->gDeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	
	this->gDeviceContext->DrawIndexed(indexCount, 0, 0);
	
}

void ModelNode::SetWorldMatrix(XMFLOAT4X4 & matrix)
{

	this->worldbuffer.worldMatrix = matrix;
	isDirty = true;
}

void ModelNode::SetWorldMatrix(XMMATRIX & matrix)
{
	XMStoreFloat4x4(&worldbuffer.worldMatrix, matrix);
	isDirty = true;

}



ModelNode::~ModelNode()
{
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(vertexBuffer);
}				

#pragma once
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

namespace Nodes {

	enum NodeType {
		MESH,
		CAMERA,
		TRANSFORM,
		MATERIAL
	
	
	};
}
struct Float3
{
	float x;
	float y;
	float z;

	Float3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	Float3() {};

	Float3(double* pos)
	{
		this->x = float(pos[0]);
		this->y = float(pos[1]);
		this->z = float(pos[2]);

	}

	Float3& operator=(double* pos)
	{
		this->x = float(pos[0]);
		this->y = float(pos[1]);
		this->z = float(pos[2]);

		return *this;
	}
};

struct Float2
{
	float x;
	float y;

	Float2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
	Float2() {};

	Float2& operator=(double* pos)
	{
		this->x = float(pos[0]);
		this->y = float(pos[1]);

		return *this;
	}
};



struct Vertex
{

	Float3 position;
	Float3 normal;
	Float3 binormal;
	Float3 tangent;
	Float2 uv;
	float logicalIndex;


	Vertex(Float3 position, Float3 normal, Float3 binormal, Float3 tangent, Float2 uv)
	{
		this->position.x = position.x;
		this->position.y = position.y;
		this->position.z = position.z;

		this->normal.x = normal.x;
		this->normal.y = normal.y;
		this->normal.z = normal.z;

		this->binormal.x = binormal.x;
		this->binormal.y = binormal.y;
		this->binormal.z = binormal.z;

		this->tangent.x = tangent.x;
		this->tangent.y = tangent.y;
		this->tangent.z = tangent.z;

		this->uv.x = uv.x;
		this->uv.y = uv.y;

	}
	Vertex() {};
};



struct WorldBuffer
{
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 normalWorldMatrix; //for faceNormals transformation
};

struct CameraBuffer
{
	DirectX::XMFLOAT4X4 camView;
	DirectX::XMFLOAT4X4 projection;
	DirectX::XMFLOAT4X4 invViewProjMatrix;
	DirectX::XMFLOAT4 camPos;


};

struct MaterialBuffer
{
	DirectX::XMFLOAT4X4 PLACEHOLDER;

};

struct LightBuffer
{
	DirectX::XMFLOAT4X4 PLACEHOLDER;
};
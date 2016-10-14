#pragma once
#include "SharedMem3DRenderer.h"
#include <string>
#include "Structures.h"
using namespace std;




struct Matrix
{
	
	//Matrix( float m00, float m01, float m02, float m03,
	//		  float m10, float m11, float m12, float m13,
	//		  float m20, float m21, float m22, float m23,
	//		  float m30, float m31, float m32, float m33);

};
enum MessageType {
	MESH,
	VERTSEGMENT,
	VERTEX,
	CAMERA,
	TRANSFORM,
	MATERIAL,
	DELETION


};

struct MainMessageHeader
{
	MessageType messageType; 
		//messageiD?
	unsigned int msgSize;
};

struct MeshMessage
{
	char nodeName[256];
	unsigned int nameLength;
	float worldMatrix[16];
	unsigned int vertexCount;
	unsigned int indexCount;
};

struct VertSegmentMessage //for n verts, but not a whole mesh,
{
	char nodeName[256];
	unsigned int nameLength;
	unsigned int numVertices;
};

struct VertexMessage // for single verts.
{
	unsigned int indexId; // the id in the array. so we know which one to update
	Vertex vert;
};

struct TransformMessage
{
	char nodeName[256];
	unsigned int nameLength;
	float matrix[16];
};

struct CameraMessage
{
	char nodeName[256];
	unsigned int nameLength;
	float viewMatrix[16];
	float projMatrix[16];
};

struct DeleteMessage
{
	char nodeName[256];
	unsigned int nameLength;
};


#pragma once
#include "TransformNode.h"
#include "SharedMem3DRenderer.h"
#include "Structures.h"
#include "BufferHandler.h"
using namespace DirectX;
class Camera :
	public TransformNode
{

private:
	
	CameraBuffer cameraBuffer;
	ID3D11Device * gDevice;
	ID3D11DeviceContext * gDeviceContext;

	XMFLOAT4 camPosition	= { 0,  0,  5,   1 };
	//XMFLOAT4 camPosition    = { 0,  0,  0,   1 };
	XMFLOAT4 camTarget		= { 0,  0,  0,   1 };
	XMFLOAT4 camUp			= { 0,  1,  0,   0 };

	void UpdateCamBuffer(); 
public:
	void UpdateIfDirty();
	bool Init(ID3D11Device *gDevice, ID3D11DeviceContext* gDeviceContext, std::string id);
	bool UpdateViewAndProj(XMFLOAT4X4& view, XMFLOAT4X4& proj);
	void SetPosition(XMFLOAT3& newPos);
	void SetPosition(Float3 &newPos);
	void SetPosition(float& x, float& y, float& z);
	Camera();
	virtual ~Camera();
};


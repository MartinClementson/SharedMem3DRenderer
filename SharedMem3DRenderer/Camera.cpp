#include "stdafx.h"
#include "Camera.h"


void Camera::UpdateCamBuffer()
{

	//_____________________________________________________________________________________________
	//									Projection Matrix
	float fovangleY = XM_PI * 0.45f;
	float aspectRatio = float(WINDOW_WIDTH / WINDOW_HEIGHT);
	float farZ = 20000.0f;
	float nearZ = 0.0001f;
	
	
	//Create projection Matrix
	DirectX::XMMATRIX tempProj = XMMatrixPerspectiveFovLH(
		(fovangleY),
		(aspectRatio),
		(nearZ),
		(farZ)
		);
	
	XMMATRIX frustumProj = tempProj;
	//Transpose the Projcetion matrix
	tempProj = XMMatrixTranspose(tempProj);
	
	//Store The projection
	 XMStoreFloat4x4(&cameraBuffer.projection, tempProj);


	//_________________________________________________________________________________________
	//                                     VIEW MATRIX

	//Create the view matrix
	//XMMATRIX world = XMLoadFloat4x4(&worldbuffer.worldMatrix);
    //XMFLOAT4 tempPos = XMFLOAT4(worldbuffer.worldMatrix._14, worldbuffer.worldMatrix._24, worldbuffer.worldMatrix._34, 1.0f);
	//XMVECTOR pos = XMLoadFloat4(&tempPos);

//DirectX::XMMATRIX tempView = XMMatrixLookAtLH(
//	(XMLoadFloat4(&camPosition)),
//	(XMLoadFloat4(&camTarget)),
//	(XMLoadFloat4(&camUp))
//	);

	

	//XMStoreFloat4x4(&cameraBuffer.camView, world);
	
	this->cameraBuffer.camPos = camPosition;

	
	ID3D11Buffer* buffer = BufferHandler::GetInstance()->Buffers()->bCameraBuffer;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	gDeviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);


	CameraBuffer* tempStructMatrices = (CameraBuffer*)mappedResource.pData;
	*tempStructMatrices = this->cameraBuffer;



	gDeviceContext->Unmap(buffer, 0);


}

bool Camera::Init(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext)
{
	this->gDevice = gDevice;
	this->gDeviceContext = gDeviceContext;
	UpdateCamBuffer();
	return true;
}

bool Camera::UpdateViewAndProj(XMFLOAT4X4 & view, XMFLOAT4X4 & proj)
{

	cameraBuffer.camView    = view;
	//cameraBuffer.projection = proj;

	isDirty = true;
	
	return true;
}

void Camera::SetPosition(XMFLOAT3 & newPos)
{
	camPosition.x = newPos.x;
	camPosition.y = newPos.y;
	camPosition.z = newPos.z;
	camPosition.w = 1.0f;
	isDirty = true;
}

void Camera::SetPosition(Float3 & newPos)
{
	camPosition.x = newPos.x;
	camPosition.y = newPos.y;
	camPosition.z = newPos.z;
	camPosition.w = 1.0f;
	isDirty = true;
}

void Camera::SetPosition(float & x, float & y, float & z)
{
	camPosition.x = x;
	camPosition.y = y;
	camPosition.z = z;
	camPosition.w = 1.0f;
	isDirty = true;
}

void Camera::UpdateIfDirty()
{

	if (isDirty)
	{

		UpdateCamBuffer();

		isDirty = false;
	}

}

Camera::Camera() : TransformNode()
{
	thisType = Nodes::NodeType::CAMERA;

	worldbuffer.worldMatrix._14 = 0.0f;
	worldbuffer.worldMatrix._24 = 0.0f;
	worldbuffer.worldMatrix._34 = 5.0f;
}


Camera::~Camera()
{
}

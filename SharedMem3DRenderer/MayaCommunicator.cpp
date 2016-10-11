#include "stdafx.h"
#include "MayaCommunicator.h"


bool MayaCommunicator::Execute()
{
	
	while (running)
	{
		
		size_t length;
		if(this->circleBuffer.Pop(msg, length) == true) // if a message has been recieved
			msgHandler.TranslateMessage(msg, length);
	
	}

	return true;
}



MayaCommunicator::MayaCommunicator()
{

	msg = new char[20 * (1 << 20)];
	SharedMemory::CommandArgs arg;
	arg.memorySize = MEMORY_SIZE_IN_MB;
	arg.producer   = IS_PRODUCER;
	arg.msDelay    = MS_DELAY;


	this->circleBuffer.Init(arg,
		msgFileName, msgMutexName,
		infoFileName, infomutexName);
}


MayaCommunicator::~MayaCommunicator()
{
	delete msg;

}







bool MessageHandler::NewMesh(MeshMessage * msg)
{


	std::shared_ptr<Vertex> vertices = std::shared_ptr<Vertex>(new Vertex[msg->vertexCount]);

	unsigned int offset = sizeof(MeshMessage);
	memcpy(vertices.get(), (char*)msg + offset, sizeof(Vertex)*msg->vertexCount);

	offset += sizeof(Vertex)*msg->vertexCount;

	std::shared_ptr<UINT> indices = std::shared_ptr<UINT>(new UINT[msg->indexCount]);

	memcpy(indices.get(), (char*)msg + offset, sizeof(UINT)* msg->indexCount);


	DirectX::XMMATRIX mToggle_XZ = DirectX::XMMATRIX(
		-1, 0, 0, 0,
		0, -1, 0, 0,
		0, 0, -1, 0,
		0, 0, 0, 1);

	DirectX::XMMATRIX world = DirectX::XMMATRIX(msg->worldMatrix);




	world = XMMatrixMultiply(world, mToggle_XZ);
	world = XMMatrixTranspose(world);



	DirectX::XMFLOAT4X4 matrixToSend;

	DirectX::XMStoreFloat4x4(&matrixToSend, world);

	ResourceManager::GetInstance()->AddNewMesh(msg->nodeName, vertices.get(), msg->vertexCount, indices.get(), msg->indexCount, &matrixToSend);

	return true;
}

bool MessageHandler::Transform(TransformMessage * msg)
{
//DirectX::XMMATRIX mToggle_XZ = DirectX::XMMATRIX(
//	-1, 0, 0,  0,
//	0,  1, 0, 0,
//	0, 0, -1, 0,
//	0, 0, 0, 1);


	
	DirectX::XMMATRIX mToggle_XZ = DirectX::XMMATRIX(
		-1, 0, 0, 0,
		 0, -1, 0, 0,
		 0, 0, -1, 0,
		 0, 0, 0, 1);
	
	
	

	std::map < string, TransformNode*> *sceneTransforms;

	sceneTransforms = &ResourceManager::GetInstance()->sceneTransforms;

	if (sceneTransforms->find(msg->nodeName) != sceneTransforms->end())
	{
		
		//the model exists
		
		DirectX::XMMATRIX world = DirectX::XMMATRIX(msg->matrix);

		
		//world.r[3].m128_f32[0] *= -1;
		//world.r[3].m128_f32[1] *= -1;
		//world.r[3].m128_f32[2] *= -1;
		//float temp = world.r[3].m128_f32[0];
		//world.r[3].m128_f32[0] *= world.r[3].m128_f32[2];
		//world.r[3].m128_f32[2] *= temp;

		
		//world.r[3].m128_f32[1] *= -1;

		world = XMMatrixMultiply( world, mToggle_XZ);
 		world = XMMatrixTranspose(world);
		


		DirectX::XMFLOAT4X4 matrixToSend;

		DirectX::XMStoreFloat4x4(&matrixToSend, world);

		sceneTransforms->at(msg->nodeName)->SetWorldMatrix(&matrixToSend);
		//dynamic_cast<ModelNode*>(sceneTransforms->at(msg->nodeName))->SetWorldMatrix(matrixToSend);
	}
	else
		return false;

	return true;
}

bool MessageHandler::UpdateCamera(CameraMessage * msg)
{

	DirectX::XMMATRIX mToggle_XZ = DirectX::XMMATRIX(
		-1, 0, 0, 0,
		0, -1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);




	std::map < string, TransformNode*> *sceneTransforms;

	sceneTransforms = &ResourceManager::GetInstance()->sceneTransforms;

	if (sceneTransforms->find(msg->nodeName) != sceneTransforms->end())
	{

		//the model exists

		DirectX::XMMATRIX world = DirectX::XMMATRIX(msg->viewMatrix);
		world.r[3].m128_f32[0] *= -1;
		world.r[3].m128_f32[1] *= -1;
		world.r[3].m128_f32[2] *= -1;
		world = XMMatrixMultiply(world, mToggle_XZ);
		world = XMMatrixTranspose(world);



		DirectX::XMFLOAT4X4 matrixToSend;

		DirectX::XMStoreFloat4x4(&matrixToSend, world);

		sceneTransforms->at(msg->nodeName)->SetWorldMatrix(&matrixToSend);
		//dynamic_cast<ModelNode*>(sceneTransforms->at(msg->nodeName))->SetWorldMatrix(matrixToSend);
	}
	else
		return false;

	return true;
}

MessageHandler::MessageHandler()
{
}

MessageHandler::~MessageHandler()
{

}

bool MessageHandler::TranslateMessage(char * msg, size_t length)
{
	MainMessageHeader* mainHead = (MainMessageHeader*)msg;
	

	switch (mainHead->messageType)
	{
	case MESH:
	{

		MeshMessage * meshHeader = (MeshMessage*)(msg + sizeof(MainMessageHeader));
		NewMesh(meshHeader);
		break;
	}
	case VERTSEGMENT:
		break;
	case VERTEX:
		break;
	case CAMERA:
	{

		CameraMessage * cameraMessage = (CameraMessage*)(msg + sizeof(MainMessageHeader));
		UpdateCamera(cameraMessage);
		break;
	}
	case TRANSFORM:
	{
		TransformMessage * transFormHeader = (TransformMessage*)(msg + sizeof(MainMessageHeader));
		Transform(transFormHeader);
	}
		break;
	case MATERIAL:
		break;
	}




	return false;
}

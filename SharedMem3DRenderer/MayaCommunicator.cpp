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

	//The memory allocated here goes into the model node on the main thread
	std::shared_ptr<Vertex> vertices = std::shared_ptr<Vertex>(new Vertex[msg->vertexCount]);
	std::shared_ptr<UINT>   indices  = std::shared_ptr<UINT>(new UINT[msg->indexCount]);

	unsigned int offset = sizeof(MeshMessage);
	memcpy(vertices.get(), (char*)msg + offset, sizeof(Vertex)*msg->vertexCount);

	offset += sizeof(Vertex)*msg->vertexCount;


	memcpy(indices.get(), (char*)msg + offset, sizeof(UINT)* msg->indexCount);


	

	DirectX::XMMATRIX world = DirectX::XMMATRIX(msg->worldMatrix);


	DirectX::XMFLOAT4X4 matrixToSend = OpenGLMatrixToDirectX(world);


	ResourceManager::GetInstance()->AddNewMesh(msg->nodeName, 
		vertices.get(),
		msg->vertexCount, 
		indices.get(), 
		msg->indexCount, 
		&matrixToSend,
		msg->materialName);

	return true;
}

bool MessageHandler::NewVertexSegment(VertSegmentMessage * msg)
{
	std::map < string, TransformNode*> *sceneTransforms;

	sceneTransforms = &ResourceManager::GetInstance()->sceneTransforms;

	if (sceneTransforms->find(msg->nodeName) != sceneTransforms->end())
	{
		if (sceneTransforms->at(msg->nodeName)->IsType(Nodes::NodeType::MESH))
		{
			ModelNode* test = (ModelNode*)sceneTransforms->at(msg->nodeName);

			unsigned int offset = sizeof(VertSegmentMessage);

			Float3* normalptr = (Float3*)((char*)msg + offset);
			offset += sizeof(Float3) * msg->numNormals;

			int* IDptr = (int*)((char*)msg + offset);
			offset += sizeof(int) * msg->numNormals;

			for (size_t i = 0; i < msg->numVertices; i++)
			{
				VertexMessage * vertMessage = (VertexMessage*)((char*)msg + offset);
				Vertex VertData = vertMessage->vert;
				UINT IndData = vertMessage->indexId;

				test->UpdateModelData(&VertData, IndData, normalptr, IDptr, msg->numNormals);
				offset += sizeof(VertexMessage);	
			}
			test->Dirtify();

		}
	}
	else
		return false;

	return true;
}

XMFLOAT4X4 MessageHandler::OpenGLMatrixToDirectX(XMMATRIX & glMatrix)
{
	DirectX::XMMATRIX mToggle_XZ = DirectX::XMMATRIX(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	//glMatrix = XMMatrixMultiply(glMatrix, mToggle_XZ);
	glMatrix = XMMatrixTranspose(glMatrix);

	XMFLOAT4X4 toReturn;

	XMStoreFloat4x4(&toReturn, glMatrix);
	return toReturn;
}

bool MessageHandler::Transform(TransformMessage * msg)
{

	std::map < string, TransformNode*> *sceneTransforms;

	sceneTransforms = &ResourceManager::GetInstance()->sceneTransforms;

	if (sceneTransforms->find(msg->nodeName) != sceneTransforms->end())
	{
		
		//the model exists
	
		DirectX::XMMATRIX world = DirectX::XMMATRIX(msg->matrix);

		DirectX::XMFLOAT4X4 matrixToSend = OpenGLMatrixToDirectX(world);

		sceneTransforms->at(msg->nodeName)->SetWorldMatrix(&matrixToSend);	
	}
	else
		return false;

	return true;
}

bool MessageHandler::UpdateCamera(CameraMessage * msg)
{
	std::map < string, TransformNode*> *sceneTransforms;

	sceneTransforms = &ResourceManager::GetInstance()->sceneTransforms;

	

			//the camera exists
			DirectX::XMMATRIX mToggle_XZ = DirectX::XMMATRIX(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, -1, 0,
				0, 0, 0, 1);



			DirectX::XMMATRIX view = DirectX::XMMATRIX(msg->viewMatrix);
			view.r[0].m128_f32[2] *= -1;
			view.r[1].m128_f32[2] *= -1;
			view.r[2].m128_f32[2] *= -1;
			view.r[3].m128_f32[2] *= -1;
			//view = XMMatrixMultiply(view, mToggle_XZ);
			view = XMMatrixTranspose(view);

			DirectX::XMFLOAT4X4 viewToSend;

			DirectX::XMStoreFloat4x4(&viewToSend, view);





			///////////////
			//Projection

			//DirectX::XMMATRIX projMult = DirectX::XMMATRIX(
			//	1, 0, 0, 0,
			//	0, 1, 0, 0,
			//	0, 0, 1, 0,
			//	0, 0, 0, 1);
			DirectX::XMMATRIX proj = DirectX::XMMATRIX(msg->projMatrix);
			////proj.r[2].m128_f32[0] *= -1;
			////proj.r[2].m128_f32[1] *= -1;
			////proj.r[2].m128_f32[3] *= -1;
			proj = XMMatrixTranspose(proj);
			//proj.r[2].m128_f32[2] *= -1;
			////proj = XMMatrixMultiply(proj, projMult);
			//
 			DirectX::XMFLOAT4X4 projToSend;

			DirectX::XMStoreFloat4x4(&projToSend, proj);

			//Create projection Matrix
		//DirectX::XMMATRIX tempProj = XMMatrixPerspectiveFovLH(
		//	(fovangleY),
		//	(WINDOW_WIDTH/WINDOW_HEIGHT),
		//	(nearZ),
		//	(farZ)
		//	);
		//
		//XMMATRIX frustumProj = tempProj;
		////Transpose the Projcetion matrix
		//tempProj = XMMatrixTranspose(tempProj); 
		//
		////Store The projection
		// XMStoreFloat4x4(&projToSend, tempProj);



			Float3* pos = &((CameraMessage*)msg)->camPos;
			

			((Camera*)sceneTransforms->at("persp"))->UpdateViewAndProj(viewToSend, projToSend,msg->camPos);
		
	
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
		{
			VertSegmentMessage * vertSegmentMessage = (VertSegmentMessage*)(msg + sizeof(MainMessageHeader));
			VertexMessage * vertMessage = (VertexMessage*)(msg + sizeof(MainMessageHeader) + sizeof(VertSegmentMessage));
			NewVertexSegment(vertSegmentMessage);
			break;
		}
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
			break;
		}
		case MATERIAL:
		{
			std::map < string, MaterialNode*> *sceneMaterials;
			sceneMaterials = &ResourceManager::GetInstance()->sceneMaterials;
			MaterialMessage * matMessage = (MaterialMessage*)(msg + sizeof(MainMessageHeader));

			TextureFile* textures = (TextureFile*)(msg + sizeof(MainMessageHeader) + sizeof(MaterialMessage));
			//This will also handle updating of already existing materials
			ResourceManager::GetInstance()->AddNewMaterial(matMessage, textures); 
			break;
		}
	
		case DELETION:
		{
			DeleteMessage* deleteHeader = (DeleteMessage*)(msg + sizeof(MainMessageHeader));
			string name(deleteHeader->nodeName);
			ResourceManager::GetInstance()->DeleteNode(name);
		}
	}




	return false;
}

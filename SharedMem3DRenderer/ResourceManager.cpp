#include "stdafx.h"
#include "ResourceManager.h"


bool ResourceManager::UpdateIfDirty()
{

	//if (deletionQueue.size() > 0)
	//{
	//	int size = deletionQueue.size();
	//	for (size_t i = 0; i < size; i++)
	//	{
	//		
	//	}
	//}
	if (isDirty)
	{

		//models.push_back(tempNewModel);
		//tempNewModel = nullptr;
		isDirty = false;
		return true;
	}
	return false;
}

bool ResourceManager::Init(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext)
{
	this->gDevice        = gDevice;
	this->gDeviceContext = gDeviceContext;
	this->testModel		 = new ModelNode();
	this->gMutex		 =  std::unique_ptr<SharedMemory::SharedMutex>( new SharedMemory::SharedMutex(mutexName));
	this->camera         = new Camera();
	
	testModel->Init(gDevice, gDeviceContext);
	if (!camera->Init(gDevice, gDeviceContext))
		return false;


	Vertex cubeVerts[8];

	cubeVerts[0].position = Float3(-0.5, 2.5,  0.5);		//0
	cubeVerts[1].position = Float3(-0.5, 0.0,  0.5);		//1
	cubeVerts[2].position = Float3(0.5, 0.0,   0.5);		//2
	cubeVerts[3].position = Float3(0.5, 2.5,   0.5);		//3
	cubeVerts[4].position = Float3(0.5, 0.0,  -0.5);		//4
	cubeVerts[5].position = Float3(0.5, 2.5,  -0.5);		//5
	cubeVerts[6].position = Float3(-0.5, 0.0, -0.5);		//6
	cubeVerts[7].position = Float3(-0.5, 2.5, -0.5);		//7


	cubeVerts[0].normal = Float3(-0.5, 2.5, 0.5);		//0
	cubeVerts[1].normal = Float3(-0.5, 0.0, 0.5);		//1
	cubeVerts[2].normal = Float3(0.5, 0.0, 0.5);		//2
	cubeVerts[3].normal = Float3(0.5, 2.5, 0.5);		//3
	cubeVerts[4].normal = Float3(0.5, 0.0, -0.5);		//4
	cubeVerts[5].normal = Float3(0.5, 2.5, -0.5);		//5
	cubeVerts[6].normal = Float3(-0.5, 0.0, -0.5);		//6
	cubeVerts[7].normal = Float3(-0.5, 2.5, -0.5);		//7

	UINT indices[36] =
	{
		0,1,2,
		0,2,3,
		3,2,4,
		3,4,5,
		5,4,6,
		5,6,7,
		7,6,1,
		7,1,0,
		0,3,5,
		0,5,7,
		1,4,2,
		1,6,4 };

	this->testModel->CreateVertexBuffer(cubeVerts, 8);
	this->testModel->CreateIndexBuffer(indices, 36);

	this->sceneTransforms["pCube1aa"] = testModel;
	this->sceneTransforms["persp"]  = camera;



	return true;
}

void ResourceManager::AddNewMesh(string name, Vertex * verts, UINT numVerts, UINT * indices, UINT numIndices, XMFLOAT4X4 * worldMatrix)
{
	while (isDirty) // make sure the rendering thread is up to date before changing
	{
		Sleep(10);
	}
	if (sceneTransforms.find(name) == sceneTransforms.end())
	{
		 //only add if it doesent already exist
		ModelNode * tempModel = new ModelNode();
		tempModel->Init(gDevice, gDeviceContext);
	
		tempModel->CreateVertexBuffer(verts, numVerts);
		tempModel->CreateIndexBuffer(indices, numIndices);
		tempModel->SetWorldMatrix(*worldMatrix);
	
	//	tempNewModel = tempModel;
		gMutex->Lock();
		sceneTransforms[name] = tempModel;
		gMutex->Unlock();
		isDirty = true;
	}
}

ResourceManager::ResourceManager()
{
	
}


bool ResourceManager::RenderModels()
{
	gMutex->Lock();
	for (auto iterator = sceneTransforms.begin(); iterator != sceneTransforms.end(); ++iterator)
	{
		TransformNode * node = iterator->second;
		if (node->IsType(Nodes::NodeType::MESH))
			((ModelNode*)node)->Render();
	}
	gMutex->Unlock();
	return true;
}

bool ResourceManager::DeleteNode(string name)
{
	gMutex->Lock();
	if (sceneTransforms.find(name) != sceneTransforms.end())
	{
		TransformNode * node = sceneTransforms.at(name);
		delete node;
		sceneTransforms.erase(name);
		gMutex->Unlock();
		return true;
	}
	gMutex->Unlock();
	return false;
}

bool ResourceManager::DeleteNode(char * name)
{
	return false;
}

ResourceManager::~ResourceManager()
{
	//delete testModel;
	
	for (auto iterator = sceneTransforms.begin(); iterator != sceneTransforms.end(); ++iterator) 
	{
		TransformNode * node = iterator->second;
			delete node;
	}
	sceneTransforms.clear();


	
}

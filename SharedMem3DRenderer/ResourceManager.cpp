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
	
	testModel->Init(gDevice, gDeviceContext,"testModel");
	if (!camera->Init(gDevice, gDeviceContext,"persp"))
		return false;


	

	this->sceneTransforms["persp"]  = camera;

	this->testMaterial = new MaterialNode();
	testMaterial->Init(gDevice, gDeviceContext);
	this->sceneMaterials["standard"] = testMaterial;

	return true;
}

void ResourceManager::AddNewMesh(string name, 
	Vertex * verts, 
	UINT numVerts,
	UINT * indices, 
	UINT numIndices,
	XMFLOAT4X4 * worldMatrix,
	char* materialName)
{
	while (isDirty) // make sure the rendering thread is up to date before changing
	{
		Sleep(10);
	}
	if (sceneTransforms.find(name) == sceneTransforms.end())
	{
		 //only add if it doesent already exist
		ModelNode * tempModel = new ModelNode(); //memory freed in destructor
		if (!tempModel->Init(gDevice, gDeviceContext,string(name)))
		{
			delete tempModel;
			return;
		}
	
		tempModel->CreateVertexBuffer(verts, numVerts);
		tempModel->CreateIndexBuffer(indices, numIndices);
		tempModel->SetWorldMatrix(*worldMatrix);
		if (strlen(materialName) != 0)
		{

			if (sceneMaterials.find(string(materialName)) == sceneMaterials.end())
			{ // if the material doesent exist, create a new one
				MaterialNode* newMat = new MaterialNode(materialName); //deleted in destructor
				newMat->Init(gDevice, gDeviceContext);
				gMutex->Lock();
				sceneMaterials[string(materialName)] = newMat;
				gMutex->Unlock();
				tempModel->SetMaterial(newMat);
			}
			else
				tempModel->SetMaterial(sceneMaterials[string(materialName)]);
		}
		else
			tempModel->SetMaterial(sceneMaterials["standard"]);

		gMutex->Lock();
		sceneTransforms[name] = tempModel;
		gMutex->Unlock();

		isDirty = true;
	}
	else
	{
		ModelNode* tempModel = (ModelNode*)sceneTransforms[name];
		gMutex->Lock();
		tempModel->UpdateAllModelData(verts, indices, numVerts, numIndices);
		gMutex->Unlock();

		
		
		if (sceneMaterials.find(string(materialName)) == sceneMaterials.end())
		{ // if the material doesent exist, create a new one
			MaterialNode* newMat = new MaterialNode(materialName); //deleted in destructor
			newMat->Init(gDevice, gDeviceContext);
			gMutex->Lock();
			sceneMaterials[string(materialName)] = newMat;
			((ModelNode*)sceneTransforms[name])->SetMaterial(newMat);
			gMutex->Unlock();
		}
		else
		{
			gMutex->Lock();
			((ModelNode*)sceneTransforms[name])->SetMaterial(sceneMaterials[string(materialName)]);
			gMutex->Unlock();

		}
			

		isDirty = true;
	}
}

ResourceManager::ResourceManager()
{
	
}


void ResourceManager::AddNewMaterial(MaterialMessage * mat, TextureFile * textures)
{

	if (sceneMaterials.find(string(mat->matName)) == sceneMaterials.end())
	{ //If it doesent exist already

		MaterialNode* newMaterial = new MaterialNode(); // memory freed in destructor
		newMaterial->Init(gDevice, gDeviceContext);

		newMaterial->CreateFromMessage(mat, textures);

		this->sceneMaterials[string(mat->matName)] = newMaterial;
	}
	else //it already exists
	{

		sceneMaterials[string(mat->matName)]->CreateFromMessage(mat, textures);
	}

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
	while(!gMutex->Lock());
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
	
	for (auto iterator = sceneTransforms.begin(); iterator != sceneTransforms.end(); ++iterator) 
	{
		TransformNode * node = iterator->second;
			delete node;
	}
	sceneTransforms.clear();



	for (auto iterator = sceneMaterials.begin(); iterator != sceneMaterials.end(); ++iterator)
	{
		MaterialNode * node = iterator->second;
		delete node;
	}
	sceneMaterials.clear();
	
}

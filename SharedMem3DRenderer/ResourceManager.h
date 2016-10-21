#pragma once

#include "SharedMem3DRenderer.h"
#include "ModelNode.h"
#include "SharedMutex.h"
#include <vector>
#include <map>
#include "Camera.h"
#include <queue>
#include <memory>
using namespace std;
class ResourceManager
{
private:
	LPCWSTR mutexName = (LPCWSTR)TEXT("resMutex");
	std::unique_ptr<SharedMemory::SharedMutex> gMutex; //for safe deletion between threads.

	bool isDirty						= false;
	ID3D11Device *gDevice				= nullptr;
	ID3D11DeviceContext* gDeviceContext = nullptr;
	vector<ModelNode*> models;
	ModelNode* tempNewModel; //keep the new model here and let the main thread get it into the pipeline
	ResourceManager();
	std::queue<TransformNode*> deletionQueue;
public:
	Camera* camera;
	bool UpdateIfDirty();
	bool Init(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext);
	std::map < string, TransformNode*> sceneTransforms;
	std::map < string, MaterialNode*> sceneMaterials;
	ModelNode* testModel; //temp
	void AddNewMesh(string name, Vertex* verts, UINT numVerts, UINT* indices, UINT numIndices, XMFLOAT4X4* worldMatrix);
	vector<ModelNode*>* Models() { return &this->models; };

	MaterialNode* testMaterial;
	
	static ResourceManager* GetInstance() {
		static ResourceManager instance;
		return &instance;
	}

	void AddNewMaterial(MaterialMessage* mat, TextureFile* textures);

	bool RenderModels();
	bool DeleteNode(string name);
	bool DeleteNode(char* name);
	~ResourceManager();
};


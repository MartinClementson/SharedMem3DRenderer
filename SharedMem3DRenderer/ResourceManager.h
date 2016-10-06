#pragma once

#include "SharedMem3DRenderer.h"
#include "ModelNode.h"
#include <vector>
#include <map>
#include "Camera.h"
#include <memory>
using namespace std;
class ResourceManager
{
private:
	bool isDirty = false;
	ID3D11Device *gDevice				= nullptr;
	ID3D11DeviceContext* gDeviceContext = nullptr;
	vector<ModelNode*> models;
	ModelNode* tempNewModel; //keep the new model here and let the main thread get it into the pipeline
	ResourceManager();
public:
	Camera camera;
	bool UpdateIfDirty();
	bool Init(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext);
	std::map < string, TransformNode*> sceneTransforms;
	ModelNode* testModel; //temp
	void AddNewMesh(string name, Vertex* verts, UINT numVerts, UINT* indices, UINT numIndices, XMFLOAT4X4* worldMatrix);
	vector<ModelNode*>* Models() { return &this->models; };
	static ResourceManager* GetInstance() {
		static ResourceManager instance;
		return &instance;
	}
	~ResourceManager();
};


#pragma once
#include "SharedMem3DRenderer.h"
#include "ShaderManager.h"
#include "Camera.h"
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#include "ResourceManager.h"
class Graphics
{
private :
	ShaderManager shaderManager;
	ResourceManager* resManager;
	Camera camera;
public:
	Graphics();
	~Graphics();
	bool Init(ID3D11Device *gDevice, ID3D11DeviceContext* gDeviceContext);

	void Render();
};


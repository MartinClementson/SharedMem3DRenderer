#pragma once
#include "SharedMem3DRenderer.h"
#include "Structures.h"
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

class BufferHandler
{
	struct Buffers
	{
		ID3D11Buffer* bWorldBuffer    = nullptr;
		ID3D11Buffer* bMaterialBuffer = nullptr;
		ID3D11Buffer* bLightBuffer    = nullptr;
		ID3D11Buffer* bCameraBuffer   = nullptr;

		~Buffers()
		{
			SAFE_RELEASE(bCameraBuffer);
			SAFE_RELEASE(bWorldBuffer);
			SAFE_RELEASE(bMaterialBuffer);
			SAFE_RELEASE(bLightBuffer);
		}
	};
private:

	ID3D11Device * gDevice;
	ID3D11DeviceContext * gDeviceContext;

	Buffers buffers;

	BufferHandler();
public:
	static BufferHandler* GetInstance() {
		static BufferHandler instance;
		return &instance;
	
	}
	bool Init(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext);
	Buffers* Buffers() { return &this->buffers; };
	~BufferHandler();
};


#pragma once
#include "SharedMem3DRenderer.h"
#include "Structures.h"
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

class BufferHandler
{
	struct Buffers
	{
		struct BufferData
		{
			
			ID3D11Buffer* gD3Dbuffer = nullptr;
			std::string gActiveInstanceName;
			BufferData()
			{
				gActiveInstanceName = "None";
			}
			template <typename T>
			bool UpdateBuffer(T* data, std::string* bufferName)
			{
				
				//if (*bufferName != gActiveInstanceName)
				//{

					D3D11_MAPPED_SUBRESOURCE mappedResource;
					ZeroMemory(&mappedResource, sizeof(mappedResource));

					BufferHandler::GetInstance()->GetContext()->Map(gD3Dbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

					T* tempBufferData = (T*)mappedResource.pData;

					*tempBufferData = *data;

					BufferHandler::GetInstance()->GetContext()->Unmap(gD3Dbuffer, 0);
					//gActiveInstanceName = *bufferName;
				
				//}
				return true;
			}


		};

		BufferData bWorldBuffer   ;
		BufferData bMaterialBuffer;
		BufferData bLightBuffer   ;
		BufferData bCameraBuffer  ;

		
		Buffers()
		{
		}

		~Buffers()
		{
			SAFE_RELEASE(bCameraBuffer	.gD3Dbuffer);
			SAFE_RELEASE(bWorldBuffer	.gD3Dbuffer);
			SAFE_RELEASE(bMaterialBuffer.gD3Dbuffer);
			SAFE_RELEASE(bLightBuffer	.gD3Dbuffer);
		}

		
	};
private:

	ID3D11Device * gDevice;
	ID3D11DeviceContext * gDeviceContext;

	Buffers buffers;

	BufferHandler() {  };
public:
	static BufferHandler* GetInstance() {
		static BufferHandler instance;
		return &instance;
	
	}
	 ID3D11DeviceContext* GetContext() { return this->gDeviceContext; };
	bool Init(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext);
	Buffers* Buffers() { return &this->buffers; };
	~BufferHandler();
};


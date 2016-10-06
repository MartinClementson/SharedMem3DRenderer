#pragma once
#include "SharedMutex.h"
#include <Windows.h>
#include <memory>
#include <string>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
using namespace std;

namespace SharedMemory
{


	struct CommandArgs
	{
		bool         producer    = false;
		double       msDelay     = 0;
		unsigned int memorySize  = 0;
	};

	

		struct SharedInformation
		{
			size_t head = 0;	//head offset
			size_t tail = 0;	//tail offset
			size_t freeMem = 0;
			size_t clients = 0;
		};

		struct MesssageHeader
		{
			int consumerQueue; // it helps that this can be negative. So that consumers can join mid-session without problem
			size_t msgId;
			size_t length;

			void Flush()
			{
				consumerQueue = 0;
				msgId = 0;
				length = 0;
			}

		};

		struct SharedMessage
		{
			MesssageHeader header;
			char* message = nullptr;
			SharedMessage() {};
			~SharedMessage()
			{
				if (message != nullptr)
					delete message;

			}
			void Flush()
			{
				//memset(this->message, '0', header.length);
				header.Flush();

			}
		};
	



	struct InitReturnValues
	{
		bool firstTimeInit = false;
		bool succeded = false;


	};

#pragma region SharedMemStruct
		struct SharedMemoryStruct {
			size_t fileSize = 0;
			HANDLE	hFileMap = NULL;
			LPCTSTR vFileView = NULL;
			SharedMemoryStruct() {};

			InitReturnValues Init(CommandArgs* info, LPCWSTR bufferName)
			{
				InitReturnValues toReturn;

				this->fileSize = size_t(info->memorySize);
				//try opening it first
				this->hFileMap = OpenFileMapping(
					FILE_MAP_ALL_ACCESS,
					FALSE,
					bufferName);
				if (hFileMap == NULL) //if it didnt open, create it
				{
					this->hFileMap = CreateFileMapping(
						INVALID_HANDLE_VALUE,	//Instead of a file in the system, we use a system paging file
						NULL,					//No extra attributes (default)
						PAGE_READWRITE,			//specifies the protection, all the views to the file need to på compatible with this!
						0,
						info->memorySize,	 // unsigned int
						bufferName
						);

					if (hFileMap == NULL) //if it still doesent work, throw error
					{

						MessageBox(GetConsoleWindow(), TEXT("Error creating fileMap"), TEXT("SharedMemoryStruct"), MB_OK);
						return toReturn;
					}
					else
						toReturn.firstTimeInit = true;
				}

				vFileView = (LPTSTR)MapViewOfFile(hFileMap, //Create the view
					FILE_MAP_ALL_ACCESS,
					0,
					0,
					info->memorySize);
				if (vFileView == NULL)
				{
					MessageBox(GetConsoleWindow(), TEXT("Error when mapping file view"), TEXT("SharedMemoryStruct"), MB_OK);
					return toReturn;
				}

				toReturn.succeded = true;
				return toReturn;

			}

			~SharedMemoryStruct() {
				if (hFileMap != NULL)
					CloseHandle(hFileMap);
				if (vFileView != NULL)
					UnmapViewOfFile(vFileView);
			}
		};
	

#pragma endregion



	class CircleBuffer
	{
	private:
		size_t*  shared_head		= 0;
		size_t*  shared_tail		= 0;
		size_t* freeMem;

		size_t  local_tail			= 0;
		size_t	messagesSent		= 0; // using this as an id system would need rework if it was to support multiple producers
		size_t	messagesRecieved	= 0;
		size_t chunkSize			= 256;

		shared_ptr<SharedMemory::SharedMemoryStruct> _MessageMem;
		shared_ptr<SharedMemory::SharedMemoryStruct> _InfoMem;

		std::unique_ptr<SharedMemory::SharedMutex> msgMutex;  // mutex											 
		std::unique_ptr<SharedMemory::SharedMutex> infoMutex; // mutex
		
		size_t CalculatePadding(size_t msgSize, size_t chunkSize);
	
	public:
	bool Push( void* msg, size_t length);
	bool Pop (char* msg, size_t& length);
	
		CircleBuffer();
		bool Init(SharedMemory::CommandArgs& info,
			LPCWSTR msgBufferName,
			LPCWSTR msgMutexName,
			LPCWSTR infoBufferName,
			LPCWSTR infoMutexName);
		void SetChunkSize(size_t size) { this->chunkSize = size; };
		shared_ptr<SharedMemory::SharedMemoryStruct> GetMessageBuffer() { return _MessageMem; };
		shared_ptr<SharedMemory::SharedMemoryStruct> GetInfoBuffer()	{ return _InfoMem;	  };
	

		virtual ~CircleBuffer();
	};
}


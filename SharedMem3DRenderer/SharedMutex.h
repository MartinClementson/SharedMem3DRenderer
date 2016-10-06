#pragma once
#include <windows.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
namespace SharedMemory
{

	class SharedMutex
	{
	private:
		HANDLE mutexHandle;			 // mutex Handle
		SharedMutex();
	public:
		SharedMutex(LPCWSTR mutexName);
		bool Lock(DWORD waitTime = INFINITE);
		void Unlock();
		virtual ~SharedMutex();
	};
}


#pragma once
#include "CircleBuffer.h"
#include "SharedMemHeaders.h"
#include "ResourceManager.h"
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define IS_PRODUCER false
#define MS_DELAY    0
#define MEMORY_SIZE_IN_MB  100 ;
/*
	This class should be in a separate thread if possible.

	The purpose of this is to check for messages from maya.

	There are two alternatives here.

	1) handle the messages here and communicate with the classes on the other thread. (update stuff)

	2) Create a new class "MessageHandler" in this thread that handles the data  accordingly.
		or better, make another thread for it.! it might be overkill though
*/


class MessageHandler
{
private:
	bool NewMesh			(MeshMessage* msg);
	bool NewVertexSegment	(VertSegmentMessage* msg);
	bool Transform			(TransformMessage* msg);
	bool UpdateCamera		(CameraMessage * msg);
	XMFLOAT4X4 OpenGLMatrixToDirectX(XMMATRIX & glMatrix);
public:
	MessageHandler();
	~MessageHandler();
	bool TranslateMessage(char* msg, size_t length);

	

};

// I keep them in two different classes, because of the potential to
// create a local circlebuffer and run them in different threads.

class MayaCommunicator
{
	
private:
	LPCWSTR msgFileName		= (LPCWSTR)TEXT("sharedMsgFile");		  // the file to be manipulated
	LPCWSTR msgMutexName	= (LPCWSTR)TEXT("sharedMsgMutex");		  // the the mutex to be used when synchronizing data							
	LPCWSTR infoFileName    = (LPCWSTR)TEXT("sharedInfoFile");		  // the file to be manipulated
	LPCWSTR infomutexName   = (LPCWSTR)TEXT("sharedInfoMutex");	  // the the mutex to be used when synchronizing data											
  //LPCWSTR writeEventName  = (LPCWSTR)TEXT("writeEvent");		  // the event handle to the writing event
private:
	char* msg;
	SharedMemory::CircleBuffer circleBuffer;
	MessageHandler msgHandler;
	bool running = true;
public:
	bool Execute();
	void Stop() { running = false; };

	MayaCommunicator();
	~MayaCommunicator();
};


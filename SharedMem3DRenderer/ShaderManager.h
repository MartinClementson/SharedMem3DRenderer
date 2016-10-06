#pragma once
#pragma once
#include "SharedMem3DRenderer.h"

enum ShaderTypes
{
	TEXTURESHADER,
	COLORSHADER,
};

#pragma region Description
/*
This class provides a way to easily handle all the shaders used in the progam.
If there are more shaders created during the project, just add them to this class.
(But don't forget to release!)

It is pretty straight forward.
All shaders are compiled when this class is initiated using init()

To specify what shader is to be used call setActiveShaders() before the render call
The parameter is a Enum. Look at Constants.h to see all available Enums.
(Don't forget to add an enum if you add a shader to this class)


*/
#pragma endregion
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
class ShaderManager
{

private:
	HRESULT hr;
	// pointers to the main device and context
	ID3D11Device *gDevice = nullptr;
	ID3D11DeviceContext* gDeviceContext = nullptr;


	//Shaders for color
	ID3D11VertexShader*		gVertexShaderColor		= nullptr;
	ID3D11GeometryShader*   gGeometryShaderColor	= nullptr;
	ID3D11PixelShader*		gPixelShaderColor		= nullptr;
	ID3D11InputLayout*		gVertexLayoutColor		= nullptr;

	//Shaders for Texture
	ID3D11VertexShader*		gVertexShaderTexture	= nullptr;
	ID3D11GeometryShader*	gGeometryShaderTexture  = nullptr;
	ID3D11PixelShader*		gPixelShaderTexture		= nullptr;
	ID3D11InputLayout*		gVertexLayoutTexture	= nullptr;

	



	//SamplerStates
	ID3D11SamplerState* gSampleState = nullptr;

	bool createShaders();

	bool createTextureShader();
	bool createColorShader();
	
	
public:
	ShaderManager();
	~ShaderManager();
	void Release();
	bool Init(ID3D11Device *gDevice, ID3D11DeviceContext* gDeviceContext);


	void setActiveShaders(ShaderTypes shader);
};



//cbuffer lightBuffer : register(b0)
//{
//	float4 lightPosition;
//	float4 lightColor;
//	float intensity;
//
//
//};
//
//cbuffer pixelShaderConstants: register(b1)
//{
//    bool normalMap;
//    bool distanceFog;
//
//};
//

cbuffer CameraBuffer : register(b1) //for light glow
{

	matrix view;
	matrix projection;
	matrix invViewProj;
	float4 camPos;


};

cbuffer materialAttributes : register(b2)
{
    float4 diffuse; 
    float4 ambient;
    float4 specularRGB;
    float  specularValue; 
	bool  usingDiffuseTex;
	bool  usingNormalTex;
	float pad;

};
//
SamplerState SampleType;

//modifies how the pixels are written to the polygon face when shaded
Texture2D shaderTexture : register(t0);
//Texture2D normalTexture : register(t2);
//Texture2D renderTexture : register(t4);


struct PS_IN
{
	float4 Pos      : SV_POSITION;
	float2 Texture  : TEXCOORD0;
	float3 Normal   : NORMAL;
	float4 wPos     : WORLDPOS;
	float4 camPos   : CAMERAPOS;
	//float3 Tangent:TANGENT;
	
};

float3 normalToWorldSpace(float3 normalMapSample, float3 normal, float3 tangent)
{
	
    // here we build the tbn basis. to transform the sampled normal to texture space
	//then we return the normal and use it with our calculations

	//Convert from [0,1] to [-1,1]
	float3 normalT = 2.0f * normalMapSample - 1.0f;

	//Build basis
	float3 N = normal;

	//Make sure tangent is completely orthogonal to normal
	float3 T = normalize(tangent - dot(tangent, N)* N); //Read page 582
	float3 B = cross(N, T); //Bitangent

	//Create the "Texture Space" matrix
	float3x3 TBN = float3x3(T, B, N);

	//Convert normal from normal map to texture space and store in input.normal

	float3 bumpedNormal = mul(normalT, TBN);

	return bumpedNormal;

}

float4 PS_main(PS_IN input) : SV_TARGET
{
	
	//return float4(0.5f,0.5f,0.0f, 1);
	//return float4((input.Normal + float3(0.2f,0.2f,0.2f)), 1);
	//return float4(input.Pos);
    float4 lightPosition = float4(5.0f, 10.0f, 0.0f,1.0f);
	float4 lightColor    = float4(1.0f, 1.0f, 1.0f, 1.0f);
    float3 normal = input.Normal;

	//return camPos;
  //  if(normalMap == true)
  //  {
  //  //sampling the normal
  //  float3 normalSample = normalTexture.Sample(SampleType, input.Texture).rgb;
  //
  //   //normal = normalToWorldSpace(normalSample, input.Normal, input.Tangent);
  //  }
//
//
//
////The light ray from the vert position to the light
////normalized to be used as a direction vector
float3 vRay = normalize((lightPosition - input.wPos)).xyz;
//
////create a ray from the vert pos to the camera.
float3 v = normalize(input.camPos - input.wPos).xyz;
//
////Reflect is used in the specular shading
float3 r = reflect(-vRay, normalize(normal));
//
////Calculate how much of the pixel is to be lit "intensity"
float fDot =  saturate(dot(vRay, normalize(normal)));

//   
//
//float3 lightColor = mul(color, intensity);
//

//
float3 specularLight = { specularRGB.xyz * pow(max(dot(r,v),0.0f),specularValue) };
//    float3 textureSample;
//

float3 color;
float3 textureSample = shaderTexture.Sample(SampleType, input.Texture).xyz;
	
if (usingDiffuseTex == true)
	color = textureSample;
else
	color = diffuse.xyz;
	

///////
//Computing the final color with a "late add" of the specular light.
//with late add the computation is modular, allowing for multiple lights
//
//late add is : texturecolor * (diffuse + ambient) + specular
//
// 3d game programming book. p.330
///////

float3 ambientCol = ambient.xyz;

float3 diffuse = lightColor.xyz * fDot ;

//finalCol = textureSample* finalCol; // texture * (diffuse + ambient)
float3 finalCol;
if (specularValue > 0.0f)
	finalCol = saturate(color* (ambient.xyz + diffuse) + specularLight);
else
	finalCol = saturate(color* (ambient.xyz + diffuse));


finalCol.x = saturate(finalCol.x);
finalCol.y = saturate(finalCol.y);
finalCol.z = saturate(finalCol.z);

	//float4 col = { finalCol,1.0 };
  //
  //float4 col = { diffuse, 1.0 };
	//float4 col = { float3(1.0f,0.0f,0.0f), 1.0 };
	float4 col = { finalCol, 1.0 };
    return col;
}
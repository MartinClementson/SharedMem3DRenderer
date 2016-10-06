cbuffer lightBuffer
{
	float4 lightPosition;
	float4 lightColor;
	float intensity;


};

SamplerState SampleType;
textureCUBE skyBoxTexture : register(t1);
texture2D renderTexture : register(t2);
struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
	float3 normal : NORMAL;
	float4 wPos: WORLDPOS;
	float3 camPos : CAMERAPOS;

};


float4 PS_main(PS_IN input) : SV_TARGET
{

 //The light ray from the vert position to the light
//normalized to be used as a direction vector
float3 vRay = normalize((float3)(lightPosition - input.wPos));

//create a ray from the vert pos to the camera.
float3 v = normalize(input.camPos - input.wPos.xyz);

//Reflect is used in the specular shading
float3 r = reflect(-vRay, normalize(input.normal));

//Calculate how much of the pixel is to be lit
float fDot = max(0.0f, dot(normalize(vRay), normalize(input.normal)));

float3 color = lightColor.xyz;

float3 lightColor = mul(color, intensity);

float shinyPower = 20.0f;

float3 specularLight = { lightColor.xyz * pow(max(dot(r,v),0.0),shinyPower) };


float3 ambient = { 0.1f, 0.1f, 0.1f };



///////
//Computing the final color with a "late add" of the specular light.
//with late add the computation is modular, allowing for multiple lights
//
//late add is : texturecolor * (diffuse + ambient) + specular
//
// 3d game programming book. p.330
///////


float3 diffuse = lightColor * fDot;

float3 finalCol = (diffuse + ambient);
finalCol = input.color * finalCol; //(texture or color) * (diffuse + ambient)
finalCol = finalCol + specularLight; // + specular




//Calculate enviroment reflections
float3 incident = input.wPos - input.camPos;
float3 ref = reflect(incident, normalize(input.normal));

float4 reflectionColor = skyBoxTexture.Sample(SampleType, ref);
finalCol += reflectionColor.xyz * 0.5;
//finalCol.x = max(finalCol.x, 1.0);
//finalCol.y = max(finalCol.y, 1.0);
//finalCol.z = max(finalCol.z, 1.0);
float4 col = { finalCol,1.0 };
//float4 col = { (ambient + diffuse + specularLight),1.0 }; // old calculation

return col;
}
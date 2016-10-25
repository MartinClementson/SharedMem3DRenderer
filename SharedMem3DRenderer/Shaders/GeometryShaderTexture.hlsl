cbuffer WorldBuffer : register(b0)
{
	matrix world;
	matrix normalWorld;
};


cbuffer CameraBuffer : register(b1)
{

	matrix view;
	matrix projection;
    matrix invViewProj;
    float4 camPos;
   

};

struct GSinput
{

    float4 Pos     : SV_POSITION;
    float3 Normal  : NORMALS;
    float2 Texture : TEXCOORD0;
	
};

struct GSOutput
{
	float4 Pos : SV_POSITION;
	float2 Texture: TEXCOORD0;
	float3 Normal : NORMAL;
	float4 wPos : WORLDPOS;
	float4 camPos: CAMERAPOS;

};

[maxvertexcount(3)]
void GS_main(
	triangle GSinput input[3],
	inout TriangleStream< GSOutput > output)
{
    float3 faceEdgeA = input[1].Pos - input[0].Pos;
    float3 faceEdgeB = input[2].Pos - input[0].Pos;
    float3 faceNormal = normalize(cross(faceEdgeA, faceEdgeB));
	faceNormal.x = faceNormal.x * -1;
	faceNormal.y = faceNormal.y * -1;
	faceNormal.z = faceNormal.z * -1;

	//////////////////////////////////////////////////
	///////////Back-face culling test/////////////////

    //multiply the face normal into world space
    float3 faceNormal2 = normalize(mul(float4(faceNormal, 1.0f), normalWorld)).xyz;
    
    //multiply one of the vertices into world space
    float3 viewDir = mul(input[0].Pos, world);
    
    //Create a vector from that vertex to the cameras position
    viewDir = normalize(viewDir - camPos.xyz);
    
    //Take the dot product of the flipped vert<-cam vector and the normal
    float dt = dot(-viewDir, faceNormal2);
    
    //if the dot product is less than 0, The angle is more than 90 degrees, which means we can't see it
	///////////////////////////////////////////////////////////

	//Note: To turn it off, Comment the IF statement below. 
	//To show that it works : Create a vector from 0,0,-1  to viewDir,
   // if (dt > 0)
   // {
		//combining the matrices for simpler use, also more efficient
        matrix combinedMatrix = mul(world, mul(view, projection));
		//matrix combinedMatrix = mul(projection, mul(view, world));
		//matrix combinedMatrix = mul(world, view);
		//combinedMatrix = mul(combinedMatrix, projection);
        for (uint i = 0; i < 3; i++)
        {
            GSOutput element;
            element.Pos     = mul(input[i].Pos, combinedMatrix);
            element.wPos    = mul(input[i].Pos, world);
            element.camPos  = camPos;
            element.Texture = input[i].Texture;
            element.Normal = normalize(mul(float4(input[i].Normal,1.0f), normalWorld));
			//element.Normal = faceNormal2;//normalize(mul(float4(faceNormal, 1.0f), normalWorld));
    
            output.Append(element);
        }
    //}
}



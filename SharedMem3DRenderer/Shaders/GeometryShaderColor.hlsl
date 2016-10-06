cbuffer worldConstantBuffer : register(b0) 
{
	matrix world;
	matrix normalWorld;
};


cbuffer cameraConstantBuffer  : register(b1)
{

	matrix projection;
	matrix view;
	float3 camLook;
	float3 camPos;
	// normalworld?
};

struct GSinput
{

	float4 pos : SV_POSITION;
	float3 color : COLOR;

};

struct GSOutput
{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
	float3 normal : NORMAL;
	float4 wPos : WORLDPOS;
	float3 camPos: CAMERAPOS;
};

[maxvertexcount(3)]
void GS_main(
	triangle GSinput input[3],
	inout TriangleStream< GSOutput > output)

{
	float3 faceEdgeA = input[1].pos - input[0].pos;
	float3 faceEdgeB = input[2].pos - input[0].pos;
	float3 faceNormal = normalize(cross(faceEdgeA, faceEdgeB));
	


	

	//////////////////////////////////////////////////
	///////////Back-face culling test/////////////////

	//multiply the face normal into world space
	float3 faceNormal2 = normalize(mul(faceNormal, normalWorld));

	//multiply one of the vertices into world space
	float3 viewDir = mul(input[0].pos, world);

	//Create a vector from that vertex to the cameras position
	viewDir = normalize(viewDir - camPos);

	//Take the dot product of the flipped vert<-cam vector and the normal
	float dt = dot(-viewDir, faceNormal2);

	//if the dot product is less than 0, The angle is more than 90 degrees, which means we can't see it
	///////////////////////////////////////////////////////////

	//Note: To turn it off, Comment the IF statement below. 
	//To show that it works : Create a vector from 0,0,-1  to viewDir,



	if (dt > 0)
	{
		//combining the matrices for simpler use, also more efficient

		matrix combinedMatrix = mul(world, mul(view, projection));
		//matrix combinedMatrix = mul(world, view);
		//combinedMatrix = mul(combinedMatrix, projection);

		for (uint i = 0; i < 3; i++)
		{
			GSOutput element;
			element.pos = input[i].pos;
			element.pos = mul(element.pos, combinedMatrix);
			element.wPos = mul(input[i].pos, world);
			element.camPos = camPos;
			element.color = input[i].color;


			element.normal = normalize(mul(faceNormal, normalWorld));
			output.Append(element);
		}
	}
}
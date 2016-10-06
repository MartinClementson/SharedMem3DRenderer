//creating structs to simplify the handling
struct VS_IN
{
	float3 Pos      : POSITION;
	float3 Normal   : NORMALS;
	float2 Texture  : TEXCOORD0;
};

struct VS_OUT
{
	float4 Pos     : SV_POSITION;
	float3 Normal  : NORMALS;
	float2 Texture : TEXCOORD0;

	
};



VS_OUT VS_main( VS_IN input )
{
	VS_OUT output; //perhaps its supposed to say VS_OUT output = (VS_OUT)0;

	//setting the attributes
	output.Pos      = float4(input.Pos, 1.0f);
	output.Texture  = input.Texture;
	output.Normal   = input.Normal;
	return output;
}
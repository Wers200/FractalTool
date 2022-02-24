struct VS_INPUT {
	float3 position : POSITION;
};

struct PS_INPUT {
	float4 position : SV_POSITION;
	float2 UV : TEXCOORD0;
};

PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output;
	output.position = float4(input.position, 1);

	output.UV = (input.position.xy + 1) / 2;
	output.UV.y = 1 - output.UV.y;

	return output;
}
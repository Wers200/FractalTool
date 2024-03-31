Texture2D<uint> data : register(t0);

struct PS_INPUT {
	float4 position : SV_POSITION;
	float2 UV : TEXCOORD0;
};

cbuffer global : register(b0) {
	float4 zoom	: Zoom;
	float4 prevZoom : PreviewZoom;
	float2 c	: C;
	uint2 size	: Size;
	uint maxIter	: MaxIter;
	float time	: Time;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	uint iter = data.Load(int3(input.position.xy, 0)).x;

	float3 color;
	switch(iter) {
		case 1:
		color = float3(45.0/255, 64.0/255, 89.0/255);
		break;

		case 2:
		color = float3(234.0/255, 84.0/255, 85.0/255);
		break;

		case 3:
		color = float3(255.0/255, 212.0/255, 96.0/255);
		break;
	}

	float4 rPZ = float4(
		prevZoom.x < prevZoom.z ? prevZoom.x : prevZoom.z, 
		prevZoom.y < prevZoom.w ? prevZoom.y : prevZoom.w, 
		prevZoom.x > prevZoom.z ? prevZoom.x : prevZoom.z,
		prevZoom.y > prevZoom.w ? prevZoom.y : prevZoom.w
	);

	if (input.UV.x >= rPZ.x && input.UV.x <= rPZ.z && input.UV.y >= rPZ.y && input.UV.y <= rPZ.w) {
		color += (abs(input.UV.x - rPZ.x) <= 2.0 / size.x || abs(input.UV.x - rPZ.z) <= 2.0 / size.x || abs(input.UV.y - rPZ.y) <= 2.0 / size.y || abs(input.UV.y - rPZ.w) <= 2.0 / size.y) ? 0.8 : 0.2;
	}

	return float4(color, 1);
}
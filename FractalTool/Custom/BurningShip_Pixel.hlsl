Texture2D<uint> data : register(t0);

struct PS_INPUT {
	float4 position : SV_POSITION;
	float2 UV : TEXCOORD0;
};

cbuffer global : register(b0) {
	float4 zoom		: Zoom;
	float4 prevZoom : PreviewZoom;
	float2 c		: C;
	uint2 size		: Size;
	uint maxIter	: MaxIter;
	float time		: Time;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	uint iter = data.Load(int3(input.position.xy, 0)).x - 1;

	float3 color = float3(iter == -1 ? 0 : (1 - pow(1.1, -(float)iter)), 0, 0);

	// Get real selection rectangle (where x is 100% left and etc.)
	float4 rPZ = float4(
		prevZoom.x < prevZoom.z ? prevZoom.x : prevZoom.z, 
		prevZoom.y < prevZoom.w ? prevZoom.y : prevZoom.w, 
		prevZoom.x > prevZoom.z ? prevZoom.x : prevZoom.z,
		prevZoom.y > prevZoom.w ? prevZoom.y : prevZoom.w
	);
	// Change the colors inside the zoom selection rectangle
	if (input.UV.x >= rPZ.x && input.UV.x <= rPZ.z && input.UV.y >= rPZ.y && input.UV.y <= rPZ.w) {
		color += /* Check if the pixel is in the selection border */ (abs(input.UV.x - rPZ.x) <= 2.0f / size.x || abs(input.UV.x - rPZ.z) <= 2.0f / size.x 
			|| abs(input.UV.y - rPZ.y) <= 2.0f / size.y || abs(input.UV.y - rPZ.w) <= 2.0f / size.y) ? 0.8 : 0.2;
	}

	return float4(color, 1);
}
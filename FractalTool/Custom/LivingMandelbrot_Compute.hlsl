cbuffer global : register(b0) {
	float4 zoom		: Zoom;
	float4 prevZoom : PreviewZoom;
	float2 c		: C;
	uint2 size		: Size;
	uint maxIter	: MaxIter;
	float time		: Time;
};

static uint block = floor((size.x * size.y) / 1024);
RWStructuredBuffer<uint> output : register(u0);

uint getIter(uint x, uint y) {
	float zx = ((float)x / size.x) * (zoom.z - zoom.x) + zoom.x;
	float zy = ((float)y / size.y) * (zoom.w - zoom.y) + zoom.y;

	float v_zx = 0.02 * cos(0.2 * time);
	float v_zy = 0.02 * sin(0.2 * time);

	uint j = 1;
	while (v_zx * v_zx + v_zy * v_zy < 4 && j <= maxIter) {
		float temp = v_zx * v_zx - v_zy * v_zy + zx; 
		v_zy = 2.0 * v_zx * v_zy + zy;
		v_zx = temp;
		j++;
	}

	if (j == maxIter + 1) j = 0;

	return j;
}

void iteration(uint k) {
	uint x = k % size.x;
	uint y = floor(k / (float)size.x);

	output[k] = getIter(x, y);
}

[numthreads(1024, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID) {
	for (uint k = 0; k < block; k++) {
		iteration(block * DTid.x + k);
	}
	if (block == 0 && DTid.x < size.x * size.y) {
		iteration(DTid.x);
	}
	else if (DTid.x == 1023 && block * 1024 < size.x * size.y) {
		for (uint k = block * 1024; k < size.x * size.y; k++) {
			iteration(k);
		}
	}
}
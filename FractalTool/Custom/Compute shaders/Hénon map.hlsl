cbuffer global : register(b0) {
	float4 zoom	: Zoom;
	float4 prevZoom : PreviewZoom;
	float2 c	: C;
	uint2 size	: Size;
	uint maxIter	: MaxIter;
	float time 	: Time;
	bool setType 	: SetType;
};

static uint block = floor((size.x * size.y) / 1024);
RWStructuredBuffer<uint> output : register(u0);

uint getIter(uint x, uint y) {
	float z_x = 1.5 * (((float)x / size.x) * (zoom.z - zoom.x) + zoom.x);
	float z_y = 1.5 * (-(((float)y / size.y) * (zoom.w - zoom.y) + zoom.y));

	float2 v = float2(z_x, z_y);
	float2 c_a = float2(z_x, z_y);
	if(setType) {
		z_x *= 20/3.0;
		z_y *= 20/3.0;
		v = float2(z_x, z_y);
		c_a = float2(c.x, c.y);
	}

	uint j = 1;
	while (dot(v, v) < 800 && j <= maxIter) {
		float temp = 1 - c_a.x * v.x * v.x + v.y; 
		v.y = c_a.y * v.x;
		v.x = temp;
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

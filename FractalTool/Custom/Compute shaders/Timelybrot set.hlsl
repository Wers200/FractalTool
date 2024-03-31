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

float2 c_m(float2 c_1, float2 c_2)
{ return float2(c_1.x * c_2.x - c_1.y * c_2.y, c_1.y * c_2.x + c_1.x * c_2.y); }

uint getIter(uint x, uint y) {
	float z_x = ((float)x / size.x) * (zoom.z - zoom.x) + zoom.x;
	float z_y = ((float)y / size.y) * (zoom.w - zoom.y) + zoom.y;

	float2 v = float2(sqrt(0.5) * sin(0.5 * time), sqrt(0.5) * cos(0.5 * time));
	float2 c_a = float2(z_x, z_y);
	if (setType) {
		c_a = c + v;
		v = float2(z_x, z_y);
	}

	uint j = 1;
	while (dot(v, v) < 4 && j <= maxIter) {
		v = c_m(c_m(c_m(v, v), v), v) + c_a;
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
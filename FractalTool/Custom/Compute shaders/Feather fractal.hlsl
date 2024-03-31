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

float2 cx_cube(float2 a) {
  float x_2 = a.x * a.x;
  float y_2 = a.y * a.y;
  float d = x_2 - y_2;
  return float2(a.x * (d - y_2 - y_2), a.y * (x_2 + x_2 + d));
}

float2 cx_div(float2 a, float2 b) {
  float denom = 1.0 / (b.x * b.x + b.y * b.y);
  return float2(a.x * b.x + a.y * b.y, a.y * b.x - a.x * b.y) * denom;
}

uint getIter(uint x, uint y) {
	float z_x = -2 * (((float)x / size.x) * (zoom.z - zoom.x) + zoom.x);
	float z_y = 2 * (((float)y / size.y) * (zoom.w - zoom.y) + zoom.y);
	
	float2 z = float2(z_x, z_y);
	float2 v = float2(0, 0);
	if (setType) {
		z = c;
		v = float2(z_x, z_y);
	}

	uint j = 1;
	while (dot(v, v) < 1024 && j <= maxIter) {
		v = cx_div(cx_cube(v), float2(1 + v.x * v.x, v.y * v.y)) + z; 
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
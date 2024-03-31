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
	// Convert the x and y to the complex plane
	float z_x = ((float)x / size.x) * (zoom.z - zoom.x) + zoom.x;
	float z_y = ((float)y / size.y) * (zoom.w - zoom.y) + zoom.y;

	// Set up the calculation of either the Mandelbrot or the Julia version
	float2 v = float2(0, 0);
	float2 c_a = float2(z_x, z_y);
	if (setType) {
		v = float2(z_x, z_y);
		c_a = c;
	}

	// Get the maximum z^2 + c iteration (where it is not outside of the circle with radius of two)
	uint j = 1;
	while (dot(v, v) < 4 && j <= maxIter) {
		float temp = v.x * v.x - v.y * v.y + c_a.x;
		v.y = 2 * v.x * v.y + c_a.y;
		v.x = temp;
		j++;
	}

	// To discern between the insides of the fractal and not
	if (j == maxIter + 1) j = 0;

	return j;
}

void iteration(uint k) {
	// Get x and y from k (buffer index)
	uint x = k % size.x;
	uint y = floor(k / (float)size.x);

	output[k] = getIter(x, y);
}

[numthreads(1024, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID) {
	// Simple calculation parallelisation
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

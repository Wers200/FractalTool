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
	// Convert the x and y to the complex plane
	float zx = ((float)x / size.x) * (zoom.z - zoom.x) + zoom.x;
	float zy = ((float)y / size.y) * (zoom.w - zoom.y) + zoom.y;

	// Get the maximum z^2 + c iteration (where it is not outside of the circle with radius of two)
	uint j = 1;
	while (zx * zx + zy * zy < 4 && j <= maxIter) {
		float temp = zx * zx - zy * zy + c.x;
		zy = 2.0 * zx * zy + c.y;
		zx = temp;
		j++;
	}

	// To discern between good values and not so
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
	// Trivial parallelisation
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
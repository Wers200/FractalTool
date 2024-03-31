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
	float z_x = ((float)x / size.x) * (zoom.z - zoom.x) + zoom.x;
	float z_y = ((float)y / size.y) * (zoom.w - zoom.y) + zoom.y;

	uint j = 1;
	while(j <= maxIter) {
		float expr = 3 * pow((z_x * z_x + z_y * z_y), 2);
		float temp = z_x / 3 - (z_x * z_x - z_y * z_y) / expr;
		z_y -= z_y / 3 + 2 * z_x * z_y / expr - c.y;
		z_x -= temp - c.x;

		j++;
	}

	float dist1 = (z_x - 1) * (z_x - 1) + z_y * z_y;
	float dist2 = (z_x + 0.5) * (z_x + 0.5) + (z_y - sqrt(3)/2) * (z_y - sqrt(3)/2);
	float dist3 = (z_x + 0.5) * (z_x + 0.5) + (z_y + sqrt(3)/2) * (z_y + sqrt(3)/2);
	uint root = 1;

	if(dist1 < dist2 && dist1 < dist3) root = 1;
	else if(dist2 < dist1 && dist2 < dist3) root = 2;
	else root = 3;

	return root;
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
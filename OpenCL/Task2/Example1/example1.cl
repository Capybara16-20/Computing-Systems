__kernel void example1(__global float* input,
	__global float* output)
{
	uint id = get_global_id(0);
	output[id] = sqrt(input[id] * 2) / 3;
}

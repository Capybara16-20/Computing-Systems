__kernel void example2(__global float* a,
	__global float* b, __global float* x)
{
	uint id = get_global_id(0);
	x[id] = pow(a[id], (float)id) * b[id];
}

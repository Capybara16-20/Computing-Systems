__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE |
CLK_ADDRESS_CLAMP | CLK_FILTER_NEAREST;
__kernel void example1(read_only image2d_t src_image,
	write_only image2d_t dst_image)
{
	uint x = get_global_id(0);
	uint y = get_global_id(1);
	uint width = get_global_size(0);
	uint height = get_global_size(1);
	int2 coord = (int2)(x, y);
	uint4 color = read_imageui(src_image, sampler, coord);
	uint gray = (uint)((color.x + color.y + color.z) / 3);
	uint4 newColor = (uint4)(gray, gray, gray, 255);
	write_imageui(dst_image, coord, newColor);
}



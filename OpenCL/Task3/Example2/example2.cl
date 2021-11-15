__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE |
CLK_ADDRESS_CLAMP | CLK_FILTER_NEAREST;
__kernel void example2(read_only image2d_t src_image,
	write_only image2d_t red_image,
	write_only image2d_t green_image, 
	write_only image2d_t blue_image)
{
	uint x = get_global_id(0);
	uint y = get_global_id(1);
	uint width = get_global_size(0);
	uint height = get_global_size(1);
	int2 coord = (int2)(x, y);
	uint4 color = read_imageui(src_image, sampler, coord);
	uint gray = (uint)((color.x + color.y + color.z) / 3);
	
	uint4 red = (uint4)(color.x, 0, 0, 255);
	uint4 green = (uint4)(0, color.y, 0, 255);
	uint4 blue = (uint4)(0, 0, color.z, 255);

	write_imageui(red_image, coord, red);
	write_imageui(green_image, coord, green);
	write_imageui(blue_image, coord, blue);
}


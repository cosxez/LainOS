struct multiboot_info
{
	unsigned int type;
	unsigned int size;
	unsigned long long frbuff_addr;
	unsigned int frbuff_pitch;
	unsigned int frbuff_width;
	unsigned int frbuff_height;
	unsigned char frbuff_bpp;
	unsigned char frbuff_type;
	unsigned short res;
}__attribute__((packed));

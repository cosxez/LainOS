#include <structmultiboot.h>

void kernel_main(unsigned long saddr)
{
	struct multiboot_info *mlt=(struct multiboot_info*)(saddr+8);

	while (mlt->type!=0)
	{
		if (mlt->type==8)
		{
			unsigned int *bfv=(unsigned int*)mlt->frbuff_addr;
			for (unsigned int i=0;i<(mlt->frbuff_width * mlt->frbuff_height);i++)
			{
				bfv[i]=0x4b3b72ac;
			}
			break;
		}
		mlt = (struct multiboot_info*)((unsigned char *)mlt + ((mlt->size + 7) & ~7));
	}
	while (1){__asm__("hlt");}
}

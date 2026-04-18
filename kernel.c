#include <structmultiboot.h>
#include <idt_table.h>
#include <font.h>

struct multiboot_info *mlt;
unsigned int *bfv;

unsigned int curpos_x=0;
unsigned int curpos_y=0;
unsigned char font_size=1;

struct s_idte idte[256];
struct s_idtt idtt;

extern unsigned char _binary_cvnt_whld_start[];

extern unsigned char _binary_font_cwqf_start[];
extern unsigned char _binary_font_cwqf_end[];

void curpos(unsigned int x,unsigned int y)
{
	if (x>mlt->frbuff_width)
	{
		if (y<mlt->frbuff_height)
		{
			curpos_y=y+30*font_size;curpos_x=0;
		}
	}
	else{curpos_x=x;curpos_y=y;}
}

void set_pixel(unsigned int x, unsigned int y,unsigned int color)
{
	bfv[y*mlt->frbuff_width+x]=color;
}

void cursor_draw(unsigned int color)
{
	for (unsigned int i=curpos_x;i<curpos_x+10;i++){bfv[curpos_y*mlt->frbuff_width+i]=color;}
}
void cursor_del(unsigned int color)
{
	for (unsigned int i=curpos_x;i<curpos_x+10;i++){bfv[curpos_y*mlt->frbuff_width+i]=color;}
}

void printc(char c, unsigned int color)
{
	unsigned short mgfc = 0;
	for (unsigned int i = 0; i < sizeof(fmcl) / sizeof(fmcl[0]); i++)
	{
		if (c == fmcl[i].ch)
		{
			mgfc = fmcl[i].mg;break;
		}
	}
	if (mgfc == 0){return;}

	unsigned char *crp = _binary_font_cwqf_start;
	while (crp < _binary_font_cwqf_end)
	{
		if (*(unsigned short*)crp == mgfc)
		{
			crp += 2;
			unsigned char lines = *crp;
			crp++;

			for (unsigned char l = 0; l < lines; l++)
			{
				unsigned char b1 = crp[0];
				unsigned char b2 = crp[1];
				crp += 2;

				int x1 = curpos_x + ((b1 >> 4) * font_size);
				int y1 = curpos_y + ((b1 & 0xf) * -font_size);
				int x2 = curpos_x + ((b2 >> 4) * font_size);
				int y2 = curpos_y + ((b2 & 0xf) * -font_size);

				int dx = (x2 > x1) ? (x2 - x1) : (x1 - x2);
				int dy = (y2 > y1) ? (y2 - y1) : (y1 - y2);
				int sx = (x1 < x2) ? 1 : -1;
				int sy = (y1 < y2) ? 1 : -1;
				int err = dx - dy;

				while (1)
				{
					set_pixel(x1, y1, color);
					if (x1 == x2 && y1 == y2) {break;}
					int e2 = 2 * err;
					if (e2 > -dy){err -= dy; x1 += sx;}
					if (e2 < dx){err += dx; y1 += sy;}
				}
			}
			curpos(curpos_x+10*font_size,curpos_y);
			return;
		}
		crp++;
	}
}

void print(char* str,unsigned int color)
{
	for (unsigned int i=0;str[i]!='\0';i++){if (str[i]=='\n'){curpos(0,curpos_y+30*font_size);};printc(str[i],color);}
}

void kernel_main(unsigned long saddr)
{
	mlt=(struct multiboot_info*)(saddr+8);

	while (mlt->type!=0)
	{
		if (mlt->type==8)
		{
			break;
		}
		mlt = (struct multiboot_info*)((unsigned char *)mlt + ((mlt->size + 7) & ~7));
	}

	bfv=(unsigned int*)mlt->frbuff_addr;
	
	for (unsigned int y=0;y<172;y++)
	{
		for (unsigned int x=0;x<172;x++)
		{
			unsigned char red=_binary_cvnt_whld_start[(y*172+x)*3+8];
			unsigned char green=_binary_cvnt_whld_start[(y*172+x)*3+8+1];
			unsigned char blue=_binary_cvnt_whld_start[(y*172+x)*3+8+2];
			bfv[y*mlt->frbuff_width+x]=(red<<16) | (green<<8) | blue;
			curpos_x=x;
		}
		curpos_y=y;
	}
	curpos(0,curpos_y+30*font_size);
	print("LainOS v0.1\n",0x00ffffff);
	printc('>',0x00ffffff);

	while (1){__asm__("hlt");}
}

#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <linux/input.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <font.h>

struct fb_var_screeninfo frd;
volatile unsigned int *bfv;

unsigned int curpos_x=0;
unsigned int curpos_y=0;
unsigned char font_size=1;

extern unsigned char _binary_cvnt_whld_start[];

extern unsigned char _binary_font_cwqf_start[];
extern unsigned char _binary_font_cwqf_end[];

void curpos(unsigned int x,unsigned int y)
{
	if (x>frd.xres)
	{
		if (y<frd.yres)
		{
			curpos_y=y+20*font_size;curpos_x=0;
		}
	}
	else{curpos_x=x;curpos_y=y;}
}

void set_pixel(unsigned int x, unsigned int y,unsigned int color)
{
	bfv[y*frd.xres+x]=color;
}

void draw_line(int x1, int y1, int x2, int y2, unsigned int color)
{
	int dx = (x2 > x1) ? (x2 - x1) : (x1 - x2);
    	int dy = (y2 > y1) ? (y2 - y1) : (y1 - y2);
    	int sx = (x1 < x2) ? 1 : -1;
    	int sy = (y1 < y2) ? 1 : -1;
    	int err = dx - dy;

    	while (1)
	{
        	if (x1>=0 && x1<frd.xres && y1>=0 && y1<frd.yres)
		{
           		bfv[y1*frd.xres+x1]=color;
        	}

		if (x1==x2 && y1==y2){break;}
		int e2=2 * err;
		if (e2>-dy){err-=dy;x1+=sx;}
		if (e2<dx){err+=dx;y1+=sy;}
	}
}

void cursor_fill(unsigned int color)
{
	draw_line(curpos_x-2*font_size,curpos_y+30*font_size,curpos_x,curpos_y+20*font_size,color);
	draw_line(curpos_x,curpos_y+20*font_size,curpos_x+2*font_size,curpos_y+30*font_size,color);
}

void printc(char c, unsigned int color)
{
	cursor_fill(0x00000000);
	unsigned short mgfc = 0;
	for (unsigned int i = 0; i < sizeof(fmcl) / sizeof(fmcl[0]); i++)
	{
		if (c == fmcl[i].ch)
		{
			mgfc = fmcl[i].mg;break;
		}
	}
	if (mgfc == 0){return;}

	if (curpos_x>(frd.xres-(20*font_size))){curpos(frd.xres+10,curpos_y);}
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
				int y1 = curpos_y + ((b1 & 0xf) * font_size);
				int x2 = curpos_x + ((b2 >> 4) * font_size);
				int y2 = curpos_y + ((b2 & 0xf) * font_size);

				int dx = (x2 > x1) ? (x2 - x1) : (x1 - x2);
				int dy = (y2 > y1) ? (y2 - y1) : (y1 - y2);
				int sx = (x1 < x2) ? 1 : -1;
				int sy = (y1 < y2) ? 1 : -1;
				int err = dx - dy;

				while (1)
				{
					bfv[y1*frd.xres+x1]=color;
					if (x1 == x2 && y1 == y2) {break;}
					int e2 = 2 * err;
					if (e2 > -dy){err -= dy; x1 += sx;}
					if (e2 < dx){err += dx; y1 += sy;}
				}
			}
			curpos(curpos_x+12*font_size,curpos_y);
			return;
		}
		crp++;
	}
}

void print(char* str,unsigned int color)
{
	for (unsigned int i=0;str[i]!='\0';i++){if (str[i]=='\n'){curpos(0,curpos_y+30*font_size);};printc(str[i],color);}
}

int main()
{
	mkdir("/proc", 0755);
	mount("proc","/proc","proc",0,NULL);
	mkdir("/dev", 0755);
	mount("devtmpfs","/dev","devtmpfs",0,NULL);
	mkdir("/sys", 0755);
	mount("sysfs","/sys","sysfs",0,NULL);
	
	int dstty=open("/dev/tty0",O_RDWR);
	int dssls=open("/dev/null",O_RDWR);
	ioctl(dstty,KDSETMODE,KD_GRAPHICS);
	dup2(dssls,0);dup2(dssls,1);dup2(dssls,2);
	
	struct fb_fix_screeninfo ofsml;

	int frbuff=open("/dev/fb0",O_RDWR | O_SYNC);
	
	ioctl(frbuff,FBIOGET_VSCREENINFO,&frd);
	ioctl(frbuff,FBIOGET_FSCREENINFO,&ofsml);
	size_t map_size = (ofsml.smem_len + 4095) & ~4095;
	bfv=(unsigned int*)mmap(0,map_size,PROT_READ | PROT_WRITE, MAP_SHARED,frbuff,0);

	for (unsigned int i=0;i<frd.xres*frd.yres;i++){bfv[i]=0x00000000;}

	int fkb = -1;
	char dnfkb[32];
	for (int i = 0; i < 10; i++)
	{
		sprintf(dnfkb, "/dev/input/event%d", i);
		int fd = open(dnfkb, O_RDONLY | O_NONBLOCK);
		if (fd < 0){continue;}

		unsigned long kbt[16];
		for (int j=0;j<16;j++){kbt[j] = 0;}

		if (ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(kbt)), kbt) >= 0)
		{
			if (kbt[30 / (sizeof(long) * 8)] & (1UL << (30 % (sizeof(long) * 8))))
			{
				fkb=fd;
				ioctl(fkb, EVIOCGRAB, 1);
				break;
			}
		}
		close(fd);
	}
	if (fkb < 0){print("Error: keyboard dont init\n",0x00ff0000);}

	struct input_event e;
	for (unsigned int y=0;y<172;y++)
	{
		for (unsigned int x=0;x<172;x++)
		{
			unsigned char red=_binary_cvnt_whld_start[(y*172+x)*3+8];
			unsigned char green=_binary_cvnt_whld_start[(y*172+x)*3+8+1];
			unsigned char blue=_binary_cvnt_whld_start[(y*172+x)*3+8+2];
			bfv[y*frd.xres+x]=(red<<16) | (green<<8) | blue;
			curpos_x=x;
		}
		curpos_y=y;
	}
	curpos(0,curpos_y+30*font_size);
	print("LainOS v0.1\n",0x00ffffff);
	printc('>',0x00ffffff);
	
	unsigned char isprd_shift=0;
	unsigned char isprd_spec=0;
	unsigned char isprd_ctrl=0;
	while(1)
	{
		while (read(fkb,&e,sizeof(e))>0)
		{	
			if (e.type!=EV_KEY){break;}
			if (e.code==KEY_LEFTSHIFT || e.code==KEY_RIGHTSHIFT){isprd_shift=e.value;}
			if (e.code==KEY_LEFTMETA){isprd_spec=e.value;}
			if (e.value==1 || e.value==2)
			{
				if (e.code==KEY_ENTER){cursor_fill(0x00000000);curpos(0,curpos_y+20*font_size);printc('>',0x00ffffff);};
				if (e.code==KEY_BACKSPACE)
				{
					if (curpos_x>(13*font_size))
					{
						cursor_fill(0x00000000);
						curpos(curpos_x-12*font_size,curpos_y);
						for (int i=curpos_y;i<curpos_y+20*font_size;i++)
						{
							for (int j=curpos_x;j<curpos_x+13*font_size;j++)
							{
								bfv[i*frd.xres+j]=0x00000000;
							}
						}
					}
				}

				for (unsigned int i = 0; i < sizeof(lscfl) / sizeof(lscfl[0]); i++)
				{
					if (e.code==lscfl[i].lsc)
					{
						if (isprd_shift!=0){printc(lscfl[i].sch,0x00ffffff);}
						else{printc(lscfl[i].ch,0x00ffffff);}
						break;
					}
				}
			}
		}
		cursor_fill(0x00ffffff);usleep(10000);
	}
}

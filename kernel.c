#define WHITE 0x0F

unsigned short curpos_x=0,curpos_y=0;
unsigned char curcolor=WHITE;

static inline void outb(unsigned short port, unsigned short value)
{
	asm volatile("outb %b0, %w1" : : "a"(value), "Nd"(port));
}

static inline unsigned char inb(unsigned short port)
{
	unsigned char ret;
	asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

struct idt_struct
{
	unsigned short base_lo;
	unsigned short sel;
	unsigned char anull;
	unsigned char flag;
	unsigned short base_h;
} __attribute__((packed));

struct lidt_struct
{
	unsigned short limit;
	unsigned int base;
}__attribute__((packed));

struct gdt_struct
{
	unsigned short limit_low;
	unsigned short base_low;
	unsigned char  base_middle;
	unsigned char  access;
	unsigned char  granularity;
	unsigned char  base_high;

}__attribute__((packed));

struct lgdt_struct
{
	unsigned short limit;
	unsigned int base;
}__attribute__((packed));

struct idt_struct idts[256];
struct lidt_struct idtp;

struct gdt_struct gdts[3];
struct lgdt_struct gdtp;

const char keys[]={0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '};

char cmd_buffer[128];
unsigned short cp_buff_cur=0;

struct comd
{
	char* func_name;
	void (*func)();
};

void pic_map()
{
	outb(0x20,0x11);
	outb(0xA0,0x11);
	outb(0x21,0x20);
	outb(0xA1,0x28);
   	outb(0x21,0x04);
    	outb(0xA1,0x02);
	outb(0x21,0x01);
	outb(0xA1,0x01);
       	outb(0x21,0xFD);
  	outb(0xA1,0xFF);
}

extern void read_kbd_scan();

void set_idt_gate(int n,unsigned int handler)
{
	idts[n].base_lo=handler & 0xFFFF;
	idts[n].base_h=(handler >> 16) & 0xFFFF;
	idts[n].sel=0x08;
	idts[n].flag=0x8E;
	idts[n].anull=0;
}

void gdt_set_gate(int num, unsigned int base, unsigned int limit, unsigned char access, unsigned char gran)
{
	gdts[num].base_low=(base & 0xFFFF);
	gdts[num].base_middle=(base >> 16) & 0xFF;
	gdts[num].base_high=(base >> 24) & 0xFF;
	gdts[num].limit_low=(limit & 0xFFFF);
	gdts[num].granularity=(limit >> 16) & 0x0F;
	gdts[num].granularity |=gran & 0xF0;
	gdts[num].access=access;
}

void gdt_table()
{
	gdtp.limit = (sizeof(struct gdt_struct) * 3) - 1;
        gdtp.base  = (unsigned int)&gdts;

	gdt_set_gate(0, 0, 0, 0, 0);
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

	asm volatile("lgdt (%0)" : : "r" (&gdtp));
}

void idt_table()
{
	for (int i=0;i<256;i++){idts[i].base_lo=0;idts[i].sel=0;idts[i].anull=0;idts[i].flag=0;idts[i].base_h=0;}
	idtp.limit=(sizeof(struct idt_struct)*256)-1;
	idtp.base=(unsigned int)&idts;

	pic_map();

	set_idt_gate(33,(unsigned int)read_kbd_scan);
	asm volatile("lidt (%0)" : : "r" (&idtp));
}

void curpos(unsigned short x, unsigned short y)
{
	curpos_x=x;
	curpos_y=y;

	if (curpos_x>=80){curpos_x=0;curpos_y+=1;}

	if (curpos_y>=25)
	{
		volatile char* video_mem=(volatile char*)0xB8000;
		for (int i=0;i<24*80*2;i++)
		{
			video_mem[i]=video_mem[i+80*2];
		}
		
		for (int i=0;i<80*2;i+=2)
		{
			video_mem[24*80*2+i]=' ';
			video_mem[24*80*2+i+1]=WHITE;
		}
		curpos_x=0;curpos_y=24;
	}
	unsigned short index=curpos_y*80+curpos_x;

	outb(0x3D4,0x0F);
	outb(0x3D5,(unsigned short)(index & 0xFF));

	outb(0x3D4,0x0E);
	outb(0x3D5,(unsigned short)((index>>8)) & 0xFF);
}

void clear()
{
	volatile char* video_mem=(volatile char*)0xB8000;

	for (unsigned short i=0;i<26;i++)
	{
		for (unsigned short j=0;j<81;j++)
		{
			unsigned short index=i*80+j;
			video_mem[index*2]=' ';
			video_mem[index*2+1]=WHITE;
		}
	}
	curpos_x=0;curpos_y=0;
	curpos(curpos_x,curpos_y);
}

void printc(char c,unsigned char color)
{
	volatile char* video_mem=(volatile char*)0xB8000;

	video_mem[(curpos_y*80+curpos_x)*2]=c;
	video_mem[(curpos_y*80+curpos_x)*2+1]=color;
	curpos_x+=1;
	curpos(curpos_x,curpos_y);
}

void println(const char* string,unsigned char color)
{
	volatile char* video_mem=(volatile char*)0xB8000;

	for (int i=0;string[i]!='\0';i++)
	{
		unsigned short index=curpos_y*80+curpos_x;
		video_mem[(index+i)*2]=string[i];
		video_mem[(index+i)*2+1]=color;
	}
	curpos_x=0;
	curpos(curpos_x,curpos_y+=1);
}

void reboot()
{
	clear();
	println("System rebooting",WHITE);

	while (inb(0x64) & 0x02);

	outb(0x64,0xFE);
	println("Cannot be reboot",WHITE);
}

void shutdown()
{
	clear();
	println("System shutdown",WHITE);
	outb(0x604, 0x2000);
	outb(0x4004, 0x3400);
	outb(0xB004, 0x2000);
	outb(0xB2, 0xBE);

	println("Cannot be shutdown",WHITE);
	println("Try reboot",WHITE);
	reboot();
}

void adduser()
{
}

extern void help();

struct comd commands[]={{"help",help},{"clear",clear},{"shutdown",shutdown},{"reboot",reboot},{"adduser",adduser}};

void help()
{
	curpos_x=0;
	curpos(curpos_x,curpos_y+=1);
	for (unsigned int i=0;i<sizeof(commands)/sizeof(struct comd);i++)
	{
		println(commands[i].func_name,curcolor);
	}
}

void cmd_handler()
{
	unsigned char buff_size=0;
	unsigned char is_uc=0x01;
	for (int i=0;cmd_buffer[i]!='\0';i++){buff_size+=1;}

	for (unsigned int i=0;i<sizeof(commands) / sizeof(struct comd);i++)
	{
		unsigned char is_func=0x01;
		for (int j=0;cmd_buffer[j]!='\0' || commands[i].func_name[j]!='\0';j++)
		{
			if (cmd_buffer[j]!=commands[i].func_name[j]){is_func=0x00;break;}
		}
		unsigned char com_size=0;
		for (int j=0;commands[i].func_name[j]!='\0';j++){com_size+=1;}

		if (is_func==0x01 && buff_size>=com_size){commands[i].func();is_uc=0x00;break;}
	}
	if (is_uc==0x01){println("Unknow command",curcolor);}
	for (int i=0;i<128;i++){cmd_buffer[i]=' ';}
	cp_buff_cur=0;
	printc('>',curcolor);
}

void keyboard_read()
{
	unsigned char scancode=inb(0x60);
	
	if (!(scancode &0x80))
	{
		char c=keys[scancode];
		
		if (c>0 && scancode!=0x1C)
		{
			printc(c,curcolor);
			curpos(curpos_x,curpos_y);
			if (cp_buff_cur<127)
			{
				cmd_buffer[cp_buff_cur]=c;
				cp_buff_cur+=1;
				cmd_buffer[cp_buff_cur+1]='\0';
			}
			else
			{
				cp_buff_cur=0;
				cmd_buffer[cp_buff_cur]=c;
				cp_buff_cur+=1;
				cmd_buffer[cp_buff_cur]='\0';
			}
		}
		else
		{
			if (scancode==0x1C)
			{
				curpos_x=0;curpos(curpos_x,curpos_y+=1);
				cmd_handler();
			}
			if (scancode==0x0E && curpos_x>1)
			{
				curpos(curpos_x-=1,curpos_y);printc(' ',curcolor);curpos(curpos_x-=1,curpos_y);
				cp_buff_cur-=1;
				cmd_buffer[cp_buff_cur]='\0';
			}
		}
	}	
	outb(0x20,0x20);
}

void kernel_main()
{
	clear();
	println("LainOS",curcolor);
	printc('>',curcolor);

	while (1)
	{
		asm volatile("hlt");
	}
}

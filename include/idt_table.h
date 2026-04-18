struct s_idte
{
	unsigned short offl;
	unsigned short selcts;
	unsigned char ist;
	unsigned char tpatt;
	unsigned short offm;
	unsigned int offt;
	unsigned int res;
}__attribute__((packed));

struct s_idtt
{
	unsigned short limit;
	unsigned long long bs;
}__attribute__((packed));

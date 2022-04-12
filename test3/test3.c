#include "../lib/lib.c"

#define MAX_SYMBUF_LEN		64


char symbuf[MAX_SYMBUF_LEN];
char *linebufp;


int getnum(char *str)
{
	int numb, neg;
	
	numb = 0;
	if(*str=='-'){
		neg = -1;
		++str;
	}
	else{
		neg = 1;
	}
	if(memcmp(str, "0x", 2)==0){	// 0x ...
		str += 2;
		for(;;){
			if(*str>='0' && *str<='9'){
				numb = numb*16 + *str - '0';
			}
			else if(*str>='A' && *str<='F'){
				numb = numb*16 + *str -'A' +10;
			}
			else if(*str>='a' && *str<='f'){
				numb = numb*16 + *str -'a' +10;
			}
			else{
				return numb*neg;
			}
			str++;
		}
	}
	else{
		for(;;){
			if(*str>='0' && *str<='9'){
				numb = numb*10 + *str - '0';
			}
			else{
				return numb*neg;
			}
			str++;
		}
	}
}

void spskip()
{
	while(*linebufp==' ' || *linebufp=='\t'){
		linebufp++;
	}
}

void getsym()
{
	int i;
	
	spskip();
	for(i=0; i<MAX_SYMBUF_LEN-1; i++){
		if(*linebufp==' ' || *linebufp=='\t' || *linebufp==0 || *linebufp=='\n'){
			symbuf[i] = 0;
			return;
		}
		symbuf[i] = *linebufp++;
	}
	symbuf[i] = 0;
}

void help()
{
	putch('\n');
	puts("> help                    ... help message\n");
	puts("> dump <addr> <len>       ... hex dump\n");
	puts("> exit                    ... exit\n");
	putch('\n');
}

void dump(unsigned int addr, unsigned int len)
{
	int i, pre_len;
	
	if(len==0){
		len = 16;
	}
	pre_len = (addr & 0xf);
	addr &= 0xfffffff0;
	puts("addr:  0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n");
	if(pre_len){
		printf_4x(addr);
		putch(':');
		for(i=0; i<pre_len; i++){
			puts("   ");
		}
		for( ; i<16; i++){
			putch(' ');
			printf_2x(*(char*)(addr+i));
			if(--len==0){
				putch('\n');
				return;
			}
		}
		putch('\n');
		addr += 16;
	}
	for(;;){
		printf_4x(addr);
		putch(':');
		for(i=0; i<16; i++){
			putch(' ');
			printf_2x(*(char*)(addr+i));
			if(--len==0){
				putch('\n');
				return;
			}
		}
		putch('\n');
		addr += 16;
	}
}

int main()
{
	char buf[256];
	unsigned int addr, len;
	
	puts("\nhelp message is \"help\"\n\n");
	for(;;){
		puts("> ");
		gets(buf, 256);
		linebufp = buf;
		getsym();
		if(strcmp(symbuf, "help")==0){
			help();
		}
		else if(strcmp(symbuf, "dump")==0){
			getsym();
			addr = getnum(symbuf);
			getsym();
			len  = getnum(symbuf);
			dump(addr, len);
		}
		else if(strcmp(symbuf, "exit")==0){
			return 0;
		}
		else{
			puts("*** error\n");
		}
	}
}

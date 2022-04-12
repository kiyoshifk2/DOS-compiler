
//	1文字キー入力
int getchA()
{
	_asm{
		;ah=0x01: エコーあり Ctrl-C チェックあり
		;ah=0x06: エコーあり Ctrl-Cチェックなし
		;ah=0x07: エコーなし Ctrl-Cチェックなし
		;ah=0x08: エコーなし Ctrl-Cチェックあり
		mov		ah,0x01
		int		0x21
		xor		ebx,ebx
		mov		bl,al
		mov		eax,ebx
	}
}

int getch()
{
	int c;
	
	while((c=getchA())==0)
		;
	if(c=='\r'){
		c = '\n';
	}
	return c;
}

void putchA(char c)
{
	_asm{
		mov		dl,[ebp+_putch@_c]
		mov		ah,0x02
		int		0x21
	}
}

void putch(char c)
{
	if(c=='\n'){
		putchA('\r');
	}
	putchA(c);
}

void gets(char *str, int len)
{
	int i;
	
	for(i=0; i<len; i++){
		if((*str=getch())=='\n'){
			putch('\n');
			*str = 0;
			return;
		}
		if(*str=='\b'){
			if(i){
				--i;
				--str;
			}
			--i;
			--str;
		}
		++str;
	}
}

void puts(char *str)
{
	while(*str){
		putch(*str++);
	}
}

void exit(char code)
{
	_asm{
		mov		al,[ebp+_exit@_code]
		mov		ah,0x4c
		int		0x21
	}
}

void printf_1x(unsigned int d)
{
	d &= 0x0000000f;
	if(d < 10){
		putch('0' + d);
	}
	else{
		putch('A' + d - 10);
	}
}

void printf_2x(unsigned int d)
{
	printf_1x(d/16 & 0xf);
	printf_1x(d & 0xf);
}

void printf_4x(unsigned int d)
{
	printf_2x(d/0x100 & 0xff);
	printf_2x(d & 0xff);
}

void printf_8x(unsigned int d)
{
	printf_4x(d/0x10000 & 0xffff);
	printf_4x(d & 0xffff);
}

void printf_x(unsigned int d)
{
	if(d / 16){
		printf_x(d/16);
	}
	printf_1x(d);
}

void printf_u(unsigned int d)
{
	
	if(d / 10){
		printf_u(d / 10);
	}
	putch('0' + d%10);
}

void printf_d(int d)
{
	if(d < 0){
		putch('-');
		d = -d;
	}
	printf_u(d);
}

int memcmp(void *dst, void *src, unsigned int size)
{
	int i;
	int tmp;
	unsigned char *dst1;
	unsigned char *src1;
	
	dst1 = (unsigned char *)dst;
	src1 = (unsigned char *)src;
	for(i=0; i<size; ++i){
		if((tmp = dst1[i]-src1[i])){
			return tmp;
		}
	}
	return 0;
}

int strcmp(char *dst, char *src)
{
	int i;
	int tmp;
	
	for(i=0; ; ++i){
		if((tmp = (unsigned char)dst[i]-(unsigned char)src[i]))
			return tmp;
		if(src[i]==0)
			return 0;
	}
}

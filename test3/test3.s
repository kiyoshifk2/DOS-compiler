[BITS 16]
	org		0x100
	jmp		startup
;   0:   /*#$& test3.c */
;   0:   /*#$& ../lib/lib.c */
;   1: 
;   2: //	1文字キー入力
;   3: int getchA()
;---------- function _getchA ----------
_getchA:
	push	ebp
	mov		ebp, esp
;---------- parser_function_block ----------
;   4: {
;---------- parser_asm ----------
;   5: 	_asm{
		;ah=0x01: エコーあり Ctrl-C チェックあり
		;ah=0x06: エコーあり Ctrl-Cチェックなし
		;ah=0x07: エコーなし Ctrl-Cチェックなし
		;ah=0x08: エコーなし Ctrl-Cチェックあり
		mov		ah,0x01
		int		0x21
		xor		ebx,ebx
		mov		bl,al
		mov		eax,ebx
L2416:
;  16: }
	pop		ebp
	ret
;  17: 
;  18: int getch()
_getch@_c	equ	0
;---------- function _getch ----------
_getch:
	push	ebp
	add		esp, -4		; auto variable
	mov		ebp, esp
;---------- parser_function_block ----------
;  19: {
;  20: 	int c;
;---------- parser_while ----------
;  21: 	
;  22: 	while((c=getchA())==0)
L2418:
;---------- c32_expr ----------
;---------- c32_expr ----------
;---------- call _getchA ----------
	call	_getchA		;expr_function_call
	mov		[ebp+_getch@_c], eax	;expr_eq
;---------- c32_expr end ------
	mov		eax, [ebp+_getch@_c]		;convert_com20
	push	eax		;factor8
	mov		eax, 0x0		;convert_com20
	pop		ebx		;factor8
	cmp		eax, ebx	;factor8
	mov		eax, 1	;factor8-9
	jz		L2424		;factor8-10
	dec		eax		;factor8-11
L2424:		;factor8
;---------- c32_expr end ------
	and		eax, eax
	jz		L2417
L2419:
;---------- parser_block ----------
;  23: 		;
	jmp		L2418
L2417:
;---------- parser_if ----------
;  24: 	if(c=='\r'){
;---------- c32_expr ----------
	mov		eax, [ebp+_getch@_c]		;convert_com20
	push	eax		;factor8
	mov		eax, 0xd		;convert_com20
	pop		ebx		;factor8
	cmp		eax, ebx	;factor8
	mov		eax, 1	;factor8-9
	jz		L2453		;factor8-10
	dec		eax		;factor8-11
L2453:		;factor8
;---------- c32_expr end ------
	and		eax, eax
	jz		L2447
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  25: 		c = '\n';
	mov		eax, 0xa		;convert_com20
	mov		[ebp+_getch@_c], eax	;expr_eq
;---------- c32_expr end ------
;  26: 	}
	jmp		L2448
L2447:
L2448:
;----------  parser_return  ---------
;  27: 	return c;
;---------- c32_expr ----------
;---------- c32_expr end ------
	mov		eax, [ebp+_getch@_c]		;convert_com20
	add		esp, 4		; auto variable clear
	pop		ebp
	ret
;  28: }
	add		esp, 4		; auto variable clear
	pop		ebp
	ret
;  29: 
;  30: void putchA(char c)
_putchA@_c	equ	6
;---------- function _putchA ----------
_putchA:
	push	ebp
	mov		ebp, esp
;---------- parser_function_block ----------
;  31: {
;---------- parser_asm ----------
;  32: 	_asm{
		mov		dl,[ebp+_putch@_c]
		mov		ah,0x02
		int		0x21
L2484:
;  37: }
	pop		ebp
	ret
;  38: 
;  39: void putch(char c)
_putch@_c	equ	6
;---------- function _putch ----------
_putch:
	push	ebp
	mov		ebp, esp
;---------- parser_function_block ----------
;  40: {
;---------- parser_if ----------
;  41: 	if(c=='\n'){
;---------- c32_expr ----------
	mov		al, [ebp+_putch@_c]		;convert_com20
	cbw		;convert_com10
	db 0x66
	db 0x98		;convert_com10
	push	eax		;factor8
	mov		eax, 0xa		;convert_com20
	pop		ebx		;factor8
	cmp		eax, ebx	;factor8
	mov		eax, 1	;factor8-9
	jz		L2491		;factor8-10
	dec		eax		;factor8-11
L2491:		;factor8
;---------- c32_expr end ------
	and		eax, eax
	jz		L2485
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  42: 		putchA('\r');
;---------- call _putchA ----------
	sub		esp, 0x4	;expr_function_call
	mov		eax, 0xd		;convert_com20
	mov		[esp+0x0], eax		;expr_function_call
	call	_putchA		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
;  43: 	}
	jmp		L2486
L2485:
L2486:
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  44: 	putchA(c);
;---------- call _putchA ----------
	sub		esp, 0x4	;expr_function_call
	mov		al, [ebp+_putch@_c]		;convert_com20
	cbw		;convert_com10
	db 0x66
	db 0x98		;convert_com10
	mov		[esp+0x0], eax		;expr_function_call
	call	_putchA		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
;  45: }
	pop		ebp
	ret
;  46: 
;  47: void gets(char *str, int len)
_gets@_str	equ	10
_gets@_len	equ	14
_gets@_i	equ	0
;---------- function _gets ----------
_gets:
	push	ebp
	add		esp, -4		; auto variable
	mov		ebp, esp
;---------- parser_function_block ----------
;  48: {
;  49: 	int i;
;---------- parser_for ----------
;  50: 	
;  51: 	for(i=0; i<len; i++){
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
	mov		eax, 0x0		;convert_com20
	mov		[ebp+_gets@_i], eax	;expr_eq
;---------- c32_expr end ------
L2532:
;---------- c32_expr ----------
	mov		eax, [ebp+_gets@_i]		;convert_com20
	push	eax		;factor9
	mov		eax, [ebp+_gets@_len]		;convert_com20
	pop		ebx		;factor9
	cmp		ebx, eax	;factor9
	mov		eax, 1	;factor9
	jl		L2555		;factor9
	dec		eax		;factor9
L2555:		;factor8
;---------- c32_expr end ------
	and		eax, eax
	jz		L2530
	jmp		L2533
L2531:
;---------- c32_expr ----------
	mov		ebx, [ebp+_gets@_i]		;factor13
	add		dword	[ebp+_gets@_i], 0x1	;factor13
	mov		eax, ebx	;factor13
;---------- c32_expr end ------
	jmp		L2532
L2533:
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_if ----------
;  52: 		if((*str=getch())=='\n'){
;---------- c32_expr ----------
;---------- c32_expr ----------
	mov		eax, [ebp+_gets@_str]		;convert_com20
	push	eax		;factor1
;---------- call _getch ----------
	call	_getch		;expr_function_call
	mov		ebx, eax	;expr_eq
	pop		eax		;expr_eq
	mov		[eax], bl	;expr_eq
;---------- c32_expr end ------
	mov		al, [eax]		;convert_com20
	cbw		;convert_com10
	db 0x66
	db 0x98		;convert_com10
	push	eax		;factor8
	mov		eax, 0xa		;convert_com20
	pop		ebx		;factor8
	cmp		eax, ebx	;factor8
	mov		eax, 1	;factor8-9
	jz		L2574		;factor8-10
	dec		eax		;factor8-11
L2574:		;factor8
;---------- c32_expr end ------
	and		eax, eax
	jz		L2568
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  53: 			putch('\n');
;---------- call _putch ----------
	sub		esp, 0x4	;expr_function_call
	mov		eax, 0xa		;convert_com20
	mov		[esp+0x0], eax		;expr_function_call
	call	_putch		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  54: 			*str = 0;
	mov		eax, [ebp+_gets@_str]		;convert_com20
	push	eax		;factor1
	mov		eax, 0x0		;convert_com20
	mov		ebx, eax	;expr_eq
	pop		eax		;expr_eq
	mov		[eax], bl	;expr_eq
;---------- c32_expr end ------
;----------  parser_return  ---------
;  55: 			return;
	add		esp, 4		; auto variable clear
	pop		ebp
	ret
;  56: 		}
	jmp		L2569
L2568:
L2569:
;---------- parser_if ----------
;  57: 		if(*str=='\b'){
;---------- c32_expr ----------
	mov		eax, [ebp+_gets@_str]		;convert_com20
	mov		al, [eax]		;convert_com20
	cbw		;convert_com10
	db 0x66
	db 0x98		;convert_com10
	push	eax		;factor8
	mov		eax, 0x8		;convert_com20
	pop		ebx		;factor8
	cmp		eax, ebx	;factor8
	mov		eax, 1	;factor8-9
	jz		L2639		;factor8-10
	dec		eax		;factor8-11
L2639:		;factor8
;---------- c32_expr end ------
	and		eax, eax
	jz		L2633
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_if ----------
;  58: 			if(i){
;---------- c32_expr ----------
;---------- c32_expr end ------
	mov		eax, [ebp+_gets@_i]		;convert_com20
	and		eax, eax
	jz		L2648
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  59: 				--i;
	sub		dword	[ebp+_gets@_i], 0x1	;factor13
;---------- c32_expr end ------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  60: 				--str;
	sub		dword	[ebp+_gets@_str], 0x1	;factor13
;---------- c32_expr end ------
;  61: 			}
	jmp		L2649
L2648:
L2649:
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  62: 			--i;
	sub		dword	[ebp+_gets@_i], 0x1	;factor13
;---------- c32_expr end ------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  63: 			--str;
	sub		dword	[ebp+_gets@_str], 0x1	;factor13
;---------- c32_expr end ------
;  64: 		}
	jmp		L2634
L2633:
L2634:
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  65: 		++str;
	add		dword	[ebp+_gets@_str], 0x1	;factor13
;---------- c32_expr end ------
;  66: 	}
	jmp		L2531
L2530:
;  67: }
	add		esp, 4		; auto variable clear
	pop		ebp
	ret
;  68: 
;  69: void puts(char *str)
_puts@_str	equ	6
;---------- function _puts ----------
_puts:
	push	ebp
	mov		ebp, esp
;---------- parser_function_block ----------
;  70: {
;---------- parser_while ----------
;  71: 	while(*str){
L2709:
;---------- c32_expr ----------
	mov		eax, [ebp+_puts@_str]		;convert_com20
;---------- c32_expr end ------
	mov		al, [eax]		;convert_com20
	cbw		;convert_com10
	db 0x66
	db 0x98		;convert_com10
	and		eax, eax
	jz		L2708
L2710:
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  72: 		putch(*str++);
;---------- call _putch ----------
	sub		esp, 0x4	;expr_function_call
	mov		ebx, [ebp+_puts@_str]		;factor13
	add		dword	[ebp+_puts@_str], 0x1	;factor13
	mov		eax, ebx	;factor13
	mov		al, [eax]		;convert_com20
	cbw		;convert_com10
	db 0x66
	db 0x98		;convert_com10
	mov		[esp+0x0], eax		;expr_function_call
	call	_putch		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
;  73: 	}
	jmp		L2709
L2708:
;  74: }
	pop		ebp
	ret
;  75: 
;  76: void exit(char code)
_exit@_code	equ	6
;---------- function _exit ----------
_exit:
	push	ebp
	mov		ebp, esp
;---------- parser_function_block ----------
;  77: {
;---------- parser_asm ----------
;  78: 	_asm{
		mov		al,[ebp+_exit@_code]
		mov		ah,0x4c
		int		0x21
L2739:
;  83: }
	pop		ebp
	ret
;  84: 
;  85: void printf_1x(unsigned int d)
_printf_1x@_d	equ	6
;---------- function _printf_1x ----------
_printf_1x:
	push	ebp
	mov		ebp, esp
;---------- parser_function_block ----------
;  86: {
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  87: 	d &= 0x0000000f;
	mov		eax, 0xf		;convert_com20
	and		[ebp+_printf_1x@_d], eax	;expr_and_eq
;---------- c32_expr end ------
;---------- parser_if ----------
;  88: 	if(d < 10){
;---------- c32_expr ----------
	mov		eax, [ebp+_printf_1x@_d]		;convert_com20
	push	eax		;factor9
	mov		eax, 0xa		;convert_com20
	pop		ebx		;factor9
	cmp		ebx, eax	;factor9
	mov		eax, 1	;factor9
	jb		L2763		;factor9
	dec		eax		;factor9
L2763:		;factor8
;---------- c32_expr end ------
	and		eax, eax
	jz		L2756
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  89: 		putch('0' + d);
;---------- call _putch ----------
	sub		esp, 0x4	;expr_function_call
	mov		eax, [ebp+_printf_1x@_d]		;convert_com20
	mov		ebx, eax	;factor11
	mov		eax, 0x30		;convert_com20
	add		eax, ebx	;factor11
	mov		[esp+0x0], eax		;expr_function_call
	call	_putch		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
;  90: 	}
	jmp		L2757
L2756:
;  91: 	else{
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  92: 		putch('A' + d - 10);
;---------- call _putch ----------
	sub		esp, 0x4	;expr_function_call
	mov		eax, [ebp+_printf_1x@_d]		;convert_com20
	mov		ebx, eax	;factor11
	mov		eax, 0x41		;convert_com20
	add		eax, ebx	;factor11
	push	eax		;factor11
	mov		eax, 0xa		;convert_com20
	pop		ebx		;factor11
	sub		ebx, eax	;factor11
	mov		eax, ebx	;factor11
	mov		[esp+0x0], eax		;expr_function_call
	call	_putch		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
;  93: 	}
L2757:
;  94: }
	pop		ebp
	ret
;  95: 
;  96: void printf_2x(unsigned int d)
_printf_2x@_d	equ	6
;---------- function _printf_2x ----------
_printf_2x:
	push	ebp
	mov		ebp, esp
;---------- parser_function_block ----------
;  97: {
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  98: 	printf_1x(d/16 & 0xf);
;---------- call _printf_1x ----------
	sub		esp, 0x4	;expr_function_call
	mov		eax, 0x10		;convert_com20
	mov		ebx, eax	;factor12
	mov		eax, [ebp+_printf_2x@_d]		;convert_com20
	xor		edx, edx	;factor12
	div		ebx		;factor12
	and		eax, 0xf	;factor7
	mov		[esp+0x0], eax		;expr_function_call
	call	_printf_1x		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  99: 	printf_1x(d & 0xf);
;---------- call _printf_1x ----------
	sub		esp, 0x4	;expr_function_call
	mov		eax, 0xf		;convert_com20
	mov		ebx, eax	;factor7
	mov		eax, [ebp+_printf_2x@_d]		;convert_com20
	and		eax, ebx	;factor7
	mov		[esp+0x0], eax		;expr_function_call
	call	_printf_1x		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
; 100: }
	pop		ebp
	ret
; 101: 
; 102: void printf_4x(unsigned int d)
_printf_4x@_d	equ	6
;---------- function _printf_4x ----------
_printf_4x:
	push	ebp
	mov		ebp, esp
;---------- parser_function_block ----------
; 103: {
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 104: 	printf_2x(d/0x100 & 0xff);
;---------- call _printf_2x ----------
	sub		esp, 0x4	;expr_function_call
	mov		eax, 0x100		;convert_com20
	mov		ebx, eax	;factor12
	mov		eax, [ebp+_printf_4x@_d]		;convert_com20
	xor		edx, edx	;factor12
	div		ebx		;factor12
	and		eax, 0xff	;factor7
	mov		[esp+0x0], eax		;expr_function_call
	call	_printf_2x		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 105: 	printf_2x(d & 0xff);
;---------- call _printf_2x ----------
	sub		esp, 0x4	;expr_function_call
	mov		eax, 0xff		;convert_com20
	mov		ebx, eax	;factor7
	mov		eax, [ebp+_printf_4x@_d]		;convert_com20
	and		eax, ebx	;factor7
	mov		[esp+0x0], eax		;expr_function_call
	call	_printf_2x		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
; 106: }
	pop		ebp
	ret
; 107: 
; 108: void printf_8x(unsigned int d)
_printf_8x@_d	equ	6
;---------- function _printf_8x ----------
_printf_8x:
	push	ebp
	mov		ebp, esp
;---------- parser_function_block ----------
; 109: {
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 110: 	printf_4x(d/0x10000 & 0xffff);
;---------- call _printf_4x ----------
	sub		esp, 0x4	;expr_function_call
	mov		eax, 0x10000		;convert_com20
	mov		ebx, eax	;factor12
	mov		eax, [ebp+_printf_8x@_d]		;convert_com20
	xor		edx, edx	;factor12
	div		ebx		;factor12
	and		eax, 0xffff	;factor7
	mov		[esp+0x0], eax		;expr_function_call
	call	_printf_4x		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 111: 	printf_4x(d & 0xffff);
;---------- call _printf_4x ----------
	sub		esp, 0x4	;expr_function_call
	mov		eax, 0xffff		;convert_com20
	mov		ebx, eax	;factor7
	mov		eax, [ebp+_printf_8x@_d]		;convert_com20
	and		eax, ebx	;factor7
	mov		[esp+0x0], eax		;expr_function_call
	call	_printf_4x		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
; 112: }
	pop		ebp
	ret
; 113: 
; 114: void printf_x(unsigned int d)
_printf_x@_d	equ	6
;---------- function _printf_x ----------
_printf_x:
	push	ebp
	mov		ebp, esp
;---------- parser_function_block ----------
; 115: {
;---------- parser_if ----------
; 116: 	if(d / 16){
;---------- c32_expr ----------
	mov		eax, 0x10		;convert_com20
	mov		ebx, eax	;factor12
	mov		eax, [ebp+_printf_x@_d]		;convert_com20
	xor		edx, edx	;factor12
	div		ebx		;factor12
;---------- c32_expr end ------
	and		eax, eax
	jz		L2944
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 117: 		printf_x(d/16);
;---------- call _printf_x ----------
	sub		esp, 0x4	;expr_function_call
	mov		eax, 0x10		;convert_com20
	mov		ebx, eax	;factor12
	mov		eax, [ebp+_printf_x@_d]		;convert_com20
	xor		edx, edx	;factor12
	div		ebx		;factor12
	mov		[esp+0x0], eax		;expr_function_call
	call	_printf_x		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
; 118: 	}
	jmp		L2945
L2944:
L2945:
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 119: 	printf_1x(d);
;---------- call _printf_1x ----------
	sub		esp, 0x4	;expr_function_call
	mov		eax, [ebp+_printf_x@_d]		;convert_com20
	mov		[esp+0x0], eax		;expr_function_call
	call	_printf_1x		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
; 120: }
	pop		ebp
	ret
; 121: 
; 122: void printf_u(unsigned int d)
_printf_u@_d	equ	6
;---------- function _printf_u ----------
_printf_u:
	push	ebp
	mov		ebp, esp
;---------- parser_function_block ----------
; 123: {
;---------- parser_if ----------
; 124: 	
; 125: 	if(d / 10){
;---------- c32_expr ----------
	mov		eax, 0xa		;convert_com20
	mov		ebx, eax	;factor12
	mov		eax, [ebp+_printf_u@_d]		;convert_com20
	xor		edx, edx	;factor12
	div		ebx		;factor12
;---------- c32_expr end ------
	and		eax, eax
	jz		L2990
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 126: 		printf_u(d / 10);
;---------- call _printf_u ----------
	sub		esp, 0x4	;expr_function_call
	mov		eax, 0xa		;convert_com20
	mov		ebx, eax	;factor12
	mov		eax, [ebp+_printf_u@_d]		;convert_com20
	xor		edx, edx	;factor12
	div		ebx		;factor12
	mov		[esp+0x0], eax		;expr_function_call
	call	_printf_u		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
; 127: 	}
	jmp		L2991
L2990:
L2991:
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 128: 	putch('0' + d%10);
;---------- call _putch ----------
	sub		esp, 0x4	;expr_function_call
	mov		eax, 0xa		;convert_com20
	mov		ebx, eax	;factor12
	mov		eax, [ebp+_printf_u@_d]		;convert_com20
	xor		edx, edx	;factor12
	div		ebx		;factor12
	mov		eax, edx	;factor12
	mov		ebx, eax	;factor11
	mov		eax, 0x30		;convert_com20
	add		eax, ebx	;factor11
	mov		[esp+0x0], eax		;expr_function_call
	call	_putch		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
; 129: }
	pop		ebp
	ret
; 130: 
; 131: void printf_d(int d)
_printf_d@_d	equ	6
;---------- function _printf_d ----------
_printf_d:
	push	ebp
	mov		ebp, esp
;---------- parser_function_block ----------
; 132: {
;---------- parser_if ----------
; 133: 	if(d < 0){
;---------- c32_expr ----------
	mov		eax, [ebp+_printf_d@_d]		;convert_com20
	push	eax		;factor9
	mov		eax, 0x0		;convert_com20
	pop		ebx		;factor9
	cmp		ebx, eax	;factor9
	mov		eax, 1	;factor9
	jl		L3047		;factor9
	dec		eax		;factor9
L3047:		;factor8
;---------- c32_expr end ------
	and		eax, eax
	jz		L3040
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 134: 		putch('-');
;---------- call _putch ----------
	sub		esp, 0x4	;expr_function_call
	mov		eax, 0x2d		;convert_com20
	mov		[esp+0x0], eax		;expr_function_call
	call	_putch		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 135: 		d = -d;
	mov		eax, [ebp+_printf_d@_d]		;convert_com20
	neg	 	eax		;factor13
	mov		[ebp+_printf_d@_d], eax	;expr_eq
;---------- c32_expr end ------
; 136: 	}
	jmp		L3041
L3040:
L3041:
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 137: 	printf_u(d);
;---------- call _printf_u ----------
	sub		esp, 0x4	;expr_function_call
	mov		eax, [ebp+_printf_d@_d]		;convert_com20
	mov		[esp+0x0], eax		;expr_function_call
	call	_printf_u		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
; 138: }
	pop		ebp
	ret
; 139: 
; 140: int memcmp(void *dst, void *src, unsigned int size)
_memcmp@_dst	equ	22
_memcmp@_src	equ	26
_memcmp@_size	equ	30
_memcmp@_dst1	equ	0
_memcmp@_src1	equ	4
_memcmp@_i	equ	8
_memcmp@_tmp	equ	12
;---------- function _memcmp ----------
_memcmp:
	push	ebp
	add		esp, -16		; auto variable
	mov		ebp, esp
;---------- parser_function_block ----------
; 141: {
; 142: 	int i;
; 143: 	int tmp;
; 144: 	unsigned char *dst1;
; 145: 	unsigned char *src1;
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 146: 	
; 147: 	dst1 = (unsigned char *)dst;
	mov		eax, [ebp+_memcmp@_dst]		;convert_com20
	mov		ebx, eax	;factor13
	xor		eax, eax	;factor13
	mov		eax, ebx	;factor13
	mov		[ebp+_memcmp@_dst1], eax	;expr_eq
;---------- c32_expr end ------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 148: 	src1 = (unsigned char *)src;
	mov		eax, [ebp+_memcmp@_src]		;convert_com20
	mov		ebx, eax	;factor13
	xor		eax, eax	;factor13
	mov		eax, ebx	;factor13
	mov		[ebp+_memcmp@_src1], eax	;expr_eq
;---------- c32_expr end ------
;---------- parser_for ----------
; 149: 	for(i=0; i<size; ++i){
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
	mov		eax, 0x0		;convert_com20
	mov		[ebp+_memcmp@_i], eax	;expr_eq
;---------- c32_expr end ------
L3140:
;---------- c32_expr ----------
	mov		eax, [ebp+_memcmp@_i]		;convert_com20
	push	eax		;factor9
	mov		eax, [ebp+_memcmp@_size]		;convert_com20
	pop		ebx		;factor9
	cmp		ebx, eax	;factor9
	mov		eax, 1	;factor9
	jl		L3163		;factor9
	dec		eax		;factor9
L3163:		;factor8
;---------- c32_expr end ------
	and		eax, eax
	jz		L3138
	jmp		L3141
L3139:
;---------- c32_expr ----------
	add		dword	[ebp+_memcmp@_i], 0x1	;factor13
;---------- c32_expr end ------
	mov		eax, [ebp+_memcmp@_i]		;convert_com20
	jmp		L3140
L3141:
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_if ----------
; 150: 		if((tmp = dst1[i]-src1[i])){
;---------- c32_expr ----------
;---------- c32_expr ----------
	mov		eax, [ebp+_memcmp@_dst1]		;convert_com20
	push	eax		;factor15
;---------- c32_expr ----------
;---------- c32_expr end ------
	mov		eax, [ebp+_memcmp@_i]		;convert_com20
	mov		ebx, 0x1	;factor15
	mul		ebx		;factor15
	pop		ebx		;factor15
	add		eax, ebx	;factor15
	sub		ebx, ebx	;convert_com20
	mov		bl, [eax]		;convert_com20
	mov		eax, ebx	;convert_com20
	push	eax		;factor11
	mov		eax, [ebp+_memcmp@_src1]		;convert_com20
	push	eax		;factor15
;---------- c32_expr ----------
;---------- c32_expr end ------
	mov		eax, [ebp+_memcmp@_i]		;convert_com20
	mov		ebx, 0x1	;factor15
	mul		ebx		;factor15
	pop		ebx		;factor15
	add		eax, ebx	;factor15
	sub		ebx, ebx	;convert_com20
	mov		bl, [eax]		;convert_com20
	mov		eax, ebx	;convert_com20
	pop		ebx		;factor11
	sub		ebx, eax	;factor11
	mov		eax, ebx	;factor11
	mov		[ebp+_memcmp@_tmp], eax	;expr_eq
;---------- c32_expr end ------
;---------- c32_expr end ------
	mov		eax, [ebp+_memcmp@_tmp]		;convert_com20
	and		eax, eax
	jz		L3178
;---------- parser_block ----------
;---------- parser_function_block ----------
;----------  parser_return  ---------
; 151: 			return tmp;
;---------- c32_expr ----------
;---------- c32_expr end ------
	mov		eax, [ebp+_memcmp@_tmp]		;convert_com20
	add		esp, 16		; auto variable clear
	pop		ebp
	ret
; 152: 		}
	jmp		L3179
L3178:
L3179:
; 153: 	}
	jmp		L3139
L3138:
;----------  parser_return  ---------
; 154: 	return 0;
;---------- c32_expr ----------
;---------- c32_expr end ------
	mov		eax, 0x0		;convert_com20
	add		esp, 16		; auto variable clear
	pop		ebp
	ret
; 155: }
	add		esp, 16		; auto variable clear
	pop		ebp
	ret
; 156: 
; 157: int strcmp(char *dst, char *src)
_strcmp@_dst	equ	14
_strcmp@_src	equ	18
_strcmp@_i	equ	0
_strcmp@_tmp	equ	4
;---------- function _strcmp ----------
_strcmp:
	push	ebp
	add		esp, -8		; auto variable
	mov		ebp, esp
;---------- parser_function_block ----------
; 158: {
; 159: 	int i;
; 160: 	int tmp;
;---------- parser_for ----------
; 161: 	
; 162: 	for(i=0; ; ++i){
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
	mov		eax, 0x0		;convert_com20
	mov		[ebp+_strcmp@_i], eax	;expr_eq
;---------- c32_expr end ------
L3240:
	jmp		L3241
L3239:
;---------- c32_expr ----------
	add		dword	[ebp+_strcmp@_i], 0x1	;factor13
;---------- c32_expr end ------
	mov		eax, [ebp+_strcmp@_i]		;convert_com20
	jmp		L3240
L3241:
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_if ----------
; 163: 		if((tmp = (unsigned char)dst[i]-(unsigned char)src[i]))
;---------- c32_expr ----------
;---------- c32_expr ----------
	mov		eax, [ebp+_strcmp@_dst]		;convert_com20
	push	eax		;factor15
;---------- c32_expr ----------
;---------- c32_expr end ------
	mov		eax, [ebp+_strcmp@_i]		;convert_com20
	mov		ebx, 0x1	;factor15
	mul		ebx		;factor15
	pop		ebx		;factor15
	add		eax, ebx	;factor15
	mov		al, [eax]		;convert_com20
	cbw		;convert_com10
	db 0x66
	db 0x98		;convert_com10
	mov		ebx, eax	;factor13
	xor		eax, eax	;factor13
	mov		al, bl	;factor13
	push	eax		;factor11
	mov		eax, [ebp+_strcmp@_src]		;convert_com20
	push	eax		;factor15
;---------- c32_expr ----------
;---------- c32_expr end ------
	mov		eax, [ebp+_strcmp@_i]		;convert_com20
	mov		ebx, 0x1	;factor15
	mul		ebx		;factor15
	pop		ebx		;factor15
	add		eax, ebx	;factor15
	mov		al, [eax]		;convert_com20
	cbw		;convert_com10
	db 0x66
	db 0x98		;convert_com10
	mov		ebx, eax	;factor13
	xor		eax, eax	;factor13
	mov		al, bl	;factor13
	pop		ebx		;factor11
	sub		ebx, eax	;factor11
	mov		eax, ebx	;factor11
	mov		[ebp+_strcmp@_tmp], eax	;expr_eq
;---------- c32_expr end ------
;---------- c32_expr end ------
	mov		eax, [ebp+_strcmp@_tmp]		;convert_com20
	and		eax, eax
	jz		L3268
;---------- parser_block ----------
;----------  parser_return  ---------
; 164: 			return tmp;
;---------- c32_expr ----------
;---------- c32_expr end ------
	mov		eax, [ebp+_strcmp@_tmp]		;convert_com20
	add		esp, 8		; auto variable clear
	pop		ebp
	ret
	jmp		L3269
L3268:
L3269:
;---------- parser_if ----------
; 165: 		if(src[i]==0)
;---------- c32_expr ----------
	mov		eax, [ebp+_strcmp@_src]		;convert_com20
	push	eax		;factor15
;---------- c32_expr ----------
;---------- c32_expr end ------
	mov		eax, [ebp+_strcmp@_i]		;convert_com20
	mov		ebx, 0x1	;factor15
	mul		ebx		;factor15
	pop		ebx		;factor15
	add		eax, ebx	;factor15
	mov		al, [eax]		;convert_com20
	cbw		;convert_com10
	db 0x66
	db 0x98		;convert_com10
	push	eax		;factor8
	mov		eax, 0x0		;convert_com20
	pop		ebx		;factor8
	cmp		eax, ebx	;factor8
	mov		eax, 1	;factor8-9
	jz		L3330		;factor8-10
	dec		eax		;factor8-11
L3330:		;factor8
;---------- c32_expr end ------
	and		eax, eax
	jz		L3324
;---------- parser_block ----------
;----------  parser_return  ---------
; 166: 			return 0;
;---------- c32_expr ----------
;---------- c32_expr end ------
	mov		eax, 0x0		;convert_com20
	add		esp, 8		; auto variable clear
	pop		ebp
	ret
	jmp		L3325
L3324:
L3325:
; 167: 	}
	jmp		L3239
L3238:
; 168: }
	add		esp, 8		; auto variable clear
	pop		ebp
	ret
;   0:   /*#$& test3.c */
;   2: 
;   4: 
;   5: 
;   6: char symbuf[64];
;---------- c32_expr ----------
;---------- c32_expr end ------
_symbuf:	resb	0x40	;parser_global_array
;   7: char *linebufp;
_linebufp:	resb	0x4
;   8: 
;   9: 
;  10: int getnum(char *str)
_getnum@_str	equ	14
_getnum@_numb	equ	0
_getnum@_neg	equ	4
;---------- function _getnum ----------
_getnum:
	push	ebp
	add		esp, -8		; auto variable
	mov		ebp, esp
;---------- parser_function_block ----------
;  11: {
;  12: 	int numb, neg;
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  13: 	
;  14: 	numb = 0;
	mov		eax, 0x0		;convert_com20
	mov		[ebp+_getnum@_numb], eax	;expr_eq
;---------- c32_expr end ------
;---------- parser_if ----------
;  15: 	if(*str=='-'){
;---------- c32_expr ----------
	mov		eax, [ebp+_getnum@_str]		;convert_com20
	mov		al, [eax]		;convert_com20
	cbw		;convert_com10
	db 0x66
	db 0x98		;convert_com10
	push	eax		;factor8
	mov		eax, 0x2d		;convert_com20
	pop		ebx		;factor8
	cmp		eax, ebx	;factor8
	mov		eax, 1	;factor8-9
	jz		L3383		;factor8-10
	dec		eax		;factor8-11
L3383:		;factor8
;---------- c32_expr end ------
	and		eax, eax
	jz		L3377
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  16: 		neg = -1;
	mov		eax, 0xffffffff		;convert_com20
	mov		[ebp+_getnum@_neg], eax	;expr_eq
;---------- c32_expr end ------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  17: 		++str;
	add		dword	[ebp+_getnum@_str], 0x1	;factor13
;---------- c32_expr end ------
;  18: 	}
	jmp		L3378
L3377:
;  19: 	else{
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  20: 		neg = 1;
	mov		eax, 0x1		;convert_com20
	mov		[ebp+_getnum@_neg], eax	;expr_eq
;---------- c32_expr end ------
;  21: 	}
L3378:
;---------- parser_if ----------
;  22: 	if(memcmp(str, "0x", 2)==0){	// 0x ...
;---------- c32_expr ----------
;---------- call _memcmp ----------
	sub		esp, 0xc	;expr_function_call
	mov		eax, [ebp+_getnum@_str]		;convert_com20
	mov		[esp+0x0], eax		;expr_function_call
	jmp		L3463
L3462:
	db		0x30
	db		0x78
	db		0x0
L3463:
	mov		eax, L3462		;convert_com20
	mov		[esp+0x4], eax		;expr_function_call
	mov		eax, 0x2		;convert_com20
	mov		[esp+0x8], eax		;expr_function_call
	call	_memcmp		;expr_function_call
	add		esp, 0xc	;expr_function_call
	push	eax		;factor8
	mov		eax, 0x0		;convert_com20
	pop		ebx		;factor8
	cmp		eax, ebx	;factor8
	mov		eax, 1	;factor8-9
	jz		L3442		;factor8-10
	dec		eax		;factor8-11
L3442:		;factor8
;---------- c32_expr end ------
	and		eax, eax
	jz		L3436
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  23: 		str += 2;
	mov		eax, 0x2		;convert_com20
	add		[ebp+_getnum@_str], eax	;expr_plus_eq
;---------- c32_expr end ------
;---------- parser_for ----------
;  24: 		for(;;){
L3493:
	jmp		L3494
L3492:
	jmp		L3493
L3494:
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_if ----------
;  25: 			if(*str>='0' && *str<='9'){
;---------- c32_expr ----------
	mov		eax, [ebp+_getnum@_str]		;convert_com20
	mov		al, [eax]		;convert_com20
	cbw		;convert_com10
	db 0x66
	db 0x98		;convert_com10
	push	eax		;factor9
	mov		eax, 0x30		;convert_com20
	pop		ebx		;factor9
	cmp		ebx, eax	;factor9
	mov		eax, 1	;factor9
	jge		L3502		;factor9
	dec		eax		;factor9
L3502:		;factor8
	and		eax, eax	;factor4
	jz		L3500
	mov		eax, [ebp+_getnum@_str]		;convert_com20
	mov		al, [eax]		;convert_com20
	cbw		;convert_com10
	db 0x66
	db 0x98		;convert_com10
	push	eax		;factor9
	mov		eax, 0x39		;convert_com20
	pop		ebx		;factor9
	cmp		ebx, eax	;factor9
	mov		eax, 1	;factor9
	jle		L3511		;factor9
	dec		eax		;factor9
L3511:		;factor8
L3500:		;factor4
;---------- c32_expr end ------
	and		eax, eax
	jz		L3495
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  26: 				numb = numb*16 + *str - '0';
	mov		eax, 0x10		;convert_com20
	mov		ebx, eax	;factor12
	mov		eax, [ebp+_getnum@_numb]		;convert_com20
	imul	ebx		;factor12
	push	eax		;factor11
	mov		eax, [ebp+_getnum@_str]		;convert_com20
	mov		al, [eax]		;convert_com20
	cbw		;convert_com10
	db 0x66
	db 0x98		;convert_com10
	pop		ebx		;factor11
	add		eax, ebx	;factor11
	push	eax		;factor11
	mov		eax, 0x30		;convert_com20
	pop		ebx		;factor11
	sub		ebx, eax	;factor11
	mov		eax, ebx	;factor11
	mov		[ebp+_getnum@_numb], eax	;expr_eq
;---------- c32_expr end ------
;  27: 			}
	jmp		L3496
L3495:
;  28: 			else if(*str>='A' && *str<='F'){
;---------- parser_block ----------
;---------- parser_if ----------
;---------- c32_expr ----------
	mov		eax, [ebp+_getnum@_str]		;convert_com20
	mov		al, [eax]		;convert_com20
	cbw		;convert_com10
	db 0x66
	db 0x98		;convert_com10
	push	eax		;factor9
	mov		eax, 0x41		;convert_com20
	pop		ebx		;factor9
	cmp		ebx, eax	;factor9
	mov		eax, 1	;factor9
	jge		L3549		;factor9
	dec		eax		;factor9
L3549:		;factor8
	and		eax, eax	;factor4
	jz		L3547
	mov		eax, [ebp+_getnum@_str]		;convert_com20
	mov		al, [eax]		;convert_com20
	cbw		;convert_com10
	db 0x66
	db 0x98		;convert_com10
	push	eax		;factor9
	mov		eax, 0x46		;convert_com20
	pop		ebx		;factor9
	cmp		ebx, eax	;factor9
	mov		eax, 1	;factor9
	jle		L3558		;factor9
	dec		eax		;factor9
L3558:		;factor8
L3547:		;factor4
;---------- c32_expr end ------
	and		eax, eax
	jz		L3542
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  29: 				numb = numb*16 + *str -'A' +10;
	mov		eax, 0x10		;convert_com20
	mov		ebx, eax	;factor12
	mov		eax, [ebp+_getnum@_numb]		;convert_com20
	imul	ebx		;factor12
	push	eax		;factor11
	mov		eax, [ebp+_getnum@_str]		;convert_com20
	mov		al, [eax]		;convert_com20
	cbw		;convert_com10
	db 0x66
	db 0x98		;convert_com10
	pop		ebx		;factor11
	add		eax, ebx	;factor11
	push	eax		;factor11
	mov		eax, 0x41		;convert_com20
	pop		ebx		;factor11
	sub		ebx, eax	;factor11
	mov		eax, ebx	;factor11
	push	eax		;factor11
	mov		eax, 0xa		;convert_com20
	pop		ebx		;factor11
	add		eax, ebx	;factor11
	mov		[ebp+_getnum@_numb], eax	;expr_eq
;---------- c32_expr end ------
;  30: 			}
	jmp		L3543
L3542:
;  31: 			else if(*str>='a' && *str<='f'){
;---------- parser_block ----------
;---------- parser_if ----------
;---------- c32_expr ----------
	mov		eax, [ebp+_getnum@_str]		;convert_com20
	mov		al, [eax]		;convert_com20
	cbw		;convert_com10
	db 0x66
	db 0x98		;convert_com10
	push	eax		;factor9
	mov		eax, 0x61		;convert_com20
	pop		ebx		;factor9
	cmp		ebx, eax	;factor9
	mov		eax, 1	;factor9
	jge		L3598		;factor9
	dec		eax		;factor9
L3598:		;factor8
	and		eax, eax	;factor4
	jz		L3596
	mov		eax, [ebp+_getnum@_str]		;convert_com20
	mov		al, [eax]		;convert_com20
	cbw		;convert_com10
	db 0x66
	db 0x98		;convert_com10
	push	eax		;factor9
	mov		eax, 0x66		;convert_com20
	pop		ebx		;factor9
	cmp		ebx, eax	;factor9
	mov		eax, 1	;factor9
	jle		L3607		;factor9
	dec		eax		;factor9
L3607:		;factor8
L3596:		;factor4
;---------- c32_expr end ------
	and		eax, eax
	jz		L3591
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  32: 				numb = numb*16 + *str -'a' +10;
	mov		eax, 0x10		;convert_com20
	mov		ebx, eax	;factor12
	mov		eax, [ebp+_getnum@_numb]		;convert_com20
	imul	ebx		;factor12
	push	eax		;factor11
	mov		eax, [ebp+_getnum@_str]		;convert_com20
	mov		al, [eax]		;convert_com20
	cbw		;convert_com10
	db 0x66
	db 0x98		;convert_com10
	pop		ebx		;factor11
	add		eax, ebx	;factor11
	push	eax		;factor11
	mov		eax, 0x61		;convert_com20
	pop		ebx		;factor11
	sub		ebx, eax	;factor11
	mov		eax, ebx	;factor11
	push	eax		;factor11
	mov		eax, 0xa		;convert_com20
	pop		ebx		;factor11
	add		eax, ebx	;factor11
	mov		[ebp+_getnum@_numb], eax	;expr_eq
;---------- c32_expr end ------
;  33: 			}
	jmp		L3592
L3591:
;  34: 			else{
;---------- parser_block ----------
;---------- parser_function_block ----------
;----------  parser_return  ---------
;  35: 				return numb*neg;
;---------- c32_expr ----------
	mov		eax, [ebp+_getnum@_neg]		;convert_com20
	mov		ebx, eax	;factor12
	mov		eax, [ebp+_getnum@_numb]		;convert_com20
	imul	ebx		;factor12
;---------- c32_expr end ------
	add		esp, 8		; auto variable clear
	pop		ebp
	ret
;  36: 			}
L3592:
L3543:
L3496:
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  37: 			str++;
	mov		ebx, [ebp+_getnum@_str]		;factor13
	add		dword	[ebp+_getnum@_str], 0x1	;factor13
	mov		eax, ebx	;factor13
;---------- c32_expr end ------
;  38: 		}
	jmp		L3492
L3491:
;  39: 	}
	jmp		L3437
L3436:
;  40: 	else{
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_for ----------
;  41: 		for(;;){
L3660:
	jmp		L3661
L3659:
	jmp		L3660
L3661:
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_if ----------
;  42: 			if(*str>='0' && *str<='9'){
;---------- c32_expr ----------
	mov		eax, [ebp+_getnum@_str]		;convert_com20
	mov		al, [eax]		;convert_com20
	cbw		;convert_com10
	db 0x66
	db 0x98		;convert_com10
	push	eax		;factor9
	mov		eax, 0x30		;convert_com20
	pop		ebx		;factor9
	cmp		ebx, eax	;factor9
	mov		eax, 1	;factor9
	jge		L3669		;factor9
	dec		eax		;factor9
L3669:		;factor8
	and		eax, eax	;factor4
	jz		L3667
	mov		eax, [ebp+_getnum@_str]		;convert_com20
	mov		al, [eax]		;convert_com20
	cbw		;convert_com10
	db 0x66
	db 0x98		;convert_com10
	push	eax		;factor9
	mov		eax, 0x39		;convert_com20
	pop		ebx		;factor9
	cmp		ebx, eax	;factor9
	mov		eax, 1	;factor9
	jle		L3678		;factor9
	dec		eax		;factor9
L3678:		;factor8
L3667:		;factor4
;---------- c32_expr end ------
	and		eax, eax
	jz		L3662
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  43: 				numb = numb*10 + *str - '0';
	mov		eax, 0xa		;convert_com20
	mov		ebx, eax	;factor12
	mov		eax, [ebp+_getnum@_numb]		;convert_com20
	imul	ebx		;factor12
	push	eax		;factor11
	mov		eax, [ebp+_getnum@_str]		;convert_com20
	mov		al, [eax]		;convert_com20
	cbw		;convert_com10
	db 0x66
	db 0x98		;convert_com10
	pop		ebx		;factor11
	add		eax, ebx	;factor11
	push	eax		;factor11
	mov		eax, 0x30		;convert_com20
	pop		ebx		;factor11
	sub		ebx, eax	;factor11
	mov		eax, ebx	;factor11
	mov		[ebp+_getnum@_numb], eax	;expr_eq
;---------- c32_expr end ------
;  44: 			}
	jmp		L3663
L3662:
;  45: 			else{
;---------- parser_block ----------
;---------- parser_function_block ----------
;----------  parser_return  ---------
;  46: 				return numb*neg;
;---------- c32_expr ----------
	mov		eax, [ebp+_getnum@_neg]		;convert_com20
	mov		ebx, eax	;factor12
	mov		eax, [ebp+_getnum@_numb]		;convert_com20
	imul	ebx		;factor12
;---------- c32_expr end ------
	add		esp, 8		; auto variable clear
	pop		ebp
	ret
;  47: 			}
L3663:
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  48: 			str++;
	mov		ebx, [ebp+_getnum@_str]		;factor13
	add		dword	[ebp+_getnum@_str], 0x1	;factor13
	mov		eax, ebx	;factor13
;---------- c32_expr end ------
;  49: 		}
	jmp		L3659
L3658:
;  50: 	}
L3437:
;  51: }
	add		esp, 8		; auto variable clear
	pop		ebp
	ret
;  52: 
;  53: void spskip()
;---------- function _spskip ----------
_spskip:
	push	ebp
	mov		ebp, esp
;---------- parser_function_block ----------
;  54: {
;---------- parser_while ----------
;  55: 	while(*linebufp==' ' || *linebufp=='\t'){
L3728:
;---------- c32_expr ----------
	mov		eax, [_linebufp]		;convert_com20
	mov		al, [eax]		;convert_com20
	cbw		;convert_com10
	db 0x66
	db 0x98		;convert_com10
	push	eax		;factor8
	mov		eax, 0x20		;convert_com20
	pop		ebx		;factor8
	cmp		eax, ebx	;factor8
	mov		eax, 1	;factor8-9
	jz		L3734		;factor8-10
	dec		eax		;factor8-11
L3734:		;factor8
	and		eax, eax	;factor3
	jnz		L3732
	mov		eax, [_linebufp]		;convert_com20
	mov		al, [eax]		;convert_com20
	cbw		;convert_com10
	db 0x66
	db 0x98		;convert_com10
	push	eax		;factor8
	mov		eax, 0x9		;convert_com20
	pop		ebx		;factor8
	cmp		eax, ebx	;factor8
	mov		eax, 1	;factor8-9
	jz		L3745		;factor8-10
	dec		eax		;factor8-11
L3745:		;factor8
L3732:		;factor3
;---------- c32_expr end ------
	and		eax, eax
	jz		L3727
L3729:
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  56: 		linebufp++;
	mov		ebx, [_linebufp]		;factor13
	add		dword	[_linebufp], 0x1	;factor13
	mov		eax, ebx	;factor13
;---------- c32_expr end ------
;  57: 	}
	jmp		L3728
L3727:
;  58: }
	pop		ebp
	ret
;  59: 
;  60: void getsym()
_getsym@_i	equ	0
;---------- function _getsym ----------
_getsym:
	push	ebp
	add		esp, -4		; auto variable
	mov		ebp, esp
;---------- parser_function_block ----------
;  61: {
;  62: 	int i;
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  63: 	
;  64: 	spskip();
;---------- call _spskip ----------
	call	_spskip		;expr_function_call
;---------- c32_expr end ------
;---------- parser_for ----------
;  65: 	for(i=0; i<64-1; i++){
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
	mov		eax, 0x0		;convert_com20
	mov		[ebp+_getsym@_i], eax	;expr_eq
;---------- c32_expr end ------
L3772:
;---------- c32_expr ----------
	mov		eax, [ebp+_getsym@_i]		;convert_com20
	push	eax		;factor9
	mov		eax, 0x3f		;convert_com20
	pop		ebx		;factor9
	cmp		ebx, eax	;factor9
	mov		eax, 1	;factor9
	jl		L3795		;factor9
	dec		eax		;factor9
L3795:		;factor8
;---------- c32_expr end ------
	and		eax, eax
	jz		L3770
	jmp		L3773
L3771:
;---------- c32_expr ----------
	mov		ebx, [ebp+_getsym@_i]		;factor13
	add		dword	[ebp+_getsym@_i], 0x1	;factor13
	mov		eax, ebx	;factor13
;---------- c32_expr end ------
	jmp		L3772
L3773:
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_if ----------
;  66: 		if(*linebufp==' ' || *linebufp=='\t' || *linebufp==0 || *linebufp=='\n'){
;---------- c32_expr ----------
	mov		eax, [_linebufp]		;convert_com20
	mov		al, [eax]		;convert_com20
	cbw		;convert_com10
	db 0x66
	db 0x98		;convert_com10
	push	eax		;factor8
	mov		eax, 0x20		;convert_com20
	pop		ebx		;factor8
	cmp		eax, ebx	;factor8
	mov		eax, 1	;factor8-9
	jz		L3816		;factor8-10
	dec		eax		;factor8-11
L3816:		;factor8
	and		eax, eax	;factor3
	jnz		L3814
	mov		eax, [_linebufp]		;convert_com20
	mov		al, [eax]		;convert_com20
	cbw		;convert_com10
	db 0x66
	db 0x98		;convert_com10
	push	eax		;factor8
	mov		eax, 0x9		;convert_com20
	pop		ebx		;factor8
	cmp		eax, ebx	;factor8
	mov		eax, 1	;factor8-9
	jz		L3827		;factor8-10
	dec		eax		;factor8-11
L3827:		;factor8
	and		eax, eax	;factor3
	jnz		L3825
	mov		eax, [_linebufp]		;convert_com20
	mov		al, [eax]		;convert_com20
	cbw		;convert_com10
	db 0x66
	db 0x98		;convert_com10
	push	eax		;factor8
	mov		eax, 0x0		;convert_com20
	pop		ebx		;factor8
	cmp		eax, ebx	;factor8
	mov		eax, 1	;factor8-9
	jz		L3838		;factor8-10
	dec		eax		;factor8-11
L3838:		;factor8
	and		eax, eax	;factor3
	jnz		L3836
	mov		eax, [_linebufp]		;convert_com20
	mov		al, [eax]		;convert_com20
	cbw		;convert_com10
	db 0x66
	db 0x98		;convert_com10
	push	eax		;factor8
	mov		eax, 0xa		;convert_com20
	pop		ebx		;factor8
	cmp		eax, ebx	;factor8
	mov		eax, 1	;factor8-9
	jz		L3849		;factor8-10
	dec		eax		;factor8-11
L3849:		;factor8
L3836:		;factor3
L3825:		;factor3
L3814:		;factor3
;---------- c32_expr end ------
	and		eax, eax
	jz		L3810
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  67: 			symbuf[i] = 0;
;---------- c32_expr ----------
;---------- c32_expr end ------
	mov		eax, [ebp+_getsym@_i]		;convert_com20
	mov		ebx, 0x1	;factor15
	mul		ebx		;factor15
	add		eax, _symbuf		;factor15
	push	eax		;factor1
	mov		eax, 0x0		;convert_com20
	mov		ebx, eax	;expr_eq
	pop		eax		;expr_eq
	mov		[eax], bl	;expr_eq
;---------- c32_expr end ------
;----------  parser_return  ---------
;  68: 			return;
	add		esp, 4		; auto variable clear
	pop		ebp
	ret
;  69: 		}
	jmp		L3811
L3810:
L3811:
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  70: 		symbuf[i] = *linebufp++;
;---------- c32_expr ----------
;---------- c32_expr end ------
	mov		eax, [ebp+_getsym@_i]		;convert_com20
	mov		ebx, 0x1	;factor15
	mul		ebx		;factor15
	add		eax, _symbuf		;factor15
	push	eax		;factor1
	mov		ebx, [_linebufp]		;factor13
	add		dword	[_linebufp], 0x1	;factor13
	mov		eax, ebx	;factor13
	mov		al, [eax]		;convert_com20
	cbw		;convert_com10
	db 0x66
	db 0x98		;convert_com10
	mov		ebx, eax	;expr_eq
	pop		eax		;expr_eq
	mov		[eax], bl	;expr_eq
;---------- c32_expr end ------
;  71: 	}
	jmp		L3771
L3770:
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  72: 	symbuf[i] = 0;
;---------- c32_expr ----------
;---------- c32_expr end ------
	mov		eax, [ebp+_getsym@_i]		;convert_com20
	mov		ebx, 0x1	;factor15
	mul		ebx		;factor15
	add		eax, _symbuf		;factor15
	push	eax		;factor1
	mov		eax, 0x0		;convert_com20
	mov		ebx, eax	;expr_eq
	pop		eax		;expr_eq
	mov		[eax], bl	;expr_eq
;---------- c32_expr end ------
;  73: }
	add		esp, 4		; auto variable clear
	pop		ebp
	ret
;  74: 
;  75: void help()
;---------- function _help ----------
_help:
	push	ebp
	mov		ebp, esp
;---------- parser_function_block ----------
;  76: {
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  77: 	putch('\n');
;---------- call _putch ----------
	sub		esp, 0x4	;expr_function_call
	mov		eax, 0xa		;convert_com20
	mov		[esp+0x0], eax		;expr_function_call
	call	_putch		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  78: 	puts("> help                    ... help message\n");
;---------- call _puts ----------
	sub		esp, 0x4	;expr_function_call
	jmp		L3965
L3964:
	db		0x3e
	db		0x20
	db		0x68
	db		0x65
	db		0x6c
	db		0x70
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x2e
	db		0x2e
	db		0x2e
	db		0x20
	db		0x68
	db		0x65
	db		0x6c
	db		0x70
	db		0x20
	db		0x6d
	db		0x65
	db		0x73
	db		0x73
	db		0x61
	db		0x67
	db		0x65
	db		0xa
	db		0x0
L3965:
	mov		eax, L3964		;convert_com20
	mov		[esp+0x0], eax		;expr_function_call
	call	_puts		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  79: 	puts("> dump <addr> <len>       ... hex dump\n");
;---------- call _puts ----------
	sub		esp, 0x4	;expr_function_call
	jmp		L3983
L3982:
	db		0x3e
	db		0x20
	db		0x64
	db		0x75
	db		0x6d
	db		0x70
	db		0x20
	db		0x3c
	db		0x61
	db		0x64
	db		0x64
	db		0x72
	db		0x3e
	db		0x20
	db		0x3c
	db		0x6c
	db		0x65
	db		0x6e
	db		0x3e
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x2e
	db		0x2e
	db		0x2e
	db		0x20
	db		0x68
	db		0x65
	db		0x78
	db		0x20
	db		0x64
	db		0x75
	db		0x6d
	db		0x70
	db		0xa
	db		0x0
L3983:
	mov		eax, L3982		;convert_com20
	mov		[esp+0x0], eax		;expr_function_call
	call	_puts		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  80: 	puts("> exit                    ... exit\n");
;---------- call _puts ----------
	sub		esp, 0x4	;expr_function_call
	jmp		L4001
L4000:
	db		0x3e
	db		0x20
	db		0x65
	db		0x78
	db		0x69
	db		0x74
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x20
	db		0x2e
	db		0x2e
	db		0x2e
	db		0x20
	db		0x65
	db		0x78
	db		0x69
	db		0x74
	db		0xa
	db		0x0
L4001:
	mov		eax, L4000		;convert_com20
	mov		[esp+0x0], eax		;expr_function_call
	call	_puts		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  81: 	putch('\n');
;---------- call _putch ----------
	sub		esp, 0x4	;expr_function_call
	mov		eax, 0xa		;convert_com20
	mov		[esp+0x0], eax		;expr_function_call
	call	_putch		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
;  82: }
	pop		ebp
	ret
;  83: 
;  84: void dump(unsigned int addr, unsigned int len)
_dump@_addr	equ	14
_dump@_len	equ	18
_dump@_i	equ	0
_dump@_pre_len	equ	4
;---------- function _dump ----------
_dump:
	push	ebp
	add		esp, -8		; auto variable
	mov		ebp, esp
;---------- parser_function_block ----------
;  85: {
;  86: 	int i, pre_len;
;---------- parser_if ----------
;  87: 	
;  88: 	if(len==0){
;---------- c32_expr ----------
	mov		eax, [ebp+_dump@_len]		;convert_com20
	push	eax		;factor8
	mov		eax, 0x0		;convert_com20
	pop		ebx		;factor8
	cmp		eax, ebx	;factor8
	mov		eax, 1	;factor8-9
	jz		L4024		;factor8-10
	dec		eax		;factor8-11
L4024:		;factor8
;---------- c32_expr end ------
	and		eax, eax
	jz		L4018
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  89: 		len = 16;
	mov		eax, 0x10		;convert_com20
	mov		[ebp+_dump@_len], eax	;expr_eq
;---------- c32_expr end ------
;  90: 	}
	jmp		L4019
L4018:
L4019:
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  91: 	pre_len = (addr & 0xf);
;---------- c32_expr ----------
	mov		eax, 0xf		;convert_com20
	mov		ebx, eax	;factor7
	mov		eax, [ebp+_dump@_addr]		;convert_com20
	and		eax, ebx	;factor7
;---------- c32_expr end ------
	mov		[ebp+_dump@_pre_len], eax	;expr_eq
;---------- c32_expr end ------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  92: 	addr &= 0xfffffff0;
	mov		eax, 0xfffffff0		;convert_com20
	and		[ebp+_dump@_addr], eax	;expr_and_eq
;---------- c32_expr end ------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  93: 	puts("addr:  0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n");
;---------- call _puts ----------
	sub		esp, 0x4	;expr_function_call
	jmp		L4108
L4107:
	db		0x61
	db		0x64
	db		0x64
	db		0x72
	db		0x3a
	db		0x20
	db		0x20
	db		0x30
	db		0x20
	db		0x20
	db		0x31
	db		0x20
	db		0x20
	db		0x32
	db		0x20
	db		0x20
	db		0x33
	db		0x20
	db		0x20
	db		0x34
	db		0x20
	db		0x20
	db		0x35
	db		0x20
	db		0x20
	db		0x36
	db		0x20
	db		0x20
	db		0x37
	db		0x20
	db		0x20
	db		0x38
	db		0x20
	db		0x20
	db		0x39
	db		0x20
	db		0x20
	db		0x61
	db		0x20
	db		0x20
	db		0x62
	db		0x20
	db		0x20
	db		0x63
	db		0x20
	db		0x20
	db		0x64
	db		0x20
	db		0x20
	db		0x65
	db		0x20
	db		0x20
	db		0x66
	db		0xa
	db		0x0
L4108:
	mov		eax, L4107		;convert_com20
	mov		[esp+0x0], eax		;expr_function_call
	call	_puts		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
;---------- parser_if ----------
;  94: 	if(pre_len){
;---------- c32_expr ----------
;---------- c32_expr end ------
	mov		eax, [ebp+_dump@_pre_len]		;convert_com20
	and		eax, eax
	jz		L4109
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  95: 		printf_4x(addr);
;---------- call _printf_4x ----------
	sub		esp, 0x4	;expr_function_call
	mov		eax, [ebp+_dump@_addr]		;convert_com20
	mov		[esp+0x0], eax		;expr_function_call
	call	_printf_4x		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  96: 		putch(':');
;---------- call _putch ----------
	sub		esp, 0x4	;expr_function_call
	mov		eax, 0x3a		;convert_com20
	mov		[esp+0x0], eax		;expr_function_call
	call	_putch		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
;---------- parser_for ----------
;  97: 		for(i=0; i<pre_len; i++){
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
	mov		eax, 0x0		;convert_com20
	mov		[ebp+_dump@_i], eax	;expr_eq
;---------- c32_expr end ------
L4153:
;---------- c32_expr ----------
	mov		eax, [ebp+_dump@_i]		;convert_com20
	push	eax		;factor9
	mov		eax, [ebp+_dump@_pre_len]		;convert_com20
	pop		ebx		;factor9
	cmp		ebx, eax	;factor9
	mov		eax, 1	;factor9
	jl		L4176		;factor9
	dec		eax		;factor9
L4176:		;factor8
;---------- c32_expr end ------
	and		eax, eax
	jz		L4151
	jmp		L4154
L4152:
;---------- c32_expr ----------
	mov		ebx, [ebp+_dump@_i]		;factor13
	add		dword	[ebp+_dump@_i], 0x1	;factor13
	mov		eax, ebx	;factor13
;---------- c32_expr end ------
	jmp		L4153
L4154:
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
;  98: 			puts("   ");
;---------- call _puts ----------
	sub		esp, 0x4	;expr_function_call
	jmp		L4206
L4205:
	db		0x20
	db		0x20
	db		0x20
	db		0x0
L4206:
	mov		eax, L4205		;convert_com20
	mov		[esp+0x0], eax		;expr_function_call
	call	_puts		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
;  99: 		}
	jmp		L4152
L4151:
;---------- parser_for ----------
; 100: 		for( ; i<16; i++){
L4209:
;---------- c32_expr ----------
	mov		eax, [ebp+_dump@_i]		;convert_com20
	push	eax		;factor9
	mov		eax, 0x10		;convert_com20
	pop		ebx		;factor9
	cmp		ebx, eax	;factor9
	mov		eax, 1	;factor9
	jl		L4216		;factor9
	dec		eax		;factor9
L4216:		;factor8
;---------- c32_expr end ------
	and		eax, eax
	jz		L4207
	jmp		L4210
L4208:
;---------- c32_expr ----------
	mov		ebx, [ebp+_dump@_i]		;factor13
	add		dword	[ebp+_dump@_i], 0x1	;factor13
	mov		eax, ebx	;factor13
;---------- c32_expr end ------
	jmp		L4209
L4210:
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 101: 			putch(' ');
;---------- call _putch ----------
	sub		esp, 0x4	;expr_function_call
	mov		eax, 0x20		;convert_com20
	mov		[esp+0x0], eax		;expr_function_call
	call	_putch		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 102: 			printf_2x(*(char*)(addr+i));
;---------- call _printf_2x ----------
	sub		esp, 0x4	;expr_function_call
;---------- c32_expr ----------
	mov		eax, [ebp+_dump@_i]		;convert_com20
	mov		ebx, eax	;factor11
	mov		eax, [ebp+_dump@_addr]		;convert_com20
	add		eax, ebx	;factor11
;---------- c32_expr end ------
	mov		al, [eax]		;convert_com20
	cbw		;convert_com10
	db 0x66
	db 0x98		;convert_com10
	mov		[esp+0x0], eax		;expr_function_call
	call	_printf_2x		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
;---------- parser_if ----------
; 103: 			if(--len==0){
;---------- c32_expr ----------
	sub		dword	[ebp+_dump@_len], 0x1	;factor13
	mov		eax, [ebp+_dump@_len]		;convert_com20
	push	eax		;factor8
	mov		eax, 0x0		;convert_com20
	pop		ebx		;factor8
	cmp		eax, ebx	;factor8
	mov		eax, 1	;factor8-9
	jz		L4281		;factor8-10
	dec		eax		;factor8-11
L4281:		;factor8
;---------- c32_expr end ------
	and		eax, eax
	jz		L4275
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 104: 				putch('\n');
;---------- call _putch ----------
	sub		esp, 0x4	;expr_function_call
	mov		eax, 0xa		;convert_com20
	mov		[esp+0x0], eax		;expr_function_call
	call	_putch		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
;----------  parser_return  ---------
; 105: 				return;
	add		esp, 8		; auto variable clear
	pop		ebp
	ret
; 106: 			}
	jmp		L4276
L4275:
L4276:
; 107: 		}
	jmp		L4208
L4207:
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 108: 		putch('\n');
;---------- call _putch ----------
	sub		esp, 0x4	;expr_function_call
	mov		eax, 0xa		;convert_com20
	mov		[esp+0x0], eax		;expr_function_call
	call	_putch		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 109: 		addr += 16;
	mov		eax, 0x10		;convert_com20
	add		[ebp+_dump@_addr], eax	;expr_plus_eq
;---------- c32_expr end ------
; 110: 	}
	jmp		L4110
L4109:
L4110:
;---------- parser_for ----------
; 111: 	for(;;){
L4340:
	jmp		L4341
L4339:
	jmp		L4340
L4341:
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 112: 		printf_4x(addr);
;---------- call _printf_4x ----------
	sub		esp, 0x4	;expr_function_call
	mov		eax, [ebp+_dump@_addr]		;convert_com20
	mov		[esp+0x0], eax		;expr_function_call
	call	_printf_4x		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 113: 		putch(':');
;---------- call _putch ----------
	sub		esp, 0x4	;expr_function_call
	mov		eax, 0x3a		;convert_com20
	mov		[esp+0x0], eax		;expr_function_call
	call	_putch		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
;---------- parser_for ----------
; 114: 		for(i=0; i<16; i++){
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
	mov		eax, 0x0		;convert_com20
	mov		[ebp+_dump@_i], eax	;expr_eq
;---------- c32_expr end ------
L4376:
;---------- c32_expr ----------
	mov		eax, [ebp+_dump@_i]		;convert_com20
	push	eax		;factor9
	mov		eax, 0x10		;convert_com20
	pop		ebx		;factor9
	cmp		ebx, eax	;factor9
	mov		eax, 1	;factor9
	jl		L4399		;factor9
	dec		eax		;factor9
L4399:		;factor8
;---------- c32_expr end ------
	and		eax, eax
	jz		L4374
	jmp		L4377
L4375:
;---------- c32_expr ----------
	mov		ebx, [ebp+_dump@_i]		;factor13
	add		dword	[ebp+_dump@_i], 0x1	;factor13
	mov		eax, ebx	;factor13
;---------- c32_expr end ------
	jmp		L4376
L4377:
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 115: 			putch(' ');
;---------- call _putch ----------
	sub		esp, 0x4	;expr_function_call
	mov		eax, 0x20		;convert_com20
	mov		[esp+0x0], eax		;expr_function_call
	call	_putch		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 116: 			printf_2x(*(char*)(addr+i));
;---------- call _printf_2x ----------
	sub		esp, 0x4	;expr_function_call
;---------- c32_expr ----------
	mov		eax, [ebp+_dump@_i]		;convert_com20
	mov		ebx, eax	;factor11
	mov		eax, [ebp+_dump@_addr]		;convert_com20
	add		eax, ebx	;factor11
;---------- c32_expr end ------
	mov		al, [eax]		;convert_com20
	cbw		;convert_com10
	db 0x66
	db 0x98		;convert_com10
	mov		[esp+0x0], eax		;expr_function_call
	call	_printf_2x		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
;---------- parser_if ----------
; 117: 			if(--len==0){
;---------- c32_expr ----------
	sub		dword	[ebp+_dump@_len], 0x1	;factor13
	mov		eax, [ebp+_dump@_len]		;convert_com20
	push	eax		;factor8
	mov		eax, 0x0		;convert_com20
	pop		ebx		;factor8
	cmp		eax, ebx	;factor8
	mov		eax, 1	;factor8-9
	jz		L4464		;factor8-10
	dec		eax		;factor8-11
L4464:		;factor8
;---------- c32_expr end ------
	and		eax, eax
	jz		L4458
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 118: 				putch('\n');
;---------- call _putch ----------
	sub		esp, 0x4	;expr_function_call
	mov		eax, 0xa		;convert_com20
	mov		[esp+0x0], eax		;expr_function_call
	call	_putch		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
;----------  parser_return  ---------
; 119: 				return;
	add		esp, 8		; auto variable clear
	pop		ebp
	ret
; 120: 			}
	jmp		L4459
L4458:
L4459:
; 121: 		}
	jmp		L4375
L4374:
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 122: 		putch('\n');
;---------- call _putch ----------
	sub		esp, 0x4	;expr_function_call
	mov		eax, 0xa		;convert_com20
	mov		[esp+0x0], eax		;expr_function_call
	call	_putch		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 123: 		addr += 16;
	mov		eax, 0x10		;convert_com20
	add		[ebp+_dump@_addr], eax	;expr_plus_eq
;---------- c32_expr end ------
; 124: 	}
	jmp		L4339
L4338:
; 125: }
	add		esp, 8		; auto variable clear
	pop		ebp
	ret
; 126: 
; 127: int main()
_main@_buf	equ	0
_main@_addr	equ	256
_main@_len	equ	260
;---------- function _main ----------
_main:
	push	ebp
	add		esp, -264		; auto variable
	mov		ebp, esp
;---------- parser_function_block ----------
; 128: {
; 129: 	char buf[256];
;---------- c32_expr ----------
;---------- c32_expr end ------
; 130: 	unsigned int addr, len;
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 131: 	
; 132: 	puts("\nhelp message is \"help\"\n\n");
;---------- call _puts ----------
	sub		esp, 0x4	;expr_function_call
	jmp		L4546
L4545:
	db		0xa
	db		0x68
	db		0x65
	db		0x6c
	db		0x70
	db		0x20
	db		0x6d
	db		0x65
	db		0x73
	db		0x73
	db		0x61
	db		0x67
	db		0x65
	db		0x20
	db		0x69
	db		0x73
	db		0x20
	db		0x22
	db		0x68
	db		0x65
	db		0x6c
	db		0x70
	db		0x22
	db		0xa
	db		0xa
	db		0x0
L4546:
	mov		eax, L4545		;convert_com20
	mov		[esp+0x0], eax		;expr_function_call
	call	_puts		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
;---------- parser_for ----------
; 133: 	for(;;){
L4549:
	jmp		L4550
L4548:
	jmp		L4549
L4550:
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 134: 		puts("> ");
;---------- call _puts ----------
	sub		esp, 0x4	;expr_function_call
	jmp		L4568
L4567:
	db		0x3e
	db		0x20
	db		0x0
L4568:
	mov		eax, L4567		;convert_com20
	mov		[esp+0x0], eax		;expr_function_call
	call	_puts		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 135: 		gets(buf, 256);
;---------- call _gets ----------
	sub		esp, 0x8	;expr_function_call
	mov		eax, ebp
	add		eax, _main@_buf
	mov		[esp+0x0], eax		;expr_function_call
	mov		eax, 0x100		;convert_com20
	mov		[esp+0x4], eax		;expr_function_call
	call	_gets		;expr_function_call
	add		esp, 0x8	;expr_function_call
;---------- c32_expr end ------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 136: 		linebufp = buf;
	mov		eax, ebp
	add		eax, _main@_buf
	mov		[_linebufp], eax	;expr_eq
;---------- c32_expr end ------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 137: 		getsym();
;---------- call _getsym ----------
	call	_getsym		;expr_function_call
;---------- c32_expr end ------
;---------- parser_if ----------
; 138: 		if(strcmp(symbuf, "help")==0){
;---------- c32_expr ----------
;---------- call _strcmp ----------
	sub		esp, 0x8	;expr_function_call
	mov		eax, _symbuf		;convert_com20
	mov		[esp+0x0], eax		;expr_function_call
	jmp		L4644
L4643:
	db		0x68
	db		0x65
	db		0x6c
	db		0x70
	db		0x0
L4644:
	mov		eax, L4643		;convert_com20
	mov		[esp+0x4], eax		;expr_function_call
	call	_strcmp		;expr_function_call
	add		esp, 0x8	;expr_function_call
	push	eax		;factor8
	mov		eax, 0x0		;convert_com20
	pop		ebx		;factor8
	cmp		eax, ebx	;factor8
	mov		eax, 1	;factor8-9
	jz		L4623		;factor8-10
	dec		eax		;factor8-11
L4623:		;factor8
;---------- c32_expr end ------
	and		eax, eax
	jz		L4617
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 139: 			help();
;---------- call _help ----------
	call	_help		;expr_function_call
;---------- c32_expr end ------
; 140: 		}
	jmp		L4618
L4617:
; 141: 		else if(strcmp(symbuf, "dump")==0){
;---------- parser_block ----------
;---------- parser_if ----------
;---------- c32_expr ----------
;---------- call _strcmp ----------
	sub		esp, 0x8	;expr_function_call
	mov		eax, _symbuf		;convert_com20
	mov		[esp+0x0], eax		;expr_function_call
	jmp		L4683
L4682:
	db		0x64
	db		0x75
	db		0x6d
	db		0x70
	db		0x0
L4683:
	mov		eax, L4682		;convert_com20
	mov		[esp+0x4], eax		;expr_function_call
	call	_strcmp		;expr_function_call
	add		esp, 0x8	;expr_function_call
	push	eax		;factor8
	mov		eax, 0x0		;convert_com20
	pop		ebx		;factor8
	cmp		eax, ebx	;factor8
	mov		eax, 1	;factor8-9
	jz		L4662		;factor8-10
	dec		eax		;factor8-11
L4662:		;factor8
;---------- c32_expr end ------
	and		eax, eax
	jz		L4656
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 142: 			getsym();
;---------- call _getsym ----------
	call	_getsym		;expr_function_call
;---------- c32_expr end ------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 143: 			addr = getnum(symbuf);
;---------- call _getnum ----------
	sub		esp, 0x4	;expr_function_call
	mov		eax, _symbuf		;convert_com20
	mov		[esp+0x0], eax		;expr_function_call
	call	_getnum		;expr_function_call
	add		esp, 0x4	;expr_function_call
	mov		[ebp+_main@_addr], eax	;expr_eq
;---------- c32_expr end ------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 144: 			getsym();
;---------- call _getsym ----------
	call	_getsym		;expr_function_call
;---------- c32_expr end ------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 145: 			len  = getnum(symbuf);
;---------- call _getnum ----------
	sub		esp, 0x4	;expr_function_call
	mov		eax, _symbuf		;convert_com20
	mov		[esp+0x0], eax		;expr_function_call
	call	_getnum		;expr_function_call
	add		esp, 0x4	;expr_function_call
	mov		[ebp+_main@_len], eax	;expr_eq
;---------- c32_expr end ------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 146: 			dump(addr, len);
;---------- call _dump ----------
	sub		esp, 0x8	;expr_function_call
	mov		eax, [ebp+_main@_addr]		;convert_com20
	mov		[esp+0x0], eax		;expr_function_call
	mov		eax, [ebp+_main@_len]		;convert_com20
	mov		[esp+0x4], eax		;expr_function_call
	call	_dump		;expr_function_call
	add		esp, 0x8	;expr_function_call
;---------- c32_expr end ------
; 147: 		}
	jmp		L4657
L4656:
; 148: 		else if(strcmp(symbuf, "exit")==0){
;---------- parser_block ----------
;---------- parser_if ----------
;---------- c32_expr ----------
;---------- call _strcmp ----------
	sub		esp, 0x8	;expr_function_call
	mov		eax, _symbuf		;convert_com20
	mov		[esp+0x0], eax		;expr_function_call
	jmp		L4802
L4801:
	db		0x65
	db		0x78
	db		0x69
	db		0x74
	db		0x0
L4802:
	mov		eax, L4801		;convert_com20
	mov		[esp+0x4], eax		;expr_function_call
	call	_strcmp		;expr_function_call
	add		esp, 0x8	;expr_function_call
	push	eax		;factor8
	mov		eax, 0x0		;convert_com20
	pop		ebx		;factor8
	cmp		eax, ebx	;factor8
	mov		eax, 1	;factor8-9
	jz		L4781		;factor8-10
	dec		eax		;factor8-11
L4781:		;factor8
;---------- c32_expr end ------
	and		eax, eax
	jz		L4775
;---------- parser_block ----------
;---------- parser_function_block ----------
;----------  parser_return  ---------
; 149: 			return 0;
;---------- c32_expr ----------
;---------- c32_expr end ------
	mov		eax, 0x0		;convert_com20
	add		esp, 264		; auto variable clear
	pop		ebp
	ret
; 150: 		}
	jmp		L4776
L4775:
; 151: 		else{
;---------- parser_block ----------
;---------- parser_function_block ----------
;---------- parser_dainyuu ----------
;---------- c32_expr ----------
; 152: 			puts("*** error\n");
;---------- call _puts ----------
	sub		esp, 0x4	;expr_function_call
	jmp		L4831
L4830:
	db		0x2a
	db		0x2a
	db		0x2a
	db		0x20
	db		0x65
	db		0x72
	db		0x72
	db		0x6f
	db		0x72
	db		0xa
	db		0x0
L4831:
	mov		eax, L4830		;convert_com20
	mov		[esp+0x0], eax		;expr_function_call
	call	_puts		;expr_function_call
	add		esp, 0x4	;expr_function_call
;---------- c32_expr end ------
; 153: 		}
L4776:
L4657:
L4618:
; 154: 	}
	jmp		L4548
L4547:
; 155: }
	add		esp, 264		; auto variable clear
	pop		ebp
	ret
;----------  startup  ----------
startup:
	call	_main
	int		0x20	;exit
	jmp		$
; edi: dst_addr
; esi: src_addr
; ecx: byte
startup_memcpy:
	mov		bl, [esi]
	mov		[edi], bl
	inc		esi
	inc		edi
	dec		ecx
	jnz		startup_memcpy
	ret

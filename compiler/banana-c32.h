#define BIT16
//#define __XC32
//#define DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <setjmp.h>
#ifndef __XC32
#include <io.h>
#define ut_printf printf
#define stack_check()
#endif
#include <fcntl.h>
#include <stdarg.h>
#ifdef __XC32
#include "apps.h"
#define fprintf	c32_fprintf
#endif

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int  uint;

#define NO_10006F
//#define NO10032B
//#define NO10030A

#define MAX_PARAMETER		10		// �֐��̈����͍ő� 10�ł���
#define MAX_SYMBUF_LEN		32		// �V���{�����̍ő�l
#define MAX_VARIABLE		20		// �P�֐��� auto+parameter �ϐ��̍ő吔
#define MAX_ARRAY_DIM		7		// �z��ő原����

#ifdef NO_10006F
	#define MAX_MALLOC_AREA		0x5e00
	#define MAX_STRING_LEN		1024	// token �� string ��Ԃ��G���A�T�C�Y
	#define MAX_INIT_STRING_LEN	4096	// �������� string ���ꎞ�I�ɕۑ�����G���A�̃T�C�Y
	#define MAX_SRC_BUFFER_LEN	4096	// src buffer
	#define MAX_BUFF_LEN		4096	// output_buffer �̃T�C�Y
	#define MAX_SYMBOL_TABLE_G	200		// global �V���{���e�[�u���̍ő�V���{����
//	#define RAM_BASE		0xa0010000	// RAM �G���A�̊J�n�A�h���X
	#define RAM_TOP			0xa0016000	// �v���O�����G���A�擪 - RAM�Ƀv���O������u����
	#define RAM_SIZE		(RAM_TOP - RAM_BASE)  // RAM �G���A�̃o�C�g��
	#define FLUSH_TOP		0x9d000000	// flush �������擪     - ROM�Ƀv���O������u����
	#ifndef __XC32
		#define RAM_BASE	0xa0010000	// RAM �G���A�̊J�n�A�h���X
	#endif
#endif
#ifdef NO10032B
	#define MAX_MALLOC_AREA		0x5e00
	#define MAX_STRING_LEN		1024	// token �� string ��Ԃ��G���A�T�C�Y
	#define MAX_INIT_STRING_LEN	4096	// �������� string ���ꎞ�I�ɕۑ�����G���A�̃T�C�Y
	#define MAX_SRC_BUFFER_LEN	4096	// src buffer
	#define MAX_BUFF_LEN		4096	// output_buffer �̃T�C�Y
	#define MAX_SYMBOL_TABLE_G	200		// global �V���{���e�[�u���̍ő�V���{����
	#define RAM_BASE		0xa0008000
	#define RAM_TOP			0xa0010000
	#define RAM_SIZE		(RAM_TOP - RAM_BASE)  // RAM �G���A�̃o�C�g��
	#define FLUSH_TOP		0x9d000000	// flush �������擪     - ROM�Ƀv���O������u����
#endif
#ifdef NO10030A
	#define MAX_MALLOC_AREA		9000
	#define MAX_STRING_LEN		512		// token �� string ��Ԃ��G���A�T�C�Y
	#define MAX_INIT_STRING_LEN	1024	// �������� string ���ꎞ�I�ɕۑ�����G���A�̃T�C�Y
	#define MAX_SRC_BUFFER_LEN	3072	// src buffer
	#define MAX_BUFF_LEN		3072	// output_buffer �̃T�C�Y
	#define MAX_SYMBOL_TABLE_G	120		// global �V���{���e�[�u���̍ő�V���{����
	#ifndef __XC32
		#define RAM_BASE	0xa000a000
		#define RAM_TOP		0xa000b000
		#define RAM_SIZE	(RAM_TOP - RAM_BASE)  // RAM �G���A�̃o�C�g��
		#define FLUSH_TOP	0x9d000000	// flush �������擪     - ROM�Ƀv���O������u����
	#endif
#endif



#define VERSION			"banana-c32 v-305"
#ifndef __XC32
#define abs1(x)			((x)<0 ? -(x) : (x))
#endif


#define TYPE_SYMBOL			1		// ��ʃV���{���ł͂����Ԃ�

#define TYPE_L_KAKKO		10
#define TYPE_R_KAKKO		11
#define TYPE_L_NAMIKAKKO	12
#define TYPE_R_NAMIKAKKO	13
#define TYPE_L_KAKUKAKKO	14
#define TYPE_R_KAKUKAKKO	15
#define TYPE_SEMIKORON		16
#define TYPE_KORON			17
#define TYPE_PLUS			18
#define TYPE_MINUS			19
#define TYPE_PLUS_PLUS		20
#define TYPE_MINUS_MINUS	21
#define TYPE_ASTERISK		22
#define TYPE_SLUSH			23
#define TYPE_PERCENT		24
#define TYPE_AND			25
#define TYPE_OR				26
#define TYPE_KANMA			27
#define TYPE_EQ				28
#define TYPE_EQEQ			29		// ==
#define TYPE_NOTEQ			30		// !=
#define TYPE_GT				31		// >
#define TYPE_GE				32		// >=
#define TYPE_LT				33		// <
#define TYPE_LE				34		// <=
#define TYPE_PLUS_EQ		35		// +=
#define TYPE_MINUS_EQ		36		// -=
#define TYPE_ASTERISK_EQ	37		// *=
#define TYPE_SLUSH_EQ		38		// /=
#define TYPE_PERSENT_EQ		39		// %=
#define TYPE_OR_EQ			40		// |=
#define TYPE_AND_EQ			41		// &=
#define TYPE_L_SHIFT		42		// <<
#define TYPE_R_SHIFT		43		// >>
#define TYPE_XOR			44		// ^
#define	TYPE_L_SHIFT_EQ		45		// <<=
#define TYPE_R_SHIFT_EQ		46		// >>=
#define TYPE_XOR_EQ			47		// ^=
#define TYPE_OR_OR			48		// ||
#define TYPE_AND_AND		49		// &&
#define TYPE_QUESTION		50		// ?
#define TYPE_TILDE			51		// ~
#define TYPE_NOT			52		// !
#define TYPE_PERIOD			53		// .
#define TYPE_YAZIRUSI		54		// ->

#define TYPE_UNSIGNED		100
#define TYPE_CHAR			101
#define TYPE_SHORT			102
#define TYPE_INT			103
#define TYPE_LONG			104
#define TYPE_VOID			105
#define TYPE_IF				106
#define TYPE_ELSE			107
#define TYPE_RETURN			108
#define TYPE_GOTO			109
#define TYPE_WHILE			110
#define TYPE_CONTINUE		111
#define TYPE_BREAK			112
#define TYPE_STRING			113
#define TYPE_FOR			114
#define TYPE_INTERRUPT		115
#define TYPE_ASM			116
#define TYPE_TEST			117
#define TYPE_UCHAR			118
#define TYPE_USHORT			119
#define TYPE_UINT			120
#define TYPE_ULONG			121
#define TYPE_SIZEOF			122
#define TYPE_SWITCH			123
#define TYPE_CASE			124
#define TYPE_DEFAULT		125
#define TYPE_DO				126
#define TYPE_CONST			127
#define TYPE_STRUCT			128

#define TYPE_DIGIT			150
#define TYPE_EOF			151		// EOF �ƌ��� token
#define TYPE_ERROR			152		// error token �p

/***	TYPE_SYMBOL �� attr	***/
#define ATTR_CHAR			0x1
#define ATTR_SHORT			0x2
#define ATTR_INT			0x4
#define ATTR_UNSIGNED		0x8

#define ATTR_POINTER		0x30	// 0x10: 1���̃|�C���^�A0x20: 2���̃|�C���^
#define ATTR_POINTER1		0x10
#define ATTR_ARRAY			0x1c0	// 0x40: 1�����A0x80: 2�����A... 0x1c0:6����
#define ATTR_ARRAY1			0x40
#define ATTR_FUNC			0x200
#define ATTR_ARRAY_P		0x400	// �z����֐��̈����ɂ�����A�z��ւ̃|�C���^�����ɂȂ�
#define ATTR_VOID			0x800
#define ATTR_CONST			0x1000
#define ATTR_STRUCT			0x2000

#define ATTR_CSI_MASK			(ATTR_CHAR | ATTR_INT | ATTR_SHORT)
#define ATTR_CSIU_MASK			(ATTR_CHAR | ATTR_INT | ATTR_SHORT | ATTR_UNSIGNED)

#define PASS1				1
#define PASS2				2
#define PASS3				3

#define COM20				20
#define COM21				21
#define COM22				22

#define FLAG_01				0x01
#define FLAG_02				0x02
#define FLAG_04				0x04
#define FLAG_08				0x08
#define FLAG_10				0x10
#define FLAG_20				0x20
#define FLAG_40				0x40

#ifdef PRINTF_DEBUG
#define c32_printf(fmt, ...)	{fprintf(stderr, fmt, __VA_ARGS__);fflush(stderr);}
#else
#define printxx	c32_printf
#endif

struct attr_s{
	char *symbuf;				// �V���{���e�[�u���� struct �{�̂̏ꍇ sturct ��������
	int attr;
};

struct symtbl{					// �ϐ����֐����̋�ʂ� attr �ł��    56byte
	char *symbuf;				// �ϐ����i_goban�j
	char *label;				// �ϐ����x���i_game@_goban�j
	uint value;					// �A�h���X�A�I�t�Z�b�g�A���x���ԍ�
	uint init;					// �ϐ������l
	struct func_tbl *func_tbl;	// �֐����o�^�G���A
//	void *work;					// ���[�N�G���A�Aglobal �z��̏����l�e�[�u��
//	char *work_bitmap;			// work �� ���x���ԍ��ł��邱�Ƃ�����
	int size;					// �ϐ��o�C�g��

	ushort work_cnt;			// ���[�N�G���A�̃f�[�^��
	ushort xyz_size[MAX_ARRAY_DIM];			// �Y�����T�C�Y
	ushort xyz_dim;				// �z�񎟌��� 0�`7
	char reg[4];				// ���[�h���ׂ����W�X�^���i��: "s0"�j
	
	struct attr_s attr;			// �ϐ�/�֐��̌^
	uchar  type;				// �\���Ŏg�p����
	uchar  flag1;				// FLAG_01: parameter �ϐ�
								// FLAG_02: auto �ϐ�
								// FLAG_04: global �ϐ�
								// FLAG_08: global �̏���������Ă���ϐ��ł���
								// FLAG_10: �����l���e�[�u���ɂȂ��Ă���i_variable@init�j
								// FLAG_20: & ���Z�q���g�p����Ă���
								// FLAG_40: �֐��錾�ł͂Ȃ��Ċ֐��{�̂����łɍ���Ă���
};

struct func_tbl{				// �֐�����o�^����ׂ̃e�[�u��      88byte
	struct symtbl *variable_tbl[MAX_VARIABLE];	// auto+param �ϐ��̃V���{���e�[�u��
	ushort auto_size;			// auto �̃o�C�g��
	uchar variable_tbl_cnt;		// parameter + auto �ϐ��̐�
	uchar max_s_reg_numb;		// �֐����� $ra, $s0�` �̃G���A���m�ۂ��ׂ�
	uchar param_numb;			// parameter ��
};

struct cvs{
	char buff[MAX_MALLOC_AREA];								// 24KB �V���{�����i�[�p
	char string_buf[MAX_STRING_LEN];						// 1KB  c32_token_process() �� string ��Ԃ����Ɏg�p����o�b�t�@
	char tmp[MAX_STRING_LEN];								// 1KB  �ꎞ�g�p

	struct symtbl tbl_g[MAX_SYMBOL_TABLE_G];				// 12KB  global �V���{���e�[�u��
	char output_buffer[MAX_BUFF_LEN];						// 4K
	char src_buffer[MAX_SRC_BUFFER_LEN];					// 20KB
	char init_string[MAX_INIT_STRING_LEN];					// ���������� string �ۑ��o�b�t�@
};															// 

struct src_ptr{					// �p�[�T�[���[���o�b�N�ׂ̈̃X�g���N�`���[
	char *c32_linebufp;
	char *c32_linebuf;
	int line_ptr;
};

struct expr_param{
	int comment;
	// COM20:		// �萔/struct	operand = "eax", "0x1234"
	//				   ���l�Ȃ�		value=0x1234, numb_flag=1
	// COM21:		// �z��			operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"
	// COM22:		// �ϐ��A�h���X	operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]"
	char operand[MAX_SYMBUF_LEN];	// �I�y�����h
	int value;
	int numb_flag;				// ���l(COM20)�܂��͕�����萔(COM21)�Ȃ� 1
	struct symtbl *func;		// �e�֐��̃V���{���e�[�u��
	struct struct_tbl *tbl;		// struct �̃V���{���e�[�u��
	struct attr_s attr;
	ushort xyz_size[MAX_ARRAY_DIM];	// �Y�����T�C�Y
	ushort xyz_dim;				// �z�񎟌��� 0�`7
};

struct struct_elem{
	struct struct_elem *next;
	struct symtbl tbl;
};

struct struct_tbl{
	struct struct_tbl *next;
	struct struct_elem *elem;
	char *symbuf;
	int size;
};

extern struct cvs *cv;
extern struct struct_tbl *c32_st_root;
extern char *c32_linebuf;
extern char *c32_linebufp;
extern char c32_symbuf[MAX_SYMBUF_LEN];
extern char c32_src_filename[MAX_SYMBUF_LEN];
extern int c32_label_counter;
extern int c32_function_counter;			// �֐��ɒʂ��ԍ���U�邽�߁Atoken.func_numb
//extern int c32_output_buffer_line_numb;
extern int c32_tbl_ptr_g;					// �V���{���e�[�u���|�C���^
extern int c32_buff_ptr;
extern int c32_init_string_ptr;
extern int c32_no_printx;
extern int c32_no_label_out;
extern int c32_pass;
extern int c32_s_numb, c32_max_s_numb;
extern int c32_ram_addr;					// �g�p���Ă��� RAM �̍ŏ�ʃA�h���X
extern const char *c32_err_msg[];
extern jmp_buf c32_env;
extern int c32_max_src_buffer;				// src_buffer �g�p�ʂ̋L�^
extern int c32_max_output_buffer;			// output_buffer �g�p�ʂ̋L�^
extern int c32_max_init_string;				// init_string �g�p�ʂ̋L�^
extern int c32_init_string_flag;			// factor �ŏo�͂� init_string[] �ɂ���t���O
extern int must_save_t_reg;				// bit0�`bit9: t0�`t9, bit12(0x1000): v0
#ifdef __XC32
extern SYS_FS_HANDLE c32_src_fp, c32_asm_fp;
#else
extern FILE *c32_src_fp, *c32_asm_fp;
#endif


#define E_NO_ERROR						0
#define E_L_KAKKO_MISSING				1
#define E_R_KAKKO_MISSING				2
#define E_L_NAMIKAKKO_MISSING			3
#define E_R_NAMIKAKKO_MISSING			4
#define E_L_KAKUKAKKO_MISSING			5
#define E_R_KAKUKAKKO_MISSING			6
#define E_SEMIKORON_MISSING				7
#define E_SYNTAX_ERROR					8
#define E_UNDEFINED_SYMBOL				9
#define E_UNEXPECTED_EOF				10
#define E_TOO_LONG_SYMBOL				11
#define E_INTERNAL_ERROR				12
#define E_MULTIPLE_DEFINITION			13
#define E_TO_BE_DEFINE					14
#define E_NEED_POINTER					15
#define E_CAN_NOT_USE_POINTER			16
#define E_OUT_OF_MEMORY					17
#define E_INPUT_FILE_ERROR				18
#define E_ATTR_MISMATCH					19
#define E_NEED_CONSTANT					20
#define E_TOO_MANY_VARIABLE				21
#define E_ILLEGAL_ELSE					22
#define E_ILLEGAL_CONTINUE				23
#define E_ILLEGAL_BREAK					24
#define E_EXPRESSION_TOO_COMPLEX		25
#define E_PARAM_NUMBER_ERROR			26
#define E_WHILE_MISSING					27
#define E_TABLE_OVERFLOW			28
#define E_NOT_STRUCTURE					29
#define E_VARIABLE_TYPE_ERROR			30


#ifdef ERR_MSG_DEFINE
const char *c32_err_msg[]={
	"E_NO_ERROR",							// 0
	"E: (  missing",						// 1
	"E: )  missing",						// 2
	"E: {  missing",						// 3
	"E: }  missing",						// 4
	"E: [  missing",						// 5
	"E: ]  missing",						// 6
	"E: ;  missing",						// 7
	"E_SYNTAX_ERROR",						// 8
	"E_UNDEFINED_SYMBOL",					// 9
	"E_UNEXPECTED_EOF",						// 10
	"E_TOO_LONG_SYMBOL",					// 11
	"E_INTERNAL_ERROR",						// 12
	"E_MULTIPLE_DEFINITION",				// 13
	"E_TO_BE_DEFINE",						// 14
	"E_NEED_POINTER",						// 15
	"E_CAN_NOT_USE_POINTER",				// 16
	"E_OUT_OF_MEMORY",						// 17
	"E_INPUT_FILE_ERROR",					// 18
	"E_ATTR_MISMATCH",						// 19
	"E_NEED_CONSTANT",						// 20
	"E_TOO_MANY_VARIABLE",					// 21
	"E_ILLEGAL_ELSE",						// 22
	"E_ILLEGAL_CONTINUE",					// 23
	"E_ILLEGAL_BREAK",						// 24
	"E_EXPRESSION_TOO_COMPLEX",				// 25
	"E_PARAM_NUMBER_ERROR",					// 26
	"E_WHILE_MISSING",						// 27
	"E_TABLE_OVERFLOW",					// 28
	"E_NOT_STRUCTURE",						// 29
	"E_VARIABLE_TYPE_ERROR",				// 30
};
#endif

/*
*	V300 2021-1-16
*	�v�Z���Ŏg���鉉�Z�q�͉��L�̒ʂ�ł�
*	, = += -= *= /= %= &= ^= |= <<= >>= ?: || && | ^ & == != < <= > >= << >> + - * / %
*	! ~ ++ -- - * & (type) sizeof [] ()
*	���䕶�͉��L�̒ʂ�ł�
*	goto/continue/break/for/do/while/if/else/return/__test__/_asm/switch/case/default
*	���p�J�^�J�i�\��/�L�[����
*	struct �͓����Ă��Ȃ�
*	���������_�͓����Ă��Ȃ�
*	auto �ϐ��̏������͓����Ă��Ȃ�
*	�ϐ��錾�ŕϐ������J���}�ŋ�؂��Đ錾����̂̓|�C���^�̏ꍇ�͕W���d�l����O���
*	�������z��̏������ł��P�d�̑傩�����ł����鎖�����o���Ȃ�
*	const �͎g���Ȃ�
*	extern �͎g���Ȃ�
*	�v���v���Z�b�T�� #include �� #define �̂ݎg����
*
*	V301 2021-1-19
*	3�����z��錾/���p (�֐������͂܂�����Ă��Ȃ�)
*	3���̃|�C���^
*	expr.cpp �̊֐��̈����� struct expr_param *expr_p1; �ɂ���
*
*	V302 2021-2-5
*	7�����z��܂Ŏg�p�ł���悤�ɂ���
*	t0/1/9 ��ʗp���[�N�G���A�At2�`t8 expr() �p���[�N�G���A
*	expr() �ő�����郌�W�� expr_p1->reg �Ŏw�肷��
*	const ����ꂽ
*
*	v303 2021-3-7
*	No10006F �{�[�h�p�̃R���p�C��
*	����������W�X�^���w�肷��̂ł͂Ȃ��ĕԂ��悤�ɂ���
*
*	v304 2021-3-16
*	No10032B �{�[�h�p�̃R���p�C��
*	struct ����ꂽ
*
*	v305 2021-4-11
*	No10006F/No10032B �p�R���p�C��
*	���������_������
*	�z�񖼂�萔�Ƃ��Ĉ����Ă��Ȃ��ג萔���Ƃ��Ĕz�񖼂��g�p�ł��Ȃ�
*	extern ���T�|�[�g���Ă��Ȃ�
*	auto �ϐ��̏��������g���Ȃ�
*/
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

#define MAX_PARAMETER		10		// 関数の引数は最大 10個である
#define MAX_SYMBUF_LEN		32		// シンボル長の最大値
#define MAX_VARIABLE		20		// １関数の auto+parameter 変数の最大数
#define MAX_ARRAY_DIM		7		// 配列最大次元数

#ifdef NO_10006F
	#define MAX_MALLOC_AREA		0x5e00
	#define MAX_STRING_LEN		1024	// token で string を返すエリアサイズ
	#define MAX_INIT_STRING_LEN	4096	// 初期化で string を一時的に保存するエリアのサイズ
	#define MAX_SRC_BUFFER_LEN	4096	// src buffer
	#define MAX_BUFF_LEN		4096	// output_buffer のサイズ
	#define MAX_SYMBOL_TABLE_G	200		// global シンボルテーブルの最大シンボル数
//	#define RAM_BASE		0xa0010000	// RAM エリアの開始アドレス
	#define RAM_TOP			0xa0016000	// プログラムエリア先頭 - RAMにプログラムを置く時
	#define RAM_SIZE		(RAM_TOP - RAM_BASE)  // RAM エリアのバイト数
	#define FLUSH_TOP		0x9d000000	// flush メモリ先頭     - ROMにプログラムを置く時
	#ifndef __XC32
		#define RAM_BASE	0xa0010000	// RAM エリアの開始アドレス
	#endif
#endif
#ifdef NO10032B
	#define MAX_MALLOC_AREA		0x5e00
	#define MAX_STRING_LEN		1024	// token で string を返すエリアサイズ
	#define MAX_INIT_STRING_LEN	4096	// 初期化で string を一時的に保存するエリアのサイズ
	#define MAX_SRC_BUFFER_LEN	4096	// src buffer
	#define MAX_BUFF_LEN		4096	// output_buffer のサイズ
	#define MAX_SYMBOL_TABLE_G	200		// global シンボルテーブルの最大シンボル数
	#define RAM_BASE		0xa0008000
	#define RAM_TOP			0xa0010000
	#define RAM_SIZE		(RAM_TOP - RAM_BASE)  // RAM エリアのバイト数
	#define FLUSH_TOP		0x9d000000	// flush メモリ先頭     - ROMにプログラムを置く時
#endif
#ifdef NO10030A
	#define MAX_MALLOC_AREA		9000
	#define MAX_STRING_LEN		512		// token で string を返すエリアサイズ
	#define MAX_INIT_STRING_LEN	1024	// 初期化で string を一時的に保存するエリアのサイズ
	#define MAX_SRC_BUFFER_LEN	3072	// src buffer
	#define MAX_BUFF_LEN		3072	// output_buffer のサイズ
	#define MAX_SYMBOL_TABLE_G	120		// global シンボルテーブルの最大シンボル数
	#ifndef __XC32
		#define RAM_BASE	0xa000a000
		#define RAM_TOP		0xa000b000
		#define RAM_SIZE	(RAM_TOP - RAM_BASE)  // RAM エリアのバイト数
		#define FLUSH_TOP	0x9d000000	// flush メモリ先頭     - ROMにプログラムを置く時
	#endif
#endif



#define VERSION			"banana-c32 v-305"
#ifndef __XC32
#define abs1(x)			((x)<0 ? -(x) : (x))
#endif


#define TYPE_SYMBOL			1		// 一般シンボルではこれを返す

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
#define TYPE_EOF			151		// EOF と言う token
#define TYPE_ERROR			152		// error token 用

/***	TYPE_SYMBOL の attr	***/
#define ATTR_CHAR			0x1
#define ATTR_SHORT			0x2
#define ATTR_INT			0x4
#define ATTR_UNSIGNED		0x8

#define ATTR_POINTER		0x30	// 0x10: 1次のポインタ、0x20: 2次のポインタ
#define ATTR_POINTER1		0x10
#define ATTR_ARRAY			0x1c0	// 0x40: 1次元、0x80: 2次元、... 0x1c0:6次元
#define ATTR_ARRAY1			0x40
#define ATTR_FUNC			0x200
#define ATTR_ARRAY_P		0x400	// 配列を関数の引数にしたら、配列へのポインタ属性になる
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
	char *symbuf;				// シンボルテーブルが struct 本体の場合 sturct 名が入る
	int attr;
};

struct symtbl{					// 変数か関数かの区別は attr でやる    56byte
	char *symbuf;				// 変数名（_goban）
	char *label;				// 変数ラベル（_game@_goban）
	uint value;					// アドレス、オフセット、ラベル番号
	uint init;					// 変数初期値
	struct func_tbl *func_tbl;	// 関数情報登録エリア
//	void *work;					// ワークエリア、global 配列の初期値テーブル
//	char *work_bitmap;			// work が ラベル番号であることを示す
	int size;					// 変数バイト数

	ushort work_cnt;			// ワークエリアのデータ数
	ushort xyz_size[MAX_ARRAY_DIM];			// 添え字サイズ
	ushort xyz_dim;				// 配列次元数 0〜7
	char reg[4];				// ロードすべきレジスタ名（例: "s0"）
	
	struct attr_s attr;			// 変数/関数の型
	uchar  type;				// 予約語で使用する
	uchar  flag1;				// FLAG_01: parameter 変数
								// FLAG_02: auto 変数
								// FLAG_04: global 変数
								// FLAG_08: global の初期化されている変数である
								// FLAG_10: 初期値がテーブルになっている（_variable@init）
								// FLAG_20: & 演算子が使用されている
								// FLAG_40: 関数宣言ではなくて関数本体がすでに作られている
};

struct func_tbl{				// 関数情報を登録する為のテーブル      88byte
	struct symtbl *variable_tbl[MAX_VARIABLE];	// auto+param 変数のシンボルテーブル
	ushort auto_size;			// auto のバイト数
	uchar variable_tbl_cnt;		// parameter + auto 変数の数
	uchar max_s_reg_numb;		// 関数頭で $ra, $s0〜 のエリアを確保すべき
	uchar param_numb;			// parameter 数
};

struct cvs{
	char buff[MAX_MALLOC_AREA];								// 24KB シンボル名格納用
	char string_buf[MAX_STRING_LEN];						// 1KB  c32_token_process() で string を返す時に使用するバッファ
	char tmp[MAX_STRING_LEN];								// 1KB  一時使用

	struct symtbl tbl_g[MAX_SYMBOL_TABLE_G];				// 12KB  global シンボルテーブル
	char output_buffer[MAX_BUFF_LEN];						// 4K
	char src_buffer[MAX_SRC_BUFFER_LEN];					// 20KB
	char init_string[MAX_INIT_STRING_LEN];					// 初期化時の string 保存バッファ
};															// 

struct src_ptr{					// パーサーロールバックの為のストラクチャー
	char *c32_linebufp;
	char *c32_linebuf;
	int line_ptr;
};

struct expr_param{
	int comment;
	// COM20:		// 定数/struct	operand = "eax", "0x1234"
	//				   数値なら		value=0x1234, numb_flag=1
	// COM21:		// 配列			operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"
	// COM22:		// 変数アドレス	operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]"
	char operand[MAX_SYMBUF_LEN];	// オペランド
	int value;
	int numb_flag;				// 数値(COM20)または文字列定数(COM21)なら 1
	struct symtbl *func;		// 親関数のシンボルテーブル
	struct struct_tbl *tbl;		// struct のシンボルテーブル
	struct attr_s attr;
	ushort xyz_size[MAX_ARRAY_DIM];	// 添え字サイズ
	ushort xyz_dim;				// 配列次元数 0〜7
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
extern int c32_function_counter;			// 関数に通し番号を振るため、token.func_numb
//extern int c32_output_buffer_line_numb;
extern int c32_tbl_ptr_g;					// シンボルテーブルポインタ
extern int c32_buff_ptr;
extern int c32_init_string_ptr;
extern int c32_no_printx;
extern int c32_no_label_out;
extern int c32_pass;
extern int c32_s_numb, c32_max_s_numb;
extern int c32_ram_addr;					// 使用している RAM の最上位アドレス
extern const char *c32_err_msg[];
extern jmp_buf c32_env;
extern int c32_max_src_buffer;				// src_buffer 使用量の記録
extern int c32_max_output_buffer;			// output_buffer 使用量の記録
extern int c32_max_init_string;				// init_string 使用量の記録
extern int c32_init_string_flag;			// factor で出力を init_string[] にするフラグ
extern int must_save_t_reg;				// bit0〜bit9: t0〜t9, bit12(0x1000): v0
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
*	計算式で使える演算子は下記の通りです
*	, = += -= *= /= %= &= ^= |= <<= >>= ?: || && | ^ & == != < <= > >= << >> + - * / %
*	! ~ ++ -- - * & (type) sizeof [] ()
*	制御文は下記の通りです
*	goto/continue/break/for/do/while/if/else/return/__test__/_asm/switch/case/default
*	半角カタカナ表示/キー入力
*	struct は入っていない
*	浮動小数点は入っていない
*	auto 変数の初期化は入っていない
*	変数宣言で変数名をカンマで区切って宣言するのはポインタの場合は標準仕様から外れる
*	多次元配列の初期化でも１重の大かっこでくくる事しか出来ない
*	const は使えない
*	extern は使えない
*	プリプロセッサは #include と #define のみ使える
*
*	V301 2021-1-19
*	3次元配列宣言/引用 (関数引数はまだ入れていない)
*	3次のポインタ
*	expr.cpp の関数の引数を struct expr_param *expr_p1; にした
*
*	V302 2021-2-5
*	7次元配列まで使用できるようにした
*	t0/1/9 一般用ワークエリア、t2〜t8 expr() 用ワークエリア
*	expr() で代入するレジを expr_p1->reg で指定する
*	const を入れた
*
*	v303 2021-3-7
*	No10006F ボード用のコンパイラ
*	代入したレジスタを指定するのではなくて返すようにする
*
*	v304 2021-3-16
*	No10032B ボード用のコンパイラ
*	struct を入れた
*
*	v305 2021-4-11
*	No10006F/No10032B 用コンパイラ
*	浮動小数点が無い
*	配列名を定数として扱っていない為定数式として配列名を使用できない
*	extern をサポートしていない
*	auto 変数の初期化が使えない
*/
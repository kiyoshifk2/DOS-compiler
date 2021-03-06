#include "stdafx.h"
#include "banana-c32.h"
#include "function.h"


//	第１項は変数アドレス属性になっている


/********************************************************************************/
/*		c32_expr_r_shift_eq														*/
/*		return: 求めた 値の attr												*/
/*		COM20: 定数	operand = "eax", "0x1234"  数値なら	value=0x1234, numb_flag=1	 */
/*		COM21: 配列名 operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: 変数   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		value : 定数の時、定数を返すエリア										*/
/*		used: 1ならば第１項の eax が push されている							*/
/********************************************************************************/
int c32_expr_r_shift_eq(struct expr_param *expr_p1, struct expr_param *expr_p2, int used)
{
	if(expr_p1->attr.attr & (ATTR_POINTER | ATTR_ARRAY)){
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
	}
	if(expr_p2->attr.attr & (ATTR_POINTER | ATTR_ARRAY))
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);

	// shift 量を cl に作る
//	if(c32_direct(expr_p2->comment)){	// 第２項が直接アドレスならば
		c32_printf("	mov		%s, %s	;expr_r_shift_eq\n", c32_attr_to_cx(&expr_p2->attr), expr_p2->operand);
//	}
//	else{
//		c32_printf("	mov		%s, [%s]	;expr_r_shift_eq\n", c32_attr_to_cx(&expr_p2->attr), expr_p2->operand);
//	}
	
	// shift する
	if(used){
		c32_printf("	pop		eax		;expr_r_shift_eq\n");
	}
//AAAAA	c32_printf("	shr		%s	[%s], cl	;expr_r_shift_eq\n", c32_attr_to_d_w_b(&expr_p1->attr), expr_p1->operand);
	c32_printf("	shr		%s	%s, cl	;expr_r_shift_eq\n", c32_attr_to_d_w_b(&expr_p1->attr), expr_p1->operand);

	return 0;
}
/********************************************************************************/
/*		c32_expr_l_shift_eq														*/
/*		return: 求めた 値の attr												*/
/*		COM20: 定数	operand = "eax", "0x1234"  数値なら	value=0x1234, numb_flag=1	 */
/*		COM21: 配列名 operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: 変数   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		value : 定数の時、定数を返すエリア										*/
/*		used: 1ならば第１項の eax が push されている							*/
/********************************************************************************/
int c32_expr_l_shift_eq(struct expr_param *expr_p1, struct expr_param *expr_p2, int used)
{
	if(expr_p1->attr.attr & (ATTR_POINTER | ATTR_ARRAY)){
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
	}
	if(expr_p2->attr.attr & (ATTR_POINTER | ATTR_ARRAY))
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);

	// shift 量を cl に作る
//	if(c32_direct(expr_p2->comment)){	// 第２項が直接アドレスならば
		c32_printf("	mov		%s, %s	;expr_r_shift_eq\n", c32_attr_to_cx(&expr_p2->attr), expr_p2->operand);
//	}
//	else{
//		c32_printf("	mov		%s, [%s]	;expr_r_shift_eq\n", c32_attr_to_cx(&expr_p2->attr), expr_p2->operand);
//	}
	
	// shift する
	if(used){
		c32_printf("	pop		eax		;expr_r_shift_eq\n");	// 第１項
	}
	c32_printf("	shl		%s	%s, cl	;expr_r_shift_eq\n", c32_attr_to_d_w_b(&expr_p1->attr), expr_p1->operand);

	return 0;
}
/********************************************************************************/
/*		c32_expr_xor_eq															*/
/*		return: 求めた 値の attr												*/
/*		COM20: 定数	operand = "eax", "0x1234"  数値なら	value=0x1234, numb_flag=1	 */
/*		COM21: 配列名 operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: 変数   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		value : 定数の時、定数を返すエリア										*/
/*		used: 1ならば第１項の eax が push されている							*/
/********************************************************************************/
int c32_expr_xor_eq(struct expr_param *expr_p1, struct expr_param *expr_p2, int used)
{
	if(expr_p1->attr.attr & (ATTR_POINTER | ATTR_ARRAY)){
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
	}
	if(expr_p2->attr.attr & (ATTR_POINTER | ATTR_ARRAY))
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);

	convert_com20(expr_p2);		// 第２項を eax に入れる
	if(used){
		c32_printf("	mov		ecx, eax	;expr_xor_eq\n");	// ecx = eax  第２項
		c32_printf("	pop		eax		;expr_xor_eq\n");		// 第１項
		c32_printf("	xor		%s, %s	;expr_xor_eq\n", expr_p1->operand, c32_attr_to_cx(&expr_p2->attr));
	}
	else{
		c32_printf("	xor		%s, %s	;expr_xor_eq\n", expr_p1->operand, c32_attr_to_ax(&expr_p1->attr));
	}

	return 0;
}
/********************************************************************************/
/*		c32_expr_or_eq															*/
/*		return: 求めた 値の attr												*/
/*		COM20: 定数	operand = "eax", "0x1234"  数値なら	value=0x1234, numb_flag=1	 */
/*		COM21: 配列名 operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: 変数   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		value : 定数の時、定数を返すエリア										*/
/*		used: 1ならば第１項の eax が push されている							*/
/********************************************************************************/
int c32_expr_or_eq(struct expr_param *expr_p1, struct expr_param *expr_p2, int used)
{
	if(expr_p1->attr.attr & (ATTR_POINTER | ATTR_ARRAY)){
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
	}
	if(expr_p2->attr.attr & (ATTR_POINTER | ATTR_ARRAY))
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);

	convert_com20(expr_p2);		// 第２項を eax に入れる
	if(used){
		c32_printf("	mov		ecx, eax	;expr_or_eq\n");	// ecx = eax  第２項
		c32_printf("	pop		eax		;expr_or_eq\n");		// 第１項
		c32_printf("	or		%s, %s	;expr_or_eq\n", expr_p1->operand, c32_attr_to_cx(&expr_p2->attr));
	}
	else{
		c32_printf("	or		%s, %s	;expr_or_eq\n", expr_p1->operand, c32_attr_to_ax(&expr_p1->attr));
	}

	return 0;
}
/********************************************************************************/
/*		c32_expr_and_eq															*/
/*		return: 求めた 値の attr												*/
/*		COM20: 定数	operand = "eax", "0x1234"  数値なら	value=0x1234, numb_flag=1	 */
/*		COM21: 配列名 operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: 変数   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		value : 定数の時、定数を返すエリア										*/
/*		used: 1ならば第１項の eax が push されている							*/
/********************************************************************************/
int c32_expr_and_eq(struct expr_param *expr_p1, struct expr_param *expr_p2, int used)
{
	if(expr_p1->attr.attr & (ATTR_POINTER | ATTR_ARRAY)){
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
	}
	if(expr_p2->attr.attr & (ATTR_POINTER | ATTR_ARRAY))
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);

	convert_com20(expr_p2);		// 第２項を eax に入れる
	if(used){
		c32_printf("	mov		ecx, eax	;expr_and_eq\n");	// ecx = eax  第２項
		c32_printf("	pop		eax		;expr_and_eq\n");		// 第１項
		c32_printf("	and		%s, %s	;expr_and_eq\n", expr_p1->operand, c32_attr_to_cx(&expr_p2->attr));
	}
	else{
		c32_printf("	and		%s, %s	;expr_and_eq\n", expr_p1->operand, c32_attr_to_ax(&expr_p1->attr));
	}

	return 0;
}
/********************************************************************************/
/*		c32_expr_percent_eq														*/
/*		return: 求めた 値の attr												*/
/*		COM20: 定数	operand = "eax", "0x1234"  数値なら	value=0x1234, numb_flag=1	 */
/*		COM21: 配列名 operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: 変数   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		value : 定数の時、定数を返すエリア										*/
/*		used: 1ならば第１項の eax が push されている							*/
/********************************************************************************/
int c32_expr_percent_eq(struct expr_param *expr_p1, struct expr_param *expr_p2, int used)
{
	if(expr_p1->attr.attr & (ATTR_POINTER | ATTR_ARRAY)){
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
	}
	if(expr_p2->attr.attr & (ATTR_POINTER | ATTR_ARRAY))
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);

	convert_com20(expr_p2);		// eax = 第２項
	c32_printf("	mov		ebx, eax	;expr_percent_eq\n");	// ebx=第２項
	// 今のところは eax を壊しても第１項は壊れない
	c32_printf("	mov		%s, %s	;expr_percent_eq\n", c32_attr_to_ax(&expr_p1->attr), expr_p1->operand); // 第１項内容を eax に
	if((expr_p1->attr.attr & ATTR_UNSIGNED)==0){	// 符号付きなら符号拡張
		c32_printf("	db		0x99	;expr_percnt_eq\n");	// eax → edx-eax
	}
	else{
		c32_printf("	sub		edx, edx	;expr_percent_eq\n");	// clear
	}
	if((expr_p1->attr.attr & ATTR_UNSIGNED) || (expr_p1->attr.attr & ATTR_UNSIGNED)){	// unsigned
		c32_printf("	div		ebx		;expr_percent_eq\n");	// 符号なし割り算
	}
	else{
		c32_printf("	idiv	ebx		;expr_percent_eq\n");	// 符号付き割り算
	}
	if(used){		// 第１項は eax を使用する
		c32_printf("	pop		eax		;expr_persent_eq\n");
	}
	c32_printf("	mov		%s, %s	;expr_persent_eq\n", expr_p1->operand, c32_attr_to_dx(&expr_p1->attr));	// edx 結果

	return 0;
}
/********************************************************************************/
/*		c32_expr_slush_eq														*/
/*		return: 求めた 値の attr												*/
/*		COM20: 定数	operand = "eax", "0x1234"  数値なら	value=0x1234, numb_flag=1	 */
/*		COM21: 配列名 operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: 変数   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		value : 定数の時、定数を返すエリア										*/
/*		used: 1ならば第１項の eax が push されている							*/
/********************************************************************************/
int c32_expr_slush_eq(struct expr_param *expr_p1, struct expr_param *expr_p2, int used)
{
	if(expr_p1->attr.attr & (ATTR_POINTER | ATTR_ARRAY)){
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
	}
	if(expr_p2->attr.attr & (ATTR_POINTER | ATTR_ARRAY))
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);

	convert_com20(expr_p2);		// eax = 第２項
	c32_printf("	mov		ebx, eax	;expr_slush_eq\n");	// ebx=第２項
	// 今のところは eax を壊しても第１項は壊れない
	c32_printf("	mov		%s, %s	;expr_slush_eq\n", c32_attr_to_ax(&expr_p1->attr), expr_p1->operand); // 第１項内容を eax に
	if((expr_p1->attr.attr & ATTR_UNSIGNED)==0){	// 符号付きなら符号拡張
		c32_printf("	db		0x99	;expr_slush_eq\n");	// eax → edx-eax
	}
	else{
		c32_printf("	sub		edx, edx	;expr_slush_eq\n");	// clear
	}
	if((expr_p1->attr.attr & ATTR_UNSIGNED) || (expr_p1->attr.attr & ATTR_UNSIGNED)){	// unsigned
		c32_printf("	div		ebx		;expr_slush_eq\n");	// 符号なし割り算
	}
	else{
		c32_printf("	idiv	ebx		;expr_slush_eq\n");	// 符号付き割り算
	}
	if(used){		// 第１項は eax を使用する
		c32_printf("	mov		edx, eax	;expr_slush_eq\n");	// edx 結果
		c32_printf("	pop		eax		;expr_slush_eq\n");
		c32_printf("	mov		%s, %s	;expr_slush_eq\n", expr_p1->operand, c32_attr_to_dx(&expr_p1->attr));	// edx 結果
	}
	else{
		c32_printf("	mov		%s, %s	;expr_slush_eq\n", expr_p1->operand, c32_attr_to_ax(&expr_p1->attr));	// eax 結果
	}

	return 0;
}
/********************************************************************************/
/*		c32_expr_asterisk_eq													*/
/*		return: 求めた 値の attr												*/
/*		COM20: 定数	operand = "eax", "0x1234"  数値なら	value=0x1234, numb_flag=1	 */
/*		COM21: 配列名 operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: 変数   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		value : 定数の時、定数を返すエリア										*/
/*		used: 1ならば第１項の eax が push されている							*/
/********************************************************************************/
int c32_expr_asterisk_eq(struct expr_param *expr_p1, struct expr_param *expr_p2, int used)
{
	if(expr_p1->attr.attr & (ATTR_POINTER | ATTR_ARRAY)){
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
	}
	if(expr_p2->attr.attr & (ATTR_POINTER | ATTR_ARRAY))
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);

	convert_com20(expr_p2);		// eax = 第２項
	c32_printf("	mov		ebx, eax	;expr_asterisk_eq\n");	// ebx=第２項
	// 今のところは eax を壊しても第１項は壊れない
	c32_printf("	mov		%s, %s	;expr_asterisk_eq\n", c32_attr_to_ax(&expr_p1->attr), expr_p1->operand); // 第１項内容を eax に
	if((expr_p1->attr.attr & ATTR_UNSIGNED) || (expr_p1->attr.attr & ATTR_UNSIGNED)){	// unsigned
		c32_printf("	mul		ebx		;expr_asterisk_eq\n");	// 符号なし割り算
	}
	else{
		c32_printf("	imul	ebx		;expr_asterisk_eq\n");	// 符号付き割り算
	}
	if(used){		// 第１項は eax を使用する
		c32_printf("	mov		edx, eax	;expr_asterisk_eq\n");	// edx 結果
		c32_printf("	pop		eax		;expr_asterisk_eq\n");
		c32_printf("	mov		%s, %s	;expr_asterisk_eq\n", expr_p1->operand, c32_attr_to_dx(&expr_p1->attr));	// edx 結果
	}
	else{
		c32_printf("	mov		%s, %s	;expr_asterisk_eq\n", expr_p1->operand, c32_attr_to_ax(&expr_p1->attr));	// eax 結果
	}

	return 0;
}
/********************************************************************************/
/*		c32_expr_minus_eq														*/
/*		return: 求めた 値の attr												*/
/*		COM20: 定数	operand = "eax", "0x1234"  数値なら	value=0x1234, numb_flag=1	 */
/*		COM21: 配列名 operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: 変数   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		value : 定数の時、定数を返すエリア										*/
/*		used: 1ならば第１項の eax が push されている							*/
/********************************************************************************/
int c32_expr_minus_eq(struct expr_param *expr_p1, struct expr_param *expr_p2, int used)
{
	int shift;

	if(expr_p2->attr.attr & (ATTR_POINTER | ATTR_ARRAY))
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);

	convert_com20(expr_p2);		// 第２項を eax に入れる
	if(expr_p1->attr.attr & (ATTR_POINTER | ATTR_ARRAY)){	// ポインタへの整数加算
		if(used){
			c32_printf("	mov		ebx, eax	;expr_minus_eq\n");	// ebx=第２項
			c32_printf("	pop		eax		;expr_minus_eq\n");		// eax 第１項
			shift = c32_attr_to_shift_bit(&expr_p1->attr);
			if(shift==1){
				c32_printf("	add		ebx, ebx	;expr_minus_eq\n");	// x2
			}
			else if(shift==2){
				c32_printf("	add		ebx, ebx	;expr_minus_eq\n");	// x2
				c32_printf("	add		ebx, ebx	;expr_minus_eq\n");	// x2
			}
			c32_printf("	sub		%s, ebx	;expr_minus_eq\n", expr_p1->operand);
		}
		else{
			shift = c32_attr_to_shift_bit(&expr_p1->attr);
			if(shift==1){
				c32_printf("	add		eax, eax	;expr_minus_eq\n");	// x2
			}
			else if(shift==2){
				c32_printf("	add		eax, eax	;expr_minus_eq\n");	// x2
				c32_printf("	add		eax, eax	;expr_minus_eq\n");	// x2
			}
			c32_printf("	sub		%s, eax	;expr_minus_eq\n", expr_p1->operand);
		}
	}
	else{		// 整数同士の演算
		if(used){
			c32_printf("	mov		ebx, eax	;expr_minus_eq\n");	// ebx=第２項
			c32_printf("	pop		eax		;expr_minus_eq\n");		// eax 第１項
			c32_printf("	sub		%s, ebx	;expr_minus_eq\n", expr_p1->operand);
		}
		else{
			c32_printf("	sub		%s, eax	;expr_minus_eq\n", expr_p1->operand);
		}
	}

	return 0;
}
/********************************************************************************/
/*		c32_expr_plus_eq														*/
/*		return: 求めた 値の attr												*/
/*		COM20: 定数	operand = "eax", "0x1234"  数値なら	value=0x1234, numb_flag=1	 */
/*		COM21: 配列名 operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: 変数   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		value : 定数の時、定数を返すエリア										*/
/*		used: 1ならば第１項の eax が push されている							*/
/********************************************************************************/
int c32_expr_plus_eq(struct expr_param *expr_p1, struct expr_param *expr_p2, int used)
{
	int shift;

	if(expr_p2->attr.attr & (ATTR_POINTER | ATTR_ARRAY))
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);

	convert_com20(expr_p2);		// 第２項を eax に入れる
	if(expr_p1->attr.attr & (ATTR_POINTER | ATTR_ARRAY)){	// ポインタへの整数加算
		if(used){
			c32_printf("	mov		ebx, eax	;expr_plus_eq\n");	// ebx=第２項
			c32_printf("	pop		eax		;expr_plus_eq\n");		// eax 第１項
			shift = c32_attr_to_shift_bit(&expr_p1->attr);
			if(shift==1){
				c32_printf("	add		ebx, ebx	;expr_plus_eq\n");	// x2
			}
			else if(shift==2){
				c32_printf("	add		ebx, ebx	;expr_plus_eq\n");	// x2
				c32_printf("	add		ebx, ebx	;expr_plus_eq\n");	// x2
			}
			c32_printf("	add		%s, ebx	;expr_plus_eq\n", expr_p1->operand);
		}
		else{
			shift = c32_attr_to_shift_bit(&expr_p1->attr);
			if(shift==1){
				c32_printf("	add		eax, eax	;expr_plus_eq\n");	// x2
			}
			else if(shift==2){
				c32_printf("	add		eax, eax	;expr_plus_eq\n");	// x2
				c32_printf("	add		eax, eax	;expr_plus_eq\n");	// x2
			}
			c32_printf("	add		%s, eax	;expr_plus_eq\n", expr_p1->operand);
		}
	}
	else{		// 整数同士の演算
		if(used){
			c32_printf("	mov		ebx, eax	;expr_plus_eq\n");	// ebx=第２項
			c32_printf("	pop		eax		;expr_plus_eq\n");		// eax 第１項
			c32_printf("	add		%s, ebx	;expr_plus_eq\n", expr_p1->operand);
		}
		else{
			c32_printf("	add		%s, eax	;expr_plus_eq\n", expr_p1->operand);
		}
	}

	return 0;
}
/********************************************************************************/
/*		c32_expr_eq																*/
/*		return: 求めた 値の attr												*/
/*		COM20: 定数	operand = "eax", "0x1234"  数値なら	value=0x1234, numb_flag=1	 */
/*		COM21: 配列名 operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: 変数   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		value : 定数の時、定数を返すエリア										*/
/*		used: 1ならば第１項の eax が push されている							*/
/********************************************************************************/
int c32_expr_eq(struct expr_param *expr_p1, struct expr_param *expr_p2, int used)
{
	convert_com20(expr_p2);		// eax に値を入れる
	if(used==0){			// 第１項は eax を使っていない
		c32_printf("	mov		%s, %s	;expr_eq\n", expr_p1->operand, c32_attr_to_ax(&expr_p1->attr));
	}
	else{			// 第１項は eax を使っている
		c32_printf("	mov		ebx, eax	;expr_eq\n");	// 第２項の reg を ebx にする
		c32_printf("	pop		eax		;expr_eq\n");		// 第１項の eax 復元
		c32_printf("	mov		%s, %s	;expr_eq\n", expr_p1->operand, c32_attr_to_bx(&expr_p1->attr));
	}
	return 0;
}

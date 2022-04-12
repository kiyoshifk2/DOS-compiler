#include "stdafx.h"
#include "banana-c32.h"
#include "function.h"


//	‘æ‚P€‚Í•Ï”ƒAƒhƒŒƒX‘®«‚É‚È‚Á‚Ä‚¢‚é


/********************************************************************************/
/*		c32_expr_r_shift_eq														*/
/*		return: ‹‚ß‚½ ’l‚Ì attr												*/
/*		COM20: ’è”	operand = "eax", "0x1234"  ”’l‚È‚ç	value=0x1234, numb_flag=1	 */
/*		COM21: ”z—ñ–¼ operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: •Ï”   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		value : ’è”‚ÌA’è”‚ğ•Ô‚·ƒGƒŠƒA										*/
/*		used: 1‚È‚ç‚Î‘æ‚P€‚Ì eax ‚ª push ‚³‚ê‚Ä‚¢‚é							*/
/********************************************************************************/
int c32_expr_r_shift_eq(struct expr_param *expr_p1, struct expr_param *expr_p2, int used)
{
	if(expr_p1->attr.attr & (ATTR_POINTER | ATTR_ARRAY)){
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
	}
	if(expr_p2->attr.attr & (ATTR_POINTER | ATTR_ARRAY))
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);

	// shift —Ê‚ğ cl ‚Éì‚é
//	if(c32_direct(expr_p2->comment)){	// ‘æ‚Q€‚ª’¼ÚƒAƒhƒŒƒX‚È‚ç‚Î
		c32_printf("	mov		%s, %s	;expr_r_shift_eq\n", c32_attr_to_cx(&expr_p2->attr), expr_p2->operand);
//	}
//	else{
//		c32_printf("	mov		%s, [%s]	;expr_r_shift_eq\n", c32_attr_to_cx(&expr_p2->attr), expr_p2->operand);
//	}
	
	// shift ‚·‚é
	if(used){
		c32_printf("	pop		eax		;expr_r_shift_eq\n");
	}
//AAAAA	c32_printf("	shr		%s	[%s], cl	;expr_r_shift_eq\n", c32_attr_to_d_w_b(&expr_p1->attr), expr_p1->operand);
	c32_printf("	shr		%s	%s, cl	;expr_r_shift_eq\n", c32_attr_to_d_w_b(&expr_p1->attr), expr_p1->operand);

	return 0;
}
/********************************************************************************/
/*		c32_expr_l_shift_eq														*/
/*		return: ‹‚ß‚½ ’l‚Ì attr												*/
/*		COM20: ’è”	operand = "eax", "0x1234"  ”’l‚È‚ç	value=0x1234, numb_flag=1	 */
/*		COM21: ”z—ñ–¼ operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: •Ï”   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		value : ’è”‚ÌA’è”‚ğ•Ô‚·ƒGƒŠƒA										*/
/*		used: 1‚È‚ç‚Î‘æ‚P€‚Ì eax ‚ª push ‚³‚ê‚Ä‚¢‚é							*/
/********************************************************************************/
int c32_expr_l_shift_eq(struct expr_param *expr_p1, struct expr_param *expr_p2, int used)
{
	if(expr_p1->attr.attr & (ATTR_POINTER | ATTR_ARRAY)){
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
	}
	if(expr_p2->attr.attr & (ATTR_POINTER | ATTR_ARRAY))
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);

	// shift —Ê‚ğ cl ‚Éì‚é
//	if(c32_direct(expr_p2->comment)){	// ‘æ‚Q€‚ª’¼ÚƒAƒhƒŒƒX‚È‚ç‚Î
		c32_printf("	mov		%s, %s	;expr_r_shift_eq\n", c32_attr_to_cx(&expr_p2->attr), expr_p2->operand);
//	}
//	else{
//		c32_printf("	mov		%s, [%s]	;expr_r_shift_eq\n", c32_attr_to_cx(&expr_p2->attr), expr_p2->operand);
//	}
	
	// shift ‚·‚é
	if(used){
		c32_printf("	pop		eax		;expr_r_shift_eq\n");	// ‘æ‚P€
	}
	c32_printf("	shl		%s	%s, cl	;expr_r_shift_eq\n", c32_attr_to_d_w_b(&expr_p1->attr), expr_p1->operand);

	return 0;
}
/********************************************************************************/
/*		c32_expr_xor_eq															*/
/*		return: ‹‚ß‚½ ’l‚Ì attr												*/
/*		COM20: ’è”	operand = "eax", "0x1234"  ”’l‚È‚ç	value=0x1234, numb_flag=1	 */
/*		COM21: ”z—ñ–¼ operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: •Ï”   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		value : ’è”‚ÌA’è”‚ğ•Ô‚·ƒGƒŠƒA										*/
/*		used: 1‚È‚ç‚Î‘æ‚P€‚Ì eax ‚ª push ‚³‚ê‚Ä‚¢‚é							*/
/********************************************************************************/
int c32_expr_xor_eq(struct expr_param *expr_p1, struct expr_param *expr_p2, int used)
{
	if(expr_p1->attr.attr & (ATTR_POINTER | ATTR_ARRAY)){
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
	}
	if(expr_p2->attr.attr & (ATTR_POINTER | ATTR_ARRAY))
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);

	convert_com20(expr_p2);		// ‘æ‚Q€‚ğ eax ‚É“ü‚ê‚é
	if(used){
		c32_printf("	mov		ecx, eax	;expr_xor_eq\n");	// ecx = eax  ‘æ‚Q€
		c32_printf("	pop		eax		;expr_xor_eq\n");		// ‘æ‚P€
		c32_printf("	xor		%s, %s	;expr_xor_eq\n", expr_p1->operand, c32_attr_to_cx(&expr_p2->attr));
	}
	else{
		c32_printf("	xor		%s, %s	;expr_xor_eq\n", expr_p1->operand, c32_attr_to_ax(&expr_p1->attr));
	}

	return 0;
}
/********************************************************************************/
/*		c32_expr_or_eq															*/
/*		return: ‹‚ß‚½ ’l‚Ì attr												*/
/*		COM20: ’è”	operand = "eax", "0x1234"  ”’l‚È‚ç	value=0x1234, numb_flag=1	 */
/*		COM21: ”z—ñ–¼ operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: •Ï”   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		value : ’è”‚ÌA’è”‚ğ•Ô‚·ƒGƒŠƒA										*/
/*		used: 1‚È‚ç‚Î‘æ‚P€‚Ì eax ‚ª push ‚³‚ê‚Ä‚¢‚é							*/
/********************************************************************************/
int c32_expr_or_eq(struct expr_param *expr_p1, struct expr_param *expr_p2, int used)
{
	if(expr_p1->attr.attr & (ATTR_POINTER | ATTR_ARRAY)){
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
	}
	if(expr_p2->attr.attr & (ATTR_POINTER | ATTR_ARRAY))
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);

	convert_com20(expr_p2);		// ‘æ‚Q€‚ğ eax ‚É“ü‚ê‚é
	if(used){
		c32_printf("	mov		ecx, eax	;expr_or_eq\n");	// ecx = eax  ‘æ‚Q€
		c32_printf("	pop		eax		;expr_or_eq\n");		// ‘æ‚P€
		c32_printf("	or		%s, %s	;expr_or_eq\n", expr_p1->operand, c32_attr_to_cx(&expr_p2->attr));
	}
	else{
		c32_printf("	or		%s, %s	;expr_or_eq\n", expr_p1->operand, c32_attr_to_ax(&expr_p1->attr));
	}

	return 0;
}
/********************************************************************************/
/*		c32_expr_and_eq															*/
/*		return: ‹‚ß‚½ ’l‚Ì attr												*/
/*		COM20: ’è”	operand = "eax", "0x1234"  ”’l‚È‚ç	value=0x1234, numb_flag=1	 */
/*		COM21: ”z—ñ–¼ operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: •Ï”   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		value : ’è”‚ÌA’è”‚ğ•Ô‚·ƒGƒŠƒA										*/
/*		used: 1‚È‚ç‚Î‘æ‚P€‚Ì eax ‚ª push ‚³‚ê‚Ä‚¢‚é							*/
/********************************************************************************/
int c32_expr_and_eq(struct expr_param *expr_p1, struct expr_param *expr_p2, int used)
{
	if(expr_p1->attr.attr & (ATTR_POINTER | ATTR_ARRAY)){
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
	}
	if(expr_p2->attr.attr & (ATTR_POINTER | ATTR_ARRAY))
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);

	convert_com20(expr_p2);		// ‘æ‚Q€‚ğ eax ‚É“ü‚ê‚é
	if(used){
		c32_printf("	mov		ecx, eax	;expr_and_eq\n");	// ecx = eax  ‘æ‚Q€
		c32_printf("	pop		eax		;expr_and_eq\n");		// ‘æ‚P€
		c32_printf("	and		%s, %s	;expr_and_eq\n", expr_p1->operand, c32_attr_to_cx(&expr_p2->attr));
	}
	else{
		c32_printf("	and		%s, %s	;expr_and_eq\n", expr_p1->operand, c32_attr_to_ax(&expr_p1->attr));
	}

	return 0;
}
/********************************************************************************/
/*		c32_expr_percent_eq														*/
/*		return: ‹‚ß‚½ ’l‚Ì attr												*/
/*		COM20: ’è”	operand = "eax", "0x1234"  ”’l‚È‚ç	value=0x1234, numb_flag=1	 */
/*		COM21: ”z—ñ–¼ operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: •Ï”   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		value : ’è”‚ÌA’è”‚ğ•Ô‚·ƒGƒŠƒA										*/
/*		used: 1‚È‚ç‚Î‘æ‚P€‚Ì eax ‚ª push ‚³‚ê‚Ä‚¢‚é							*/
/********************************************************************************/
int c32_expr_percent_eq(struct expr_param *expr_p1, struct expr_param *expr_p2, int used)
{
	if(expr_p1->attr.attr & (ATTR_POINTER | ATTR_ARRAY)){
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
	}
	if(expr_p2->attr.attr & (ATTR_POINTER | ATTR_ARRAY))
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);

	convert_com20(expr_p2);		// eax = ‘æ‚Q€
	c32_printf("	mov		ebx, eax	;expr_percent_eq\n");	// ebx=‘æ‚Q€
	// ¡‚Ì‚Æ‚±‚ë‚Í eax ‚ğ‰ó‚µ‚Ä‚à‘æ‚P€‚Í‰ó‚ê‚È‚¢
	c32_printf("	mov		%s, %s	;expr_percent_eq\n", c32_attr_to_ax(&expr_p1->attr), expr_p1->operand); // ‘æ‚P€“à—e‚ğ eax ‚É
	if((expr_p1->attr.attr & ATTR_UNSIGNED)==0){	// •„†•t‚«‚È‚ç•„†Šg’£
		c32_printf("	db		0x99	;expr_percnt_eq\n");	// eax ¨ edx-eax
	}
	else{
		c32_printf("	sub		edx, edx	;expr_percent_eq\n");	// clear
	}
	if((expr_p1->attr.attr & ATTR_UNSIGNED) || (expr_p1->attr.attr & ATTR_UNSIGNED)){	// unsigned
		c32_printf("	div		ebx		;expr_percent_eq\n");	// •„†‚È‚µŠ„‚èZ
	}
	else{
		c32_printf("	idiv	ebx		;expr_percent_eq\n");	// •„†•t‚«Š„‚èZ
	}
	if(used){		// ‘æ‚P€‚Í eax ‚ğg—p‚·‚é
		c32_printf("	pop		eax		;expr_persent_eq\n");
	}
	c32_printf("	mov		%s, %s	;expr_persent_eq\n", expr_p1->operand, c32_attr_to_dx(&expr_p1->attr));	// edx Œ‹‰Ê

	return 0;
}
/********************************************************************************/
/*		c32_expr_slush_eq														*/
/*		return: ‹‚ß‚½ ’l‚Ì attr												*/
/*		COM20: ’è”	operand = "eax", "0x1234"  ”’l‚È‚ç	value=0x1234, numb_flag=1	 */
/*		COM21: ”z—ñ–¼ operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: •Ï”   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		value : ’è”‚ÌA’è”‚ğ•Ô‚·ƒGƒŠƒA										*/
/*		used: 1‚È‚ç‚Î‘æ‚P€‚Ì eax ‚ª push ‚³‚ê‚Ä‚¢‚é							*/
/********************************************************************************/
int c32_expr_slush_eq(struct expr_param *expr_p1, struct expr_param *expr_p2, int used)
{
	if(expr_p1->attr.attr & (ATTR_POINTER | ATTR_ARRAY)){
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
	}
	if(expr_p2->attr.attr & (ATTR_POINTER | ATTR_ARRAY))
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);

	convert_com20(expr_p2);		// eax = ‘æ‚Q€
	c32_printf("	mov		ebx, eax	;expr_slush_eq\n");	// ebx=‘æ‚Q€
	// ¡‚Ì‚Æ‚±‚ë‚Í eax ‚ğ‰ó‚µ‚Ä‚à‘æ‚P€‚Í‰ó‚ê‚È‚¢
	c32_printf("	mov		%s, %s	;expr_slush_eq\n", c32_attr_to_ax(&expr_p1->attr), expr_p1->operand); // ‘æ‚P€“à—e‚ğ eax ‚É
	if((expr_p1->attr.attr & ATTR_UNSIGNED)==0){	// •„†•t‚«‚È‚ç•„†Šg’£
		c32_printf("	db		0x99	;expr_slush_eq\n");	// eax ¨ edx-eax
	}
	else{
		c32_printf("	sub		edx, edx	;expr_slush_eq\n");	// clear
	}
	if((expr_p1->attr.attr & ATTR_UNSIGNED) || (expr_p1->attr.attr & ATTR_UNSIGNED)){	// unsigned
		c32_printf("	div		ebx		;expr_slush_eq\n");	// •„†‚È‚µŠ„‚èZ
	}
	else{
		c32_printf("	idiv	ebx		;expr_slush_eq\n");	// •„†•t‚«Š„‚èZ
	}
	if(used){		// ‘æ‚P€‚Í eax ‚ğg—p‚·‚é
		c32_printf("	mov		edx, eax	;expr_slush_eq\n");	// edx Œ‹‰Ê
		c32_printf("	pop		eax		;expr_slush_eq\n");
		c32_printf("	mov		%s, %s	;expr_slush_eq\n", expr_p1->operand, c32_attr_to_dx(&expr_p1->attr));	// edx Œ‹‰Ê
	}
	else{
		c32_printf("	mov		%s, %s	;expr_slush_eq\n", expr_p1->operand, c32_attr_to_ax(&expr_p1->attr));	// eax Œ‹‰Ê
	}

	return 0;
}
/********************************************************************************/
/*		c32_expr_asterisk_eq													*/
/*		return: ‹‚ß‚½ ’l‚Ì attr												*/
/*		COM20: ’è”	operand = "eax", "0x1234"  ”’l‚È‚ç	value=0x1234, numb_flag=1	 */
/*		COM21: ”z—ñ–¼ operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: •Ï”   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		value : ’è”‚ÌA’è”‚ğ•Ô‚·ƒGƒŠƒA										*/
/*		used: 1‚È‚ç‚Î‘æ‚P€‚Ì eax ‚ª push ‚³‚ê‚Ä‚¢‚é							*/
/********************************************************************************/
int c32_expr_asterisk_eq(struct expr_param *expr_p1, struct expr_param *expr_p2, int used)
{
	if(expr_p1->attr.attr & (ATTR_POINTER | ATTR_ARRAY)){
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
	}
	if(expr_p2->attr.attr & (ATTR_POINTER | ATTR_ARRAY))
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);

	convert_com20(expr_p2);		// eax = ‘æ‚Q€
	c32_printf("	mov		ebx, eax	;expr_asterisk_eq\n");	// ebx=‘æ‚Q€
	// ¡‚Ì‚Æ‚±‚ë‚Í eax ‚ğ‰ó‚µ‚Ä‚à‘æ‚P€‚Í‰ó‚ê‚È‚¢
	c32_printf("	mov		%s, %s	;expr_asterisk_eq\n", c32_attr_to_ax(&expr_p1->attr), expr_p1->operand); // ‘æ‚P€“à—e‚ğ eax ‚É
	if((expr_p1->attr.attr & ATTR_UNSIGNED) || (expr_p1->attr.attr & ATTR_UNSIGNED)){	// unsigned
		c32_printf("	mul		ebx		;expr_asterisk_eq\n");	// •„†‚È‚µŠ„‚èZ
	}
	else{
		c32_printf("	imul	ebx		;expr_asterisk_eq\n");	// •„†•t‚«Š„‚èZ
	}
	if(used){		// ‘æ‚P€‚Í eax ‚ğg—p‚·‚é
		c32_printf("	mov		edx, eax	;expr_asterisk_eq\n");	// edx Œ‹‰Ê
		c32_printf("	pop		eax		;expr_asterisk_eq\n");
		c32_printf("	mov		%s, %s	;expr_asterisk_eq\n", expr_p1->operand, c32_attr_to_dx(&expr_p1->attr));	// edx Œ‹‰Ê
	}
	else{
		c32_printf("	mov		%s, %s	;expr_asterisk_eq\n", expr_p1->operand, c32_attr_to_ax(&expr_p1->attr));	// eax Œ‹‰Ê
	}

	return 0;
}
/********************************************************************************/
/*		c32_expr_minus_eq														*/
/*		return: ‹‚ß‚½ ’l‚Ì attr												*/
/*		COM20: ’è”	operand = "eax", "0x1234"  ”’l‚È‚ç	value=0x1234, numb_flag=1	 */
/*		COM21: ”z—ñ–¼ operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: •Ï”   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		value : ’è”‚ÌA’è”‚ğ•Ô‚·ƒGƒŠƒA										*/
/*		used: 1‚È‚ç‚Î‘æ‚P€‚Ì eax ‚ª push ‚³‚ê‚Ä‚¢‚é							*/
/********************************************************************************/
int c32_expr_minus_eq(struct expr_param *expr_p1, struct expr_param *expr_p2, int used)
{
	int shift;

	if(expr_p2->attr.attr & (ATTR_POINTER | ATTR_ARRAY))
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);

	convert_com20(expr_p2);		// ‘æ‚Q€‚ğ eax ‚É“ü‚ê‚é
	if(expr_p1->attr.attr & (ATTR_POINTER | ATTR_ARRAY)){	// ƒ|ƒCƒ“ƒ^‚Ö‚Ì®”‰ÁZ
		if(used){
			c32_printf("	mov		ebx, eax	;expr_minus_eq\n");	// ebx=‘æ‚Q€
			c32_printf("	pop		eax		;expr_minus_eq\n");		// eax ‘æ‚P€
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
	else{		// ®”“¯m‚Ì‰‰Z
		if(used){
			c32_printf("	mov		ebx, eax	;expr_minus_eq\n");	// ebx=‘æ‚Q€
			c32_printf("	pop		eax		;expr_minus_eq\n");		// eax ‘æ‚P€
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
/*		return: ‹‚ß‚½ ’l‚Ì attr												*/
/*		COM20: ’è”	operand = "eax", "0x1234"  ”’l‚È‚ç	value=0x1234, numb_flag=1	 */
/*		COM21: ”z—ñ–¼ operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: •Ï”   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		value : ’è”‚ÌA’è”‚ğ•Ô‚·ƒGƒŠƒA										*/
/*		used: 1‚È‚ç‚Î‘æ‚P€‚Ì eax ‚ª push ‚³‚ê‚Ä‚¢‚é							*/
/********************************************************************************/
int c32_expr_plus_eq(struct expr_param *expr_p1, struct expr_param *expr_p2, int used)
{
	int shift;

	if(expr_p2->attr.attr & (ATTR_POINTER | ATTR_ARRAY))
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);

	convert_com20(expr_p2);		// ‘æ‚Q€‚ğ eax ‚É“ü‚ê‚é
	if(expr_p1->attr.attr & (ATTR_POINTER | ATTR_ARRAY)){	// ƒ|ƒCƒ“ƒ^‚Ö‚Ì®”‰ÁZ
		if(used){
			c32_printf("	mov		ebx, eax	;expr_plus_eq\n");	// ebx=‘æ‚Q€
			c32_printf("	pop		eax		;expr_plus_eq\n");		// eax ‘æ‚P€
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
	else{		// ®”“¯m‚Ì‰‰Z
		if(used){
			c32_printf("	mov		ebx, eax	;expr_plus_eq\n");	// ebx=‘æ‚Q€
			c32_printf("	pop		eax		;expr_plus_eq\n");		// eax ‘æ‚P€
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
/*		return: ‹‚ß‚½ ’l‚Ì attr												*/
/*		COM20: ’è”	operand = "eax", "0x1234"  ”’l‚È‚ç	value=0x1234, numb_flag=1	 */
/*		COM21: ”z—ñ–¼ operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: •Ï”   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		value : ’è”‚ÌA’è”‚ğ•Ô‚·ƒGƒŠƒA										*/
/*		used: 1‚È‚ç‚Î‘æ‚P€‚Ì eax ‚ª push ‚³‚ê‚Ä‚¢‚é							*/
/********************************************************************************/
int c32_expr_eq(struct expr_param *expr_p1, struct expr_param *expr_p2, int used)
{
	convert_com20(expr_p2);		// eax ‚É’l‚ğ“ü‚ê‚é
	if(used==0){			// ‘æ‚P€‚Í eax ‚ğg‚Á‚Ä‚¢‚È‚¢
		c32_printf("	mov		%s, %s	;expr_eq\n", expr_p1->operand, c32_attr_to_ax(&expr_p1->attr));
	}
	else{			// ‘æ‚P€‚Í eax ‚ğg‚Á‚Ä‚¢‚é
		c32_printf("	mov		ebx, eax	;expr_eq\n");	// ‘æ‚Q€‚Ì reg ‚ğ ebx ‚É‚·‚é
		c32_printf("	pop		eax		;expr_eq\n");		// ‘æ‚P€‚Ì eax •œŒ³
		c32_printf("	mov		%s, %s	;expr_eq\n", expr_p1->operand, c32_attr_to_bx(&expr_p1->attr));
	}
	return 0;
}

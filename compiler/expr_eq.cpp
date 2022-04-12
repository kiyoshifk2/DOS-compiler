#include "stdafx.h"
#include "banana-c32.h"
#include "function.h"


//	��P���͕ϐ��A�h���X�����ɂȂ��Ă���


/********************************************************************************/
/*		c32_expr_r_shift_eq														*/
/*		return: ���߂� �l�� attr												*/
/*		COM20: �萔	operand = "eax", "0x1234"  ���l�Ȃ�	value=0x1234, numb_flag=1	 */
/*		COM21: �z�� operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: �ϐ�   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		value : �萔�̎��A�萔��Ԃ��G���A										*/
/*		used: 1�Ȃ�Α�P���� eax �� push ����Ă���							*/
/********************************************************************************/
int c32_expr_r_shift_eq(struct expr_param *expr_p1, struct expr_param *expr_p2, int used)
{
	if(expr_p1->attr.attr & (ATTR_POINTER | ATTR_ARRAY)){
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
	}
	if(expr_p2->attr.attr & (ATTR_POINTER | ATTR_ARRAY))
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);

	// shift �ʂ� cl �ɍ��
//	if(c32_direct(expr_p2->comment)){	// ��Q�������ڃA�h���X�Ȃ��
		c32_printf("	mov		%s, %s	;expr_r_shift_eq\n", c32_attr_to_cx(&expr_p2->attr), expr_p2->operand);
//	}
//	else{
//		c32_printf("	mov		%s, [%s]	;expr_r_shift_eq\n", c32_attr_to_cx(&expr_p2->attr), expr_p2->operand);
//	}
	
	// shift ����
	if(used){
		c32_printf("	pop		eax		;expr_r_shift_eq\n");
	}
//AAAAA	c32_printf("	shr		%s	[%s], cl	;expr_r_shift_eq\n", c32_attr_to_d_w_b(&expr_p1->attr), expr_p1->operand);
	c32_printf("	shr		%s	%s, cl	;expr_r_shift_eq\n", c32_attr_to_d_w_b(&expr_p1->attr), expr_p1->operand);

	return 0;
}
/********************************************************************************/
/*		c32_expr_l_shift_eq														*/
/*		return: ���߂� �l�� attr												*/
/*		COM20: �萔	operand = "eax", "0x1234"  ���l�Ȃ�	value=0x1234, numb_flag=1	 */
/*		COM21: �z�� operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: �ϐ�   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		value : �萔�̎��A�萔��Ԃ��G���A										*/
/*		used: 1�Ȃ�Α�P���� eax �� push ����Ă���							*/
/********************************************************************************/
int c32_expr_l_shift_eq(struct expr_param *expr_p1, struct expr_param *expr_p2, int used)
{
	if(expr_p1->attr.attr & (ATTR_POINTER | ATTR_ARRAY)){
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
	}
	if(expr_p2->attr.attr & (ATTR_POINTER | ATTR_ARRAY))
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);

	// shift �ʂ� cl �ɍ��
//	if(c32_direct(expr_p2->comment)){	// ��Q�������ڃA�h���X�Ȃ��
		c32_printf("	mov		%s, %s	;expr_r_shift_eq\n", c32_attr_to_cx(&expr_p2->attr), expr_p2->operand);
//	}
//	else{
//		c32_printf("	mov		%s, [%s]	;expr_r_shift_eq\n", c32_attr_to_cx(&expr_p2->attr), expr_p2->operand);
//	}
	
	// shift ����
	if(used){
		c32_printf("	pop		eax		;expr_r_shift_eq\n");	// ��P��
	}
	c32_printf("	shl		%s	%s, cl	;expr_r_shift_eq\n", c32_attr_to_d_w_b(&expr_p1->attr), expr_p1->operand);

	return 0;
}
/********************************************************************************/
/*		c32_expr_xor_eq															*/
/*		return: ���߂� �l�� attr												*/
/*		COM20: �萔	operand = "eax", "0x1234"  ���l�Ȃ�	value=0x1234, numb_flag=1	 */
/*		COM21: �z�� operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: �ϐ�   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		value : �萔�̎��A�萔��Ԃ��G���A										*/
/*		used: 1�Ȃ�Α�P���� eax �� push ����Ă���							*/
/********************************************************************************/
int c32_expr_xor_eq(struct expr_param *expr_p1, struct expr_param *expr_p2, int used)
{
	if(expr_p1->attr.attr & (ATTR_POINTER | ATTR_ARRAY)){
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
	}
	if(expr_p2->attr.attr & (ATTR_POINTER | ATTR_ARRAY))
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);

	convert_com20(expr_p2);		// ��Q���� eax �ɓ����
	if(used){
		c32_printf("	mov		ecx, eax	;expr_xor_eq\n");	// ecx = eax  ��Q��
		c32_printf("	pop		eax		;expr_xor_eq\n");		// ��P��
		c32_printf("	xor		%s, %s	;expr_xor_eq\n", expr_p1->operand, c32_attr_to_cx(&expr_p2->attr));
	}
	else{
		c32_printf("	xor		%s, %s	;expr_xor_eq\n", expr_p1->operand, c32_attr_to_ax(&expr_p1->attr));
	}

	return 0;
}
/********************************************************************************/
/*		c32_expr_or_eq															*/
/*		return: ���߂� �l�� attr												*/
/*		COM20: �萔	operand = "eax", "0x1234"  ���l�Ȃ�	value=0x1234, numb_flag=1	 */
/*		COM21: �z�� operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: �ϐ�   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		value : �萔�̎��A�萔��Ԃ��G���A										*/
/*		used: 1�Ȃ�Α�P���� eax �� push ����Ă���							*/
/********************************************************************************/
int c32_expr_or_eq(struct expr_param *expr_p1, struct expr_param *expr_p2, int used)
{
	if(expr_p1->attr.attr & (ATTR_POINTER | ATTR_ARRAY)){
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
	}
	if(expr_p2->attr.attr & (ATTR_POINTER | ATTR_ARRAY))
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);

	convert_com20(expr_p2);		// ��Q���� eax �ɓ����
	if(used){
		c32_printf("	mov		ecx, eax	;expr_or_eq\n");	// ecx = eax  ��Q��
		c32_printf("	pop		eax		;expr_or_eq\n");		// ��P��
		c32_printf("	or		%s, %s	;expr_or_eq\n", expr_p1->operand, c32_attr_to_cx(&expr_p2->attr));
	}
	else{
		c32_printf("	or		%s, %s	;expr_or_eq\n", expr_p1->operand, c32_attr_to_ax(&expr_p1->attr));
	}

	return 0;
}
/********************************************************************************/
/*		c32_expr_and_eq															*/
/*		return: ���߂� �l�� attr												*/
/*		COM20: �萔	operand = "eax", "0x1234"  ���l�Ȃ�	value=0x1234, numb_flag=1	 */
/*		COM21: �z�� operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: �ϐ�   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		value : �萔�̎��A�萔��Ԃ��G���A										*/
/*		used: 1�Ȃ�Α�P���� eax �� push ����Ă���							*/
/********************************************************************************/
int c32_expr_and_eq(struct expr_param *expr_p1, struct expr_param *expr_p2, int used)
{
	if(expr_p1->attr.attr & (ATTR_POINTER | ATTR_ARRAY)){
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
	}
	if(expr_p2->attr.attr & (ATTR_POINTER | ATTR_ARRAY))
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);

	convert_com20(expr_p2);		// ��Q���� eax �ɓ����
	if(used){
		c32_printf("	mov		ecx, eax	;expr_and_eq\n");	// ecx = eax  ��Q��
		c32_printf("	pop		eax		;expr_and_eq\n");		// ��P��
		c32_printf("	and		%s, %s	;expr_and_eq\n", expr_p1->operand, c32_attr_to_cx(&expr_p2->attr));
	}
	else{
		c32_printf("	and		%s, %s	;expr_and_eq\n", expr_p1->operand, c32_attr_to_ax(&expr_p1->attr));
	}

	return 0;
}
/********************************************************************************/
/*		c32_expr_percent_eq														*/
/*		return: ���߂� �l�� attr												*/
/*		COM20: �萔	operand = "eax", "0x1234"  ���l�Ȃ�	value=0x1234, numb_flag=1	 */
/*		COM21: �z�� operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: �ϐ�   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		value : �萔�̎��A�萔��Ԃ��G���A										*/
/*		used: 1�Ȃ�Α�P���� eax �� push ����Ă���							*/
/********************************************************************************/
int c32_expr_percent_eq(struct expr_param *expr_p1, struct expr_param *expr_p2, int used)
{
	if(expr_p1->attr.attr & (ATTR_POINTER | ATTR_ARRAY)){
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
	}
	if(expr_p2->attr.attr & (ATTR_POINTER | ATTR_ARRAY))
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);

	convert_com20(expr_p2);		// eax = ��Q��
	c32_printf("	mov		ebx, eax	;expr_percent_eq\n");	// ebx=��Q��
	// ���̂Ƃ���� eax ���󂵂Ă���P���͉��Ȃ�
	c32_printf("	mov		%s, %s	;expr_percent_eq\n", c32_attr_to_ax(&expr_p1->attr), expr_p1->operand); // ��P�����e�� eax ��
	if((expr_p1->attr.attr & ATTR_UNSIGNED)==0){	// �����t���Ȃ畄���g��
		c32_printf("	db		0x99	;expr_percnt_eq\n");	// eax �� edx-eax
	}
	else{
		c32_printf("	sub		edx, edx	;expr_percent_eq\n");	// clear
	}
	if((expr_p1->attr.attr & ATTR_UNSIGNED) || (expr_p1->attr.attr & ATTR_UNSIGNED)){	// unsigned
		c32_printf("	div		ebx		;expr_percent_eq\n");	// �����Ȃ�����Z
	}
	else{
		c32_printf("	idiv	ebx		;expr_percent_eq\n");	// �����t������Z
	}
	if(used){		// ��P���� eax ���g�p����
		c32_printf("	pop		eax		;expr_persent_eq\n");
	}
	c32_printf("	mov		%s, %s	;expr_persent_eq\n", expr_p1->operand, c32_attr_to_dx(&expr_p1->attr));	// edx ����

	return 0;
}
/********************************************************************************/
/*		c32_expr_slush_eq														*/
/*		return: ���߂� �l�� attr												*/
/*		COM20: �萔	operand = "eax", "0x1234"  ���l�Ȃ�	value=0x1234, numb_flag=1	 */
/*		COM21: �z�� operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: �ϐ�   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		value : �萔�̎��A�萔��Ԃ��G���A										*/
/*		used: 1�Ȃ�Α�P���� eax �� push ����Ă���							*/
/********************************************************************************/
int c32_expr_slush_eq(struct expr_param *expr_p1, struct expr_param *expr_p2, int used)
{
	if(expr_p1->attr.attr & (ATTR_POINTER | ATTR_ARRAY)){
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
	}
	if(expr_p2->attr.attr & (ATTR_POINTER | ATTR_ARRAY))
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);

	convert_com20(expr_p2);		// eax = ��Q��
	c32_printf("	mov		ebx, eax	;expr_slush_eq\n");	// ebx=��Q��
	// ���̂Ƃ���� eax ���󂵂Ă���P���͉��Ȃ�
	c32_printf("	mov		%s, %s	;expr_slush_eq\n", c32_attr_to_ax(&expr_p1->attr), expr_p1->operand); // ��P�����e�� eax ��
	if((expr_p1->attr.attr & ATTR_UNSIGNED)==0){	// �����t���Ȃ畄���g��
		c32_printf("	db		0x99	;expr_slush_eq\n");	// eax �� edx-eax
	}
	else{
		c32_printf("	sub		edx, edx	;expr_slush_eq\n");	// clear
	}
	if((expr_p1->attr.attr & ATTR_UNSIGNED) || (expr_p1->attr.attr & ATTR_UNSIGNED)){	// unsigned
		c32_printf("	div		ebx		;expr_slush_eq\n");	// �����Ȃ�����Z
	}
	else{
		c32_printf("	idiv	ebx		;expr_slush_eq\n");	// �����t������Z
	}
	if(used){		// ��P���� eax ���g�p����
		c32_printf("	mov		edx, eax	;expr_slush_eq\n");	// edx ����
		c32_printf("	pop		eax		;expr_slush_eq\n");
		c32_printf("	mov		%s, %s	;expr_slush_eq\n", expr_p1->operand, c32_attr_to_dx(&expr_p1->attr));	// edx ����
	}
	else{
		c32_printf("	mov		%s, %s	;expr_slush_eq\n", expr_p1->operand, c32_attr_to_ax(&expr_p1->attr));	// eax ����
	}

	return 0;
}
/********************************************************************************/
/*		c32_expr_asterisk_eq													*/
/*		return: ���߂� �l�� attr												*/
/*		COM20: �萔	operand = "eax", "0x1234"  ���l�Ȃ�	value=0x1234, numb_flag=1	 */
/*		COM21: �z�� operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: �ϐ�   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		value : �萔�̎��A�萔��Ԃ��G���A										*/
/*		used: 1�Ȃ�Α�P���� eax �� push ����Ă���							*/
/********************************************************************************/
int c32_expr_asterisk_eq(struct expr_param *expr_p1, struct expr_param *expr_p2, int used)
{
	if(expr_p1->attr.attr & (ATTR_POINTER | ATTR_ARRAY)){
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
	}
	if(expr_p2->attr.attr & (ATTR_POINTER | ATTR_ARRAY))
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);

	convert_com20(expr_p2);		// eax = ��Q��
	c32_printf("	mov		ebx, eax	;expr_asterisk_eq\n");	// ebx=��Q��
	// ���̂Ƃ���� eax ���󂵂Ă���P���͉��Ȃ�
	c32_printf("	mov		%s, %s	;expr_asterisk_eq\n", c32_attr_to_ax(&expr_p1->attr), expr_p1->operand); // ��P�����e�� eax ��
	if((expr_p1->attr.attr & ATTR_UNSIGNED) || (expr_p1->attr.attr & ATTR_UNSIGNED)){	// unsigned
		c32_printf("	mul		ebx		;expr_asterisk_eq\n");	// �����Ȃ�����Z
	}
	else{
		c32_printf("	imul	ebx		;expr_asterisk_eq\n");	// �����t������Z
	}
	if(used){		// ��P���� eax ���g�p����
		c32_printf("	mov		edx, eax	;expr_asterisk_eq\n");	// edx ����
		c32_printf("	pop		eax		;expr_asterisk_eq\n");
		c32_printf("	mov		%s, %s	;expr_asterisk_eq\n", expr_p1->operand, c32_attr_to_dx(&expr_p1->attr));	// edx ����
	}
	else{
		c32_printf("	mov		%s, %s	;expr_asterisk_eq\n", expr_p1->operand, c32_attr_to_ax(&expr_p1->attr));	// eax ����
	}

	return 0;
}
/********************************************************************************/
/*		c32_expr_minus_eq														*/
/*		return: ���߂� �l�� attr												*/
/*		COM20: �萔	operand = "eax", "0x1234"  ���l�Ȃ�	value=0x1234, numb_flag=1	 */
/*		COM21: �z�� operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: �ϐ�   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		value : �萔�̎��A�萔��Ԃ��G���A										*/
/*		used: 1�Ȃ�Α�P���� eax �� push ����Ă���							*/
/********************************************************************************/
int c32_expr_minus_eq(struct expr_param *expr_p1, struct expr_param *expr_p2, int used)
{
	int shift;

	if(expr_p2->attr.attr & (ATTR_POINTER | ATTR_ARRAY))
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);

	convert_com20(expr_p2);		// ��Q���� eax �ɓ����
	if(expr_p1->attr.attr & (ATTR_POINTER | ATTR_ARRAY)){	// �|�C���^�ւ̐������Z
		if(used){
			c32_printf("	mov		ebx, eax	;expr_minus_eq\n");	// ebx=��Q��
			c32_printf("	pop		eax		;expr_minus_eq\n");		// eax ��P��
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
	else{		// �������m�̉��Z
		if(used){
			c32_printf("	mov		ebx, eax	;expr_minus_eq\n");	// ebx=��Q��
			c32_printf("	pop		eax		;expr_minus_eq\n");		// eax ��P��
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
/*		return: ���߂� �l�� attr												*/
/*		COM20: �萔	operand = "eax", "0x1234"  ���l�Ȃ�	value=0x1234, numb_flag=1	 */
/*		COM21: �z�� operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: �ϐ�   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		value : �萔�̎��A�萔��Ԃ��G���A										*/
/*		used: 1�Ȃ�Α�P���� eax �� push ����Ă���							*/
/********************************************************************************/
int c32_expr_plus_eq(struct expr_param *expr_p1, struct expr_param *expr_p2, int used)
{
	int shift;

	if(expr_p2->attr.attr & (ATTR_POINTER | ATTR_ARRAY))
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);

	convert_com20(expr_p2);		// ��Q���� eax �ɓ����
	if(expr_p1->attr.attr & (ATTR_POINTER | ATTR_ARRAY)){	// �|�C���^�ւ̐������Z
		if(used){
			c32_printf("	mov		ebx, eax	;expr_plus_eq\n");	// ebx=��Q��
			c32_printf("	pop		eax		;expr_plus_eq\n");		// eax ��P��
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
	else{		// �������m�̉��Z
		if(used){
			c32_printf("	mov		ebx, eax	;expr_plus_eq\n");	// ebx=��Q��
			c32_printf("	pop		eax		;expr_plus_eq\n");		// eax ��P��
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
/*		return: ���߂� �l�� attr												*/
/*		COM20: �萔	operand = "eax", "0x1234"  ���l�Ȃ�	value=0x1234, numb_flag=1	 */
/*		COM21: �z�� operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: �ϐ�   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		value : �萔�̎��A�萔��Ԃ��G���A										*/
/*		used: 1�Ȃ�Α�P���� eax �� push ����Ă���							*/
/********************************************************************************/
int c32_expr_eq(struct expr_param *expr_p1, struct expr_param *expr_p2, int used)
{
	convert_com20(expr_p2);		// eax �ɒl������
	if(used==0){			// ��P���� eax ���g���Ă��Ȃ�
		c32_printf("	mov		%s, %s	;expr_eq\n", expr_p1->operand, c32_attr_to_ax(&expr_p1->attr));
	}
	else{			// ��P���� eax ���g���Ă���
		c32_printf("	mov		ebx, eax	;expr_eq\n");	// ��Q���� reg �� ebx �ɂ���
		c32_printf("	pop		eax		;expr_eq\n");		// ��P���� eax ����
		c32_printf("	mov		%s, %s	;expr_eq\n", expr_p1->operand, c32_attr_to_bx(&expr_p1->attr));
	}
	return 0;
}

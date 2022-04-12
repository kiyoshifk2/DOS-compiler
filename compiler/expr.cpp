#include "stdafx.h"
#include "banana-c32.h"
#include "function.h"



#if 0	// new
	switch(expr_p1->comment){
		case COM20:		// �萔			operand = "eax", "0x1234"
						// ���l�Ȃ�		value=0x1234, numb_flag=1
			break;
		case COM21:		// �z��		operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"
			break;
		case COM22:		// �ϐ��A�h���X	operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]"
			break;
		default:
			c32_error_message(E_FATAL_ERROR, __LINE__, __FILE__);
	}
#endif



static int factor5_sub(int attr1, int attr2);
void convert_com22(struct expr_param *expr_p1);


/********************************************************************************/
/*		expr_function_call														*/
/*		int sub(int a, int b, int c, int d, int e, int f){ int g;}				*/
/*		stack:	f																*/
/*				e																*/
/*				d																*/
/*				c																*/
/*				b																*/
/*				a																*/
/*				�߂�A�h���X													*/
/*			......................... call ���̃X�^�b�N							*/
/*				ebp																*/
/*				g																*/
/********************************************************************************/
static void expr_function_call(struct expr_param *expr_p1, struct symtbl *ptr)
{
#ifdef DEBUG
printf("=== start expr_function_call\n");
#endif
	int i, type, param_numb;
	struct symtbl *func = expr_p1->func;
	
	c32_token_process(0, 0, &type);				// '(' ���m�F����
	if(type != TYPE_L_KAKKO)
		c32_error_message(E_L_KAKKO_MISSING, __LINE__, __FILE__);
	
	param_numb = ptr->func_tbl->param_numb;
	if(param_numb)
		c32_printf("	sub		esp, 0x%x	;expr_function_call\n", param_numb*4);
	if(param_numb==0){
		c32_token_process(0, 0, &type);
		if(type != TYPE_R_KAKKO)
			c32_error_message(E_R_KAKKO_MISSING, __LINE__, __FILE__);
	}
	else{
		for(i=0; i<param_numb; i++){
			memset(expr_p1, 0, sizeof(struct expr_param));
			expr_p1->func = func;
			c32_factor1(expr_p1);				// c32_expr() �ł� ',' ���Z�q�����������Ă��܂�
											// �֐��̈����̌v�Z
			convert_com20(expr_p1);		// eax: �l
			
			c32_printf("	mov		[esp+0x%x], eax		;expr_function_call\n", i*4);
			c32_token_process(0, 0, &type);			// ',' ���� ')'
			if(type==TYPE_KANMA)
				continue;
			else if(type==TYPE_R_KAKKO)
				break;
			else
				c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
		}
		if(i != param_numb-1)
			c32_error_message(E_PARAM_NUMBER_ERROR, __LINE__, __FILE__);
	}
	c32_printf("	call	%s		;expr_function_call\n", ptr->symbuf);

	if(param_numb)
		c32_printf("	add		esp, 0x%x	;expr_function_call\n", param_numb*4);
#ifdef DEBUG
printf("=== end expr_function_call\n");
#endif
}

static void expr_function_call_pass1(struct expr_param *expr_p1)
{
#ifdef DEBUG
printf("=== start expr_function_call_pass1\n");
#endif
	int type;
	struct src_ptr src_ptr;
	struct expr_param expr_p2;
	
	c32_token_process(0, 0, &type);				// '(' ���m�F����
	if(type != TYPE_L_KAKKO)
		c32_error_message(E_L_KAKKO_MISSING, __LINE__, __FILE__);
	c32_src_ptr_save(&src_ptr);
	c32_token_process(0, 0, &type);				// ')' �𒲂ׂ�
	if(type != TYPE_R_KAKKO){
		c32_src_ptr_restore(&src_ptr);
		expr_p2 = *expr_p1;
		for(;;){
			memset(&expr_p2, 0, sizeof(expr_p2));
			expr_p2.func = expr_p1->func;
			c32_factor1(&expr_p2);				// �֐��̈����̌v�Z
			convert_com20(&expr_p2);
			c32_token_process(0, 0, &type);
			if(type==TYPE_R_KAKKO)
				break;
			if(type != TYPE_KANMA)
				c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
		}
	}
#ifdef DEBUG
printf("=== end expr_function_call_pass1\n");
#endif
}
/********************************************************************************/
/*		factor																	*/
/*		COM20: �萔	operand = "eax", "0x1234"  ���l�Ȃ�	value=0x1234, numb_flag=1	 */
/*		COM21: �z�� operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: �ϐ�   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		return: eax �Ɍ��ʂ������Ă���ƂP�ɂȂ邻���łȂ���΂O				*/
/*		value : �萔�̎��A�萔��Ԃ��G���A										*/
/*		func �� 0 �̏ꍇ�� global table �̂݌�������							*/
/********************************************************************************/
static int factor(struct expr_param *expr_p1)
{
#ifdef DEBUG
printf("=== start factor\n");
#endif
	int i, type, used;
	struct symtbl *ptr;
	struct src_ptr src_ptr;
	struct symtbl *func = expr_p1->func;
	struct struct_tbl *tbl = expr_p1->tbl;
	
//	c32_printf(";---------- factor ----------\n");
	memset(expr_p1, 0, sizeof(struct expr_param));
	expr_p1->func = func;
	expr_p1->tbl  = tbl;

	ptr = c32_token_process(expr_p1->func, expr_p1->tbl, &type);
	if(expr_p1->tbl){		// struct �� element
		if(ptr==0){
			c32_error_message(E_UNDEFINED_SYMBOL, __LINE__, __FILE__);
		}
		expr_p1->xyz_dim = ptr->xyz_dim;
		memcpy(expr_p1->xyz_size, ptr->xyz_size, sizeof(expr_p1->xyz_size));
		c32_printf("	mov		eax, 0x%x	;factor\n", ptr->value);
		strcpy(expr_p1->operand, "eax");
		expr_p1->comment = COM20;	// eax
		expr_p1->numb_flag = 0;
		expr_p1->attr = ptr->attr;	// struct ���͂��̒��ɓ����Ă���
#ifdef DEBUG
printf("=== end factor\n");
#endif
		return 1;
	}
	if(type==TYPE_DIGIT){					// �萔 ====================
		expr_p1->comment = COM20;			// ���l
		expr_p1->numb_flag = 1;
		expr_p1->value = *((uint*)cv->string_buf);
		sprintf(expr_p1->operand, "0x%x", expr_p1->value);
//		expr_p1->attr.attr = c32_make_attr_from_constant(expr_p1->value);
		expr_p1->attr.attr = ATTR_INT;
#ifdef DEBUG
printf("=== end factor\n");
#endif
		return 0;
	}
	else if(type==TYPE_STRING){				// ������ ====================
		int v_numb = c32_label_counter++;
		int p_numb = c32_label_counter++;
		if(c32_init_string_flag){		// struct �̏������p
			c32_init_string_ptr += sprintf(&cv->init_string[c32_init_string_ptr], "L%d:\n", v_numb);
			for(i=0; i<(int)strlen(cv->string_buf)+1; i++){
				c32_init_string_ptr += sprintf(&cv->init_string[c32_init_string_ptr], "	db		0x%x\n", (uchar)cv->string_buf[i]);
			}
			if(c32_init_string_ptr > MAX_INIT_STRING_LEN){
				c32_error_message(E_OUT_OF_MEMORY, __LINE__, __FILE__);
			}
			if(c32_init_string_ptr > c32_max_init_string){
				c32_max_init_string = c32_init_string_ptr;
			}
		}
		else{
			if(c32_no_label_out==0){
				c32_printf("	jmp		L%d\n", p_numb);
				c32_printf("L%d:\n", v_numb);
				for(i=0; i<(int)strlen(cv->string_buf)+1; i++){
					c32_printf("	db		0x%x\n", (uchar)cv->string_buf[i]);
				}
				c32_printf("L%d:\n", p_numb);
			}
		}
		sprintf(expr_p1->operand, "L%d", v_numb);
		expr_p1->comment = COM21;			// ������z��
		expr_p1->numb_flag = 1;
		expr_p1->attr.attr = ATTR_CHAR | ATTR_ARRAY;
#ifdef DEBUG
printf("=== end factor\n");
#endif
		return 0;
	}
	else if(type==TYPE_L_KAKKO){			// '('  ====================	(c32_expr) �̂�
		used = c32_expr(expr_p1);
		
		c32_token_process(0, 0, &type);
		if(type==TYPE_R_KAKKO){				// ')'
#ifdef DEBUG
printf("=== end factor\n");
#endif
			return used;
		}
		c32_error_message(E_R_KAKKO_MISSING, __LINE__, __FILE__);
	}
	else if(type==TYPE_SYMBOL){				// symbol  ====================   �֐�/�ϐ�
//		expr_p1->ptr = ptr;
		if(ptr==0){							// undef
			c32_error_message(E_UNDEFINED_SYMBOL, __LINE__, __FILE__);
		}
		if(ptr->attr.attr & ATTR_FUNC){			// �֐�������  --------------------
			c32_printf(";---------- call %s ----------\n", ptr->symbuf);
			if(c32_pass==PASS1){
				expr_function_call_pass1(expr_p1);
			}
			else{
				expr_function_call(expr_p1, ptr);	// �֐��Ăяo��
			}
			expr_p1->comment = COM20;			// �v�Z������, acc
			expr_p1->attr = ptr->attr;		// ATTR_FUNC ���N���A���鎖
			strcpy(expr_p1->operand, "eax");
			expr_p1->numb_flag = 0;
#ifdef DEBUG
printf("=== end factor\n");
#endif
			return 1;
		}
		else if(ptr->attr.attr & ATTR_STRUCT){	// struct ----------------------
			expr_p1->xyz_dim = ptr->xyz_dim;
			memcpy(expr_p1->xyz_size, ptr->xyz_size, sizeof(expr_p1->xyz_size));
			if(ptr->flag1 & FLAG_04){	// global
				c32_printf("	mov		eax, %s		;factor\n", ptr->label);
			}
			else{						// auto/param
				c32_printf("	mov		eax, ebp+%s	;factor\n", ptr->label);
			}
			strcpy(expr_p1->operand, "eax");
			expr_p1->comment = COM20;	// eax
			expr_p1->numb_flag = 0;
			expr_p1->attr = ptr->attr;	// struct ���͂��̒��ɓ����Ă���
#ifdef DEBUG
printf("=== end factor\n");
#endif
			return 1;
		}
		else if(ptr->attr.attr & ATTR_ARRAY){	// array -----------------------
			expr_p1->xyz_dim = ptr->xyz_dim;
			memcpy(expr_p1->xyz_size, ptr->xyz_size, sizeof(expr_p1->xyz_size));
			if(ptr->flag1 & FLAG_04){		// global
				strcpy(expr_p1->operand, ptr->label);
				expr_p1->comment = COM21;	// �z�񖼃I�y�����h
				expr_p1->numb_flag = 0;
				expr_p1->attr = ptr->attr;
#ifdef DEBUG
printf("=== end factor\n");
#endif
				return 0;
			}
			else{							// auto/param
				c32_printf("	mov		eax, ebp\n");
				c32_printf("	add		eax, %s\n", ptr->label);
				strcpy(expr_p1->operand, "eax");
				expr_p1->comment = COM21;		// �z�񖼃I�y�����h
				expr_p1->numb_flag = 0;
				expr_p1->attr = ptr->attr;
#ifdef DEBUG
printf("=== end factor\n");
#endif
				return 1;
			}
		}
		else{								// �P���ϐ�  --------------------
			expr_p1->xyz_dim = ptr->xyz_dim;
			memcpy(expr_p1->xyz_size, ptr->xyz_size, sizeof(expr_p1->xyz_size));
			c32_src_ptr_save(&src_ptr);
			c32_token_process(0, 0, &type);
			c32_src_ptr_restore(&src_ptr);
			if(type==TYPE_L_KAKKO){			// �֐��|�C���^
				c32_error_message(E_TO_BE_DEFINE, __LINE__, __FILE__);
				
//				expr_function_call_pointer(expr_p1, ptr);
//				expr_p1->mode = MODE0;
//				expr_p1->attr = ptr->attr;
//				return;
			}
			if(ptr->flag1 & FLAG_04){		// global
				sprintf(expr_p1->operand, "[%s]", ptr->label);
				expr_p1->comment = COM22;	// �ϐ��A�h���X�I�y�����h
				expr_p1->numb_flag = 0;
			}
			else{							// auto/param
				sprintf(expr_p1->operand, "[ebp+%s]", ptr->label);
				expr_p1->comment = COM22;		// �ϐ��A�h���X
				expr_p1->numb_flag = 0;
			}
			expr_p1->attr = ptr->attr;
#ifdef DEBUG
printf("=== end factor\n");
#endif
			return 0;
		}
	}
	else{
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
	}
#ifdef DEBUG
printf("=== end factor\n");
#endif
	return 0;
}
/********************************************************************************/
/*		factor16																*/
/*		COM20: �萔	operand = "eax", "0x1234"  ���l�Ȃ�	value=0x1234, numb_flag=1	 */
/*		COM21: �z�� operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: �ϐ�   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		return: eax �Ɍ��ʂ������Ă���ƂP�ɂȂ邻���łȂ���΂O				*/
/*		value : �萔�̎��A�萔��Ԃ��G���A										*/
/*		func �� 0 �̏ꍇ�� global table �̂݌�������							*/
/********************************************************************************/
static int factor16(struct expr_param *expr_p1)
{
#ifdef DEBUG
printf("=== start factor16\n");
#endif
	return factor(expr_p1);
#ifdef DEBUG
printf("=== end factor16\n");
#endif
}
/********************************************************************************/
/*		factor15																*/
/*		mode=0: �v�Z�������i���W�X�^�j											*/
/*		mode=1: ���W�X�^����													*/
/*		mode=2: �萔															*/
/*		mode=6: ���x���A�h���X�����Avalue=label_numb							*/
/*																				*/
/*		value : �萔�̎��A�萔��Ԃ��G���A										*/
/*		func �� 0 �̏ꍇ�� global table �̂݌�������							*/
/*		factor16 []																*/
/********************************************************************************/
static int factor15(struct expr_param *expr_p1)
{
//	return factor16(expr_p1);

#ifdef DEBUG
printf("=== start factor15\n");
#endif
	int i, type, used, size;
	struct src_ptr src_ptr;
	struct expr_param expr_p2;
	
	used = factor16(expr_p1);

	for(;;){
		c32_src_ptr_save(&src_ptr);
		c32_token_process(0, 0, &type);				// '[' ���e�X�g����
		if(type != TYPE_L_KAKUKAKKO){
			c32_src_ptr_restore(&src_ptr);
#ifdef DEBUG
printf("=== end factor15\n");
#endif
			return used;
		}
		/***	'[' ������		***/
		if(expr_p1->attr.attr & ATTR_ARRAY){		// �z�� -----------
			if(used){
				convert_com20(expr_p1);
				c32_printf("	push	eax		;factor15\n");	// push p1
			}
		
			memset(&expr_p2, 0, sizeof(expr_p2));
			expr_p2.func = expr_p1->func;
			expr_p2.tbl  = expr_p1->tbl;
			c32_expr(&expr_p2);							// �z��̑�����
		
			size = c32_attr_to_byte(&expr_p1->attr);
			for(i=1; i<expr_p1->xyz_dim; i++){
				size *= expr_p1->xyz_size[i];
			}
			
			if(used){
				if(expr_p2.comment==COM20 && expr_p2.numb_flag){
					c32_printf("	pop		eax		;factor15\n");	// eax p1
					c32_printf("	add		eax, 0x%x	;factor15\n", size*expr_p2.value);	// eax: �A�h���X
				}
				else{
					convert_com20(&expr_p2);	// eax: p2
					c32_printf("	mov		ebx, 0x%x	;factor15\n", size);
					c32_printf("	mul		ebx		;factor15\n", size);	// eax: �I�t�Z�b�g
					c32_printf("	pop		ebx		;factor15\n");	// p1
					c32_printf("	add		eax, ebx	;factor15\n");
				}
			}
			else if(expr_p2.comment==COM20 && expr_p2.numb_flag){
				if(strcmp(expr_p1->operand, "eax")){
					c32_printf("	mov		eax, %s		;factor15\n", expr_p1->operand);
				}
				c32_printf("	add		eax, 0x%x	;factor15\n", size*expr_p2.value);	// eax: �A�h���X
			}
			else{
				convert_com20(&expr_p2);	// eax: p2
				c32_printf("	mov		ebx, 0x%x	;factor15\n", size);
				c32_printf("	mul		ebx		;factor15\n");		// eax: �I�t�Z�b�g
				c32_printf("	add		eax, %s		;factor15\n", expr_p1->operand);
			}
			strcpy(expr_p1->operand, "eax");
			expr_p1->comment = COM21;
			expr_p1->numb_flag = 0;
			
			memmove(&expr_p1->xyz_size[0], &expr_p1->xyz_size[1], expr_p1->xyz_dim*sizeof(ushort));
			expr_p1->attr.attr -= ATTR_ARRAY1;
			if(--expr_p1->xyz_dim==0){		// �ϐ���
				strcpy(expr_p1->operand, "[eax]");
				expr_p1->comment = COM22;	// �ϐ�
			}
			
			c32_token_process(0, 0, &type);
			if(type != TYPE_R_KAKUKAKKO){
				c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
			}
//			return 1;
			used = 1;
		}
		else if(expr_p1->attr.attr & ATTR_POINTER){		// �|�C���^ ----------
			convert_com20(expr_p1);		// eax: p1
			c32_printf("	push	eax		;factor15\n");	// push p1
		
			expr_p1->attr.attr -= ATTR_POINTER1;
			
			memset(&expr_p2, 0, sizeof(expr_p2));
			expr_p2.func = expr_p1->func;
			expr_p2.tbl  = expr_p1->tbl;
			c32_expr(&expr_p2);							// �z��̑�����
		
			size = c32_attr_to_byte(&expr_p1->attr);
//			for(i=1; i<expr_p1->xyz_dim; i++){
//				size *= expr_p1->xyz_size[i];
//			}
			
			if(expr_p2.comment==COM20 && expr_p2.numb_flag){
				c32_printf("	pop		eax		;factor15\n");	// eax p1
				c32_printf("	add		eax, 0x%x	;factor15\n", size*expr_p2.value);	// eax: �A�h���X
			}
			else{
				convert_com20(&expr_p2);	// eax: p2
				c32_printf("	mov		ebx, 0x%x	;factor15\n", size);
				c32_printf("	mul		ebx		;factor15\n", size);	// eax: �I�t�Z�b�g
				c32_printf("	pop		ebx		;factor15\n");	// p1
				c32_printf("	add		eax, ebx	;factor15\n");
			}
			strcpy(expr_p1->operand, "[eax]");
			expr_p1->comment = COM22;
			expr_p1->numb_flag = 0;
			
//			memmove(&expr_p1->xyz_size[0], &expr_p1->xyz_size[1], expr_p1->xyz_dim*sizeof(ushort));
//			expr_p1->attr.attr -= ATTR_ARRAY1;
//			if(--expr_p1->xyz_dim==0){		// �ϐ���
//				strcpy(expr_p1->operand, "[eax]");
//				expr_p1->comment = COM22;	// �ϐ�
//			}
			
			c32_token_process(0, 0, &type);
			if(type != TYPE_R_KAKUKAKKO){
				c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
			}
//			return 1;
			used = 1;
		}
		else{
			c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
		}
	}
}
/********************************************************************************/
/*		factor14																*/
/*		COM20: �萔	operand = "eax", "0x1234"  ���l�Ȃ�	value=0x1234, numb_flag=1	 */
/*		COM21: �z�� operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: �ϐ�   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		return: eax �Ɍ��ʂ������Ă���ƂP�ɂȂ邻���łȂ���΂O				*/
/*		value : �萔�̎��A�萔��Ԃ��G���A										*/
/*		factor15.symbol  factor16->symbol										*/
/********************************************************************************/
static int factor14(struct expr_param *expr_p1)
{
//	return factor16(expr_p1);

#ifdef DEBUG
printf("=== start factor14\n");
#endif
	int type, used, first;
//	char buf[20];
	struct expr_param expr_p2;
	struct src_ptr src_ptr;
	struct struct_tbl *tbl;
//	struct struct_elem *elem;
	
	used = factor15(expr_p1);
	
	first = 1;
	for(;;){
		c32_src_ptr_save(&src_ptr);
		c32_token_process(0, 0, &type);
		if(type==TYPE_PERIOD){
			// struct, �|�C���^�łȂ� �z��ł��Ȃ�
			if((expr_p1->attr.attr & ATTR_STRUCT)==0 || (expr_p1->attr.attr & ATTR_POINTER)
					|| (expr_p1->attr.attr & ATTR_ARRAY)){
				c32_error_message(E_NOT_STRUCTURE, __LINE__, __FILE__);
			}
next1:;
			/***	�X�g���N�`���[�e�[�u���T�[�`	***/
			for(tbl=c32_st_root; tbl; tbl=tbl->next){
				if(strcmp(expr_p1->attr.symbuf, tbl->symbuf)==0)
					break;
			}
			if(tbl==0){
				c32_error_message(E_UNDEFINED_SYMBOL, __LINE__, __FILE__);
			}
			if(c32_is_sym_top()==0)
				c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);	// error
	
			convert_com20(expr_p1);		// eax:����
			c32_printf("	push	eax		;factor14\n");
			memset(&expr_p2, 0, sizeof(expr_p2));
			expr_p2.func = expr_p1->func;
			expr_p2.tbl  = tbl;
			factor15(&expr_p2);
			
			convert_com20(&expr_p2);		// eax: ����
			c32_printf("	pop		ebx		;factor14\n");
			c32_printf("	add		eax, ebx	;factor14\n");	// eax �ɃA�h���X���o���Ă���
			
//			strcpy(expr_p2.operand, "[eax]");
//			expr_p2.comment = COM22;
//			convert_com20(&expr_p2);
	
			expr_p2.tbl  = expr_p1->tbl;
			*expr_p1 = expr_p2;
			used = 1;
			first = 0;
			continue;
//AAAAA		return 1;
		}
		else if(type==TYPE_YAZIRUSI){
			if((expr_p1->attr.attr & ATTR_STRUCT)==0 || (expr_p1->attr.attr & ATTR_ARRAY)
					|| (expr_p1->attr.attr & ATTR_POINTER) != ATTR_POINTER1){
				c32_error_message(E_NOT_STRUCTURE, __LINE__, __FILE__);
			}
			if(first==0){
				strcpy(expr_p1->operand, "[eax]");
				expr_p1->comment = COM22;
				convert_com20(expr_p1);
			}
			goto next1;
		}
		else if(first){
			c32_src_ptr_restore(&src_ptr);
#ifdef DEBUG
printf("=== end factor14\n");
#endif
			return used;
		}
		else{
			c32_src_ptr_restore(&src_ptr);	
	
			strcpy(expr_p1->operand, "[eax]");
			expr_p1->comment = COM22;
			convert_com20(expr_p1);
#ifdef DEBUG
printf("=== end factor14\n");
#endif
			return 1;
		}
	}
}
/********************************************************************************/
/*		c32_strip																*/
/*		[label] �� label														*/
/*		[eax]   �� eax															*/
/*		[ebp+1] �� eax=ebp+1													*/
/*		return 1:eax ���g���Ă���												*/
/********************************************************************************/
int c32_strip(struct expr_param *expr_p1)
{
#ifdef DEBUG
printf("=== start c32_strip\n");
#endif
	char buf[MAX_SYMBUF_LEN];
	char *pt;
	
	strcpy(buf, expr_p1->operand);
	if(buf[0] != '['){
		c32_error_message(E_INTERNAL_ERROR, __LINE__, __FILE__);
	}
	memmove(&buf[0], &buf[1], MAX_SYMBUF_LEN);	// '[' �͂����
	if(buf[strlen(buf)-1] != ']'){
		c32_error_message(E_INTERNAL_ERROR, __LINE__, __FILE__);
	}
	buf[strlen(buf)-1] = 0;
	expr_p1->attr.attr += ATTR_POINTER1;
	if((expr_p1->attr.attr & ATTR_POINTER)==0){
		c32_error_message(E_TABLE_OVERFLOW, __LINE__, __FILE__);
	}
	strcpy(expr_p1->operand, buf);
	expr_p1->numb_flag = 0;
	if(strcmp(buf, "eax")==0){		// eax ������
		expr_p1->comment = COM20;	// �萔
#ifdef DEBUG
printf("=== end c32_strip\n");
#endif
		return 1;
	}
	if((pt=strstr(buf, "ebp"))==0){	// ebp �͂Ȃ�����
		if(buf[0]>='0' && buf[0]<='9'){
			expr_p1->numb_flag = 1;
		}
		expr_p1->comment = COM20;	// �萔
#ifdef DEBUG
printf("=== end c32_strip\n");
#endif
		return 0;
	}
	if(pt != buf){
		c32_error_message(E_INTERNAL_ERROR, __LINE__, __FILE__);
	}
	c32_printf("	mov		eax, ebp	;c32_strip\n");
	c32_printf("	add		eax, %s		;c32_strip\n", &buf[4]);
	strcpy(expr_p1->operand, "eax");
	expr_p1->comment = COM20;		// �萔
#ifdef DEBUG
printf("=== end c32_strip\n");
#endif
	return 1;
}
/********************************************************************************/
/*		factor13																*/
/*		COM20: �萔	operand = "eax", "0x1234"  ���l�Ȃ�	value=0x1234, numb_flag=1	 */
/*		COM21: �z�� operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: �ϐ�   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		return: eax �Ɍ��ʂ������Ă���ƂP�ɂȂ邻���łȂ���΂O				*/
/*		value : �萔�̎��A�萔��Ԃ��G���A										*/
/*		func �� 0 �̏ꍇ�� global table �̂݌�������							*/
/*		! ~ ++ -- - * & (type) sizeof											*/
/********************************************************************************/
static int factor13(struct expr_param *expr_p1)
{
//	return factor14(expr_p1);

#ifdef DEBUG
printf("=== start factor13\n");
#endif
	struct attr_s attr2;
	int i, type, ret, used;
	struct src_ptr src_ptr;
//	struct symtbl *ptr;
	int top_numb = c32_label_counter++;
	int end_numb = c32_label_counter++;
	char buf[MAX_SYMBUF_LEN];
	
	c32_src_ptr_save(&src_ptr);
	c32_token_process(0, 0, &type);
	if(type==TYPE_L_KAKKO){					// (type) ����
		ret = c32_char_short_int_unsigned_pointer(&attr2);
		if(ret==0){
			// if �𔲂��� factor14 ++ -- �������s��
			c32_src_ptr_restore(&src_ptr);
//			return factor14(expr_p1);	// factor13() �ɂ͏o���Ȃ�
		}
		else {
			c32_token_process(0, 0, &type);			// ')' ���m�F����
			if (type != TYPE_R_KAKKO)
				c32_error_message(E_R_KAKKO_MISSING, __LINE__, __FILE__);

			factor13(expr_p1);
			convert_com20(expr_p1);		// eax �Ɍ��ʂ�����
			expr_p1->attr = attr2;		// �L���X�g�� attr �ɂ���
			// ebx �� eax �ɕ����g��
			if(attr2.attr & ATTR_UNSIGNED){
				c32_printf("	mov		ebx, eax	;factor13\n");	// ebx �ɕۑ�
				c32_printf("	xor		eax, eax	;factor13\n");	// eax clear
				c32_printf("	mov		%s, %s	;factor13\n", c32_attr_to_ax(&attr2), c32_attr_to_bx(&attr2));
			}
			else{
				if(c32_attr_to_byte(&attr2)==1){
					c32_printf("	cbw		;convert_com10\n");
#ifdef BIT16
					c32_printf("	db 0x66\n");
#endif
					c32_printf("	db 0x98		;convert_com10\n");
				}
				else if(c32_attr_to_byte(&attr2)==2){
#ifdef BIT16
					c32_printf("	db 0x66\n");
#endif
					c32_printf("	db 0x98		;convert_com10\n");
				}
			}
#ifdef DEBUG
printf("=== end factor13\n");
#endif
			return 1;
		}
	}
	else if(type==TYPE_ASTERISK){			// '*'
		used = factor13(expr_p1);
		
		if(expr_p1->attr.attr & ATTR_ARRAY){	// �z��Ȃ��
			// array �̎���������
			expr_p1->attr.attr -= ATTR_ARRAY1;
			if(expr_p1->xyz_dim==0){
				c32_error_message(E_INTERNAL_ERROR, __LINE__, __FILE__);
			}
			memmove(&expr_p1->xyz_size[0], &expr_p1->xyz_size[1], sizeof(ushort)*expr_p1->xyz_dim);
			// �z��łȂ��Ȃ�����ϐ�������
			if((expr_p1->attr.attr & ATTR_ARRAY)==0){
				strcpy(buf, expr_p1->operand);
				sprintf(expr_p1->operand, "[%s]", buf);
				expr_p1->comment = COM22;
			}
#ifdef DEBUG
printf("=== end factor13\n");
#endif
			return used;
		}
		else if(expr_p1->attr.attr & ATTR_POINTER){	// �|�C���^�Ȃ��
			if(expr_p1->comment==COM20){
				convert_com22(expr_p1);		// [eax]
//AAAAA				convert_com20(expr_p1);		// eax = [eax]
#ifdef DEBUG
printf("=== end factor13\n");
#endif
				return 1;
			}
			else if(expr_p1->comment==COM22){
				convert_com20(expr_p1);		// eax: �ϐ����e�܂�|�C���^�l
				convert_com22(expr_p1);		// [eax]
//AAAAA				convert_com20(expr_p1);		// eax = [eax]
#ifdef DEBUG
printf("=== end factor13\n");
#endif
				return 1;
			}
			else{
				c32_error_message(E_INTERNAL_ERROR, __LINE__, __FILE__);
			}
		}
		else{
			c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
		}
	}
	else if(type==TYPE_AND){				// '&'
		used = factor13(expr_p1);
		
		if(expr_p1->attr.attr & ATTR_ARRAY){	// �z�񖼂� & ���Z�̓G���[
			c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
		}
		convert_com22(expr_p1);		// �A�h���X�����ɂ���  used �͕ω����Ȃ�
		// pointer �������グ��
		expr_p1->attr.attr += ATTR_POINTER1;
		if((expr_p1->attr.attr & ATTR_POINTER)==0){
			c32_error_message(E_TABLE_OVERFLOW, __LINE__, __FILE__);
		}
		used = c32_strip(expr_p1);		// [] ���͂����
#ifdef DEBUG
printf("=== end factor13\n");
#endif
		return used;
	}
	else if(type==TYPE_PLUS_PLUS){			// ++
		int tmp;
		used = factor13(expr_p1);
		
		// �z��֎~�Astruct �� pointer �̂݋���
		if(expr_p1->attr.attr & ATTR_ARRAY){
			c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
		}
		if((expr_p1->attr.attr & ATTR_STRUCT) && !(expr_p1->attr.attr & ATTR_POINTER)){
			c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
		}
		
		if(expr_p1->attr.attr & ATTR_POINTER){
			expr_p1->attr.attr -= ATTR_POINTER1;
			tmp = c32_attr_to_byte(&expr_p1->attr);	// ���������
			expr_p1->attr.attr += ATTR_POINTER1;
		}
		else{
			tmp = 1;
		}
		switch(expr_p1->comment){
			case COM20:				// �萔
				c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
			case COM21:				// �z��擪�A�h���X
				c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
			case COM22:				// �ϐ����e
				break;
		}
		
		c32_printf("	add		%s	%s, 0x%x	;factor13\n", c32_attr_to_d_w_b(&expr_p1->attr), expr_p1->operand, tmp);
#ifdef DEBUG
printf("=== end factor13\n");
#endif
		return used;
	}
	else if(type==TYPE_MINUS_MINUS){		// --
		int tmp;
		used = factor13(expr_p1);
		
		// �z��֎~�Astruct �� pointer �̂݋���
		if(expr_p1->attr.attr & ATTR_ARRAY){
			c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
		}
		if((expr_p1->attr.attr & ATTR_STRUCT) && !(expr_p1->attr.attr & ATTR_POINTER)){
			c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
		}

		if(expr_p1->attr.attr & ATTR_POINTER){
			expr_p1->attr.attr -= ATTR_POINTER1;
			tmp = c32_attr_to_byte(&expr_p1->attr);	// ���������
			expr_p1->attr.attr += ATTR_POINTER1;
		}
		else{
			tmp = 1;
		}
		switch(expr_p1->comment){
			case COM20:				// �萔
				c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
			case COM21:				// �z��擪�A�h���X
				c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
			case COM22:				// �ϐ����e
				break;
		}

		c32_printf("	sub		%s	%s, 0x%x	;factor13\n", c32_attr_to_d_w_b(&expr_p1->attr), expr_p1->operand, tmp);
#ifdef DEBUG
printf("=== end factor13\n");
#endif
		return used;
	}
	else if(type==TYPE_MINUS){				// '-'
		used = factor13(expr_p1);
		
		if(expr_p1->attr.attr & (ATTR_POINTER|ATTR_ARRAY|ATTR_STRUCT|ATTR_FUNC)){
			c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
		}
		if(expr_p1->comment==COM20 && expr_p1->numb_flag){	// ���l
			expr_p1->value = -expr_p1->value;
			sprintf(expr_p1->operand, "0x%x", expr_p1->value);
#ifdef DEBUG
printf("=== end factor13\n");
#endif
			return 0;
		}
		convert_com20(expr_p1);		// eax:����
		c32_printf("	neg	 	eax		;factor13\n");
#ifdef DEBUG
printf("=== end factor13\n");
#endif
		return 1;
	}
	else if(type==TYPE_TILDE){				// '~'
		used = factor13(expr_p1);
		
		if(expr_p1->attr.attr & (ATTR_POINTER|ATTR_ARRAY|ATTR_STRUCT|ATTR_FUNC)){
			c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
		}
		if(expr_p1->comment==COM20 && expr_p1->numb_flag){	// ���l
			expr_p1->value = ~expr_p1->value;
			sprintf(expr_p1->operand, "0x%x", expr_p1->value);
#ifdef DEBUG
printf("=== end factor13\n");
#endif
			return 0;
		}
		convert_com20(expr_p1);		// eax:����
		c32_printf("	not		eax		;factor13\n");
#ifdef DEBUG
printf("=== end factor13\n");
#endif
		return 1;
	}
	else if(type==TYPE_NOT){				// '!'
		used = factor13(expr_p1);
		
		// �z��֎~�Astruct �� pointer �̂݋���
		if(expr_p1->attr.attr & ATTR_ARRAY){
			c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
		}
		if((expr_p1->attr.attr & ATTR_STRUCT) && !(expr_p1->attr.attr & ATTR_POINTER)){
			c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
		}
		if(expr_p1->comment==COM20 && expr_p1->numb_flag){	// ���l
			if(expr_p1->value){
				expr_p1->value = 0;
			}
			else{
				expr_p1->value = 1;
			}
			sprintf(expr_p1->operand, "0x%x", expr_p1->value);
#ifdef DEBUG
printf("=== end factor13\n");
#endif
			return 0;
		}
		c32_printf("	sub		%s	%s, 0	;factor13\n", c32_attr_to_d_w_b(&expr_p1->attr), expr_p1->operand);
		c32_printf("	mov		eax, 1	;factor13\n");
		c32_printf("	jz		L%d		;factor13\n", end_numb);
		c32_printf("	xor		eax, eax	;factor13\n");
		c32_printf("L%d:		;factor13\n", end_numb);
		expr_p1->comment = COM20;
		expr_p1->numb_flag = 0;
		strcpy(expr_p1->operand, "eax");
#ifdef DEBUG
printf("=== end factor13\n");
#endif
		return 1;
	}
	else if(type==TYPE_SIZEOF){				// sizeof variable / sizeof int
		c32_src_ptr_save(&src_ptr);
		ret = c32_char_short_int_unsigned_pointer(&attr2);
		if(ret==0){
			c32_src_ptr_restore(&src_ptr);
			c32_spskip();
			if(*c32_linebufp=='('){
				c32_linebufp++;
				c32_src_ptr_save(&src_ptr);
				ret = c32_char_short_int_unsigned_pointer(&attr2);
				if(ret){
					c32_token_process(0, 0, &type);
					if(type != TYPE_R_KAKKO){
						c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
					}
					expr_p1->value = c32_attr_to_byte(&attr2);
					expr_p1->comment = COM20;		// ���l
					expr_p1->numb_flag = 1;
					sprintf(expr_p1->operand, "0x%x", expr_p1->value);
#ifdef DEBUG
printf("=== end factor13\n");
#endif
					return 0;
				}
				c32_src_ptr_restore(&src_ptr);
				factor14(expr_p1);
				c32_token_process(0, 0, &type);
				if(type != TYPE_R_KAKKO){
						c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
				}
			}
			else{
				factor14(expr_p1);
			}
			expr_p1->value = c32_attr_to_byte(&expr_p1->attr);
			
			if(expr_p1->attr.attr & ATTR_ARRAY){
				for(i=0; i<expr_p1->xyz_dim; i++){
					expr_p1->value *= expr_p1->xyz_size[i];
				}
			}
			expr_p1->comment = COM20;		// ���l
			expr_p1->numb_flag = 1;
			sprintf(expr_p1->operand, "0x%x", expr_p1->value);
#ifdef DEBUG
printf("=== end factor13\n");
#endif
			return 0;
		}
		expr_p1->value = c32_attr_to_byte(&attr2);
		expr_p1->comment = COM20;		// ���l
		expr_p1->numb_flag = 1;
		sprintf(expr_p1->operand, "0x%x", expr_p1->value);
#ifdef DEBUG
printf("=== end factor13\n");
#endif
		return 0;
	}
	// �O�u���Z�q����������
	c32_src_ptr_restore(&src_ptr);
	used = factor14(expr_p1);						// factor13() �ɂ͏o���Ȃ�
	
	c32_src_ptr_save(&src_ptr);
	c32_token_process(0, 0, &type);
	if(type==TYPE_PLUS_PLUS){			// variable++
		int tmp;
		
		// �z��֎~�Astruct �� pointer �̂݋���
		if(expr_p1->attr.attr & ATTR_ARRAY){
			c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
		}
		if((expr_p1->attr.attr & ATTR_STRUCT) && !(expr_p1->attr.attr & ATTR_POINTER)){
			c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
		}
		if(expr_p1->attr.attr & ATTR_POINTER){
			expr_p1->attr.attr -= ATTR_POINTER1;
			tmp = c32_attr_to_byte(&expr_p1->attr);	// ���������
			expr_p1->attr.attr += ATTR_POINTER1;
		}
		else{
			tmp = 1;
		}
		switch(expr_p1->comment){
			case COM20:				// �萔
				c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
			case COM21:				// �z��擪�A�h���X
				c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
			case COM22:				// �ϐ����e
				break;
		}
		c32_printf("	mov		ebx, %s		;factor13\n", expr_p1->operand);
		c32_printf("	add		%s	%s, 0x%x	;factor13\n", c32_attr_to_d_w_b(&expr_p1->attr), expr_p1->operand, tmp);
		c32_printf("	mov		eax, ebx	;factor13\n");
		strcpy(expr_p1->operand, "eax");
		expr_p1->comment = COM20;
#ifdef DEBUG
printf("=== end factor13\n");
#endif
		return 1;
	}
	else if(type==TYPE_MINUS_MINUS){		// variable--
		int tmp;
		
		// �z��֎~�Astruct �� pointer �̂݋���
		if(expr_p1->attr.attr & ATTR_ARRAY){
			c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
		}
		if((expr_p1->attr.attr & ATTR_STRUCT) && !(expr_p1->attr.attr & ATTR_POINTER)){
			c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
		}
		if(expr_p1->attr.attr & ATTR_POINTER){
			expr_p1->attr.attr -= ATTR_POINTER1;
			tmp = c32_attr_to_byte(&expr_p1->attr);	// ���������
			expr_p1->attr.attr += ATTR_POINTER1;
		}
		else{
			tmp = 1;
		}
		switch(expr_p1->comment){
			case COM20:				// �萔
				c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
			case COM21:				// �z��擪�A�h���X
				c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
			case COM22:				// �ϐ����e
				break;
		}
		c32_printf("	mov		ebx, %s		;factor13\n", expr_p1->operand);
		c32_printf("	sub		%s	%s, 0x%x	;factor13\n", c32_attr_to_d_w_b(&expr_p1->attr), expr_p1->operand, tmp);
		c32_printf("	mov		eax, ebx	;factor13\n");
		strcpy(expr_p1->operand, "eax");
		expr_p1->comment = COM20;
#ifdef DEBUG
printf("=== end factor13\n");
#endif
		return 1;
	}
	else{
		c32_src_ptr_restore(&src_ptr);
#ifdef DEBUG
printf("=== end factor13\n");
#endif
		return used;
	}
}
/********************************************************************************/
/*		factor12																*/
/*		COM20: �萔	operand = "eax", "0x1234"  ���l�Ȃ�	value=0x1234, numb_flag=1	 */
/*		COM21: �z�� operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: �ϐ�   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		return: eax �Ɍ��ʂ������Ă���ƂP�ɂȂ邻���łȂ���΂O				*/
/*		value : �萔�̎��A�萔��Ԃ��G���A										*/
/*		func �� 0 �̏ꍇ�� global table �̂݌�������							*/
/*		factor13 * / % factor13													*/
/********************************************************************************/
static int factor12(struct expr_param *expr_p1)
{
//	return factor13(func, mode, value);

#ifdef DEBUG
printf("=== start factor12\n");
#endif
	int type, used;
	struct src_ptr src_ptr;
	struct expr_param expr_p2;
	
	used = factor13(expr_p1);
	c32_src_ptr_save(&src_ptr);
	
	c32_token_process(0, 0, &type);
	if(type==TYPE_ASTERISK || type==TYPE_SLUSH || type==TYPE_PERCENT){
		if(expr_p1->attr.attr & (ATTR_ARRAY | ATTR_STRUCT | ATTR_FUNC)){
			c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
		}
		if(expr_p1->attr.attr & ATTR_POINTER){
			c32_error_message(E_TO_BE_DEFINE, __LINE__, __FILE__);
		}
		for(;;){
			if(used){
				convert_com20(expr_p1);
				c32_printf("	push	eax		;factor12\n");	// p1 push
			}
			memset(&expr_p2, 0, sizeof(expr_p2));
			expr_p2.func = expr_p1->func;
			expr_p2.tbl  = expr_p1->tbl;
			factor13(&expr_p2);
			
			if(expr_p2.attr.attr & (ATTR_ARRAY | ATTR_STRUCT | ATTR_FUNC | ATTR_POINTER)){
				c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
			}
			if(used){	// p1 �� push ����Ă���
				convert_com20(&expr_p2);
				c32_printf("	mov		ebx, eax	;factor12\n");	// ebx = p2
				c32_printf("	pop		eax		;factor12\n");	// eax = p1
next1:;
				if(type==TYPE_ASTERISK){	// *
					if((expr_p1->attr.attr & ATTR_UNSIGNED) || (expr_p2.attr.attr & ATTR_UNSIGNED)){
						c32_printf("	mul		ebx		;factor12\n");	// eax * ebx
					}
					else{
						c32_printf("	imul	ebx		;factor12\n");	// eax * ebx
					}
					used = 1;
				}
				else{		// % /
					c32_printf("	xor		edx, edx	;factor12\n");	// edx = 0;
					if((expr_p1->attr.attr & ATTR_UNSIGNED) || (expr_p2.attr.attr & ATTR_UNSIGNED)){
						c32_printf("	div		ebx		;factor12\n");	// edx-eax / ebx
					}
					else{
						c32_printf("	idiv	ebx		;factor12\n");	// edx-eax / ebx
					}
					if(type==TYPE_PERCENT){	// %
						c32_printf("	mov		eax, edx	;factor12\n");	// eax = edx
					}
					used = 1;
//					return 1;
				}
			}
			else{ // p1/p2 = EA
				if(expr_p1->comment==COM20 && expr_p1->numb_flag && expr_p2.comment==COM20 && expr_p2.numb_flag){	// p1 p2 ���l
					if(type==TYPE_ASTERISK){	// *
						expr_p1->value *= expr_p2.value;
					}
					else if(type==TYPE_SLUSH){	// /
						expr_p1->value /= expr_p2.value;
					}
					else{						// %
						expr_p1->value %= expr_p2.value;
					}
					sprintf(expr_p1->operand, "0x%x", expr_p1->value);
					used = 0;
//					return 0;
				}
				else{
					convert_com20(&expr_p2);
					c32_printf("	mov		ebx, eax	;factor12\n");	// ebx = p2
					convert_com20(expr_p1);								// eax = p1
					goto next1;
				}
			}
			c32_src_ptr_save(&src_ptr);
			c32_token_process(0, 0, &type);
			if(type==TYPE_ASTERISK || type==TYPE_SLUSH || type==TYPE_PERCENT){
				continue;
			}
			c32_src_ptr_restore(&src_ptr);
#ifdef DEBUG
printf("=== end factor12\n");
#endif
			return used;
		}
	}
	c32_src_ptr_restore(&src_ptr);
#ifdef DEBUG
printf("=== end factor12\n");
#endif
	return used;
}
/********************************************************************************/
/*		factor11																*/
/*		COM20: �萔	operand = "eax", "0x1234"  ���l�Ȃ�	value=0x1234, numb_flag=1	 */
/*		COM21: �z�� operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: �ϐ�   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		return: eax �Ɍ��ʂ������Ă���ƂP�ɂȂ邻���łȂ���΂O				*/
/*		value : �萔�̎��A�萔��Ԃ��G���A										*/
/*		func �� 0 �̏ꍇ�� global table �̂݌�������							*/
/*		factor12 + - factor12													*/
/********************************************************************************/
static int factor11(struct expr_param *expr_p1)
{
#ifdef DEBUG
printf("=== start factor11\n");
#endif
	int type, used;
	struct src_ptr src_ptr;
	struct expr_param expr_p2;
	
	used = factor12(expr_p1);
	c32_src_ptr_save(&src_ptr);
	
	c32_token_process(0, 0, &type);
	if(type==TYPE_PLUS || type==TYPE_MINUS){
		if(expr_p1->attr.attr & (ATTR_ARRAY | ATTR_STRUCT | ATTR_FUNC)){
			c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
		}
		if(expr_p1->attr.attr & ATTR_POINTER){
			c32_error_message(E_TO_BE_DEFINE, __LINE__, __FILE__);
		}
		for(;;){
			if(used){
				convert_com20(expr_p1);			// eax = p1
				c32_printf("	push	eax		;factor11\n");	// p1 push
			}
			memset(&expr_p2, 0, sizeof(expr_p2));
			expr_p2.func = expr_p1->func;
			expr_p2.tbl  = expr_p1->tbl;
			factor12(&expr_p2);
			
			if(expr_p2.attr.attr & (ATTR_ARRAY | ATTR_STRUCT | ATTR_FUNC | ATTR_POINTER)){
				c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
			}
			if(used){	// p1 �� push ����Ă���
				convert_com20(&expr_p2);		// eax = p2
				c32_printf("	pop		ebx		;factor11\n");	// ebx = p1
next1:;
				if(type==TYPE_PLUS){	// +
					c32_printf("	add		eax, ebx	;factor11\n");	// eax = p2+p1
				}
				else{					// -
					c32_printf("	sub		ebx, eax	;factor11\n");	// ebx = p1 - p2
					c32_printf("	mov		eax, ebx	;factor11\n");	// eax = p1 - p2
				}
				used = 1;
//				return 1;
			}
			else{ // p1/p2 = EA
				if(expr_p1->comment==COM20 && expr_p1->numb_flag && expr_p2.comment==COM20 && expr_p2.numb_flag){	// p1 p2 ���l
					if(type==TYPE_PLUS){	// +
						expr_p1->value += expr_p2.value;
					}
					else{					// -
						expr_p1->value -= expr_p2.value;
					}
					sprintf(expr_p1->operand, "0x%x", expr_p1->value);
					used = 0;
//					return 0;
				}
				else{
					convert_com20(&expr_p2);
					c32_printf("	mov		ebx, eax	;factor11\n");	// ebx = p2
					convert_com20(expr_p1);								// eax = p1

					if(type==TYPE_PLUS){	// +
						c32_printf("	add		eax, ebx	;factor11\n");	// eax = p2+p1
					}
					else{					// -
						c32_printf("	sub		eax, ebx	;factor11\n");	// eax = p1 - p2
					}
					used = 1;
				}
			}
			c32_src_ptr_save(&src_ptr);
			c32_token_process(0, 0, &type);
			if(type==TYPE_PLUS || type==TYPE_MINUS){
				continue;
			}
			c32_src_ptr_restore(&src_ptr);
#ifdef DEBUG
printf("=== end factor11\n");
#endif
			return used;
		}
	}
	c32_src_ptr_restore(&src_ptr);
#ifdef DEBUG
printf("=== end factor11\n");
#endif
	return used;
}
/********************************************************************************/
/*		factor10																*/
/*		COM20: �萔	operand = "eax", "0x1234"  ���l�Ȃ�	value=0x1234, numb_flag=1	 */
/*		COM21: �z�� operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: �ϐ�   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		return: eax �Ɍ��ʂ������Ă���ƂP�ɂȂ邻���łȂ���΂O				*/
/*		value : �萔�̎��A�萔��Ԃ��G���A										*/
/*		func �� 0 �̏ꍇ�� global table �̂݌�������							*/
/*		factor11 << >> factor11													*/
/********************************************************************************/
static int factor10(struct expr_param *expr_p1)
{
//	return factor11(func, mode, value);

#ifdef DEBUG
printf("=== start factor10\n");
#endif
	int type, used;
	struct src_ptr src_ptr;
	struct expr_param expr_p2;
	
	used = factor11(expr_p1);
	c32_src_ptr_save(&src_ptr);
	
	c32_token_process(0, 0, &type);				// << >> ���e�X�g����
	if(type==TYPE_L_SHIFT || type==TYPE_R_SHIFT){
		if(used){
			convert_com20(expr_p1);
			c32_printf("	push	eax		;factor10\n");	// p1 push
		}
		memset(&expr_p2, 0, sizeof(expr_p2));
		expr_p2.func = expr_p1->func;
		expr_p2.tbl  = expr_p1->tbl;
		factor11(&expr_p2);
		
		if(expr_p1->attr.attr & (ATTR_POINTER | ATTR_ARRAY | ATTR_STRUCT)){
			c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
		}
		if(expr_p2.attr.attr & (ATTR_POINTER | ATTR_ARRAY | ATTR_STRUCT)){
			c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
		}
		if(expr_p1->comment==COM20 && expr_p1->numb_flag && expr_p2.comment==COM20 && expr_p2.numb_flag){	// p1 p2 ���l
			if(type==TYPE_L_SHIFT){
				expr_p1->value <<= expr_p2.value;
			}
			else{
				expr_p1->value >>= expr_p2.value;
			}
			sprintf(expr_p1->operand, "0x%x", expr_p1->value);
#ifdef DEBUG
printf("=== end factor10\n");
#endif
			return 0;
		}
		// shift �ʂ� cl �ɍ��
		c32_printf("	mov		%s, %s	;factor10\n", c32_attr_to_cx(&expr_p2.attr), expr_p2.operand);
		if(used==0){
			convert_com20(expr_p1);		// eax = p1
		}
		else{		// p1 �� push ����Ă���
			c32_printf("	pop		eax		;factor10\n");	// eax = p1
		}
		if(type==TYPE_L_SHIFT){
			c32_printf("	shl		eax, cl	;factor10\n");
		}
		else{
			c32_printf("	shr		eax, cl	;factor10\n");
		}
#ifdef DEBUG
printf("=== end factor10\n");
#endif
		return 1;
	}
	c32_src_ptr_restore(&src_ptr);
#ifdef DEBUG
printf("=== end factor10\n");
#endif
	return used;
}
/********************************************************************************/
/*		factor9																	*/
/*		COM20: �萔	operand = "eax", "0x1234"  ���l�Ȃ�	value=0x1234, numb_flag=1	 */
/*		COM21: �z�� operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: �ϐ�   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		return: eax �Ɍ��ʂ������Ă���ƂP�ɂȂ邻���łȂ���΂O				*/
/*		value : �萔�̎��A�萔��Ԃ��G���A										*/
/*		func �� 0 �̏ꍇ�� global table �̂݌�������							*/
/*		factor10 < <= > >= factor10												*/
/********************************************************************************/
static int factor9(struct expr_param *expr_p1)
{
//	return factor10(func, mode, value);

#ifdef DEBUG
printf("=== start factor9\n");
#endif
	int type, used_1;
	struct src_ptr src_ptr;
	struct expr_param expr_p2;
	int end_numb = c32_label_counter++;
//	struct symtbl *func = expr_p1->func;
	
	expr_p2 = *expr_p1;
	used_1 = factor10(expr_p1);
	c32_src_ptr_save(&src_ptr);
	
	c32_token_process(0, 0, &type);				// == != �𒲂ׂ�
	if(type==TYPE_LT || type==TYPE_LE || type==TYPE_GT || type==TYPE_GE){
		convert_com20(expr_p1);		// eax = p1
		c32_printf("	push	eax		;factor9\n");	// push p1
		memset(&expr_p2, 0, sizeof(expr_p2));
		expr_p2.func = expr_p1->func;
		expr_p2.tbl  = expr_p1->tbl;
		factor10(&expr_p2);
		
		convert_com20(&expr_p2);					// eax = p2
		c32_printf("	pop		ebx		;factor9\n");	// ebx = p1
		if((expr_p1->attr.attr != expr_p2.attr.attr) || (expr_p1->attr.attr & ATTR_STRUCT)){
			if(expr_p1->attr.attr & (ATTR_ARRAY | ATTR_POINTER)){
				c32_error_message(E_VARIABLE_TYPE_ERROR, __LINE__, __FILE__);
			}
		}
		c32_printf("	cmp		ebx, eax	;factor9\n");	// p1 - p2
		if(expr_p1->attr.attr & (ATTR_UNSIGNED|ATTR_POINTER|ATTR_ARRAY)){	// �����Ȃ�
			if(type==TYPE_LT){
				c32_printf("	mov		eax, 1	;factor9\n");	// eax = 0;
				c32_printf("	jb		L%d		;factor9\n", end_numb);
				c32_printf("	dec		eax		;factor9\n");	// eax = 1;
				c32_printf("L%d:		;factor8\n", end_numb);
			}
			else if(type==TYPE_LE){
				c32_printf("	mov		eax, 1	;factor9\n");	// eax = 0;
				c32_printf("	jbe		L%d		;factor9\n", end_numb);
				c32_printf("	dec		eax		;factor9\n");	// eax = 1;
				c32_printf("L%d:		;factor8\n", end_numb);
			}
			else if(type==TYPE_GT){
				c32_printf("	mov		eax, 1	;factor9\n");	// eax = 0;
				c32_printf("	ja		L%d		;factor9\n", end_numb);
				c32_printf("	dec		eax		;factor9\n");	// eax = 1;
				c32_printf("L%d:		;factor8\n", end_numb);
			}
			else if(type==TYPE_GE){
				c32_printf("	mov		eax, 1	;factor9\n");	// eax = 0;
				c32_printf("	jae		L%d		;factor9\n", end_numb);
				c32_printf("	dec		eax		;factor9\n");	// eax = 1;
				c32_printf("L%d:		;factor8\n", end_numb);
			}
			else{
				c32_error_message(E_INTERNAL_ERROR, __LINE__, __FILE__);
			}
		}
		else{			// �����t��
			if(type==TYPE_LT){
				c32_printf("	mov		eax, 1	;factor9\n");	// eax = 0;
				c32_printf("	jl		L%d		;factor9\n", end_numb);
				c32_printf("	dec		eax		;factor9\n");	// eax = 1;
				c32_printf("L%d:		;factor8\n", end_numb);
			}
			else if(type==TYPE_LE){
				c32_printf("	mov		eax, 1	;factor9\n");	// eax = 0;
				c32_printf("	jle		L%d		;factor9\n", end_numb);
				c32_printf("	dec		eax		;factor9\n");	// eax = 1;
				c32_printf("L%d:		;factor8\n", end_numb);
			}
			else if(type==TYPE_GT){
				c32_printf("	mov		eax, 1	;factor9\n");	// eax = 0;
				c32_printf("	jg		L%d		;factor9\n", end_numb);
				c32_printf("	dec		eax		;factor9\n");	// eax = 1;
				c32_printf("L%d:		;factor8\n", end_numb);
			}
			else if(type==TYPE_GE){
				c32_printf("	mov		eax, 1	;factor9\n");	// eax = 0;
				c32_printf("	jge		L%d		;factor9\n", end_numb);
				c32_printf("	dec		eax		;factor9\n");	// eax = 1;
				c32_printf("L%d:		;factor8\n", end_numb);
			}
			else{
				c32_error_message(E_INTERNAL_ERROR, __LINE__, __FILE__);
			}
		}
#ifdef DEBUG
printf("=== end factor9\n");
#endif
		return 1;
	}
	c32_src_ptr_restore(&src_ptr);
#ifdef DEBUG
printf("=== end factor9\n");
#endif
	return used_1;
}
/********************************************************************************/
/*		factor8																	*/
/*		COM20: �萔	operand = "eax", "0x1234"  ���l�Ȃ�	value=0x1234, numb_flag=1	 */
/*		COM21: �z�� operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: �ϐ�   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		return: eax �Ɍ��ʂ������Ă���ƂP�ɂȂ邻���łȂ���΂O				*/
/*		value : �萔�̎��A�萔��Ԃ��G���A										*/
/*		func �� 0 �̏ꍇ�� global table �̂݌�������							*/
/*		factor9 ==  != factor9													*/
/********************************************************************************/
static int factor8(struct expr_param *expr_p1)
{
//	return factor9(func, mode, value);

#ifdef DEBUG
printf("=== start factor8\n");
#endif
	int type, used_1;
	struct src_ptr src_ptr;
	struct expr_param expr_p2;
	int end_numb = c32_label_counter++;
//	struct symtbl *func = expr_p1->func;
	
	expr_p2 = *expr_p1;
	used_1 = factor9(expr_p1);
	c32_src_ptr_save(&src_ptr);
	
	c32_token_process(0, 0, &type);				// == != �𒲂ׂ�
	if(type==TYPE_EQEQ || type==TYPE_NOTEQ){
		convert_com20(expr_p1);		// eax = p1
		c32_printf("	push	eax		;factor8\n");	// push p1
		memset(&expr_p2, 0, sizeof(expr_p2));
		expr_p2.func = expr_p1->func;
		expr_p2.tbl  = expr_p1->tbl;
		factor9(&expr_p2);
		
		convert_com20(&expr_p2);					// eax = p2
		c32_printf("	pop		ebx		;factor8\n");	// ebx = p1
		if((expr_p1->attr.attr != expr_p2.attr.attr) || (expr_p1->attr.attr & ATTR_STRUCT)){
			if(expr_p1->attr.attr & (ATTR_ARRAY | ATTR_POINTER)){
				c32_error_message(E_VARIABLE_TYPE_ERROR, __LINE__, __FILE__);
			}
		}
		c32_printf("	cmp		eax, ebx	;factor8\n");	// eax-ebx
		if(type==TYPE_NOTEQ){		// !=
			c32_printf("	mov		eax, 1	;factor8-6\n");	// eax = 0;
			c32_printf("	jnz		L%d		;factor8-7\n", end_numb);
			c32_printf("	dec		eax		;factor8-8\n");	// eax = 1;
			c32_printf("L%d:		;factor8\n", end_numb);
#ifdef DEBUG
printf("=== end factor8\n");
#endif
			return 1;
		}
		else{		// ==
			c32_printf("	mov		eax, 1	;factor8-9\n");	// eax = 0;
			c32_printf("	jz		L%d		;factor8-10\n", end_numb);
			c32_printf("	dec		eax		;factor8-11\n");	// eax = 1;
			c32_printf("L%d:		;factor8\n", end_numb);
#ifdef DEBUG
printf("=== end factor8\n");
#endif
			return 1;
		}
	}
	c32_src_ptr_restore(&src_ptr);
#ifdef DEBUG
printf("=== end factor8\n");
#endif
	return used_1;
}
/********************************************************************************/
/*		factor7																	*/
/*		COM20: �萔	operand = "eax", "0x1234"  ���l�Ȃ�	value=0x1234, numb_flag=1	 */
/*		COM21: �z�� operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: �ϐ�   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		return: eax �Ɍ��ʂ������Ă���ƂP�ɂȂ邻���łȂ���΂O				*/
/*		value : �萔�̎��A�萔��Ԃ��G���A										*/
/*		func �� 0 �̏ꍇ�� global table �̂݌�������							*/
/*		factor8 & factor8														*/
/********************************************************************************/
static int factor7(struct expr_param *expr_p1)
{
//	return factor8(func, mode, value);

#ifdef DEBUG
printf("=== start factor7\n");
#endif
	int type, used_1, used_2;
	struct src_ptr src_ptr;
	struct expr_param expr_p2;
	
	used_1 = factor8(expr_p1);
	c32_src_ptr_save(&src_ptr);
	
	c32_token_process(0, 0, &type);				// & �𒲂ׂ�
	if(type != TYPE_AND){
		c32_src_ptr_restore(&src_ptr);
#ifdef DEBUG
printf("=== end factor7\n");
#endif
		return used_1;
	}
	/***	& ������	***/
	if(used_1){			// p1 = eax
		c32_src_ptr_save(&src_ptr);
		convert_com20(expr_p1);
		c32_printf("	push	eax		;factor7\n");	// p1 push
		memset(&expr_p2, 0, sizeof(expr_p2));
		expr_p2.func = expr_p1->func;
		expr_p2.tbl  = expr_p1->tbl;
		used_2 = factor7(&expr_p2);
		
		if(expr_p2.comment==COM20 && expr_p2.numb_flag){	// p2 ���l
			c32_src_ptr_restore(&src_ptr);
			memset(&expr_p2, 0, sizeof(expr_p2));
			expr_p2.func = expr_p1->func;
			expr_p2.tbl  = expr_p1->tbl;
			used_2 = factor7(&expr_p2);
			//	eax=p1, p2=���l
			c32_printf("	and		eax, 0x%x	;factor7\n", expr_p2.value);	// eax = p1 & p3
			expr_p1->attr.attr = factor5_sub(expr_p1->attr.attr, expr_p2.attr.attr);	//�����^�̍ő� attr
#ifdef DEBUG
printf("=== end factor7\n");
#endif
			return 1;
		}
		convert_com20(&expr_p2);	// p2 = eax
		c32_printf("	pop		ebx		;factor7\n");	// p1 = ebx
		c32_printf("	and		eax, ebx	;factor7\n");	// eax = p2 & p1
		expr_p1->attr.attr = factor5_sub(expr_p1->attr.attr, expr_p2.attr.attr);	//�����^�̍ő� attr
#ifdef DEBUG
printf("=== end factor7\n");
#endif
		return 1;
	}
	// p1 �� eax ���g�p���Ă��Ȃ�
	memset(&expr_p2, 0, sizeof(expr_p2));
	expr_p2.func = expr_p1->func;
	expr_p2.tbl  = expr_p1->tbl;
	factor7(&expr_p2);
	
	if(expr_p1->comment==COM20 && expr_p1->numb_flag && expr_p2.comment==COM20 && expr_p2.numb_flag){	// p1 p2 ���l
		expr_p1->value &= expr_p2.value;
		sprintf(expr_p1->operand, "0x%x", expr_p1->value);
#ifdef DEBUG
printf("=== end factor7\n");
#endif
		return 0;
	}
	
	convert_com20(&expr_p2);
	c32_printf("	mov		ebx, eax	;factor7\n");	// ebx = p2
	convert_com20(expr_p1);								// eax = p1
	c32_printf("	and		eax, ebx	;factor7\n");	// eax = p2 & p1
	expr_p1->attr.attr = factor5_sub(expr_p1->attr.attr, expr_p2.attr.attr);	//�����^�̍ő� attr
#ifdef DEBUG
printf("=== end factor7\n");
#endif
	return 1;
}
/********************************************************************************/
/*		factor6																	*/
/*		COM20: �萔	operand = "eax", "0x1234"  ���l�Ȃ�	value=0x1234, numb_flag=1	 */
/*		COM21: �z�� operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: �ϐ�   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		return: eax �Ɍ��ʂ������Ă���ƂP�ɂȂ邻���łȂ���΂O				*/
/*		value : �萔�̎��A�萔��Ԃ��G���A										*/
/*		func �� 0 �̏ꍇ�� global table �̂݌�������							*/
/*		factor7 ^ factor7														*/
/********************************************************************************/
static int factor6(struct expr_param *expr_p1)
{
//	return factor7(func, mode, value);

#ifdef DEBUG
printf("=== start factor6\n");
#endif
	int type, used_1, used_2;
	struct src_ptr src_ptr;
	struct expr_param expr_p2;
	
	used_1 = factor7(expr_p1);
	c32_src_ptr_save(&src_ptr);
	
	c32_token_process(0, 0, &type);				// ^ �𒲂ׂ�
	if(type != TYPE_XOR){
		c32_src_ptr_restore(&src_ptr);
#ifdef DEBUG
printf("=== end factor6\n");
#endif
		return used_1;
	}
	/***	^ ������	***/
	if(used_1){			// p1 = eax
		c32_src_ptr_save(&src_ptr);
		convert_com20(expr_p1);
		c32_printf("	push	eax		;factor6\n");	// p1 push
		memset(&expr_p2, 0, sizeof(expr_p2));
		expr_p2.func = expr_p1->func;
		expr_p2.tbl  = expr_p1->tbl;
		used_2 = factor6(&expr_p2);
		
		if(expr_p2.comment==COM20 && expr_p2.numb_flag){	// p2 ���l
			c32_src_ptr_restore(&src_ptr);
			memset(&expr_p2, 0, sizeof(expr_p2));
			expr_p2.func = expr_p1->func;
			expr_p2.tbl  = expr_p1->tbl;
			used_2 = factor6(&expr_p2);
			//	eax=p1, p2=���l
			c32_printf("	xor		eax, 0x%x	;factor6\n", expr_p2.value);	// eax = p1 ^ p3
			expr_p1->attr.attr = factor5_sub(expr_p1->attr.attr, expr_p2.attr.attr);	//�����^�̍ő� attr
#ifdef DEBUG
printf("=== end factor6\n");
#endif
			return 1;
		}
		convert_com20(&expr_p2);	// p2 = eax
		c32_printf("	pop		ebx		;factor6\n");	// p1 = ebx
		c32_printf("	xor		eax, ebx	;factor6\n");	// eax = p2 ^ p1
		expr_p1->attr.attr = factor5_sub(expr_p1->attr.attr, expr_p2.attr.attr);	//�����^�̍ő� attr
#ifdef DEBUG
printf("=== end factor6\n");
#endif
		return 1;
	}
	// p1 �� eax ���g�p���Ă��Ȃ�
	memset(&expr_p2, 0, sizeof(expr_p2));
	expr_p2.func = expr_p1->func;
	expr_p2.tbl  = expr_p1->tbl;
	factor6(&expr_p2);
	
	if(expr_p1->comment==COM20 && expr_p1->numb_flag && expr_p2.comment==COM20 && expr_p2.numb_flag){	// p1 p2 ���l
		expr_p1->value ^= expr_p2.value;
		sprintf(expr_p1->operand, "0x%x", expr_p1->value);
#ifdef DEBUG
printf("=== end factor6\n");
#endif
		return 0;
	}
	
	convert_com20(&expr_p2);
	c32_printf("	mov		ebx, eax	;factor6\n");	// ebx = p2
	convert_com20(expr_p1);								// eax = p1
	c32_printf("	xor		eax, ebx	;factor6\n");	// eax = p2 ^ p1
	expr_p1->attr.attr = factor5_sub(expr_p1->attr.attr, expr_p2.attr.attr);	//�����^�̍ő� attr
#ifdef DEBUG
printf("=== end factor6\n");
#endif
	return 1;
}
/********************************************************************************/
/*		factor5																	*/
/*		COM20: �萔	operand = "eax", "0x1234"  ���l�Ȃ�	value=0x1234, numb_flag=1	 */
/*		COM21: �z�� operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: �ϐ�   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		return: eax �Ɍ��ʂ������Ă���ƂP�ɂȂ邻���łȂ���΂O				*/
/*		value : �萔�̎��A�萔��Ԃ��G���A										*/
/*		func �� 0 �̏ꍇ�� global table �̂݌�������							*/
/*		factor6 | factor6														*/
/********************************************************************************/
//  �����^�̍ő�� attr �ɂ���
static int factor5_sub(int attr1, int attr2)
{
#ifdef DEBUG
printf("=== start factor5_sub\n");
#endif
	if (attr1 & (ATTR_POINTER | ATTR_ARRAY | ATTR_FUNC | ATTR_STRUCT)) {
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
	}
	if (attr2 & (ATTR_POINTER | ATTR_ARRAY | ATTR_FUNC | ATTR_STRUCT)) {
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
	}
	attr1 |= attr2;
	if(attr1 & ATTR_INT){
		attr1 &= ~(ATTR_SHORT | ATTR_CHAR);
	}
	else if(attr1 & ATTR_SHORT){
		attr1 &= ~ATTR_CHAR;
	}
#ifdef DEBUG
printf("=== end factor5_sub\n");
#endif
	return attr1;
}

static int factor5(struct expr_param *expr_p1)
{
//	return factor6(func, mode, value);

#ifdef DEBUG
printf("=== start factor5\n");
#endif
	int type, used_1, used_2;
	struct src_ptr src_ptr;
	struct expr_param expr_p2;
	
	used_1 = factor6(expr_p1);
	c32_src_ptr_save(&src_ptr);
	
	c32_token_process(0, 0, &type);				// | �𒲂ׂ�
	if(type != TYPE_OR){
		c32_src_ptr_restore(&src_ptr);
#ifdef DEBUG
printf("=== end factor5\n");
#endif
		return used_1;
	}
	/***	| ������	***/
	if(used_1){			// p1 = eax
		c32_src_ptr_save(&src_ptr);
		convert_com20(expr_p1);
		c32_printf("	push	eax		;factor5\n");	// p1 push
		memset(&expr_p2, 0, sizeof(expr_p2));
		expr_p2.func = expr_p1->func;
		expr_p2.tbl  = expr_p1->tbl;
		used_2 = factor5(&expr_p2);
		
		if(expr_p2.comment==COM20 && expr_p2.numb_flag){	// p2 ���l
			c32_src_ptr_restore(&src_ptr);
			memset(&expr_p2, 0, sizeof(expr_p2));
			expr_p2.func = expr_p1->func;
			expr_p2.tbl  = expr_p1->tbl;
			used_2 = factor5(&expr_p2);
			//	eax=p1, p2=���l
			c32_printf("	or		eax, 0x%x	;factor5\n", expr_p2.value);	// eax = p1 | p3
			expr_p1->attr.attr = factor5_sub(expr_p1->attr.attr, expr_p2.attr.attr);	//�����^�̍ő� attr
#ifdef DEBUG
printf("=== end factor5\n");
#endif
			return 1;
		}
		convert_com20(&expr_p2);	// p2 = eax
		c32_printf("	pop		ebx		;factor5\n");	// p1 = ebx
		c32_printf("	or		eax, ebx	;factor5\n");	// eax = p2 | p1
		expr_p1->attr.attr = factor5_sub(expr_p1->attr.attr, expr_p2.attr.attr);	//�����^�̍ő� attr
#ifdef DEBUG
printf("=== end factor5\n");
#endif
		return 1;
	}
	// p1 �� eax ���g�p���Ă��Ȃ�
	memset(&expr_p2, 0, sizeof(expr_p2));
	expr_p2.func = expr_p1->func;
	expr_p2.tbl  = expr_p1->tbl;
	factor5(&expr_p2);
	
	if(expr_p1->comment==COM20 && expr_p1->numb_flag && expr_p2.comment==COM20 && expr_p2.numb_flag){	// p1 p2 ���l
		expr_p1->value |= expr_p2.value;
		sprintf(expr_p1->operand, "0x%x", expr_p1->value);
#ifdef DEBUG
printf("=== end factor5\n");
#endif
		return 0;
	}
	
	convert_com20(&expr_p2);
	c32_printf("	mov		ebx, eax	;factor5\n");	// ebx = p2
	convert_com20(expr_p1);								// eax = p1
	c32_printf("	or		eax, ebx	;factor5\n");	// eax = p2 | p1
	expr_p1->attr.attr = factor5_sub(expr_p1->attr.attr, expr_p2.attr.attr);	//�����^�̍ő� attr
#ifdef DEBUG
printf("=== end factor5\n");
#endif
	return 1;
}
/********************************************************************************/
/*		factor4																	*/
/*		COM20: �萔	operand = "eax", "0x1234"  ���l�Ȃ�	value=0x1234, numb_flag=1	 */
/*		COM21: �z�� operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: �ϐ�   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		return: eax �Ɍ��ʂ������Ă���ƂP�ɂȂ邻���łȂ���΂O				*/
/*		value : �萔�̎��A�萔��Ԃ��G���A										*/
/*		func �� 0 �̏ꍇ�� global table �̂݌�������							*/
/*		factor5 && factor4														*/
/********************************************************************************/
static int factor4(struct expr_param *expr_p1)
{
//	return factor5(func, mode, value);

#ifdef DEBUG
printf("=== start factor4\n");
#endif
	int type, used;
	struct src_ptr src_ptr;
	int end_numb = c32_label_counter++;
	struct symtbl *func = expr_p1->func;
	struct struct_tbl *tbl = expr_p1->tbl;
	
	used = factor5(expr_p1);
	c32_src_ptr_save(&src_ptr);
	
	c32_token_process(0, 0, &type);				// && �𒲂ׂ�
	if(type != TYPE_AND_AND){
		c32_src_ptr_restore(&src_ptr);
#ifdef DEBUG
printf("=== end factor4\n");
#endif
		return used;
	}
	/***	&& ������	***/
	convert_com20(expr_p1);		// eax = p1
	c32_printf("	and		eax, eax	;factor4\n");
	c32_printf("	jz		L%d\n", end_numb);
	memset(expr_p1, 0, sizeof(struct expr_param));
	expr_p1->func = func;
	expr_p1->tbl  = tbl;
	factor4(expr_p1);
	
	convert_com20(expr_p1);
	c32_printf("L%d:		;factor4\n", end_numb);
#ifdef DEBUG
printf("=== end factor4\n");
#endif
	return 1;
}
/********************************************************************************/
/*		factor3																	*/
/*		COM20: �萔	operand = "eax", "0x1234"  ���l�Ȃ�	value=0x1234, numb_flag=1	 */
/*		COM21: �z�� operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: �ϐ�   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		return: eax �Ɍ��ʂ������Ă���ƂP�ɂȂ邻���łȂ���΂O				*/
/*		value : �萔�̎��A�萔��Ԃ��G���A										*/
/*		func �� 0 �̏ꍇ�� global table �̂݌�������							*/
/*		factor4 || factor3														*/
/********************************************************************************/
static int factor3(struct expr_param *expr_p1)
{
//	return factor4(func, mode, value);

#ifdef DEBUG
printf("=== start factor3\n");
#endif
	int type, used;
	struct src_ptr src_ptr;
	int end_numb = c32_label_counter++;
	struct symtbl *func = expr_p1->func;
	struct struct_tbl *tbl = expr_p1->tbl;
	
	used = factor4(expr_p1);
	c32_src_ptr_save(&src_ptr);
	
	c32_token_process(0, 0, &type);				// || �𒲂ׂ�
	if(type != TYPE_OR_OR){
		c32_src_ptr_restore(&src_ptr);
#ifdef DEBUG
printf("=== end factor3\n");
#endif
		return used;
	}
	/***	|| ������	***/
	convert_com20(expr_p1);		// eax = p1
	c32_printf("	and		eax, eax	;factor3\n");
	c32_printf("	jnz		L%d\n", end_numb);
	
	memset(expr_p1, 0, sizeof(struct expr_param));
	expr_p1->func = func;
	expr_p1->tbl  = tbl;
	factor3(expr_p1);
	
	convert_com20(expr_p1);		// eax = p2
	c32_printf("L%d:		;factor3\n", end_numb);
#ifdef DEBUG
printf("=== end factor3\n");
#endif
	return 1;
}
/********************************************************************************/
/*		convert_com20															*/
/*		COM20: �萔	operand = "eax", "0x1234"  ���l�Ȃ�	value=0x1234, numb_flag=1	 */
/*		COM21: �z�� operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: �ϐ�   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*		eax �ɒl������														*/
/********************************************************************************/
void convert_com20(struct expr_param *expr_p1)
{
#ifdef DEBUG
printf("=== start convert_com20\n");
#endif
	switch(expr_p1->comment){
		case COM20:		// �萔			operand = "eax", "0x1234"
						// ���l�Ȃ�		value=0x1234, numb_flag=1
			if(strcmp(expr_p1->operand, "eax")==0){
				break;
			}
			c32_printf("	mov		eax, %s		;convert_com20\n", expr_p1->operand);
			break;
		case COM21:		// �z��		operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"
			if(strcmp(expr_p1->operand, "eax")==0){
				break;
			}
			c32_printf("	mov		eax, %s		;convert_com20\n", expr_p1->operand);
			expr_p1->attr.attr |= ATTR_ARRAY_P;
			break;
		case COM22:		// �ϐ��A�h���X	operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]"
			if(expr_p1->attr.attr & ATTR_UNSIGNED){
				if(c32_attr_to_byte(&expr_p1->attr) != 4){
					c32_printf("	sub		ebx, ebx	;convert_com20\n");
					c32_printf("	mov		%s, %s		;convert_com20\n", c32_attr_to_bx(&expr_p1->attr), expr_p1->operand);
					c32_printf("	mov		eax, ebx	;convert_com20\n");
				}
				else{
					c32_printf("	mov		%s, %s		;convert_com20\n", c32_attr_to_ax(&expr_p1->attr), expr_p1->operand);
				}
			}
			else{
				c32_printf("	mov		%s, %s		;convert_com20\n", c32_attr_to_ax(&expr_p1->attr), expr_p1->operand);

				if(c32_attr_to_byte(&expr_p1->attr)==1){
					c32_printf("	cbw		;convert_com10\n");
#ifdef BIT16
					c32_printf("	db 0x66\n");
#endif
					c32_printf("	db 0x98		;convert_com10\n");
				}
				else if(c32_attr_to_byte(&expr_p1->attr)==2){
#ifdef BIT16
					c32_printf("	db 0x66\n");
#endif
					c32_printf("	db 0x98		;convert_com10\n");
				}
			}
			break;
		default:
			c32_error_message(E_INTERNAL_ERROR, __LINE__, __FILE__);
	}
	expr_p1->comment = COM20;
	strcpy(expr_p1->operand, "eax");
	expr_p1->numb_flag = 0;
#ifdef DEBUG
printf("=== end convert_com20\n");
#endif
}
/********************************************************************************/
/*		convert_com22															*/
/*		COM20: �萔	operand = "eax", "0x1234"  ���l�Ȃ�	value=0x1234, numb_flag=1	 */
/*		COM21: �z�� operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: �ϐ�   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*		�ϐ��A�h���X�����ɂ���													*/
/********************************************************************************/
void convert_com22(struct expr_param *expr_p1)
{
#ifdef DEBUG
printf("=== start convert_com22\n");
#endif
	char buf[MAX_SYMBUF_LEN];
	
	switch(expr_p1->comment){
		case COM20:		// �萔			operand = "eax", "0x1234"
						// ���l�Ȃ�		value=0x1234, numb_flag=1
			if(expr_p1->attr.attr & ATTR_POINTER){
				expr_p1->attr.attr -= ATTR_POINTER1;
			}
			else{
				c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
			}
			expr_p1->comment = COM22;
			strcpy(buf, expr_p1->operand);
			sprintf(expr_p1->operand, "[%s]", buf);
			expr_p1->numb_flag = 0;
			break;
		case COM21:		// �z��		operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"
			if((expr_p1->attr.attr & ATTR_ARRAY_P)==0){
				c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
			}
			expr_p1->attr.attr &= ~ATTR_ARRAY_P;
			expr_p1->comment = COM22;
			strcpy(buf, expr_p1->operand);
			sprintf(expr_p1->operand, "[%s]", buf);
			expr_p1->numb_flag = 0;
			break;
		case COM22:		// �ϐ��A�h���X	operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]"
			break;
		default:
			c32_error_message(E_INTERNAL_ERROR, __LINE__, __FILE__);
	}
#ifdef DEBUG
printf("=== end convert_com22\n");
#endif
}
/********************************************************************************/
/*		factor2																	*/
/*		COM20: �萔	operand = "eax", "0x1234"  ���l�Ȃ�	value=0x1234, numb_flag=1	 */
/*		COM21: �z�� operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: �ϐ�   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		return: eax �Ɍ��ʂ������Ă���ƂP�ɂȂ邻���łȂ���΂O				*/
/*		value : �萔�̎��A�萔��Ԃ��G���A										*/
/*		func �� 0 �̏ꍇ�� global table �̂݌�������							*/
/*		factor3 ? factor3 : factor3												*/
/********************************************************************************/
static int factor2(struct expr_param *expr_p1)
{
//	return factor3(func, mode, value);
	
#ifdef DEBUG
printf("=== start factor2\n");
#endif
	int type, used;
	int false_numb = c32_label_counter++;
	int end_numb = c32_label_counter++;
	struct src_ptr src_ptr;
	
	used = factor3(expr_p1);
	c32_src_ptr_save(&src_ptr);
	
	c32_token_process(0, 0, &type);				// '?' �e�X�g
	if(type != TYPE_QUESTION){				// '?' �ł͂Ȃ�
		c32_src_ptr_restore(&src_ptr);
#ifdef DEBUG
printf("=== end factor2\n");
#endif
		return used;
	}

	/***	? :  ����	***/
	convert_com20(expr_p1);			// �v�Z�������ɂ��� �� eax
	c32_printf("	and		eax, eax	;factor2\n");
	c32_printf("	jz		L%d		;factor2\n", false_numb);
	
	factor3(expr_p1);						// true �v�Z��
	convert_com20(expr_p1);
	c32_printf("	jmp		L%d		;factor2\n", end_numb);
	
	c32_token_process(0, 0, &type);				// ':' �e�X�g
	if(type != TYPE_KORON)
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
	
	c32_printf("L%d:		;factor2\n", false_numb);
	factor3(expr_p1);						// false �v�Z��
	convert_com20(expr_p1);
	c32_printf("L%d:		;factor2\n", end_numb);
#ifdef DEBUG
printf("=== end factor2\n");
#endif
	return 1;
}
/********************************************************************************/
/*		c32_factor1																*/
/*		COM20: �萔	operand = "eax", "0x1234"  ���l�Ȃ�	value=0x1234, numb_flag=1	 */
/*		COM21: �z�� operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: �ϐ�   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		return: eax �Ɍ��ʂ������Ă���ƂP�ɂȂ邻���łȂ���΂O				*/
/*		value : �萔�̎��A�萔��Ԃ��G���A										*/
/*		func �� 0 �̏ꍇ�� global table �̂݌�������							*/
/*		=  +=  -=  *=  /=  %=  &=  ^=  |=  <<=  >>=								*/
/********************************************************************************/
int c32_factor1(struct expr_param *expr_p1)
{
//	return factor2(func, mode, value);

#ifdef DEBUG
printf("=== start factor1\n");
#endif
	int type, used_1, used_2;
	struct src_ptr src_ptr;
	struct expr_param expr_p2;
	struct struct_tbl *tbl;

	expr_p2 = *expr_p1;
	/***	��ꍀ��荞��	***/
	used_1 = factor2(expr_p1);
	c32_src_ptr_save(&src_ptr);
	
	/***	���Z�q�`�F�b�N		***/
	c32_token_process(0, 0, &type);
	switch(type){
	case TYPE_EQ:
	case TYPE_PLUS_EQ:
	case TYPE_MINUS_EQ:
	case TYPE_ASTERISK_EQ:
	case TYPE_SLUSH_EQ:
	case TYPE_PERSENT_EQ:
	case TYPE_OR_EQ:
	case TYPE_AND_EQ:
	case TYPE_L_SHIFT_EQ:
	case TYPE_R_SHIFT_EQ:
	case TYPE_XOR_EQ:
		break;
	default:
		c32_src_ptr_restore(&src_ptr);
#ifdef DEBUG
printf("=== end factor1\n");
#endif
		return used_1;
	}
	
	if((expr_p1->attr.attr & ATTR_STRUCT)==0){
		convert_com22(expr_p1);		// �ϐ��A�h���X�����ɂ���
	}
	/***	��񍀎�荞��		***/
	memset(&expr_p2, 0, sizeof(expr_p2));
	expr_p2.func = expr_p1->func;
	expr_p2.tbl  = expr_p1->tbl;
	if(used_1){
		c32_printf("	push	eax		;factor1\n");
	}
	used_2 = c32_factor1(&expr_p2);
//	if(used_1){
//		c32_printf("	pop		eax		;factor1\n");
//	}
	
	/***	struct ����		***/
	if(expr_p1->attr.attr & ATTR_STRUCT){
		if(expr_p1->attr.attr & ATTR_ARRAY){	// sturct �z��̓G���[�ɂ���
			c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
		}
		if((expr_p1->attr.attr & ATTR_POINTER)==0){	// �|�C���^�������炱�̉��ŏ�������
			if((expr_p2.attr.attr & ATTR_STRUCT)==0 || (expr_p2.attr.attr & (ATTR_ARRAY | ATTR_POINTER))){	// struct �łȂ����A�|�C���^��z��̓G���[
				c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
			}
			if(strcmp(expr_p1->attr.symbuf, expr_p2.attr.symbuf)){	// struct �����قȂ�
				c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
			}
			for(tbl=c32_st_root; tbl; tbl=tbl->next){		// struct ���T�[�`����
				if(strcmp(expr_p1->attr.symbuf, tbl->symbuf)==0)
					break;
			}
			if(tbl==0){		// ���o�^
				c32_error_message(E_UNDEFINED_SYMBOL, __LINE__, __FILE__);
			}
			c32_error_message(E_TO_BE_DEFINE, __LINE__, __FILE__);
			
//			c32_printf("	mov		esi, %s		;factor1\n", expr_p2.operand);	// esi �Z�b�g�A��Q���� eax �ɂȂ��Ă���
//			convert_com20(&expr_p2);	// �v�Z�������ɂ���
//			if(used_1){		// ��P���� eax �� push ���Ă���
//				c32_printf("	pop		eax		;factor1\n");	// ��P���� eax ����
//			}
//			if(expr_p1->comment==COM0 || expr_p1->comment==COM1 || expr_p1->comment==COM11){
//				c32_printf("	mov		edi, %s		;factor1\n", expr_p1->operand);	// edi �Z�b�g
//			}
//			else{
//				c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
//			}
//			c32_printf("	mov		ecx, %d		;factor1\n", tbl->size);		// ecx �Z�b�g
//			c32_printf("	call	startup_memcpy\n");
//			return 0;		// eax �͎g��Ȃ�
		}
	}
	/***	���Z���s	***/
	// used ���Z�b�g����Ă���Ƒ�P���� eax �� push ����Ă���
	switch(type){
	case TYPE_EQ:
		c32_expr_eq(expr_p1, &expr_p2, used_1);
		break;
	case TYPE_PLUS_EQ:
		c32_expr_plus_eq(expr_p1, &expr_p2, used_1);
		break;
	case TYPE_MINUS_EQ:
		c32_expr_minus_eq(expr_p1, &expr_p2, used_1);
		break;
	case TYPE_ASTERISK_EQ:
		c32_expr_asterisk_eq(expr_p1, &expr_p2, used_1);
		break;
	case TYPE_SLUSH_EQ:
		c32_expr_slush_eq(expr_p1, &expr_p2, used_1);
		break;
	case TYPE_PERSENT_EQ:
		c32_expr_percent_eq(expr_p1, &expr_p2, used_1);
		break;
	case TYPE_OR_EQ:
		c32_expr_or_eq(expr_p1, &expr_p2, used_1);
		break;
	case TYPE_AND_EQ:
		c32_expr_and_eq(expr_p1, &expr_p2, used_1);
		break;
	case TYPE_XOR_EQ:
		c32_expr_xor_eq(expr_p1, &expr_p2, used_1);
		break;
	case TYPE_L_SHIFT_EQ:
		c32_expr_l_shift_eq(expr_p1, &expr_p2, used_1);
		break;
	case TYPE_R_SHIFT_EQ:
		c32_expr_r_shift_eq(expr_p1, &expr_p2, used_1);
		break;
	default:
		c32_error_message(E_TO_BE_DEFINE, __LINE__, __FILE__);
	}
#ifdef DEBUG
printf("=== end factor1\n");
#endif
	return used_1;
}
/********************************************************************************/
/*		c32_expr																*/
/*		COM20: �萔	operand = "eax", "0x1234"  ���l�Ȃ�	value=0x1234, numb_flag=1	 */
/*		COM21: �z�� operand="_goban+0x10", "eax", "ebp+_main@_i", "0x1234"         */
/*		COM22: �ϐ�   operand="[_goban+0x10]", "[eax]", "[ebp+_main@_i]", "[0x1234]" */
/*																				*/
/*		return: eax �Ɍ��ʂ������Ă���ƂP�ɂȂ邻���łȂ���΂O				*/
/*		value : �萔�̎��A�萔��Ԃ��G���A										*/
/*		func �� 0 �̏ꍇ�� global table �̂݌�������							*/
/*		c32_factor1 , c32_factor1 , ... , c32_factor1							*/
/********************************************************************************/
int c32_expr(struct expr_param *expr_p1)
{
#ifdef DEBUG
printf("=== start c32_expr\n");
#endif
	int type, used;
	struct src_ptr src_ptr;
	struct symtbl *func = expr_p1->func;
	struct struct_tbl *tbl = expr_p1->tbl;
	
	c32_printf(";---------- c32_expr ----------\n");
	for(;;){
		memset(expr_p1, 0, sizeof(struct expr_param));
		expr_p1->func = func;
		expr_p1->tbl  = tbl;
		used = c32_factor1(expr_p1);
		c32_src_ptr_save(&src_ptr);
		c32_token_process(0, 0, &type);				// ',' �e�X�g
		if(type==TYPE_KANMA){				// ',' ������
			continue;
		}
		c32_src_ptr_restore(&src_ptr);
		c32_printf(";---------- c32_expr end ------\n");
#ifdef DEBUG
printf("=== end c32_expr\n");
#endif
		return used;
	}
}

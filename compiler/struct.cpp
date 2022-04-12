#include "stdafx.h"
#include "banana-c32.h"
#include "function.h"


static void st_define(char *st_name);
void c32_parser_array(struct symtbl *var);
void c32_global_array_init(struct symtbl *var, int flag);
static void array_init_sub(struct symtbl *var, int dim, unsigned short size[]);
static void array_init_elem(struct symtbl *var);
static void array_dummy_init(struct symtbl *var, int dim, unsigned short size[]);
static void struct_init(struct symtbl *var);


/********************************************************************************/
/*		st_def																	*/
/*		struct ����荞�񂾂Ƃ���												*/
/*		return 0:error, 1:success												*/
/********************************************************************************/
int st_def(struct attr_s *attr)
{
	int type;
	struct src_ptr src_ptr;
	struct struct_tbl *st_tbl;
	
	if(/*attr->attr==ATTR_CONST ||*/ attr->attr==0){	// OK
		attr->attr |= ATTR_STRUCT;
		c32_getsym();						// sturct ����荞�� �� c32_symbuf
		attr->symbuf = c32_malloc(strlen(c32_symbuf)+1);
		strcpy(attr->symbuf, c32_symbuf);
		
		c32_src_ptr_save(&src_ptr);
		c32_token_process(0, 0, &type);		// '{' �e�X�g
		c32_src_ptr_restore(&src_ptr);

		if(type==TYPE_L_NAMIKAKKO){
			st_define(attr->symbuf);		// {...} �̏���
		}
		
		/***	�X�g���N�`���[�e�[�u���T�[�`	***/
		for(st_tbl=c32_st_root; st_tbl; st_tbl=st_tbl->next){
			if(strcmp(attr->symbuf, st_tbl->symbuf)==0){
				break;
			}
		}
		if(st_tbl==0){
			c32_error_message(E_UNDEFINED_SYMBOL, __LINE__, __FILE__);
		}
		
		for(;;){
			c32_src_ptr_save(&src_ptr);
			c32_token_process(0, 0, &type);
			
			if(type==TYPE_ASTERISK){
				attr->attr += ATTR_POINTER1;
				if((attr->attr & ATTR_POINTER)==0)
					return 0;				// error
				continue;
			}
			else{
				c32_src_ptr_restore(&src_ptr);
				return 1;					// success
			}
		}
	}
	return 0;								// error
}
/********************************************************************************/
/*		st_define																*/
/*		�X�g���N�`���[�� {...} �̏���											*/
/********************************************************************************/
static void st_define(char *st_name)
{
	int type, ret;
	struct attr_s attr;
//	struct symtbl *ptr;
	struct struct_tbl **st_tbl = 0;
	struct struct_elem **st_elem = 0;
	struct src_ptr src_ptr;
	
	/***	�X�g���N�`���[�e�[�u���T�[�`	***/
	for(st_tbl=&c32_st_root; *st_tbl; st_tbl=&(*st_tbl)->next){
		if(strcmp(st_name, (*st_tbl)->symbuf)==0){
			break;
		}
	}
	if (c32_pass == PASS3) {
		if (*st_tbl == 0)
			c32_error_message(E_UNDEFINED_SYMBOL, __LINE__, __FILE__);
	}
	if(c32_pass==PASS1){
		if (*st_tbl) {
			c32_error_message(E_MULTIPLE_DEFINITION, __LINE__, __FILE__);
		}
		(*st_tbl) = (struct struct_tbl*)c32_malloc4(sizeof(struct struct_tbl));
		(*st_tbl)->symbuf = c32_malloc(strlen(st_name)+1);
		strcpy((*st_tbl)->symbuf, st_name);
	}
	/***	'{' �m�F	***/
	c32_token_process(0, 0, &type);			// '{' �m�F
	if(type != TYPE_L_NAMIKAKKO){
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
	}
	/***	�G�������g��荞��	***/
	for(;;){
		ret = c32_char_short_int_unsigned_pointer(&attr);
		if(ret==0){
			c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
		}
		c32_token_process(0, 0, &type);	// �ϐ�������荞��
		if(type != TYPE_SYMBOL){
			c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
		}
		/***	�X�g���N�`���[�G�������g�e�[�u���T�[�`	***/
		for(st_elem=&(*st_tbl)->elem; *st_elem; st_elem=&(*st_elem)->next){
			if(strcmp(c32_symbuf, (*st_elem)->tbl.symbuf)==0){
				break;
			}
		}
		if (c32_pass == PASS3) {
			if (*st_elem == 0)
				c32_error_message(E_UNDEFINED_SYMBOL, __LINE__, __FILE__);
		}
		if (c32_pass == PASS1) {
			if (*st_elem) {
				c32_error_message(E_MULTIPLE_DEFINITION, __LINE__, __FILE__);
			}
			(*st_elem) = (struct struct_elem*)c32_malloc4(sizeof(struct struct_elem));
			(*st_elem)->tbl.attr = attr;
			(*st_elem)->tbl.symbuf = c32_malloc(strlen(c32_symbuf) + 1);
			strcpy((*st_elem)->tbl.symbuf, c32_symbuf);
			(*st_elem)->tbl.size = c32_attr_to_byte(&attr);
			(*st_elem)->tbl.type = TYPE_SYMBOL;
			switch(c32_attr_to_byte(&attr)){
				case 1:
					break;
				case 2:
					(*st_tbl)->size = ((*st_tbl)->size+1) & 0xfffffffe;
					break;
				default:
					(*st_tbl)->size = ((*st_tbl)->size+3) & 0xfffffffc;
					break;
			}
			(*st_elem)->tbl.value = (*st_tbl)->size;
		}
		c32_src_ptr_save(&src_ptr);
		c32_token_process(0, 0, &type);
		if(type==TYPE_SEMIKORON){
		}
		else if(type==TYPE_L_KAKUKAKKO){
			c32_src_ptr_restore(&src_ptr);
			c32_parser_array(&(*st_elem)->tbl);	// int var[x][y][z]
			
			c32_token_process(0, 0, &type);
			if(type != TYPE_SEMIKORON){
				c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
			}
		}
		if(c32_pass==PASS1){
			(*st_tbl)->size += (*st_elem)->tbl.size;
		}
		c32_src_ptr_save(&src_ptr);
		c32_token_process(0, 0, &type);
		if(type==TYPE_R_NAMIKAKKO){
			if(c32_pass==PASS1){
				(*st_tbl)->size = ((*st_tbl)->size+3) & 0xfffffffc;
			}
			return;
		}
		c32_src_ptr_restore(&src_ptr);
	}
}
/********************************************************************************/
/*		c32_global_array_init													*/
/*		'=' �̌�납�� ';' �̎�O�܂�											*/
/*		flag: �ʏ�O�ŌĂяo���A�O�Ȃ烉�x����t����							*/
/********************************************************************************/
void c32_global_array_init(struct symtbl *var, int flag)
{
	int i, len, step;
	char buf[50];
	
	if(flag==0){
		c32_init_string_ptr = 0;
		
//		if((var->attr.attr & ATTR_CONST) && (var->attr.attr & ATTR_ARRAY)){
			c32_printf("%s:		;global_array_init\n", var->symbuf);
//		}
//		else{
//			c32_printf("%s@init\n", var->symbuf);
//		}
	}
	
	/***	�����l��f���o��	***/
	array_init_sub(var, var->xyz_dim, var->xyz_size);
	c32_output_buffer_flush();
	
	/***	string ��f���o��	***/
	if(c32_pass==PASS3 && flag==0){
		step = sizeof(buf)-1;
		len = c32_init_string_ptr;
		for(i=0; i<len; i+=step){
			memcpy(buf, &cv->init_string[i], step);
			buf[step] = 0;
			fprintf(c32_asm_fp, "%s", buf);
		}
		
		if(c32_init_string_ptr > c32_max_init_string){
			c32_max_init_string = c32_init_string_ptr;
		}
		c32_init_string_ptr = 0;
	}
	/***	�z��T�C�Y���O�̎��ׂ̈ɔz��T�C�Y�Đݒ�	***/
	var->size = c32_attr_to_byte(&var->attr);
	for(i=0; i<var->xyz_dim; i++){
		var->size *= var->xyz_size[i];
	}
	var->flag1 |= FLAG_08;					// ����������Ă���ϐ�
}
/********************************************************************************/
/*		array_init_sub															*/
/********************************************************************************/
static void array_init_sub(struct symtbl *var, int dim, unsigned short size[])
{
	int i, type, tmp;
	struct src_ptr src_ptr;
	
	if(dim){
		c32_token_process(0, 0, &type);		// '{' ��荞��
		if(type != TYPE_L_NAMIKAKKO){
			c32_error_message(E_L_NAMIKAKKO_MISSING, __LINE__, __FILE__);
		}
		tmp = size[0]==0 ? 0x1000000: size[0];
		/***	�������u���b�N��z��T�C�Y�����J��Ԃ�	***/
		for(i=0; i<tmp; i++){
			array_init_sub(var, dim-1, &size[1]);
			
			c32_token_process(0, 0, &type);		// '}' ',' ��荞��
			if(type==TYPE_R_NAMIKAKKO){
				break;
			}
			else if(type==TYPE_KANMA){
				c32_src_ptr_save(&src_ptr);
				c32_token_process(0, 0, &type);
				if(type==TYPE_R_NAMIKAKKO){
					break;
				}
				c32_src_ptr_restore(&src_ptr);
				continue;
			}
		}
		if(size[0]){
			/***	�z��T�C�Y��菭�Ȃ���΂O�t�B��	***/
			if(i==size[0]){
				c32_error_message(E_TABLE_OVERFLOW, __LINE__, __FILE__);
			}
			for(i++; i<size[0]; i++){
				array_dummy_init(var, dim-1, &size[1]);
			}
		}
		else{
			/***	�z��T�C�Y����	***/
			size[0] = i+1;
		}
	}
	else{
		array_init_elem(var);
	}
}
/********************************************************************************/
/*		array_dummy_init														*/
/********************************************************************************/
static void array_dummy_init(struct symtbl *var, int dim, unsigned short size[])
{
	int i;
	
	if(dim){
		for(i=0; i<size[0]; i++){
			array_dummy_init(var, dim-1, &size[1]);
		}
	}
	else{
		for(i=0; i<c32_attr_to_byte(&var->attr); i++){
			c32_printf("	db		0\n");
		}
		c32_output_buffer_flush();
	}
}
/********************************************************************************/
/*		array_init_elem															*/
/********************************************************************************/
static void array_init_elem(struct symtbl *var)
{
	struct expr_param expr_p1;
	
	if(var->attr.attr & ATTR_STRUCT){
		struct_init(var);
	}
	else{
		memset(&expr_p1, 0, sizeof(expr_p1));
		expr_p1.func = 0;
		must_save_t_reg = 0;
		
		c32_init_string_flag = 1;
		c32_factor1(&expr_p1);				// ',' ���Z�q�s�g�p
		c32_init_string_flag = 0;
		
		c32_output_buffer_flush();
		if(expr_p1.comment==COM20 && expr_p1.numb_flag==1){			// ���l����
//			switch(c32_attr_to_byte(&var->attr)){

				c32_printf("	%s	0x%x	;array_init_elem\n", c32_attr_to_dw(&var->attr), expr_p1.value);

//				case 1:
//					c32_printf("	db		%d\n", expr_p1.value);
//					break;
//				case 2:
//					c32_printf("	dh		%d\n", expr_p1.value);
//					break;
//				case 4:
//					c32_printf("	dw		%d\n", expr_p1.value);
//					break;
//				default:
//					c32_error_message(E_INTERNAL_ERROR, __LINE__, __FILE__);
//			}
		}
		else if(expr_p1.comment==COM21 && expr_p1.numb_flag==1){		// ���x���A�h���X����
			if(c32_attr_to_byte(&var->attr) != 4){
				c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
			}
			c32_printf("	dd		%s		;array_init_elem\n", expr_p1.operand);
		}
		else{
			c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
		}
		c32_output_buffer_flush();
	}
}
/********************************************************************************/
/*		struct_init																*/
/********************************************************************************/
static void struct_init(struct symtbl *var)
{
	int i, type;
	struct struct_tbl *tbl;
	struct struct_elem *elem;
	struct src_ptr src_ptr;
	
	if((var->attr.attr & ATTR_STRUCT)==0){
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
	}
	c32_token_process(0, 0, &type);		// '{' ��荞��
	if(type != TYPE_L_NAMIKAKKO){
		c32_error_message(E_L_NAMIKAKKO_MISSING, __LINE__, __FILE__);
	}
	/***	�X�g���N�`���[�e�[�u���T�[�`	***/
	for(tbl=c32_st_root; tbl; tbl=tbl->next){
		if(strcmp(var->attr.symbuf, tbl->symbuf)==0)
			break;
	}
	if(tbl==0){
		c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
	}
//	c32_printf("L%d:	;struct_init\n", c32_label_counter++);	// 4�o�C�g�A���C���̈�
	/***	������		***/
	for(elem=tbl->elem; elem; elem=elem->next){
		c32_global_array_init(&elem->tbl, 1);	// ���x���͕t���Ȃ�
		c32_token_process(0, 0, &type);		// ',' '}'
		if(type==TYPE_R_NAMIKAKKO){
			break;
		}
		else if(type==TYPE_KANMA){
			c32_src_ptr_save(&src_ptr);
			c32_token_process(0, 0, &type);
			if(type==TYPE_R_NAMIKAKKO){
				break;
			}
			c32_src_ptr_restore(&src_ptr);
		}
		else{
			c32_error_message(E_SYNTAX_ERROR, __LINE__, __FILE__);
		}
	}
	if (elem == 0) {
		c32_error_message(E_TABLE_OVERFLOW, __LINE__, __FILE__);
	}
	/***	�O�p�b�f�B���O	***/
	for(elem=elem->next; elem; elem=elem->next){
		if(elem->tbl.attr.attr & ATTR_STRUCT){
//			c32_printf("L%d:	;struct_init\n", c32_label_counter++);	// 4�o�C�g�A���C���̈�
			for(i=0; i<elem->tbl.size; i++){
				c32_printf("	db		0\n");
				c32_output_buffer_flush();
			}
		}
		else{
			switch(c32_attr_to_byte(&elem->tbl.attr)){
				case 1:
					for(i=0; i<elem->tbl.size; i++){
						c32_printf("	db		0\n");
						c32_output_buffer_flush();
					}
					break;
				case 2:
					for(i=0; i<elem->tbl.size/2; i++){
						c32_printf("	dw		0\n");
						c32_output_buffer_flush();
					}
					break;
				case 4:
					for(i=0; i<elem->tbl.size/4; i++){
						c32_printf("	dd		0\n");
						c32_output_buffer_flush();
					}
					break;
				default:
					c32_error_message(E_INTERNAL_ERROR, __LINE__, __FILE__);
			}
		}
	}
//	c32_printf("L%d\n", c32_label_counter++);	// 4�o�C�g�A���C���̈�
	c32_output_buffer_flush();
}

/* header file of the test_functions.c
   -  I made declerations of enum boolean, line-type called line and operation code that called opcode.
   -  define const numbers which have common use in this file.
   -  relevant function in use of the file.
   -  I sorted the functions by the files that uses them   */ 


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum{false, true} boolean;
typedef enum{null, entry,_extern, data, string, order} line;
typedef enum{mov,cmp,add,sub,lea,clr,not,inc,dec,jmp,bne,red,prn,jsr,rts,stop} opcode;

#define EQUAL 0
#define FIRST_CHAR 0
#define SECOND_CHAR 1
#define TWO_OPERAND_GROUP 5
#define ONE_OPERAND_GROUP 14
#define NO_OPERAND_GROUP 16
#define NUM_OF_OPCODES 16
#define MAX_LINE_LENGTH 80

/*******************************  test_functions.c  *******************************/
boolean line_test(char *name, char *temp, line type, int count);
boolean syntax_error_check(char *name);
boolean operands_num_check(char *word1, char *word2, char *word3);
boolean general_test(char *name, line type, int count);
line line_type(char *name);
boolean commas_check(char *name, line type, int count);
boolean misfit_operands(char *name, int count);

/*******************************  basic_functions.c  *******************************/
boolean is_symbol(char *name, int length);
boolean is_order(char *name, int length);
boolean is_instruction(char *name, int length);
boolean is_data(char *name);
boolean is_string(char *name);
boolean is_extern(char *name);
boolean is_entry(char *name);
boolean is_immediate_add(char *name);
boolean is_direct_reg(char *name);
boolean is_indirect_reg(char *name);
boolean is_direct_add(char *name);
boolean is_number(char *name);
boolean is_string_name(char *name);
boolean is_ent_ext_operand(char *name);
int size(char *name);
/* header file of the second_pass.c
   -  I made declerations of enum boolean, line-type called line and operation code that called opcode.
   -  define const numbers which have common uses in this file.
   -  struct symbols, datas, order_word, value_word and reg_word which are the main structers I used to build the machine code.
   -  relevant function in use of the file.
   -  I sorted the functions by the files that uses them   */ 


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum{false, true} boolean;
typedef enum{mov,cmp,add,sub,lea,clr,not,inc,dec,jmp,bne,red,prn,jsr,rts,stop} opcode;
typedef enum{null, entry,_extern, data, string, order} line;

#define INITIAL_NUM 0
#define FIRST_CHAR 0
#define EQUAL 0
#define FIRST_CELL 0
#define SECOND_CHAR 1
#define ONE_MEMORY_WORD 1
#define FIRST_NUM 1
#define EXTERNAL 1
#define IMMEDIATE_ADD 1
#define SECOND_CELL 1
#define THIRD_CHAR 2
#define THIRD_CELL 2
#define ORDER_LENGTH 2
#define DIRECT_ADD 2
#define RELOCATABLE 2
#define TWO_MEMORY_WORDS 2
#define THREE_CELLS 3
#define THREE_MEMORY_WORD 3
#define INDIRECT_REG 4
#define ABSOLUTE 4
#define TWO_OPERAND_GROUP 5
#define NUM_OF_REGIS 8
#define DIRECT_REG 8
#define ONE_OPERAND_GROUP 14
#define NO_OPERAND_GROUP 16
#define NUM_OF_OPCODES 16
#define MAX_NAME_LEN 31
#define MAX_LINE_LENGTH 80
#define MAX_MEMORY 4096

typedef struct node *ptr;
typedef struct node
{
    char name[MAX_NAME_LEN];
    int address;
    boolean is_entry;
    boolean is_extern;
    boolean is_data;
    ptr next;
} symbols;

typedef struct n *p;
typedef struct n
{
    int address;
    char name[MAX_NAME_LEN];
    char str[MAX_LINE_LENGTH];
    boolean type_data;
    boolean type_string;
    p next;
} datas;

typedef struct node1
{
    unsigned int ARE : 3;
    unsigned int dest : 4;
    unsigned int source : 4;
    unsigned int opcode : 4;
} order_word;

typedef struct node2
{
    unsigned int ARE : 3;
    unsigned int address : 12;
} value_word;

typedef struct node3
{
    unsigned int ARE : 3;
    unsigned int dest : 3;
    unsigned int source : 3;
} reg_word;

/*******************************  second_pass.c  *******************************/
unsigned short int *second_pass(FILE *f, boolean *flag, symbols **head_sym, symbols **head_extern, int initial_ic);
boolean is_extern_symbol(symbols *temp);
boolean existed_symbol(char *name, symbols *ptr);
symbols *find_symbol(symbols **head, char *name);
int order_type(char *order, char *op1, char *op2);
int reg_number(char *name);
int address_type(char *name);
unsigned short int *create_orders_array(symbols **head, char *order, char *op1, char *op2);
symbols *create_extern_list(char *name, int address);
unsigned int num_calc(char *name);

/*******************************  basic_functions.c  *******************************/
boolean is_symbol(char *name, int length);
boolean is_immediate_add(char *name);
boolean is_direct_reg(char *name);
boolean is_indirect_reg(char *name);
boolean is_direct_add(char *name);
int size(char *name);

/*******************************  test_functions.c  *******************************/
line line_type(char *name);
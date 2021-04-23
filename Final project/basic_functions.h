/* header file of the basic_functions.c
   -  I made declerations of enum boolean and line-type called line.
   -  define const numbers which have common uses in this file.
   -  struct symbols, datas and data_word which are the main structers I used to build the machine code.
   -  relevant function in use of the file.
   -  I sorted the functions by their files that uses them   */ 


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum{false, true} boolean;
typedef enum{null, entry, _extern, data, string, order} line;

#define END_OF_STRING 0
#define FIRST_CHAR 0
#define INITIAL_NUM 0
#define INITIAL_IC 100
#define EQUAL 0
#define FIRST_NUM 1
#define ONE_MEMORY_WORD 1
#define SECOND_CHAR 1
#define TWO_MEMORY_WORDS 2
#define THREE_MEMORY_WORD 3
#define NUM_OF_CODES 4
#define TWO_OPERAND_GROUP 5
#define NUM_OF_REGIS 8
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

typedef struct node4
{
    unsigned int number : 15;
} data_word;

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
boolean is_operand_symbol(char *name);

/*******************************  assembler.c  *******************************/
unsigned short int *create_machine_code(unsigned short int *order_m_code, int ord_length, unsigned short int *inst_m_code, int inst_length);
void create_files(symbols *head_of_sym_list, symbols *head_of_extern_list, unsigned short *machine_code, boolean existed_entries, boolean existed_externals, int IC, int DC, char file[]);
unsigned short int *create_instructions_array(datas **head, int DC);

/*******************************  first_pass.c  *******************************/
boolean is_extern_symbol(symbols *temp);
boolean is_entry_symbol(symbols *temp);
boolean is_data_symbol(symbols *temp);
boolean existed_symbol(char *name, symbols *ptr);
symbols *find_symbol(symbols **head, char *name);
symbols *get_last_symbol(symbols **head);
datas *get_last_data(datas **head);
int num_count(char *name);
datas *create_data_string(datas **temp, char *name, int num);
datas *create_string(datas **temp, char *name);
int IC_calculator(char *word1, char *word2, char *word3);
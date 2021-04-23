/* header file of the main program - assembler.c
   -  I made declerations of enum boolean.
   -  define const numbers which have common uses in file.
   -  struct symbols and datas which are the main structers I used to build the machine code.
   -  relevant function in use of the file   */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum{false, true} boolean;

#define FIRST_CHAR 0
#define FIRST_CELL 0
#define INITIAL_DC 0
#define NO_FILES 1
#define SECOND_CELL 1
#define FIRST_FILE 1
#define THIRD_CELL 2
#define MAX_NAME_LEN 31
#define MAX_LINE_LENGTH 80
#define INITIAL_IC 100
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

/*******************************  assembler.c  *******************************/
int *first_pass(FILE *f, symbols **head_of_sym_list, datas **head_of_data_list, int initial_ic, int initial_dc);
unsigned short int *second_pass(FILE *f, boolean *flag, symbols **head_sym, symbols **head_extern, int initial_ic);
unsigned short int *create_instructions_array(datas **head, int DC);
unsigned short int *create_machine_code(unsigned short int *order_m_code, int ord_length, unsigned short int *inst_m_code, int inst_length);
void create_files(symbols *head_of_sym_list, symbols *head_of_extern_list, unsigned short *machine_code, boolean existed_entries, boolean existed_externals, int IC, int DC, char file[]);
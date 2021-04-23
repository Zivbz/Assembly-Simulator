/* header file of the first_pass.c
   -  I made declerations of enum boolean and line-type called line.
   -  define const numbers which have common uses in this file.
   -  struct symbols and datas which are the main structers I used to build the machine code.
   -  relevant function in use of the file.
   -  I sorted the functions by the files that uses them   */ 


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum{false, true} boolean;
typedef enum{null, entry,_extern, data, string, order} line;

#define INITIAL_NUM 0
#define FIRST_CELL 0
#define FIRST_CHAR 0
#define END_OF_LINE 1
#define FIRST_ROW 1
#define SECOND_CELL 1
#define THIRD_CELL 2
#define THREE_CELLS 3
#define MAX_NAME_LEN 31
#define MAX_LINE_LENGTH 80

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

/*******************************  first_pass.c  *******************************/
int *first_pass(FILE *f, symbols **head_of_sym_list, datas **head_of_data_list, int initial_ic, int initial_dc);
boolean is_data_symbol(symbols *temp);
boolean existed_symbol(char *name, symbols *ptr);
symbols *find_symbol(symbols **head, char *name);
symbols *get_last_symbol(symbols **head);
datas *get_last_data(datas **head);
int num_count(char *name);
datas *create_data_string(datas **temp, char *name, int num);
datas *create_string(datas **temp, char *name);
int IC_calculator(char *word1, char *word2, char *word3);

/*******************************  basic_functions.c  *******************************/
boolean is_symbol(char *name, int length);
int size(char *name);

/*******************************  test_functions.c  *******************************/
boolean line_test(char *name, char *temp, line type, int count);
line line_type(char *name);
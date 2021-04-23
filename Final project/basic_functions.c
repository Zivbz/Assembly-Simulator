/* This is the basic functions file.
    most files in the program uses many of the functions below.
    I added headline of the file that uses the function below him, and 
    explained shortly on the function itself right above each function  */

#include "basic_functions.h"

/***************************************************************************************/
/*******************************  assembler.c  *******************************/
/***************************************************************************************/

/* function that create the machine code array */
unsigned short int *create_machine_code(unsigned short int *order_m_code, int ord_length, unsigned short int *inst_m_code, int inst_length)
{
    unsigned short int *code = (unsigned short int *)malloc(MAX_MEMORY * sizeof(unsigned short int));

    code = order_m_code;
    memcpy(code + ord_length, inst_m_code, inst_length * sizeof(short int));

    return code;
}

/* function that will create the files of machine code, external, entries */
void create_files(symbols *head_of_sym_list, symbols *head_of_extern_list, unsigned short *machine_code, boolean existed_entries, boolean existed_externals, int IC, int DC, char file[])
{
    char fila_name[FILENAME_MAX];
    FILE *fptr;
    int j;

    /* create object file of the machine code */
    strcpy(fila_name, file);
    strcat(fila_name, ".obj");
    fptr = fopen(fila_name, "w");
    if (fptr)
    {
        fprintf(fptr, " %d %d\n", IC, DC);
        for (j = FIRST_CHAR; j < (IC + DC); j++)
        {
            fprintf(fptr, "%04d %05o\n", j + INITIAL_IC, machine_code[j]);
        }
        fclose(fptr);
    }
    else
    {
        fprintf(stderr, "There was a problem to create the file %s\n", fila_name);
        exit(0);
    }
    /* check for existed entries and create entries file if needed */
    if (existed_entries)
    {
        strcpy(fila_name, file);
        strcat(fila_name, ".ent");
        fptr = fopen(fila_name, "w");
        if (fptr)
        {
            while (head_of_sym_list)
            {
                if (head_of_sym_list->is_entry)
                    fprintf(fptr, "%s %04d\n", head_of_sym_list->name, head_of_sym_list->address);
                head_of_sym_list = head_of_sym_list->next;
            }
            fclose(fptr);
        }
        else
        {
            fprintf(stderr, "There was a problem to create the file %s\n", fila_name);
            exit(0);
        }
    }
    /* check for existed externals and create externals file if needed */
    if (existed_externals)
    {
        strcpy(fila_name, file);
        strcat(fila_name, ".ext");
        fptr = fopen(fila_name, "w");
        if (fptr)
        {
            while (head_of_extern_list)
            {
                fprintf(fptr, "%s %04d\n", head_of_extern_list->name, head_of_extern_list->address);
                head_of_extern_list = head_of_extern_list->next;
            }
            fclose(fptr);
        }
        else
        {
            fprintf(stderr, "There was a problem to create the file %s\n", fila_name);
            exit(0);
        }
    }
}

/* function that create final array of data in 15-bits binary numbers */
unsigned short int *create_instructions_array(datas **head, int DC)
{
    datas *d_temp_h1 = *head;
    data_word *number = (data_word *)malloc(sizeof(data_word));
    data_word *d_string = (data_word *)malloc(sizeof(data_word));
    char *temp_data_word;
    unsigned short int *data_arr = malloc(MAX_MEMORY * sizeof(short int));
    int i;
    int d_index = INITIAL_NUM;
    while (d_temp_h1)
    {
        if (d_temp_h1->type_data)
        { /* if it is a .data node will go over the string and add each number to array */
            temp_data_word = strtok(d_temp_h1->str, ",");
            while (temp_data_word)
            {
                number->number = atoi(temp_data_word);
                data_arr[d_index++] = *(unsigned short int *)*(&number);
                temp_data_word = strtok(NULL, ",");
            }
        }
        else
        { /* if it is a .string node will add every char to array and add \0 at the end */
            for (i = FIRST_CHAR; i < strlen(d_temp_h1->str); i++)
            {
                d_string->number = (d_temp_h1->str)[i];
                data_arr[d_index++] = *(unsigned short int *)*(&d_string);
            }
            data_arr[d_index++] = END_OF_STRING;
        }
        d_temp_h1 = d_temp_h1->next;
    }
    return data_arr;
}

/***************************************************************************************/
/*************************************  first_pass.c  **********************************/
/***************************************************************************************/

/* function that checks if the symbol attached to extern */
boolean is_extern_symbol(symbols *temp)
{
    if (temp->is_extern)
        return true;
    return false;
}

/* function that checks if the symbol attaches to entry */
boolean is_entry_symbol(symbols *temp)
{
    if (temp->is_entry)
        return true;
    return false;
}

/* function that checks if the symbol attaches to data line(string or data) */
boolean is_data_symbol(symbols *temp)
{
    if (temp->is_data)
        return true;
    return false;
}

/* function that checks if the symbol exist in a list of symbols */
boolean existed_symbol(char *name, symbols *ptr)
{
    while (ptr)
    {
        if (strcmp((ptr->name), name) == EQUAL)
            return true;
        ptr = ptr->next;
    }
    return false;
}

/* function that finds symbol in a list(if existed) */
symbols *find_symbol(symbols **head, char *name)
{

    symbols *h = *head;
    while ((strcmp(h->name, name) != EQUAL) && h->next)
    {
        h = h->next;
    }
    if ((strcmp(h->name, name) == EQUAL))
        return h;
    else
        return NULL;
}

/* function that returns a pointer of symbols to the last symbol node in list */
symbols *get_last_symbol(symbols **head)
{
    symbols *h = *head;
    while (h->next)
    {
        h = h->next;
    }
    return h;
}

/* function that returns a pointer of daatas to the last data node in list */
datas *get_last_data(datas **head)
{
    datas *h = *head;
    while (h->next)
    {
        h = h->next;
    }
    return h;
}

/* function that counts how many numbers are in ".data" line */
int num_count(char *name)
{
    char temp1[MAX_LINE_LENGTH], temp2[MAX_LINE_LENGTH];
    char *symbol, *word;
    int num = INITIAL_NUM;

    strcpy(temp1, name);
    strcpy(temp2, name);
    symbol = strtok(temp1, " ");
    if (is_symbol(symbol, size(symbol)))
    {
        word = strtok(NULL, ","
                            " "
                            "\n"
                            "\t");
        word = strtok(NULL, ","
                            " "
                            "\n"
                            "\t");
    }
    else
    {
        symbol = NULL;
        word = strtok(temp2, " ");
        word = strtok(NULL, ","
                            " "
                            "\n"
                            "\t");
    }

    while (word)
    {
        num++;
        word = strtok(NULL, ","
                            " "
                            "\n"
                            "\t");
    }
    return num;
}

/* function that create string of data from a given .data line:
    I made this function for a later use of the string of numbers, so I will 
    know how it is stored in data linked list and then how to use it.
    it will create a string of numbers and commas, no spaces.  */
datas *create_data_string(datas **temp, char *name, int num)
{
    char temp1[MAX_LINE_LENGTH], temp2[MAX_LINE_LENGTH];

    datas *t = *temp;
    char *symbol, *word1;
    int i;

    strcpy(temp1, name);
    strcpy(temp2, name);
    symbol = strtok(temp1, " ");
    if (is_symbol(symbol, size(symbol)))
    {
        word1 = strtok(NULL, ","
                             " "
                             "\n"
                             "\t");
        word1 = strtok(NULL, ","
                             " "
                             "\n"
                             "\t");
    }
    else
    {
        symbol = NULL;
        word1 = strtok(temp2, " ");
        word1 = strtok(NULL, ","
                             " "
                             "\n"
                             "\t");
    }
    strcpy(t->str, word1);
    for (i = FIRST_NUM; i < num; i++)
    {
        word1 = strtok(NULL, ","
                             " "
                             "\n"
                             "\t");
        strcat(t->str, ",");
        strcat(t->str, word1);
    }
    return t;
}

/* function that create string of data from a given .string line  */
datas *create_string(datas **temp, char *name)
{
    char temp1[MAX_LINE_LENGTH], temp2[MAX_LINE_LENGTH];
    datas *t = *temp;
    char *symbol, *word;

    strcpy(temp1, name);
    strcpy(temp2, name);
    symbol = strtok(temp1, " ");
    if (is_symbol(symbol, size(symbol)))
    {
        word = strtok(NULL, ","
                            " "
                            "\n"
                            "\t");
        word = strtok(NULL, "\n"
                            "\"");
    }
    else
    {
        symbol = NULL;
        word = strtok(temp2, " ");
        word = strtok(NULL, "\n"
                            "\"");
    }
    strcpy(t->str, word);
    return t;
}

/*** function that calculate the IC of given line ***/
int IC_calculator(char *word1, char *word2, char *word3)
{
    int i;
    char *order_code[NUM_OF_OPCODES] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};

    for (i = FIRST_CHAR; i < TWO_OPERAND_GROUP; i++)
    {
        if (strcmp(word1, order_code[i]) == EQUAL)
        {
            if ((is_direct_reg(word2) || is_indirect_reg(word2)) && (is_direct_reg(word3) || is_indirect_reg(word3)))
                return TWO_MEMORY_WORDS;
            else
                return THREE_MEMORY_WORD;
        }
    }
    for (; i < ONE_OPERAND_GROUP; i++)
    {
        if (strcmp(word1, order_code[i]) == EQUAL)
            return TWO_MEMORY_WORDS;
    }
    for (; i < NO_OPERAND_GROUP; i++)
    {
        if (strcmp(word1, order_code[i]) == EQUAL)
            return ONE_MEMORY_WORD;
    }
    return EOF;
}

/************************************************************************************/
/*******************************  basic_functions.c  ********************************/
/************************************************************************************/

/* funcion that checks whether a given word is a symbol */
boolean is_symbol(char *name, int length)
{
    int i;

    for (i = FIRST_CHAR; i < length; i++)
    {
        if (!isalnum(name[i]))
            return false;
    }
    if (name[length] != ':')
        return false;
    return true;
}

/* funcion that checks whether a given word is an order */
boolean is_order(char *name, int length)
{
    int i;
    char *order_code[NUM_OF_OPCODES] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};

    if (isalpha(name[FIRST_CHAR]) && (name[length] != ':'))
    {
        for (i = FIRST_CHAR; i < NUM_OF_OPCODES; i++)
        {
            if (strcmp(name, order_code[i]) == EQUAL)
                return true;
        }
    }
    return false;
}

/* funcion that checks whether a given word is a general instruction */
boolean is_instruction(char *name, int length)
{
    int i;
    char *instruction_code[NUM_OF_CODES] = {".data", ".string", ".entry", ".extern"};

    if ((name[FIRST_CHAR]) == '.')
    {
        for (i = FIRST_CHAR; i < NUM_OF_CODES; i++)
        {
            if (strcmp(name, instruction_code[i]) == EQUAL)
                return true;
        }
    }
    return false;
}

/* funcion that checks whether a given word is data instruction */
boolean is_data(char *name)
{
    if (strcmp(name, ".data") == EQUAL)
        return true;
    return false;
}

/* funcion that checks whether a given word is string instruction */
boolean is_string(char *name)
{
    if (strcmp(name, ".string") == EQUAL)
        return true;
    return false;
}

/* funcion that checks whether a given word is extern order */
boolean is_extern(char *name)
{
    if (strcmp(name, ".extern") == EQUAL)
        return true;
    return false;
}

/* funcion that checks whether a given word is entry order */
boolean is_entry(char *name)
{
    if (strcmp(name, ".entry") == EQUAL)
        return true;
    return false;
}

/* funcion that checks whether a given word is an immediate address operation */
boolean is_immediate_add(char *name)
{
    if ((name[FIRST_CHAR]) == '#')
        return true;
    return false;
}

/* funcion that checks whether a given word is a direct address operation */
boolean is_direct_add(char *name)
{
    if (!(is_immediate_add(name)) && !(is_direct_reg(name)) && !(is_indirect_reg(name)))
        return true;
    return false;
}

/* funcion that checks whether a given word is a direct register address operation */
boolean is_direct_reg(char *name)
{
    int i;
    char *regis[NUM_OF_REGIS] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

    for (i = FIRST_CHAR; i < NUM_OF_REGIS; i++)
    {
        if (strcmp(name, regis[i]) == EQUAL)
            return true;
    }
    return false;
}

/* funcion that checks whether a given word is an indirect register address operation */
boolean is_indirect_reg(char *name)
{
    int i;
    char *regis[NUM_OF_REGIS] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

    if (name[FIRST_CHAR] == '*')
    {
        for (i = FIRST_CHAR; i < NUM_OF_REGIS; i++)
        {
            char *temp = name + SECOND_CHAR; /*in order to identify the register after the first char '*'. */
            if (strcmp(temp, regis[i]) == EQUAL)
                return true;
        }
    }
    return false;
}

/* funcion that checks whether a given word is a number */
boolean is_number(char *name)
{
    int i;

    if ((name[FIRST_CHAR] == '+') || (name[FIRST_CHAR] == '-'))
    {
        for (i = SECOND_CHAR; i <= size(name); i++)
        {
            if (!isdigit(name[i]))
                return false;
        }
        return true;
    }
    else
    {
        for (i = FIRST_CHAR; i <= size(name); i++)
        {
            if (!isdigit(name[i]))
                return false;
        }
        return true;
    }
}

/* funcion that checks whether a given word is a verified string word.
    check for logic errors in string */
boolean is_string_name(char *name)
{
    char temp1[MAX_LINE_LENGTH], temp2[MAX_LINE_LENGTH];
    char *symbol, *word;
    int i;
    boolean error_flag = false;

    strcpy(temp1, name);
    strcpy(temp2, name);
    symbol = strtok(temp1, " ");
    if (is_symbol(symbol, size(symbol)))
    {
        word = strtok(NULL, " ");
        word = strtok(NULL, "");
    }
    else
    {
        symbol = NULL;
        word = strtok(temp2, " ");
        word = strtok(NULL, ""
                            "\n");
    }

    if (word[FIRST_CHAR] == '\"')
    {
        i = SECOND_CHAR;
        while (word[i] != '\"' && i != size(word))
        {
            i++;
            if (i == size(word) && word[i] == '\"')
                error_flag = false;
            else if (i == size(word))
                error_flag = true;
        }
        if (!error_flag)
        {
            i++;
            while (i != size(word))
            {
                if (word[i] != ' ')
                {
                    error_flag = true;
                }
                i++;
            }
        }
    }
    else
        error_flag = true;
    return !error_flag;
}

/* funcion that checks whether a given word is verified to be after entry/extern instruction */
boolean is_ent_ext_operand(char *name)
{
    if (name && isalpha(name[FIRST_CHAR]) && is_operand_symbol(name))
    {
        if (is_order(name, size(name)) || is_instruction(name, size(name)) || is_immediate_add(name) ||
            is_direct_reg(name) || is_indirect_reg(name) || is_number(name))
            return false;

        return true;
    }
    return false;
}

/* funcion that measure a given word(used it to fit loops sizes instead of function strlen) */
int size(char *name)
{
    return strlen(name) - 1;
}

/* funcion that checks whether a given word is an operand symbol(not in beginning of line) */
boolean is_operand_symbol(char *name)
{
    int i;
    if (!isalpha(name[FIRST_CHAR]))
        return false;
    else
    {
        for (i = FIRST_CHAR; i < strlen(name); i++)
        {
            if (!isalnum(name[i]))
                return false;
        }
    }
    return true;
}

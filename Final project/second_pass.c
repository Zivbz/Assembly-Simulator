/* This is second pass file. along the main function "second_pass" here in
    this file there is other functions in use of this pass.
    the main function second_pass is using a while loop to go over each line and a switch to determine what 
    to do. 
    This pass will build the machine coded array of the orders, create a linked list of extern symbols and their address
    for a later use and complete the symbols table(I used a linked list for that).      */

#include "second_pass.h"

/***************************************************************************************/
/***********************************  second_pass.c  ***********************************/
/***************************************************************************************/

/* second pass function */
unsigned short int *second_pass(FILE *f, boolean *flag, symbols **head_sym, symbols **head_extern, int initial_ic)
{
    char name[MAX_LINE_LENGTH];
    int row_count = FIRST_NUM, arr_index = INITIAL_NUM, i = FIRST_CHAR;
    boolean error_flag = false;
    unsigned short int *temp_arr;
    unsigned short int *ord_machine_code = (unsigned short int *)malloc(MAX_MEMORY * sizeof(unsigned short int));
    symbols *h_of_sym = *head_sym, *h_of_extern = *head_extern, *last_of_extern, *temp_sym;

    /* while loop to go over each line */
    while (fgets(name, MAX_LINE_LENGTH, f) != NULL)
    {
        char temp[MAX_LINE_LENGTH], temp_name[MAX_LINE_LENGTH];
        char *symbol, *word1, *word2, *word3;
        line line = null;

        /* breakdown the line to words */
        strcpy(temp_name, name);
        strcpy(temp, name);
        symbol = strtok(temp_name, " ");
        while (symbol[i] != '\n' && symbol[i] == EOF)
        {
            i++;
        }
        if (isspace(symbol[i]) || symbol[FIRST_CHAR] == ';')
        {
            continue;
        }
        else
        {
            if (is_symbol(symbol, size(symbol)))
            {
                word1 = strtok(NULL, ","
                                     " "
                                     "\n"
                                     "\t");
                word2 = strtok(NULL, ","
                                     " "
                                     "\n"
                                     "\t");
                word3 = strtok(NULL, ","
                                     " "
                                     "\n"
                                     "\t");
            }
            else
            {
                symbol = NULL;
                word1 = strtok(name, " ");
                word2 = strtok(NULL, ","
                                     " "
                                     "\n"
                                     "\t");
                word3 = strtok(NULL, ","
                                     " "
                                     "\n"
                                     "\t");
            }

            line = line_type(temp);

            /* according to the line type the function will enter the switch case */
            switch (line)
            {
                /* switch case: .entry - was ignored this line in first pass*/
            case entry:
                if (!h_of_sym)
                {
                    fprintf(stdout, "Error: symbol declared but not used in this file \n");
                    error_flag = true;
                }
                else
                {
                    if (existed_symbol(word2, h_of_sym))
                    {
                        temp_sym = find_symbol(&h_of_sym, word2);
                        if (is_extern_symbol(temp_sym))
                        {
                            fprintf(stdout, "Line %d: error: symbol \"%s\" is external and declared in this file \n", row_count, word2);
                            error_flag = true;
                        }
                        else
                        {
                            temp_sym->is_entry = true;
                        }
                    }
                }
                break;

            case _extern:
                break;

            case data:
                break;

            case string:
                break;

                /* switch case: order:
                this case will create a mchine code for the current line and add it
                to the order machine code array.
                in the meanwhile it will check for direct address operand in the symbol list
                and look for extern symbols and create a list of them */
            case order:

                /* in case the machine code array is empty */
                if (arr_index == INITIAL_NUM)
                {
                    /* use another function to build the machine code of the line */
                    temp_arr = create_orders_array(&h_of_sym, word1, word2, word3);
                    if (temp_arr[FIRST_CELL] == EOF)
                    {
                        error_flag = true;
                        break;
                    }
                    else if (temp_arr[SECOND_CELL] == EOF)
                    {
                        error_flag = true;
                        break;
                    }
                    ord_machine_code = temp_arr;
                    if (word2 && is_direct_add(word2))
                    {
                        /* if the operand is symbol will look for the symbol in list */
                        temp_sym = find_symbol(&h_of_sym, word2);
                        if (!temp_sym)
                        {
                            fprintf(stdout, "Error: symbol \"%s\" is not found in symbol list \n", word2);
                            error_flag = true;
                        }
                        else if (is_extern_symbol(temp_sym))
                        {
                            /* if the symbol is external will add it to external list */
                            if (!h_of_extern)
                            {
                                h_of_extern = create_extern_list(word2, arr_index + SECOND_CELL + initial_ic);
                                *head_extern = h_of_extern;
                                last_of_extern = h_of_extern;
                                strcpy(h_of_extern->name, word2);
                            }
                            else
                            {
                                last_of_extern->next = create_extern_list(word2, arr_index + SECOND_CELL + initial_ic);
                                last_of_extern = last_of_extern->next;
                            }
                        }
                    }
                    if (word3 && is_direct_add(word3))
                    {
                        /* if the second operand is symbol will look for the symbol in list */
                        temp_sym = find_symbol(&h_of_sym, word3);
                        if (!temp_sym)
                        {
                            fprintf(stdout, "Error: symbol \"%s\" is not found in symbol list \n", word3);
                            error_flag = true;
                        }
                        else if (is_extern_symbol(temp_sym))
                        {
                            /* if the symbol is external will add it to external list */
                            if (!h_of_extern)
                            {
                                h_of_extern = create_extern_list(word3, arr_index + THIRD_CELL + initial_ic);
                                *head_extern = h_of_extern;
                                last_of_extern = h_of_extern;
                                strcpy(h_of_extern->name, word3);
                            }
                            else
                            {
                                last_of_extern->next = create_extern_list(word3, arr_index + THIRD_CELL + initial_ic);
                                last_of_extern = last_of_extern->next;
                            }
                        }
                    }
                    if (word2 && word3)
                    {
                        /* updating the machine code array index. here there is a check for two operands of register - special case */
                        if ((is_direct_reg(word2) || is_indirect_reg(word2)) && (is_direct_reg(word3) || is_indirect_reg(word3)))
                            arr_index += TWO_MEMORY_WORDS;
                        else
                            arr_index += THREE_MEMORY_WORD;
                    }
                    else if (word2)
                        arr_index += TWO_MEMORY_WORDS;
                    else
                        arr_index += ONE_MEMORY_WORD;
                }

                /* in case the machine code array is not empty */
                else
                {
                    temp_arr = create_orders_array(&h_of_sym, word1, word2, word3);
                    if (temp_arr[FIRST_CELL] == EOF)
                    {
                        error_flag = true;
                        break;
                    }
                    else if (temp_arr[SECOND_CELL] == EOF)
                    {
                        error_flag = true;
                        break;
                    }
                    memcpy(ord_machine_code + arr_index, temp_arr, THREE_CELLS * sizeof(short int));
                    if (word2 && is_direct_add(word2))
                    {
                        /* if the first operand is symbol will look for the symbol in list */
                        temp_sym = find_symbol(&h_of_sym, word2);
                        if (!temp_sym)
                        {

                            fprintf(stdout, "Error: symbol \"%s\" is not found in symbol list \n", word2);
                            error_flag = true;
                        }
                        else if (is_extern_symbol(temp_sym))
                        {
                            /* if the symbol is external will add it to external list */
                            if (!h_of_extern)
                            {
                                h_of_extern = create_extern_list(word2, arr_index + SECOND_CELL + initial_ic);
                                *head_extern = h_of_extern;
                                last_of_extern = h_of_extern;
                            }
                            else
                            {
                                last_of_extern->next = create_extern_list(word2, arr_index + SECOND_CELL + initial_ic);
                                last_of_extern = last_of_extern->next;
                            }
                        }
                    }
                    if (word3 && is_direct_add(word3))
                    {
                        /* if the second operand is symbol will look for the symbol in list */
                        temp_sym = find_symbol(&h_of_sym, word3);
                        if (!temp_sym)
                        {
                            fprintf(stdout, "Error: symbol \"%s\" is not found in symbol list \n", word3);
                            error_flag = true;
                        }
                        else if (is_extern_symbol(temp_sym))
                        {
                            /* if the symbol is external will add it to external list */
                            if (!h_of_extern)
                            {
                                h_of_extern = create_extern_list(word3, arr_index + THIRD_CELL + initial_ic);
                                *head_extern = h_of_extern;
                                last_of_extern = h_of_extern;
                                strcpy(h_of_extern->name, word3);
                            }
                            else
                            {
                                last_of_extern->next = create_extern_list(word3, arr_index + THIRD_CELL + initial_ic);
                                last_of_extern = last_of_extern->next;
                            }
                        }
                    }
                    if (word2 && word3)
                    {
                        /* updating the machine code array index. here there is a check for two operands of register - special case */
                        if ((is_direct_reg(word2) || is_indirect_reg(word2)) && (is_direct_reg(word3) || is_indirect_reg(word3)))
                            arr_index += TWO_MEMORY_WORDS;
                        else
                            arr_index += THREE_MEMORY_WORD;
                    }
                    else if (word2)
                        arr_index += TWO_MEMORY_WORDS;
                    else
                        arr_index += ONE_MEMORY_WORD;
                }
                break;

            default:
                break;
            } /* end of switch */
            row_count++;
        }
    } /* end of while */

    *flag = error_flag; /* updating the original error flag */

    return ord_machine_code;
}

/* function that return the number of memory word needs to be added */
int order_type(char *order, char *op1, char *op2)
{
    int i;
    char *order_code[NUM_OF_OPCODES] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};

    for (i = FIRST_CHAR; i < TWO_OPERAND_GROUP; i++)
    {
        if (strcmp(order, order_code[i]) == EQUAL)
            return THREE_MEMORY_WORD;
    }
    for (; i < ONE_OPERAND_GROUP; i++)
    {
        if (strcmp(order, order_code[i]) == EQUAL)
            return TWO_MEMORY_WORDS;
    }
    for (; i < NO_OPERAND_GROUP; i++)
    {
        if (strcmp(order, order_code[i]) == EQUAL)
            return ONE_MEMORY_WORD;
    }
    return false;
}

/* function that return the register number of a given register(direct or indirect address) */
int reg_number(char *name)
{
    int i, num;
    char *temp;
    if (name[FIRST_CHAR] == '*')
    {
        temp = name + THIRD_CHAR;
        num = atoi(temp);
        for (i = FIRST_CHAR; i < NUM_OF_REGIS; i++)
        {
            if (num == i)
                return i;
        }
    }
    else
    {
        temp = name + SECOND_CHAR;
        num = atoi(temp);
        for (i = FIRST_CHAR; i < NUM_OF_REGIS; i++)
        {
            if (num == i)
                return i;
        }
    }
    return EOF;
}

/* function that returns the type of address of a given operand */
int address_type(char *name)
{
    if (is_immediate_add(name))
        return IMMEDIATE_ADD;
    else if (is_direct_add(name))
        return DIRECT_ADD;
    else if (is_indirect_reg(name))
        return INDIRECT_REG;
    else if (is_direct_reg(name))
        return DIRECT_REG;
    return EOF;
}

/* function that recieves line of order with 2 operands(maybe less, depend on the order) and make the machine code of it */
unsigned short int *create_orders_array(symbols **head, char *order, char *op1, char *op2)
{
    char *order_code[NUM_OF_OPCODES] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};
    int type, i;
    unsigned int temp_num;
    unsigned short int *arr;
    symbols *temp_sym, *h_sym = *head;
    order_word *order_m_word = (order_word *)malloc(sizeof(order_word)); /* order memory word */
    value_word *value = (value_word *)malloc(sizeof(value_word));
    reg_word *reg = (reg_word *)malloc(sizeof(reg_word));
    opcode order_num;

    /* setting the data for switch function */
    for (i = FIRST_CHAR; i < NUM_OF_OPCODES; i++)
    {
        if (strcmp(order, order_code[i]) == EQUAL)
            order_num = i;
    }
    arr = malloc(THREE_MEMORY_WORD * sizeof(short int)); /* this is the max line of order length */
    type = order_type(order, op1, op2);

    switch (type)
    {

        /* case of order 14-15 without operand. will add one word to machine code array */
    case ONE_MEMORY_WORD:
        order_m_word->opcode = order_num;
        order_m_word->ARE = ABSOLUTE;
        arr[FIRST_CELL] = *(unsigned short int *)*(&order_m_word);
        break;

        /* case of order 5-13 with one operand. will add two word to machine code array - one of order and one of operand */
    case TWO_MEMORY_WORDS:
        order_m_word->opcode = order_num;
        order_m_word->dest = address_type(op1);
        order_m_word->ARE = ABSOLUTE;
        arr[FIRST_CELL] = *(unsigned short int *)*(&order_m_word);

        if (is_immediate_add(op1))
        {
            temp_num = num_calc(op1);
            if (temp_num > MAX_MEMORY)
            {
                fprintf(stdout, "Error: number in immediate is not in memory range \n");
                arr[FIRST_CELL] = EOF;
            }
            else
            {
                if (op1[SECOND_CHAR] == '-')
                    temp_num += MAX_MEMORY; /* this will convert negative numbers to to unsigned binary representation */

                value->address = temp_num;
                value->ARE = ABSOLUTE;
                arr[SECOND_CELL] = *(unsigned short int *)*(&value);
            }
        }
        else if (is_direct_add(op1))
        {
            temp_sym = find_symbol(&h_sym, op1);
            if (!temp_sym)
            {
                arr[SECOND_CELL] = EOF;
            }
            else
            {
                if (!is_extern_symbol(temp_sym))
                {
                    value->address = temp_sym->address;
                    value->ARE = RELOCATABLE;
                    arr[SECOND_CELL] = *(unsigned short int *)*(&value);
                }
                else
                {
                    value->ARE = EXTERNAL;
                    arr[SECOND_CELL] = *(unsigned short int *)*(&value);
                }
            }
        }
        else if (is_indirect_reg(op1))
        {
            reg->dest = reg_number(op1);
            reg->ARE = ABSOLUTE;
            arr[SECOND_CELL] = *(unsigned short int *)*(&reg);
        }
        else if (is_direct_reg(op1))
        {
            reg->dest = reg_number(op1);
            reg->ARE = ABSOLUTE;
            arr[SECOND_CELL] = *(unsigned short int *)*(&reg);
        }

        break;

        /* case of order 0-4 with two operand. will add three word to machine code array - one of order and two of operands */
        /* special case: when two operands are direct or indirect registers, will add one word for both operands */
    case THREE_MEMORY_WORD:
        order_m_word->opcode = order_num;
        order_m_word->source = address_type(op1);
        order_m_word->dest = address_type(op2);
        order_m_word->ARE = ABSOLUTE;
        arr[FIRST_CELL] = *(unsigned short int *)*(&order_m_word);

        /* special case: two operands are direct or indirect regis */
        if ((is_indirect_reg(op1) || is_direct_reg(op1)) && (is_indirect_reg(op2) || is_direct_reg(op2)))
        {
            reg->source = reg_number(op1);
            reg->dest = reg_number(op2);
            reg->ARE = ABSOLUTE;
            arr[SECOND_CELL] = *(unsigned short int *)*(&reg);
        }
        else
        {
            /* first operand */
            if (is_immediate_add(op1))
            {
                temp_num = num_calc(op1);
                if (temp_num > MAX_MEMORY)
                {
                    fprintf(stdout, "Error: number in immediate is not in memory range \n");
                    arr[FIRST_CELL] = EOF;
                }
                else
                {
                    if (op1[SECOND_CHAR] == '-')
                        temp_num += MAX_MEMORY; /* this will convert negative numbers to to unsigned binary representation */

                    value->address = temp_num;
                    value->ARE = ABSOLUTE;
                    arr[SECOND_CELL] = *(unsigned short int *)*(&value);
                }
            }
            else if (is_direct_add(op1))
            {
                temp_sym = find_symbol(&h_sym, op1);
                if (!temp_sym)
                {
                    arr[SECOND_CELL] = EOF;
                }
                else
                {
                    if (!is_extern_symbol(temp_sym))
                    {
                        value->address = temp_sym->address;
                        value->ARE = RELOCATABLE;
                        arr[SECOND_CELL] = *(unsigned short int *)*(&value);
                    }
                    else
                    {
                        value->ARE = EXTERNAL;
                        arr[SECOND_CELL] = *(unsigned short int *)*(&value);
                    }
                }
            }
            else if (is_indirect_reg(op1))
            {
                reg->source = reg_number(op1);
                reg->ARE = ABSOLUTE;
                arr[SECOND_CELL] = *(unsigned short int *)*(&reg);
            }
            else if (is_direct_reg(op1))
            {
                reg->source = reg_number(op1);
                reg->ARE = ABSOLUTE;
                arr[SECOND_CELL] = *(unsigned short int *)*(&reg);
            }

            /* second operand */
            if (is_immediate_add(op2))
            {
                temp_num = num_calc(op2);
                if (temp_num > MAX_MEMORY)
                {
                    fprintf(stdout, "Error: number in immediate is not in memory range \n");
                    arr[FIRST_CELL] = EOF;
                }
                else
                {
                    if (op2[SECOND_CHAR] == '-')
                        temp_num += MAX_MEMORY; /* this will convert negative numbers to to unsigned binary representation */

                    value->address = temp_num;
                    value->ARE = ABSOLUTE;
                    arr[THIRD_CELL] = *(unsigned short int *)*(&value);
                }
            }
            else if (is_direct_add(op2))
            {
                temp_sym = find_symbol(&h_sym, op2);
                if (!temp_sym)
                {
                    arr[SECOND_CELL] = EOF;
                }
                else
                {
                    if (!is_extern_symbol(temp_sym))
                    {
                        value->address = temp_sym->address;
                        value->ARE = RELOCATABLE;
                        arr[THIRD_CELL] = *(unsigned short int *)*(&value);
                    }
                    else
                    {
                        value->ARE = EXTERNAL;
                        arr[THIRD_CELL] = *(unsigned short int *)*(&value);
                    }
                }
            }
            else if (is_indirect_reg(op2))
            {
                reg->dest = reg_number(op2);
                reg->ARE = ABSOLUTE;
                arr[THIRD_CELL] = *(unsigned short int *)*(&reg);
            }
            else if (is_direct_reg(op2))
            {
                reg->dest = reg_number(op2);
                reg->ARE = ABSOLUTE;
                arr[THIRD_CELL] = *(unsigned short int *)*(&reg);
            }
        }
        break;

    default:
        break;
    }
    return arr;
}

/* function that creates node of extern for extern linked list */
symbols *create_extern_list(char *name, int address)
{
    symbols *temp;
    temp = (symbols *)malloc(sizeof(symbols));
    temp->address = address;
    strcpy(temp->name, name);
    return temp;
}

/*function that create unsigned representation of number */
unsigned int num_calc(char *name)
{
    unsigned int num;

    if (name[SECOND_CHAR] == '-')
        num = atoi(name + SECOND_CHAR) + MAX_MEMORY;
    else
        num = atoi(name + SECOND_CHAR);
    return num;
}

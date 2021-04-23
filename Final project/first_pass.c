/* This is first pass file. 
    the main function first_pass is using a while loop to go over each line and a switch to determine what 
    to do in it. 
    This pass will first run a test to each line and look for errors in it.
    if no error was found the function will build line after line the symbol and data lists.
    this pass will organized and prepair all information for the second pass and for the rest of the program      */

#include "first_pass.h"

/***************************************************************************************/
/*************************************  first_pass.c  **********************************/
/***************************************************************************************/

/* first pass function */
int *first_pass(FILE *f, symbols **head_of_sym_list, datas **head_of_data_list, int initial_ic, int initial_dc)
{
    char name[MAX_LINE_LENGTH];
    int row_count = FIRST_ROW;
    line line = null;
    boolean error_flag = false;
    symbols *h_of_sym = *head_of_sym_list, *l_of_sym, *temp_h;
    datas *h_of_data = *head_of_data_list, *l_of_data;
    int IC = INITIAL_NUM, DC = INITIAL_NUM, i = FIRST_CHAR;
    int *info_array = (int *)malloc(THREE_CELLS * sizeof(int));

    IC = initial_ic;
    DC = initial_dc;

    /* loop to check every line of code in given file */
    while (fgets(name, MAX_LINE_LENGTH, f) != NULL)
    {
        char temp[MAX_LINE_LENGTH], temp_name[MAX_LINE_LENGTH], temp2[MAX_LINE_LENGTH], temp3[MAX_LINE_LENGTH];
        char *symbol, *word1, *word2, *word3;
        symbols *temp_sym, *p_symbol;
        datas *temp_data, *p_data;

        strcpy(temp3, name);

        strcpy(temp_name, name);
        strcpy(temp, name);
        symbol = strtok(temp_name, " ");

        /* check whether the line is blank or start with ';' and ignore the line if it's true */
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
            /* break down the line to words */
            if (is_symbol(symbol, size(symbol)))
            {
                word1 = strtok(NULL,
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

            /* check the line type */
            line = line_type(temp);

            /* run test of the line */
            if (line_test(temp, word1, line, row_count))
                error_flag = true;

            /* if there's an error in line there will be no need to enter the operations switch */
            if (!error_flag)
            {
                /* switch of operations - according to the line type will decide what to do and how */
                switch (line)
                {
                    /* switch case: .entry - will ignore this line in first pass*/
                case entry:
                    break;

                    /* switch case:  .extern :
                 will add symbol to symbols list after
                 making sure there is no symbol with same name,
                 or call for an error */
                case _extern:

                    if (!h_of_sym)
                    {
                        /* in case the symbols list is empty */
                        p_symbol = (symbols *)malloc(sizeof(symbols));
                        p_symbol->is_extern = true;
                        strcpy(p_symbol->name, word2);
                        h_of_sym = p_symbol;
                        *head_of_sym_list = p_symbol;
                        l_of_sym = p_symbol;
                    }
                    else
                    {
                        /* in case the symbols list is not empty */
                        if (existed_symbol(word2, h_of_sym))
                        {
                            temp_sym = find_symbol(&h_of_sym, word2);
                            if (!(temp_sym->is_extern))
                            {
                                fprintf(stdout, "Line %d: error: symbol \"%s\" is external and declared in this file \n", row_count, word2);
                                error_flag = true;
                            }
                        }
                        else
                        {
                            p_symbol = (symbols *)malloc(sizeof(symbols));
                            p_symbol->is_extern = true;
                            strcpy(p_symbol->name, word2);
                            l_of_sym = get_last_symbol(&h_of_sym);
                            l_of_sym->next = p_symbol;
                            l_of_sym = p_symbol;
                        }
                    }

                    break;

                    /* switch case: .data 
                    after check if there is a symbol in beginning of line
                    will update the data and symbols lists as needed*/
                case data:
                    if (symbol)
                    {
                        if (h_of_sym)
                        {
                            /* check if symbol(without the ':') is existed */
                            strncpy(temp2, symbol, size(symbol));
                            if (existed_symbol(temp2, h_of_sym))
                            {
                                fprintf(stdout, "Line %d: error: symbol \"%s\" is already declared in this file \n", row_count, temp2);
                                error_flag = true;
                            }
                            else
                            {
                                /*** add data to data list ***/
                                temp_data = (datas *)malloc(sizeof(datas));
                                p_data = create_data_string(&temp_data, temp, num_count(temp));
                                strncpy(p_data->name, symbol, size(symbol));
                                p_data->address = DC;
                                p_data->type_data = true;
                                p_data->type_string = false;

                                /*** add symbol to symbols list ***/
                                p_symbol = (symbols *)malloc(sizeof(symbols));
                                p_symbol->is_data = true;
                                strncpy(p_symbol->name, symbol, size(symbol));
                                p_symbol->address = DC;
                                DC = DC + num_count(temp);
                                l_of_sym = get_last_symbol(&h_of_sym);
                                l_of_sym->next = p_symbol;
                                l_of_sym = p_symbol;

                                /*** in case the data list is empty ***/
                                if (!h_of_data)
                                {
                                    h_of_data = p_data;
                                    *head_of_data_list = p_data;
                                    l_of_data = p_data;
                                }
                                else
                                {
                                    l_of_data = get_last_data(&h_of_data);
                                    l_of_data->next = p_data;
                                    l_of_data = p_data;
                                }
                            }
                        }
                        else
                        { /*** in case the list of symbols is empty ***/

                            /*** add data to data list ***/
                            temp_data = (datas *)malloc(sizeof(datas));
                            p_data = create_data_string(&temp_data, temp, num_count(temp));
                            strncpy(p_data->name, symbol, size(symbol));
                            p_data->address = DC;
                            p_data->type_data = true;
                            p_data->type_string = false;

                            /*** add symbol to symbols list ***/
                            p_symbol = (symbols *)malloc(sizeof(symbols));
                            p_symbol->is_data = true;
                            strncpy(p_symbol->name, symbol, size(symbol));
                            p_symbol->address = DC;
                            DC = DC + num_count(temp);
                            h_of_sym = p_symbol;
                            *head_of_sym_list = p_symbol;
                            l_of_sym = p_symbol;

                            /*** in case the data list is empty ***/
                            if (!h_of_data)
                            {
                                h_of_data = p_data;
                                *head_of_data_list = p_data;
                                l_of_data = p_data;
                            }
                            else
                            {
                                l_of_data = get_last_data(&h_of_data);
                                l_of_data->next = p_data;
                                l_of_data = p_data;
                            }
                        }
                    }
                    else
                    {
                        /*** data line without symbol ***/

                        /*** add data to data list ***/
                        temp_data = (datas *)malloc(sizeof(datas));
                        p_data = create_data_string(&temp_data, temp, num_count(temp));
                        p_data->address = DC;
                        p_data->type_data = true;
                        p_data->type_string = false;
                        DC = DC + num_count(temp);

                        /*** in case the data list is empty ***/
                        if (!h_of_data)
                        {
                            h_of_data = p_data;
                            *head_of_data_list = p_data;
                            l_of_data = p_data;
                        }
                        else
                        {
                            l_of_data = get_last_data(&h_of_data);
                            l_of_data->next = p_data;
                            l_of_data = p_data;
                        }
                    }

                    break;

                    /* switch case: .string:
                same as data line . after check if there is a symbol in beginning of line
                    will update the data and symbols lists as needed */
                case string:
                    if (symbol)
                    {
                        if (h_of_sym)
                        {
                            /* check if symbol(without the ':') is existed */
                            strncpy(temp2, symbol, size(symbol));
                            if (existed_symbol(temp2, h_of_sym))
                            {
                                fprintf(stdout, "Line %d: error: symbol \"%s\" is already declared in this file \n", row_count, temp2);
                                error_flag = true;
                            }
                            else
                            {
                                /*** add data to data list ***/
                                temp_data = (datas *)malloc(sizeof(datas));
                                p_data = create_string(&temp_data, temp);
                                strncpy(p_data->name, symbol, size(symbol));
                                p_data->address = DC;
                                p_data->type_string = true;
                                p_data->type_data = false;

                                /*** add symbol to symbols list ***/
                                p_symbol = (symbols *)malloc(sizeof(symbols));
                                p_symbol->is_data = true;
                                strncpy(p_symbol->name, symbol, size(symbol));
                                p_symbol->address = DC;
                                DC = DC + strlen(p_data->str) + END_OF_LINE;
                                l_of_sym = get_last_symbol(&h_of_sym);
                                l_of_sym->next = p_symbol;
                                l_of_sym = p_symbol;

                                /*** in case the data list is empty ***/
                                if (!h_of_data)
                                {
                                    h_of_data = p_data;
                                    *head_of_data_list = p_data;
                                    l_of_data = p_data;
                                }
                                else
                                {
                                    l_of_data = get_last_data(&h_of_data);
                                    l_of_data->next = p_data;
                                    l_of_data = p_data;
                                }
                            }
                        }
                        else
                        { /*** in case the list of symbols is empty ***/

                            /*** add data to data list ***/
                            temp_data = (datas *)malloc(sizeof(datas));
                            p_data = create_string(&temp_data, temp);
                            strncpy(p_data->name, symbol, size(symbol));
                            p_data->address = DC;
                            p_data->type_string = true;
                            p_data->type_data = false;

                            /*** add symbol to symbols list ***/
                            p_symbol = (symbols *)malloc(sizeof(symbols));
                            p_symbol->is_data = true;
                            strncpy(p_symbol->name, symbol, size(symbol));
                            p_symbol->address = DC;
                            DC = DC + strlen(p_data->str) + END_OF_LINE;
                            h_of_sym = p_symbol;
                            *head_of_sym_list = p_symbol;
                            l_of_sym = p_symbol;

                            /*** in case the data list is empty ***/
                            if (!h_of_data)
                            {
                                h_of_data = p_data;
                                *head_of_data_list = p_data;
                                l_of_data = p_data;
                            }
                            else
                            {
                                l_of_data = get_last_data(&h_of_data);
                                l_of_data->next = p_data;
                                l_of_data = p_data;
                            }
                        }
                    }
                    else
                    {
                        /*** line without symbol ***/

                        /*** add data to data list ***/
                        temp_data = (datas *)malloc(sizeof(datas));
                        p_data = create_string(&temp_data, temp);
                        p_data->address = DC;
                        p_data->type_string = true;
                        p_data->type_data = false;
                        DC = DC + strlen(p_data->str) + END_OF_LINE;

                        /*** in case the data list is empty ***/
                        if (!h_of_data)
                        {
                            h_of_data = p_data;
                            *head_of_data_list = p_data;
                            l_of_data = p_data;
                        }
                        else
                        {
                            l_of_data = get_last_data(&h_of_data);
                            l_of_data->next = p_data;
                            l_of_data = p_data;
                        }
                    }
                    break;

                    /* switch case: order 
                check for a symbol in beginning of line and 
                then update the IC accordingly to line */
                case order:
                    if (symbol)
                    {
                        if (h_of_sym)
                        {
                            /* check if symbol(without the ':') is existed */
                            strncpy(temp2, symbol, size(symbol));
                            if (existed_symbol(temp2, h_of_sym))
                            {
                                fprintf(stdout, "Line %d: error: symbol \"%s\" is already declared in this file \n", row_count, temp2);
                                error_flag = true;
                            }
                            else
                            {
                                /*** add symbol to symbols list ***/
                                p_symbol = (symbols *)malloc(sizeof(symbols));
                                strncpy(p_symbol->name, symbol, size(symbol));
                                p_symbol->address = IC;
                                IC += IC_calculator(word1, word2, word3);
                                l_of_sym = get_last_symbol(&h_of_sym);
                                l_of_sym->next = p_symbol;
                                l_of_sym = p_symbol;
                            }
                        }
                        else
                        {
                            /*** in case list of symbols is empty ***/
                            p_symbol = (symbols *)malloc(sizeof(symbols));
                            strncpy(p_symbol->name, symbol, size(symbol));
                            p_symbol->address = IC;
                            IC += IC_calculator(word1, word2, word3);
                            h_of_sym = p_symbol;
                            *head_of_sym_list = p_symbol;
                            l_of_sym = p_symbol;
                        }
                    }
                    else
                    {
                        /*** line without symbol will only calculate and update IC to be added ***/
                        IC += IC_calculator(word1, word2, word3);
                    }
                    break;

                default:
                    break;
                }
            }
            row_count++;
        } /* end of while */
    }
    /*** convert address of symbols with data to IC addresses if no error was found ***/
    if (!error_flag)
    {
        temp_h = h_of_sym;
        while (temp_h)
        {
            if (is_data_symbol(temp_h))
            {
                temp_h->address += IC;
            }
            temp_h = temp_h->next;
        }
        IC = IC - initial_ic;
    }
    /* rewind to beginning of file */
    rewind(f);

    /* update an array of info and return it */
    info_array[FIRST_CELL] = error_flag;
    info_array[SECOND_CELL] = IC;
    info_array[THIRD_CELL] = DC;

    return info_array;
}

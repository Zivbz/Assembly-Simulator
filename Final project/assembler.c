/* This is the main function of the whole program - assembler.c
    in here I recieved files from argument lines and open, run, and close each file
    once with for loop.
    every file is going over first pass, second pass, in if there is no errors, the function will create 
    the machine code, obj, ext,ent files with the same name as original files   */

#include "assembler.h"

int main(int argc, char *argv[])
{
    int i;
    FILE *fp;
    symbols *head_of_sym_list = NULL, *temp_head;
    symbols *head_of_extern_list = NULL;
    datas *head_of_data_list = NULL;
    boolean error_flag = false, existed_entries = false, existed_externals = false;
    char name[FILENAME_MAX];
    unsigned short int *machine_code;
    unsigned short int *order_code_arr;
    unsigned short int *instruction_code_arr;
    int IC, DC, *info;

    if (argc == NO_FILES)
    {
        fprintf(stderr, "There are no input files\n");
        exit(0);
    }
    /* loop to go over each file from command line */
    for (i = FIRST_FILE; i < argc; i++)
    {
        fp = NULL;
        head_of_sym_list = NULL;
        temp_head = NULL;
        head_of_extern_list = NULL;
        head_of_data_list = NULL;
        error_flag = false, existed_entries = false, existed_externals = false;
        machine_code = NULL;
        order_code_arr = NULL;
        instruction_code_arr = NULL;
        info = NULL;

        /* adjust file name */
        strcpy(name, argv[i]);
        strcat(name, ".as");
        if (!(fp = fopen(name, "r")))
        {
            fprintf(stderr, "There was a problem to open the file\n");
            exit(0);
        }

        /* run first pass and return an array of information: [error_flag, IC, DC] */
        info = first_pass(fp, &head_of_sym_list, &head_of_data_list, INITIAL_IC, INITIAL_DC);
        error_flag = info[FIRST_CELL];
        IC = info[SECOND_CELL];
        DC = info[THIRD_CELL];

        /* if there was no error in first pass */
        if (!error_flag)
        {
            /* run second pass and return the array of machine code of orders lines */
            order_code_arr = second_pass(fp, &error_flag, &head_of_sym_list, &head_of_extern_list, INITIAL_IC);

            /* if there was no error in second pass */
            if (!error_flag)
            {
                /* create the array of machine code of instructions line */
                instruction_code_arr = create_instructions_array(&head_of_data_list, DC);

                /* if the memory is overloaded */
                if ((IC + DC) > MAX_MEMORY)
                {
                    fprintf(stdout, "Error: assembler memory is overloaded \n");
                    error_flag = true;
                }
                else
                {
                    /* create the official machine code - comined instructions code array and orders code array */
                    machine_code = create_machine_code(order_code_arr, IC, instruction_code_arr, DC);
                }
                /* check if there are external and entries in file */
                temp_head = head_of_sym_list;
                while (temp_head)
                {
                    if (temp_head->is_entry)
                        existed_entries = true;
                    temp_head = temp_head->next;
                }
                if (head_of_extern_list)
                    existed_externals = true;
            }

            if (!error_flag)
            {
                /* if no errors happened, will create the files obj, ext,ent, as needed */
                create_files(head_of_sym_list, head_of_extern_list, machine_code, existed_entries, existed_externals, IC, DC, argv[i]);
            }
        }
    }
    /* close the current file after finish, this call will delete aloccated memory in use of this program */
    fclose(fp);

    return 0;
}
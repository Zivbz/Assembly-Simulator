/* This is test functions file. 
    the main function line_test is using diffrent functions from this file to spot an error of syntax or any kind
    in a given line of source file.      */


#include "test_functions.h"

/* funcion that run error tests on given line and return whether there was an error or not */
boolean line_test(char *name, char *temp, line type, int count)
{
    boolean error_flag = false;

    if (syntax_error_check(temp))
    {
        fprintf(stdout, "Line %d: error: wrong syntax. no meaning to line\n", count);
        error_flag = true;
    }
    if (!error_flag && general_test(name, type, count))
    {
        error_flag = true;
    }
    
    if(!error_flag && commas_check(name, type, count)){
        error_flag = true;
    }

    if(!error_flag && type == order && misfit_operands(name, count)){
        error_flag = true;
    }
    return error_flag;
}

/* function that check for missing or extra commas in line */
boolean commas_check(char *name, line type, int count)
{
    char temp1[MAX_LINE_LENGTH], temp2[MAX_LINE_LENGTH];
    char *symbol, *word;
    int i;
    boolean error_flag = false;
    boolean first_comma = false, second_comma = false;
    boolean space_flag = false;
    boolean plus_minos_flag = false;
    boolean first_number = false;
    boolean first_word = false;

    /* breakdown the line */
    strcpy(temp1, name);
    strcpy(temp2, name);
    symbol = strtok(temp1, " ");
    if (is_symbol(symbol, size(symbol)))
    {
        word = strtok(NULL, " " "\t" "\n");
        if(!(strcmp(word, "stop") == EQUAL) && !(strcmp(word, "rts") == EQUAL))
            word = strtok(NULL, "");
    }
    else
    {
        symbol = NULL;
        word = strtok(temp2, " " "\t" "\n");
        if(!(strcmp(word, "stop") == EQUAL) && !(strcmp(word, "rts") == EQUAL))
            word = strtok(NULL, "");
    }

    /* a switch case to sort the line type and work accordingly */
    switch (type)
    {
        /* case: entry:
            test all kind of logic errors in line, char by char */
    case entry:

            if(isalpha(word[FIRST_CHAR]) && word){
                for (i = SECOND_CHAR; i < strlen(word); i++)
                {
                    /* in case a char is not a number or a letter */
                    if (!isalnum(word[i])) 
                    {
                        if(isspace(word[i]))
                            space_flag = true;
                        else if(word[i] == ','){
                            if(first_comma)
                                second_comma = true;
                            else 
                                first_comma = true;
                        }
                    }
                    else{
                        if(space_flag && !first_comma){
                            error_flag = true;
                            fprintf(stdout, "Line %d: error: missing comma between words\n", count);
                            break;
                        }
                        else{
                            space_flag = false;
                            first_comma = false;
                        }
                    }
                    if (first_comma && second_comma){
                        error_flag = true;
                        fprintf(stdout, "Line %d: error: too many commas between operands\n", count);
                        break;
                    }
                }
            }

            /* in case there was a comma char after last word */
            if(first_comma){
                error_flag = true; 
                fprintf(stdout, "Line %d: error: unrecognized char after last word\n", count);
            }

            break;


        /* case: extern:
            test all kind of logic errors in line, char by char */
        case _extern:
            if(isalpha(word[FIRST_CHAR]) && word){
                for (i = SECOND_CHAR; i < strlen(word); i++)
                {
                    /* in case a char is not a number or a letter */
                    if (!isalnum(word[i])) 
                    {
                        if(isspace(word[i]))
                            space_flag = true;
                        else if(word[i] == ','){
                            if(first_comma)
                                second_comma = true;
                            else 
                                first_comma = true;
                        }
                    }
                    else{
                        if(space_flag && !first_comma){
                            error_flag = true;
                            fprintf(stdout, "Line %d: error: missing comma between words\n", count);
                            break;
                        }
                        else{
                            space_flag = false;
                            first_comma = false;
                        }
                    }
                    if (first_comma && second_comma){
                        error_flag = true;
                        fprintf(stdout, "Line %d: error: too many commas between operands\n", count);
                        break;
                    }
                }
            }

            /* in case there was a comma char after last word */
            if(first_comma){
                error_flag = true; 
                fprintf(stdout, "Line %d: error: unrecognized char after last word\n", count);
            }
            break;

        /* case: .data:
            test all kind of logic errors in line, char by char.
            here I also took in consideration the '-', '+'. */
        case data:
            if(isdigit(word[FIRST_CHAR]) || word[FIRST_CHAR] == '-' || word[FIRST_CHAR] == '+' || word[FIRST_CHAR] == ' '){
                for (i = SECOND_CHAR; i < strlen(word); i++)
                {   
                    /* in case a char is not a number */
                    if (!isdigit(word[i])) 
                    {   
                        if((word[i] == '+') || (word[i] == '-')){
                            plus_minos_flag = true;
                            first_number = true;
                        }
                        else if(isspace(word[i])){
                            if(plus_minos_flag){
                                error_flag = true;
                                fprintf(stdout, "Line %d: error: wrong syntax. sign without a number\n", count);
                            }
                            space_flag = true;
                        }
                        else if(word[i] == ','){
                            if(plus_minos_flag){
                                error_flag = true;
                                fprintf(stdout, "Line %d: error: wrong syntax. comma between sign and number\n", count);
                            }
                            if(first_comma)
                                second_comma = true;
                            else 
                                first_comma = true;
                        }
                    }
                    else{
                        if(space_flag && !first_comma && first_number){
                            error_flag = true;
                            fprintf(stdout, "Line %d: error: missing comma between words\n", count);
                            break;
                        }
                        else{
                            space_flag = false;
                            first_comma = false;
                            plus_minos_flag = false;
                            first_number = true;
                        }
                    }
                    if (first_comma && second_comma){
                        error_flag = true;
                        fprintf(stdout, "Line %d: error: too many commas between operands\n", count);
                        first_comma = false;
                        break;
                    }
                }
            }
            else if(word[FIRST_CHAR] != ' '){
                error_flag = true; 
                fprintf(stdout, "Line %d: error: comma before number\n", count);               
            }

            /* in case there was a comma char after last word */
            if(first_comma){
                error_flag = true; 
                fprintf(stdout, "Line %d: error: unrecognized char after last word\n", count);
            } 
            break;


        /* case: string:
            the string lines have another specific function for them */
        case string:
            
            break;

        /* case: order:
            test all kind of logic errors in line, char by char.
            taking care of all the kind of addresses, orders and registers */
        case order:
            if(isalpha(word[FIRST_CHAR]) || word[FIRST_CHAR] == '*' || word[FIRST_CHAR] == '#' || word[FIRST_CHAR] == ' '){
                for (i = SECOND_CHAR; i < strlen(word); i++)
                {   
                    /* in case a char is not a number */
                    if (!isalnum(word[i])) 
                    {   
                        if(isspace(word[i])){
                            space_flag = true;
                        }
                        else if(word[i] == ','){
                            if(first_comma)
                                second_comma = true;
                            else 
                                first_comma = true;
                        }
                    }
                    else{
                        if(space_flag && !first_comma && first_word){
                            error_flag = true;
                            fprintf(stdout, "Line %d: error: missing comma between words\n", count);
                            break;
                        }
                        else{
                            space_flag = false;
                            first_comma = false;
                            first_word = true;
                        }
                    }
                    if (first_comma && second_comma){
                        error_flag = true;
                        fprintf(stdout, "Line %d: error: too many commas between operands\n", count);
                        first_comma = false;
                        break;
                    }
                }
            }
            else if(word != NULL){
                error_flag = true; 
                fprintf(stdout, "Line %d: error: comma before number\n", count);               
            }

            /* in case there was a comma char after last word */
            if(first_comma){
                error_flag = true; 
                fprintf(stdout, "Line %d: error: unrecognized char after last word\n", count);
            } 
            break;


        default:
            break;
    }

    return error_flag;
}


/* function that checks if the operand of order line misfits (according to page 30) */
boolean misfit_operands(char *name, int count){
    char *order_code[NUM_OF_OPCODES] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};
    char temp1[MAX_LINE_LENGTH], temp2[MAX_LINE_LENGTH];
    char *symbol, *word1, *word2, *word3;
    int i;
    boolean error_flag = false;
    opcode order_num;

    /* breakdown the line */
    strcpy(temp1, name);
    strcpy(temp2, name);
    symbol = strtok(temp1, " ");
    if (is_symbol(symbol, size(symbol)))
    {
        word1 = strtok(NULL, " " "\t" "\n");
        word2 = strtok(NULL, " " "," "\t" "\n");
        word3 = strtok(NULL, " " "," "\t" "\n");
    }
    else
    {
        symbol = NULL;
        word1 = strtok(temp2, " " "\t" "\n");
        word2 = strtok(NULL, " " "," "\t" "\n");
        word3 = strtok(NULL, " " "," "\t" "\n");
    }


    /* setting the data for switch function */
    for (i = FIRST_CHAR; i < NUM_OF_OPCODES; i++)
    {
        if (strcmp(word1, order_code[i]) == EQUAL)
            order_num = i;
    }

    /* switch that fit correct operands to order or call an error.
    for every order, the addresses writen are the ones that it is legal to use */
    switch (order_num)
    {
    case mov:
        if(!(is_immediate_add(word2) || is_direct_add(word2) || is_direct_reg(word2) || is_indirect_reg(word2))){
            error_flag = true; 
            fprintf(stdout, "Line %d: error: illegal address word in source operand\n", count);
        }
        if(!(is_direct_add(word3) || is_direct_reg(word3) || is_indirect_reg(word3))){
            error_flag = true; 
            fprintf(stdout, "Line %d: error: illegal address word in destination operand\n", count);
        }
        break;

    case cmp:
        if(!(is_immediate_add(word2) || is_direct_add(word2) || is_direct_reg(word2) || is_indirect_reg(word2))){
            error_flag = true; 
            fprintf(stdout, "Line %d: error: illegal address word in source operand\n", count);
        }
        if(!(is_immediate_add(word3) || is_direct_add(word3) || is_direct_reg(word3) || is_indirect_reg(word3))){
            error_flag = true; 
            fprintf(stdout, "Line %d: error: illegal address word in destination operand\n", count);
        }
        break;

    case add:
        if(!(is_immediate_add(word2) || is_direct_add(word2) || is_direct_reg(word2) || is_indirect_reg(word2))){
            error_flag = true; 
            fprintf(stdout, "Line %d: error: illegal address word in source operand\n", count);
        }
        if(!(is_direct_add(word3) || is_direct_reg(word3) || is_indirect_reg(word3))){
            error_flag = true; 
            fprintf(stdout, "Line %d: error: illegal address word in destination operand\n", count);
        }
        break;

    case sub:
        if(!(is_immediate_add(word2) || is_direct_add(word2) || is_direct_reg(word2) || is_indirect_reg(word2))){
            error_flag = true; 
            fprintf(stdout, "Line %d: error: illegal address word in source operand\n", count);
        }
        if(!(is_direct_add(word3) || is_direct_reg(word3) || is_indirect_reg(word3))){
            error_flag = true; 
            fprintf(stdout, "Line %d: error: illegal address word in destination operand\n", count);
        }
        break;

    case lea:
        if(!(is_direct_add(word2))){
            error_flag = true; 
            fprintf(stdout, "Line %d: error: illegal address word in source operand\n", count);
        }
        if(!(is_direct_add(word3) || is_direct_reg(word3) || is_indirect_reg(word3))){
            error_flag = true; 
            fprintf(stdout, "Line %d: error: illegal address word in destination operand\n", count);
        }
        break;

    case clr:
        if(!(is_direct_add(word2) || is_direct_reg(word2) || is_indirect_reg(word2))){
            error_flag = true; 
            fprintf(stdout, "Line %d: error: illegal address word in destination operand\n", count);
        }
        break;

    case not:
        if(!(is_direct_add(word2) || is_direct_reg(word2) || is_indirect_reg(word2))){
            error_flag = true; 
            fprintf(stdout, "Line %d: error: illegal address word in destination operand\n", count);
        }
        break;

    case inc:
        if(!(is_direct_add(word2) || is_direct_reg(word2) || is_indirect_reg(word2))){
            error_flag = true; 
            fprintf(stdout, "Line %d: error: illegal address word in destination operand\n", count);
        }
        break;

    case dec:
        if(!(is_direct_add(word2) || is_direct_reg(word2) || is_indirect_reg(word2))){
            error_flag = true; 
            fprintf(stdout, "Line %d: error: illegal address word in destination operand\n", count);
        }
        break;

    case jmp:
        if(!(is_direct_add(word2) || is_indirect_reg(word2))){
            error_flag = true; 
            fprintf(stdout, "Line %d: error: illegal address word in destination operand\n", count);
        }
        break;

    case bne:
        if(!(is_direct_add(word2) || is_indirect_reg(word2))){
            error_flag = true; 
            fprintf(stdout, "Line %d: error: illegal address word in destination operand\n", count);
        }
        break;

    case red:
        if(!(is_direct_add(word2) || is_direct_reg(word2) || is_indirect_reg(word2))){
            error_flag = true; 
            fprintf(stdout, "Line %d: error: illegal address word in destination operand\n", count);
        }
        break;

    case prn:
        if(!(is_immediate_add(word2) || is_direct_add(word2) || is_direct_reg(word2) || is_indirect_reg(word2))){
            error_flag = true; 
            fprintf(stdout, "Line %d: error: illegal address word in destination operand\n", count);
        }
        break;

    case jsr:
        if(!(is_direct_add(word2) || is_indirect_reg(word2))){
            error_flag = true; 
            fprintf(stdout, "Line %d: error: illegal address word in destination operand\n", count);
        }
        break;

    case rts:
        break;

    case stop:
        break;
    default:
        break;
    }

    return error_flag;
}


/*  function that checks the first word of operations to have meaning  */
boolean syntax_error_check(char *name)
{
    if (is_order(name, size(name)) || is_instruction(name, size(name)))
        return false;
    return true;
}

/*  function that checks wheter the number of operands fits the order  */
boolean operands_num_check(char *word1, char *word2, char *word3)
{
    int i;
    char *order_code[NUM_OF_OPCODES] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};

    for (i = FIRST_CHAR; i < TWO_OPERAND_GROUP; i++)
    {
        if (strcmp(word1, order_code[i]) == EQUAL)
        {
            if (word2 && word3)
            {
                word3 = strtok(NULL, " "
                                     "\n"
                                     "\t");
                if (word3)
                    return false;
                return true;
            }
        }
    }
    for (; i < ONE_OPERAND_GROUP; i++)
    {
        if (strcmp(word1, order_code[i]) == EQUAL)
        {
            if (word2 && !word3)
                return true;
            return false;
        }
    }
    for (; i < NO_OPERAND_GROUP; i++)
    {
        if (strcmp(word1, order_code[i]) == EQUAL)
        {
            if (word2)
                return false;
            return true;
        }
    }
    return false;
}

/* funcion that run general test on a given line */
boolean general_test(char *name, line type, int count)
{
    char temp1[MAX_LINE_LENGTH], temp2[MAX_LINE_LENGTH], temp3[MAX_LINE_LENGTH], temp_name[MAX_LINE_LENGTH];
    char *symbol, *word1, *word2, *word3, *temp_sym;

    /*breakdown the line */
    strcpy(temp_name, name);
    strcpy(temp1, name);
    strcpy(temp2, name);
    symbol = strtok(temp1, " ");
    /* if symbol is existed we will use it without the ":" in the line */
    if(symbol){
        strcpy(temp3, name);
        temp_sym = strtok(temp3, " " ":");
    }
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
        word1 = strtok(temp2, ","
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



    switch (type)
    {

        /* switch case: .entry 
            check for general syntax check in entry line. */
    case entry:
        if (symbol){
            fprintf(stdout, "Line %d: warning: label before extern/entry\n", count);
        }
        
        if (is_ent_ext_operand(word2) && (!word3))
        {
            return false;
        }
        else if (is_ent_ext_operand(word2) && word3)
        {   /* check if there is more than one symbol */
            while (word3)
            {
                if (is_ent_ext_operand(word3))
                {
                    fprintf(stdout, "Line %d: warning: too many opernads after instruction - the computer will take only the first one\n", count);
                    return false;
                    word3 = strtok(NULL, ","
                                         " "
                                         "\n"
                                         "\t");
                }
                else
                {
                    fprintf(stdout, "Line %d: error: wrong syntax in instruction line - illegal symbol\n", count);
                    return true;
                    word3 = NULL;
                }
            }
        }
        else
        {   /* in case the first word is not a symbol */
            fprintf(stdout, "Line %d: error: wrong syntax in instruction line - illegal symbol\n", count);
            return true;
        }

        break;

        /* switch case: .extern 
            check for general syntax check in extern line. */
    case _extern:
        if (symbol)
            fprintf(stdout, "Line %d: warning: label before extern/entry\n", count);
        if (is_ent_ext_operand(word2) && (!word3))
        {
            return false;
        }
        else if (is_ent_ext_operand(word2) && word3)
        {   /* check if there is more than one symbol */
            while (word3)
            {
                if (is_ent_ext_operand(word3))
                {
                    fprintf(stdout, "Line %d: warning: too many opernads after instruction - the computer will take only the first one\n", count);
                    return false;
                    word3 = strtok(NULL, ","
                                         " "
                                         "\n"
                                         "\t");
                }
                else
                {
                    fprintf(stdout, "Line %d: error: wrong syntax in instruction line - illegal symbol\n", count);
                    return true;
                    word3 = NULL;
                }
            }
        }
        else
        {   /* in case the first word is not a symbol */
            fprintf(stdout, "Line %d: error: wrong syntax in instruction line - illegal symbol\n", count);
            return true;
        }

        break;

        /* switch case: .data 
        wiil check if a symbol has an order or register name
        and other general logic tests */
    case data:
        if(symbol){
            if(is_order(temp_sym,size(temp_sym))){
                fprintf(stdout, "Line %d: error: symbol with an order name\n", count);
                return true;
            }
            else if(is_direct_reg(temp_sym)){
                fprintf(stdout, "Line %d: error: symbol with a register name\n", count);
                return true;       
            }
        }
        if (!word2){
            fprintf(stdout, "Line %d: error: lack of data\n", count);
            return true;
        }
        else if (!is_number(word2))
        {
            fprintf(stdout, "Line %d: error: \".data\" line should have only verified numbers\n", count);
            return true;
        }
        else if (word3)
        {
            while (word3)
            {
                if (!is_number(word3))
                {
                    fprintf(stdout, "Line %d: error: \".data\" line should have only verified numbers\n", count);
                    return true;
                }
                word3 = strtok(NULL, ","
                                     " "
                                     "\n"
                                     "\t");
            }
        }

        break;

        /* switch case: .string 
        wiil check if a symbol has an order or register name
        and other general logic tests */
    case string:
        if(symbol){
            if(is_order(temp_sym,size(temp_sym))){
                fprintf(stdout, "Line %d: error: symbol with an order name\n", count);
                return true;
            }
            else if(is_direct_reg(temp_sym)){
                fprintf(stdout, "Line %d: error: symbol with a register name\n", count);
                return true;       
            }
        } /* function from basic_functions that verifeing strings */
        if (is_string_name(temp_name))
            return false;
        else
        {
            fprintf(stdout, "Line %d: error: wrong syntax in \".string\" instruction line\n", count);
            return true;
        }
        break;

        /* switch case: order 
        standared checks and run the specific function operands_num_check*/
    case order:
        if(symbol){
            if(is_order(temp_sym,size(temp_sym))){
                fprintf(stdout, "Line %d: error: symbol with an order name\n", count);
                return true;
            }
            else if(is_direct_reg(temp_sym)){
                fprintf(stdout, "Line %d: error: symbol with a register name\n", count);
                return true;       
            }
        }
        if (!operands_num_check(word1, word2, word3))
        {
            fprintf(stdout, "Line %d: error: the number of operands don't match the order\n", count);
            return true;
        }
        break;

    default:
        break;
    }
    return false;
}

/* funcion that checks what type is the line */
line line_type(char *name)
{
    char temp1[MAX_LINE_LENGTH], temp2[MAX_LINE_LENGTH];
    char *symbol, *word1;
    line line_type;

    strcpy(temp1, name);
    strcpy(temp2, name);
    symbol = strtok(temp1, " ");
    if (is_symbol(symbol, size(symbol)))
    {
        word1 = strtok(NULL, ","
                             " "
                             "\n"
                             "\t");
    }
    else
    {
        symbol = NULL;
        word1 = strtok(temp2, ","
                              " "
                              "\n"
                              "\t");
    }

    if (is_entry(word1))
        line_type = entry;
    else if (is_extern(word1))
        line_type = _extern;
    else if (is_data(word1))
        line_type = data;
    else if (is_string(word1))
        line_type = string;
    else if (is_order(word1, size(word1)))
        line_type = order;
    else
        line_type = null;
    return line_type;
}
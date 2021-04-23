assembler: assembler.o basic_functions.o first_pass.o second_pass.o test_functions.o
	gcc -g -Wall -ansi -pedantic assembler.o basic_functions.o first_pass.o second_pass.o test_functions.o -o assembler

assembler.o:	assembler.c assembler.h 
	gcc -c -Wall -ansi -pedantic assembler.c -o assembler.o

basic_functions.o:	basic_functions.c basic_functions.h
	gcc -c -Wall -ansi -pedantic basic_functions.c -o basic_functions.o

first_pass.o:	first_pass.c first_pass.h 
	gcc -c -Wall -ansi -pedantic first_pass.c -o first_pass.o

second_pass.o:	second_pass.c second_pass.h 
	gcc -c -Wall -ansi -pedantic second_pass.c -o second_pass.o

test_functions.o:	test_functions.c test_functions.h
	gcc -c -Wall -ansi -pedantic test_functions.c -o test_functions.o
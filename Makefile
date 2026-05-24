all : bibliocy

main.o : main.c
	gcc -c main.c -o main.o

library.o : library.c library.h
	gcc -c library.c -o library.o

book.o : book.c book.h
	gcc -c book.c -o book.o

user.o : user.c user.h
	gcc -c user.c -o user.o

file_io.o : file_io.c file_io.h
	gcc -c file_io.c -o file_io.o

input_helper.o : input_helper.c input_helper.h
	gcc -c input_helper.c -o input_helper.o

bibliocy : main.o library.o book.o user.o file_io.o input_helper.o
	gcc main.o library.o book.o user.o file_io.o input_helper.o -o bibliocy

clean :
	rm -f *.o bibliocy

all:
	gcc main.c library.c book.c user.c -o bibliocy

clean:
	rm -f bibliocy

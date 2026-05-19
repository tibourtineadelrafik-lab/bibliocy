#include "book.h"

typedef struct {
    Book *books;   /* tableau dynamique */
    int nbBooks;   /* nb de livres utilises */
    int capacity;  /* taille actuelle du tableau */
} Library;

void initLibrary(Library *lib);
void freeLibrary(Library *lib);

int addBookToLibrary(Library *lib, Book *b);
int createBookAndAdd(Library *lib, int id, char *title, char *author, char *category);

Book *getBookById(Library *lib, int id);
int removeBookById(Library *lib, int id);

void displayLibrary(Library *lib);

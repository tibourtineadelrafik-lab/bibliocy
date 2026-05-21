#include "book.h"

typedef struct {
    Book *books;   /* tableau dynamique */
    int nbBooks;   /* nb de livres utilises */
    int capacity;  /* taille actuelle du tableau */
} Library;

/* Initialise la bibliotheque (tableau vide). */
void initLibrary(Library *lib);
/* Libere la memoire allouee a la bibliotheque. */
void freeLibrary(Library *lib);

/* Ajoute un livre existant dans la bibliotheque. */
int addBookToLibrary(Library *lib, Book *b);
/* Cree un livre puis l'ajoute dans la bibliotheque. */
int createBookAndAdd(Library *lib, int id, char *title, char *author, char *category);

/* Retourne le livre correspondant a un id, sinon NULL. */
Book *getBookById(Library *lib, int id);
/* Supprime un livre par id. */
int removeBookById(Library *lib, int id);

/* Affiche toute la bibliotheque. */
void displayLibrary(Library *lib);

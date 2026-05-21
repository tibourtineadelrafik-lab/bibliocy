#ifndef BOOK_H
#define BOOK_H

#define MAX_TITLE    100
#define MAX_AUTHOR   100
#define MAX_CATEGORY  50
#define MAX_LOGIN     50

typedef struct {
    int  id;                      /* identifiant unique */
    char title[MAX_TITLE];        /* titre du livre */
    char author[MAX_AUTHOR];      /* auteur */
    char category[MAX_CATEGORY];  /* categorie du livre */
    int  available;               /* 1 si disponible, 0 sinon */
    char borrower[MAX_LOGIN];     /* login de l'emprunteur */
} Book;

/* Initialise un livre avec son id, titre, auteur et categorie. */
void initBook(Book *b, int id, char *title, char *author, char *category);
/* Affiche tous les details d'un livre. */
void displayBook(Book *b);
/* Affiche uniquement les livres disponibles. */
void displayAvailableBooks(Book *books, int nbBooks);
/* Retourne l'indice du livre par titre, -1 si non trouve. */
int searchByTitle(Book *books, int nbBooks, char *title);
/* Retourne l'indice du livre par auteur, -1 si non trouve. */
int searchByAuthor(Book *books, int nbBooks, char *author);
/* Affiche tous les livres d'une categorie. */
void searchByCategory(Book *books, int nbBooks, char *category);
/* Trie le tableau de livres par titre (ordre alphabetique). */
void sortByTitle(Book *books, int nbBooks);
/* Trie le tableau de livres par auteur (ordre alphabetique). */
void sortByAuthor(Book *books, int nbBooks);

#endif

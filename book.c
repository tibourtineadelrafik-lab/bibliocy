#include <stdio.h>
#include <string.h>
#include "book.h"

void initBook(Book*b, int id, char *title, char *author, char *category) {
     b->id          = id;
     b->available   = 1;
     b->borrower[0] = '\0';
     strcpy(b->title,    title);
     strcpy(b->author,   author);
     strcpy(b->category, category);
}

void displayBook(Book *b) {
    if (b == NULL) {
        printf("Erreur : livre invalide.\n");
        return;
    }
    printf("  [%d] %s\n",        b->id, b->title);
    printf("  Auteur   : %s\n",  b->author);
    printf("  Categorie: %s\n",  b->category);
     if (b->available == 1) {
        printf("  Statut   : Disponible\n");
    }else {
        printf("  Statut   : Emprunte par %s\n", b->borrower);
    }
}

void displayAvailableBooks(Book *books, int nbBooks) {
    int found = 0;
    int i;
    printf("========== Livres disponibles ==========\n");
    for (i = 0; i < nbBooks; i++) {
        if (books[i].available == 1) {
    printf("  [%d] %s\n", books[i].id, books[i].title);
    printf("  Auteur   : %s\n", books[i].author);
    found = 1;
}
    }
    if (found == 0) {
        printf("  Aucun livre disponible pour le moment.\n");
    }
    
int searchByTitle(Book *books, int nbBooks, char *title);
    int i;
    for (i = 0; i < nbBooks; i++) {
        if (strcmp(books[i].title, title) == 0) {
            return i;
            }
    }
    return -1;
}
int searchByAuthor(Book *books, int nbBooks, char *author) {
    int i;
    for (i = 0; i < nbBooks; i++) {
        if (strcmp(books[i].author, author) == 0) {
            return i;
        }
    }
    return -1;
}
void searchByCategory(Book *books, int nbBooks, char *category) {
    int found = 0;
    int i;
    printf("=== Livres de la categorie : %s ===\n", category);
    for (i = 0; i < nbBooks; i++) {
        if (strcmp(books[i].category, category) == 0) {
            displayBook(&books[i]);
            found = 1;
        }
    }
    if (found == 0) {
        printf("  Aucun livre trouve dans cette categorie.\n");
    }
}
void sortByTitle(Book *books, int nbBooks) {
    Book temp;
    int i, j, indiceMin;
    for (i = 0; i < nbBooks - 1; i++) {
        indiceMin = i;
        for (j = i + 1; j < nbBooks; j++) {
            if (strcmp(books[j].title, books[indiceMin].title) < 0) {
                indiceMin = j;
            }
        }
        if (indiceMin != i) {
            temp             = books[i];
            books[i]         = books[indiceMin];
            books[indiceMin] = temp;
        }
    }
}
 
void sortByAuthor(Book *books, int nbBooks) {
    Book temp;
    int i, j, indiceMin;
    for (i = 0; i < nbBooks - 1; i++) {
        indiceMin = i;
        for (j = i + 1; j < nbBooks; j++) {
            if (strcmp(books[j].author, books[indiceMin].author) < 0) {
                indiceMin = j;
            }
        }
        if (indiceMin != i) {
            temp             = books[i];
            books[i]         = books[indiceMin];
            books[indiceMin] = temp;
        }
    }
}

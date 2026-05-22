#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "book.h"

/* Copie un texte en minuscules dans dst. */
static void toLowerCopy(const char *src, char *dst, int maxSize) {
    int i = 0;

    if (src == NULL || dst == NULL || maxSize <= 0) {
        return;
    }

    while (src[i] != '\0' && i < maxSize - 1) {
        dst[i] = (char)tolower((unsigned char)src[i]);
        i++;
    }
    dst[i] = '\0';
}

void initBook(Book *b, int id, char *title, char *author, char *category) {
    b->id = id;
    b->available = 1;
    b->borrower[0] = '\0';

    strcpy(b->title, title);
    strcpy(b->author, author);
    strcpy(b->category, category);
}

void displayBook(Book *b) {
    if (b == NULL) {
        printf("Erreur : livre invalide.\n");
        return;
    }

    printf("  [%d] %s\n", b->id, b->title);
    printf("  Auteur   : %s\n", b->author);
    printf("  Categorie: %s\n", b->category);

    if (b->available == 1) {
        printf("  Statut   : Disponible\n");
    } else {
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
}

int searchByTitle(Book *books, int nbBooks, char *title) {
    int i;
    char searchText[MAX_TITLE];
    char currentTitle[MAX_TITLE];

    if (books == NULL || title == NULL || title[0] == '\0') {
        return -1;
    }

    /* Recherche souple: ignore majuscules/minuscules et accepte une partie du titre. */
    toLowerCopy(title, searchText, MAX_TITLE);

    for (i = 0; i < nbBooks; i++) {
        toLowerCopy(books[i].title, currentTitle, MAX_TITLE);
        if (strstr(currentTitle, searchText) != NULL) {
            return i;
        }
    }

    return -1;
}

int searchByAuthor(Book *books, int nbBooks, char *author) {
    int i;
    char searchText[MAX_AUTHOR];
    char currentAuthor[MAX_AUTHOR];

    if (books == NULL || author == NULL || author[0] == '\0') {
        return -1;
    }

    /* Meme logique que pour le titre: recherche souple et sans casse stricte. */
    toLowerCopy(author, searchText, MAX_AUTHOR);

    for (i = 0; i < nbBooks; i++) {
        toLowerCopy(books[i].author, currentAuthor, MAX_AUTHOR);
        if (strstr(currentAuthor, searchText) != NULL) {
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
    int i;
    int j;
    int indiceMin;

    /* Tri tres simple par selection sur le titre. */
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
    int i;
    int j;
    int indiceMin;

    /* Tri tres simple par selection sur l'auteur. */
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

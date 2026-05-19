#include <stdio.h>
#include <stdlib.h>

#include "library.h"

void initLibrary(Library *lib) {
    if (lib == NULL) {
        return;
    }

    lib->nbBooks = 0;
    lib->capacity = 5; /* on commence avec 5 places */
    lib->books = malloc(lib->capacity * sizeof(Book));

    if (lib->books == NULL) {
        lib->capacity = 0;
    }
}

void freeLibrary(Library *lib) {
    if (lib == NULL) {
        return;
    }

    free(lib->books);
    lib->books = NULL;
    lib->nbBooks = 0;
    lib->capacity = 0;
}

Book *getBookById(Library *lib, int id) {
    int i;

    if (lib == NULL || lib->books == NULL) {
        return NULL;
    }

    for (i = 0; i < lib->nbBooks; i++) {
        if (lib->books[i].id == id) {
            return &lib->books[i];
        }
    }

    return NULL;
}

int addBookToLibrary(Library *lib, Book *b) {
    Book *newArray;

    if (lib == NULL || b == NULL || lib->books == NULL) {
        return 0;
    }

    if (getBookById(lib, b->id) != NULL) {
        return 0; /* id deja utilise */
    }

    /* Si le tableau est plein, on ajoute 5 places. */
    if (lib->nbBooks >= lib->capacity) {
        newArray = realloc(lib->books, (lib->capacity + 5) * sizeof(Book));
        if (newArray == NULL) {
            return 0;
        }
        lib->books = newArray;
        lib->capacity = lib->capacity + 5;
    }

    lib->books[lib->nbBooks] = *b;
    lib->nbBooks++;
    return 1;
}

int createBookAndAdd(Library *lib, int id, char *title, char *author, char *category) {
    Book b;

    if (id <= 0 || title == NULL || author == NULL || category == NULL) {
        return 0;
    }

    initBook(&b, id, title, author, category);
    return addBookToLibrary(lib, &b);
}

int removeBookById(Library *lib, int id) {
    int i;
    int index = -1;

    if (lib == NULL || lib->books == NULL) {
        return 0;
    }

    for (i = 0; i < lib->nbBooks; i++) {
        if (lib->books[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        return 0;
    }

    /* On decale les cases a gauche. */
    for (i = index; i < lib->nbBooks - 1; i++) {
        lib->books[i] = lib->books[i + 1];
    }

    lib->nbBooks--;
    return 1;
}

void displayLibrary(Library *lib) {
    int i;

    if (lib == NULL || lib->books == NULL) {
        printf("Erreur: bibliotheque non initialisee.\n");
        return;
    }

    if (lib->nbBooks == 0) {
        printf("Aucun livre dans la bibliotheque.\n");
        return;
    }

    printf("=== Bibliotheque (%d livre(s)) ===\n", lib->nbBooks);
    for (i = 0; i < lib->nbBooks; i++) {
        displayBook(&lib->books[i]);
        printf("\n");
    }
}

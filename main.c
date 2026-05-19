#include <stdio.h>
#include <string.h>

#include "library.h"

/* Vide ce qui reste dans l'entree clavier. */
static void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* rien */
    }
}

/* Lit un entier au clavier, et redemande tant que ce n'est pas un nombre. */
static int readInt(const char *prompt) {
    int value;
    int ok;

    while (1) {
        printf("%s", prompt);
        ok = scanf("%d", &value);
        clearInputBuffer();

        if (ok == 1) {
            return value;
        }

        printf("Entrez un nombre valide.\n");
    }
}

/* Lit un texte simple (une ligne) au clavier. */
static void readText(const char *prompt, char *buffer, int size) {
    printf("%s", prompt);
    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    buffer[strcspn(buffer, "\n")] = '\0';
}

/* Affiche un menu tres simple. */
static void printMenu(void) {
    printf("\n===== MENU =====\n");
    printf("1. Afficher les livres\n");
    printf("2. Ajouter un livre\n");
    printf("3. Supprimer un livre\n");
    printf("0. Quitter\n");
}

int main(void) {
    Library lib;
    int running = 1;

    initLibrary(&lib);
    if (lib.books == NULL) {
        printf("Erreur memoire.\n");
        return 1;
    }

    while (running) {
        int choice;

        printMenu();
        choice = readInt("Votre choix: ");

        if (choice == 1) {
            displayLibrary(&lib);
        } else if (choice == 2) {
            int id;
            char title[MAX_TITLE];
            char author[MAX_AUTHOR];
            char category[MAX_CATEGORY];

            id = readInt("ID du livre: ");
            readText("Titre: ", title, MAX_TITLE);
            readText("Auteur: ", author, MAX_AUTHOR);
            readText("Categorie: ", category, MAX_CATEGORY);

            if (createBookAndAdd(&lib, id, title, author, category)) {
                printf("Livre ajoute.\n");
            } else {
                printf("Ajout impossible (id deja utilise ou memoire).\n");
            }
        } else if (choice == 3) {
            int idToDelete;
            idToDelete = readInt("ID du livre a supprimer: ");

            if (removeBookById(&lib, idToDelete)) {
                printf("Livre supprime.\n");
            } else {
                printf("Livre non trouve.\n");
            }
        } else if (choice == 0) {
            running = 0;
        } else {
            printf("Choix invalide.\n");
        }
    }

    freeLibrary(&lib);
    printf("Au revoir.\n");
    return 0;
}

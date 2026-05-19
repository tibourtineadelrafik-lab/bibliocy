#include <stdio.h>

#include "library.h"

int main(void) {
    Library lib;
    int index;

    initLibrary(&lib);
    if (lib.books == NULL) {
        printf("Erreur memoire.\n");
        return 1;
    }

    /* Ajouts simples pour tester le module library. */
    createBookAndAdd(&lib, 1, "Livre 1", "Auteur 1", "Categorie 1");
    createBookAndAdd(&lib, 2, "Livre 2", "Auteur 2", "Categorie 2");
    createBookAndAdd(&lib, 3, "Livre 3", "Auteur 3", "Categorie 1");

    printf("Etat initial:\n");
    displayLibrary(&lib);

    /* Exemple de recherche par titre. */
    index = searchByTitle(lib.books, lib.nbBooks, "Livre 2");
    if (index != -1) {
        printf("Livre trouve:\n");
        displayBook(&lib.books[index]);
    } else {
        printf("Livre non trouve.\n");
    }

    /* Exemple de suppression. */
    if (removeBookById(&lib, 2)) {
        printf("\nLivre 2 supprime.\n");
    }

    printf("Etat final:\n");
    displayLibrary(&lib);

    freeLibrary(&lib);
    return 0;
}

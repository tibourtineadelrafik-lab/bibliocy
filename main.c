#include <stdio.h>
#include <string.h>

#include "library.h"
#include "user.h"

#define MAX_USERS 100

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

/* Ajoute quelques livres de test pour avancer sans fichier de sauvegarde. */
static void initTestBooks(Library *lib) {
    createBookAndAdd(lib, 1, "Livre 1", "Auteur 1", "Roman");
    createBookAndAdd(lib, 2, "Livre 2", "Auteur 2", "Informatique");
    createBookAndAdd(lib, 3, "Livre 3", "Auteur 3", "Histoire");
    createBookAndAdd(lib, 4, "Livre 4", "Auteur 1", "Science");
}

/* Creation de compte tres simple. */
static int createAccount(User *users, int *userCount) {
    char login[MAX_LOGIN];
    char password[MAX_PASSWORD];
    int role = 0;

    readText("Nouveau login: ", login, MAX_LOGIN);
    readText("Nouveau mot de passe: ", password, MAX_PASSWORD);

    printf("Role:\n");
    printf("1. Etudiant\n");
    printf("2. Professeur\n");
    role = readInt("Votre choix: ");

    if (!createUserAccount(users, userCount, MAX_USERS, login, password, role)) {
        printf("Creation impossible.\n");
        return 0;
    }

    printf("Compte cree avec succes.\n");
    return 1;
}

/* Connexion tres simple par login + mot de passe. */
static User *loginAccount(User *users, int userCount) {
    char login[MAX_LOGIN];
    char password[MAX_PASSWORD];
    User *user;

    readText("Login: ", login, MAX_LOGIN);
    readText("Mot de passe: ", password, MAX_PASSWORD);

    user = loginUser(users, userCount, login, password);
    if (user == NULL) {
        printf("Connexion refusee.\n");
        return NULL;
    }

    printf("Connexion reussie: %s (%s)\n", user->login, getRoleText(user));
    return user;
}

/* Menu de connexion avant d'acceder au menu principal. */
static void printAuthMenu(void) {
    printf("\n===== AUTHENTIFICATION =====\n");
    printf("1. Se connecter\n");
    printf("2. Creer un compte\n");
    printf("0. Quitter\n");
}

/* Affiche le menu principal apres connexion. */
static void printMenu(const User *currentUser) {
    printf("\n===== MENU =====\n");
    printf("Connecte: %s (%s)\n", currentUser->login, getRoleText(currentUser));
    printf("1. Afficher les livres\n");
    printf("2. Ajouter un livre (professeur)\n");
    printf("3. Supprimer un livre (professeur)\n");
    printf("4. Se deconnecter\n");
    printf("5. Rechercher un livre par titre\n");
    printf("6. Rechercher un livre par auteur\n");
    printf("7. Afficher les livres disponibles\n");
    printf("8. Trier les livres par titre et afficher\n");
    printf("9. Trier les livres par auteur et afficher\n");
    printf("10. Emprunter un livre\n");
    printf("11. Retourner un livre\n");
    printf("12. Afficher mes emprunts\n");
    printf("0. Quitter\n");
}

int main(void) {
    Library lib;
    User users[MAX_USERS];
    User *currentUser = NULL;
    int userCount = 0;
    int running = 1;

    initLibrary(&lib);
    if (lib.books == NULL) {
        printf("Erreur memoire.\n");
        return 1;
    }

    initDefaultUsers(users, &userCount);
    initTestBooks(&lib);

    printf("Comptes de test:\n");
    printf("- etudiant1 / 1234\n");
    printf("- prof1 / 1234\n");

    /* Boucle principale: auth puis actions bibliotheque. */
    while (running) {
        int choice;

        /* Tant qu'aucun utilisateur n'est connecte, on reste sur l'ecran d'auth. */
        if (currentUser == NULL) {
            printAuthMenu();
            choice = readInt("Votre choix: ");

            if (choice == 1) {
                currentUser = loginAccount(users, userCount);
            } else if (choice == 2) {
                createAccount(users, &userCount);
            } else if (choice == 0) {
                running = 0;
            } else {
                printf("Choix invalide.\n");
            }

            continue;
        }

        printMenu(currentUser);
        choice = readInt("Votre choix: ");

        if (choice == 1) {
            displayLibrary(&lib);
        } else if (choice == 2) {
            int id;
            char title[MAX_TITLE];
            char author[MAX_AUTHOR];
            char category[MAX_CATEGORY];

            /* Regle du projet: un etudiant ne peut pas ajouter. */
            if (isStudent(currentUser)) {
                printf("Seul un professeur peut ajouter un livre.\n");
                continue;
            }

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
            char confirm[10];
            Book *bookToDelete;

            /* Regle du projet: un etudiant ne peut pas supprimer. */
            if (isStudent(currentUser)) {
                printf("Seul un professeur peut supprimer un livre.\n");
                continue;
            }

            idToDelete = readInt("ID du livre a supprimer: ");
            bookToDelete = getBookById(&lib, idToDelete);
            if (bookToDelete == NULL) {
                printf("Livre non trouve.\n");
                continue;
            }

            /* On evite de supprimer un livre qui est deja emprunte. */
            if (bookToDelete->available == 0) {
                printf("Suppression impossible: livre actuellement emprunte.\n");
                continue;
            }

            /* Petite securite: on demande une confirmation avant suppression. */
            readText("Confirmer suppression ? (o/n): ", confirm, 10);
            if (confirm[0] != 'o' && confirm[0] != 'O') {
                printf("Suppression annulee.\n");
                continue;
            }

            if (removeBookById(&lib, idToDelete)) {
                printf("Livre supprime.\n");
            } else {
                printf("Livre non trouve.\n");
            }
        } else if (choice == 5) {
            char titleToSearch[MAX_TITLE];
            int index;

            /* Recherche simple: comparaison exacte du titre saisi. */
            readText("Titre a rechercher: ", titleToSearch, MAX_TITLE);
            index = searchByTitle(lib.books, lib.nbBooks, titleToSearch);

            if (index >= 0) {
                printf("Livre trouve:\n");
                displayBook(&lib.books[index]);
            } else {
                printf("Livre non trouve.\n");
            }
        } else if (choice == 6) {
            char authorToSearch[MAX_AUTHOR];
            int index;

            readText("Auteur a rechercher: ", authorToSearch, MAX_AUTHOR);
            index = searchByAuthor(lib.books, lib.nbBooks, authorToSearch);

            if (index >= 0) {
                printf("Livre trouve:\n");
                displayBook(&lib.books[index]);
            } else {
                printf("Livre non trouve.\n");
            }
        } else if (choice == 7) {
            displayAvailableBooks(lib.books, lib.nbBooks);
        } else if (choice == 8) {
            /* Tri du tableau par titre puis affichage du resultat. */
            sortByTitle(lib.books, lib.nbBooks);
            displayLibrary(&lib);
        } else if (choice == 9) {
            /* Tri du tableau par auteur puis affichage du resultat. */
            sortByAuthor(lib.books, lib.nbBooks);
            displayLibrary(&lib);
        } else if (choice == 10) {
            int idToBorrow;
            Book *book;

            idToBorrow = readInt("ID du livre a emprunter: ");
            if (borrowBook(currentUser, lib.books, lib.nbBooks, idToBorrow)) {
                printf("Emprunt valide.\n");
            } else {
                book = getBookById(&lib, idToBorrow);
                if (book == NULL) {
                    printf("Livre non trouve.\n");
                } else if (book->available == 0) {
                    printf("Livre deja emprunte par %s.\n", book->borrower);
                } else {
                    printf("Emprunt refuse (retard ou limite atteinte).\n");
                }
            }
        } else if (choice == 11) {
            int idToReturn;
            Book *book;

            idToReturn = readInt("ID du livre a retourner: ");
            if (returnBook(currentUser, lib.books, lib.nbBooks, idToReturn)) {
                printf("Retour valide.\n");
            } else {
                book = getBookById(&lib, idToReturn);
                if (book == NULL) {
                    printf("Livre non trouve.\n");
                } else if (book->available == 1) {
                    printf("Ce livre est deja disponible.\n");
                } else {
                    printf("Retour refuse: ce livre n'est pas emprunte par vous.\n");
                }
            }
        } else if (choice == 12) {
            displayBorrowedBooks(currentUser, lib.books, lib.nbBooks);
        } else if (choice == 4) {
            currentUser = NULL;
            printf("Deconnexion.\n");
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

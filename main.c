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

            /* Regle du projet: un etudiant ne peut pas supprimer. */
            if (isStudent(currentUser)) {
                printf("Seul un professeur peut supprimer un livre.\n");
                continue;
            }

            idToDelete = readInt("ID du livre a supprimer: ");

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

#include <stdio.h>

#include "file_io.h"
#include "input_helper.h"

#define MAX_USERS 100

/* Centralise le message d'erreur de sauvegarde apres une action qui modifie les donnees. */
static void saveWithWarning(const Library *lib, const User *users, int userCount) {
    if (!saveAll(lib, users, userCount)) {
        printf("Attention: sauvegarde impossible.\n");
    }
}

/* Cree un nouveau compte utilisateur depuis la saisie clavier. */
static int createAccount(User *users, int *userCount) {
    char login[MAX_LOGIN];
    char password[MAX_PASSWORD];
    int role;

    readText("Nouveau login: ", login, MAX_LOGIN);
    readText("Nouveau mot de passe: ", password, MAX_PASSWORD);

    printf("Role:\n");
    printf("1. Etudiant\n");
    printf("2. Professeur\n");
    role = readIntInRange("Votre choix: ", 1, 2);

    if (!createUserAccount(users, userCount, MAX_USERS, login, password, role)) {
        printf("Creation impossible.\n");
        return 0;
    }

    printf("Compte cree avec succes.\n");
    return 1;
}

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

static void printAuthMenu(void) {
    printf("\n===== AUTHENTIFICATION =====\n");
    printf("1. Se connecter\n");
    printf("2. Creer un compte\n");
    printf("0. Quitter\n");
}

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
    printf("13. Rechercher un livre par categorie\n");
    printf("14. Tableau de bord\n");
    printf("0. Quitter\n");
}

static void printDashboard(const Library *lib, const User *users, int userCount, const User *currentUser) {
    int i;
    int availableBooks = 0;
    int borrowedBooks = 0;
    int studentCount = 0;
    int professorCount = 0;

    if (lib == NULL || users == NULL || currentUser == NULL) {
        printf("Donnees indisponibles.\n");
        return;
    }

    for (i = 0; i < lib->nbBooks; i++) {
        if (lib->books[i].available == 1) {
            availableBooks++;
        } else {
            borrowedBooks++;
        }
    }

    for (i = 0; i < userCount; i++) {
        if (users[i].role == STUDENT) {
            studentCount++;
        } else if (users[i].role == PROFESSOR) {
            professorCount++;
        }
    }

    printf("\n===== TABLEAU DE BORD =====\n");
    printf("Livres totaux        : %d\n", lib->nbBooks);
    printf("Livres disponibles   : %d\n", availableBooks);
    printf("Livres empruntes     : %d\n", borrowedBooks);
    printf("Utilisateurs totaux  : %d\n", userCount);
    printf("Etudiants            : %d\n", studentCount);
    printf("Professeurs          : %d\n", professorCount);
    printf("Mes emprunts         : %d\n", currentUser->nbBorrowed);
    printf("J'ai un retard       : %s\n", hasLateBooks(currentUser) ? "Oui" : "Non");
    printf("Je peux emprunter    : %s\n", canBorrow(currentUser) ? "Oui" : "Non");
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

    loadDataOrInitDefaults(&lib, users, &userCount, MAX_USERS);

    printf("Comptes de test (si premier lancement):\n");
    printf("- etudiant1 / 1234\n");
    printf("- prof1 / 1234\n");

    while (running) {
        int choice;

        if (currentUser == NULL) {
            printAuthMenu();
            choice = readIntInRange("Votre choix: ", 0, 2);

            if (choice == 1) {
                currentUser = loginAccount(users, userCount);
            } else if (choice == 2) {
                if (createAccount(users, &userCount)) {
                    saveWithWarning(&lib, users, userCount);
                }
            } else {
                running = 0;
            }

            continue;
        }

        printMenu(currentUser);
        choice = readIntInRange("Votre choix: ", 0, 14);

        if (choice == 1) {
            displayLibrary(&lib);
        } else if (choice == 2) {
            int id;
            char title[MAX_TITLE];
            char author[MAX_AUTHOR];
            char category[MAX_CATEGORY];

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
                saveWithWarning(&lib, users, userCount);
            } else {
                printf("Ajout impossible (id deja utilise ou memoire).\n");
            }
        } else if (choice == 3) {
            int idToDelete;
            char confirm[10];
            Book *bookToDelete;

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

            if (bookToDelete->available == 0) {
                printf("Suppression impossible: livre actuellement emprunte.\n");
                continue;
            }

            readText("Confirmer suppression ? (o/n): ", confirm, 10);
            if (confirm[0] != 'o' && confirm[0] != 'O') {
                printf("Suppression annulee.\n");
                continue;
            }

            if (removeBookById(&lib, idToDelete)) {
                printf("Livre supprime.\n");
                saveWithWarning(&lib, users, userCount);
            } else {
                printf("Livre non trouve.\n");
            }
        } else if (choice == 5) {
            char titleToSearch[MAX_TITLE];
            int index;

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
            sortByTitle(lib.books, lib.nbBooks);
            displayLibrary(&lib);
        } else if (choice == 9) {
            sortByAuthor(lib.books, lib.nbBooks);
            displayLibrary(&lib);
        } else if (choice == 10) {
            int idToBorrow;
            Book *book;

            idToBorrow = readInt("ID du livre a emprunter: ");
            if (borrowBook(currentUser, lib.books, lib.nbBooks, idToBorrow)) {
                printf("Emprunt valide.\n");
                saveWithWarning(&lib, users, userCount);
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
                saveWithWarning(&lib, users, userCount);
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
        } else if (choice == 13) {
            char categoryToSearch[MAX_CATEGORY];

            readText("Categorie a rechercher: ", categoryToSearch, MAX_CATEGORY);
            searchByCategory(lib.books, lib.nbBooks, categoryToSearch);
        } else if (choice == 14) {
            printDashboard(&lib, users, userCount, currentUser);
        } else if (choice == 4) {
            currentUser = NULL;
            printf("Deconnexion.\n");
        } else if (choice == 0) {
            running = 0;
        } else {
            printf("Choix invalide.\n");
        }
    }

    if (!saveAll(&lib, users, userCount)) {
        printf("Attention: sauvegarde finale impossible.\n");
    }

    freeLibrary(&lib);
    printf("Au revoir.\n");
    return 0;
}

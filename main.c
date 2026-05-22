#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "library.h"
#include "user.h"

#define MAX_USERS 100
#define BOOKS_FILE "books.txt"
#define USERS_FILE "users.txt"

static void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

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

static void readText(const char *prompt, char *buffer, int size) {
    printf("%s", prompt);
    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    buffer[strcspn(buffer, "\n")] = '\0';
}

static void initTestBooks(Library *lib) {
    createBookAndAdd(lib, 1, "Livre 1", "Auteur 1", "Roman");
    createBookAndAdd(lib, 2, "Livre 2", "Auteur 2", "Informatique");
    createBookAndAdd(lib, 3, "Livre 3", "Auteur 3", "Histoire");
    createBookAndAdd(lib, 4, "Livre 4", "Auteur 1", "Science");
}

static int saveBooks(const Library *lib) {
    FILE *file;
    int i;

    if (lib == NULL || lib->books == NULL) {
        return 0;
    }

    file = fopen(BOOKS_FILE, "w");
    if (file == NULL) {
        return 0;
    }

    fprintf(file, "%d\n", lib->nbBooks);
    for (i = 0; i < lib->nbBooks; i++) {
        const char *borrower = lib->books[i].borrower[0] == '\0' ? "_" : lib->books[i].borrower;
        fprintf(
            file,
            "%d;%s;%s;%s;%d;%s\n",
            lib->books[i].id,
            lib->books[i].title,
            lib->books[i].author,
            lib->books[i].category,
            lib->books[i].available,
            borrower
        );
    }

    fclose(file);
    return 1;
}

static int loadBooks(Library *lib) {
    FILE *file;
    int expectedCount;
    int i;

    if (lib == NULL || lib->books == NULL) {
        return 0;
    }

    file = fopen(BOOKS_FILE, "r");
    if (file == NULL) {
        return 0;
    }

    if (fscanf(file, "%d", &expectedCount) != 1 || expectedCount < 0) {
        fclose(file);
        return 0;
    }

    lib->nbBooks = 0;

    for (i = 0; i < expectedCount; i++) {
        Book b;
        char title[MAX_TITLE];
        char author[MAX_AUTHOR];
        char category[MAX_CATEGORY];
        char borrower[MAX_LOGIN];
        int id;
        int available;

        if (fscanf(file, " %d;%99[^;];%99[^;];%49[^;];%d;%49[^\n]", &id, title, author, category, &available, borrower) != 6) {
            lib->nbBooks = 0;
            fclose(file);
            return 0;
        }

        if (id <= 0 || (available != 0 && available != 1)) {
            lib->nbBooks = 0;
            fclose(file);
            return 0;
        }

        initBook(&b, id, title, author, category);
        b.available = available;

        if (available == 0) {
            if (borrower[0] == '\0' || strcmp(borrower, "_") == 0) {
                lib->nbBooks = 0;
                fclose(file);
                return 0;
            }
            strcpy(b.borrower, borrower);
        } else {
            b.borrower[0] = '\0';
        }

        if (!addBookToLibrary(lib, &b)) {
            lib->nbBooks = 0;
            fclose(file);
            return 0;
        }
    }

    fclose(file);
    return 1;
}

static int saveUsers(const User *users, int userCount) {
    FILE *file;
    int i;
    int j;

    if (users == NULL || userCount < 0) {
        return 0;
    }

    file = fopen(USERS_FILE, "w");
    if (file == NULL) {
        return 0;
    }

    fprintf(file, "%d\n", userCount);
    for (i = 0; i < userCount; i++) {
        fprintf(file, "%s;%s;%d;%d", users[i].login, users[i].password, users[i].role, users[i].nbBorrowed);
        for (j = 0; j < MAX_BORROWS; j++) {
            fprintf(file, ";%d;%ld", users[i].borrowedIds[j], users[i].deadlines[j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    return 1;
}

static int loadUsers(User *users, int *userCount, int maxUsers) {
    FILE *file;
    int expectedCount;
    int i;

    if (users == NULL || userCount == NULL || maxUsers <= 0) {
        return 0;
    }

    file = fopen(USERS_FILE, "r");
    if (file == NULL) {
        return 0;
    }

    if (fscanf(file, "%d", &expectedCount) != 1 || expectedCount < 0 || expectedCount > maxUsers) {
        fclose(file);
        return 0;
    }

    *userCount = 0;

    for (i = 0; i < expectedCount; i++) {
        char login[MAX_LOGIN];
        char password[MAX_PASSWORD];
        int role;
        int nbBorrowed;
        int j;

        if (fscanf(file, " %49[^;];%49[^;];%d;%d", login, password, &role, &nbBorrowed) != 4) {
            *userCount = 0;
            fclose(file);
            return 0;
        }

        if ((role != STUDENT && role != PROFESSOR) || nbBorrowed < 0 || nbBorrowed > MAX_BORROWS) {
            *userCount = 0;
            fclose(file);
            return 0;
        }

        initUser(&users[*userCount], login, password, role);
        users[*userCount].nbBorrowed = nbBorrowed;

        for (j = 0; j < MAX_BORROWS; j++) {
            int borrowedId;
            long deadline;

            if (fscanf(file, ";%d;%ld", &borrowedId, &deadline) != 2) {
                *userCount = 0;
                fclose(file);
                return 0;
            }

            if (j < nbBorrowed) {
                if (borrowedId <= 0 || deadline <= 0) {
                    *userCount = 0;
                    fclose(file);
                    return 0;
                }
                users[*userCount].borrowedIds[j] = borrowedId;
                users[*userCount].deadlines[j] = deadline;
            } else {
                users[*userCount].borrowedIds[j] = -1;
                users[*userCount].deadlines[j] = 0;
            }
        }

        (*userCount)++;
    }

    fclose(file);
    return 1;
}

static int saveAll(const Library *lib, const User *users, int userCount) {
    if (!saveBooks(lib)) {
        return 0;
    }
    if (!saveUsers(users, userCount)) {
        return 0;
    }
    return 1;
}

static void loadDataOrInitDefaults(Library *lib, User *users, int *userCount) {
    int booksLoaded = loadBooks(lib);
    int usersLoaded = loadUsers(users, userCount, MAX_USERS);

    if (!booksLoaded || !usersLoaded) {
        lib->nbBooks = 0;
        initTestBooks(lib);
        initDefaultUsers(users, userCount);

        if (!saveAll(lib, users, *userCount)) {
            printf("Attention: impossible de creer les fichiers de sauvegarde.\n");
        } else {
            printf("Fichiers de sauvegarde crees automatiquement.\n");
        }
    }
}

static int createAccount(User *users, int *userCount) {
    char login[MAX_LOGIN];
    char password[MAX_PASSWORD];
    int role;

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

    loadDataOrInitDefaults(&lib, users, &userCount);

    printf("Comptes de test (si premier lancement):\n");
    printf("- etudiant1 / 1234\n");
    printf("- prof1 / 1234\n");

    while (running) {
        int choice;

        if (currentUser == NULL) {
            printAuthMenu();
            choice = readInt("Votre choix: ");

            if (choice == 1) {
                currentUser = loginAccount(users, userCount);
            } else if (choice == 2) {
                if (createAccount(users, &userCount) && !saveAll(&lib, users, userCount)) {
                    printf("Attention: sauvegarde impossible.\n");
                }
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
                if (!saveAll(&lib, users, userCount)) {
                    printf("Attention: sauvegarde impossible.\n");
                }
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
                if (!saveAll(&lib, users, userCount)) {
                    printf("Attention: sauvegarde impossible.\n");
                }
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
                if (!saveAll(&lib, users, userCount)) {
                    printf("Attention: sauvegarde impossible.\n");
                }
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
                if (!saveAll(&lib, users, userCount)) {
                    printf("Attention: sauvegarde impossible.\n");
                }
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

#include <stdio.h>
#include <string.h>

#include "file_io.h"

#define BOOKS_FILE "books.txt"
#define USERS_FILE "users.txt"

/* Donnees minimales au premier lancement. */
static void initTestBooks(Library *lib) {
    createBookAndAdd(lib, 1, "One Piece", "Eiichiro Oda", "Manga");
    createBookAndAdd(lib, 2, "Jujutsu Kaisen", "Gege Akutami", "Manga");
    createBookAndAdd(lib, 3, "Five Nights at Freddy's: The Silver Eyes", "Scott Cawthon et Kira Breed-Wrisley", "Horreur");
    createBookAndAdd(lib, 4, "Pourquoi Madame Ansermin est meilleure en informatique que Monsieur Grignon", "Auteur invente", "Informatique");

    createBookAndAdd(lib, 5, "Spider-Man: Blue", "Jeph Loeb et Tim Sale", "BD Marvel");
    createBookAndAdd(lib, 6, "Batman: The Killing Joke", "Alan Moore et Brian Bolland", "BD DC Comics");
    createBookAndAdd(lib, 7, "Percy Jackson: Le Voleur de foudre", "Rick Riordan", "Fantasy");
    createBookAndAdd(lib, 8, "Harry Potter a l'ecole des sorciers", "J.K. Rowling", "Fantasy");

    createBookAndAdd(lib, 9, "League of Legends: Realms of Runeterra", "Riot Games", "Fantasy");
    createBookAndAdd(lib, 10, "Les Miserables", "Victor Hugo", "Roman");
    createBookAndAdd(lib, 11, "Notre-Dame de Paris", "Victor Hugo", "Roman");
    createBookAndAdd(lib, 12, "Le Seigneur des anneaux", "J.R.R. Tolkien", "Fantasy");

    createBookAndAdd(lib, 13, "Hunger Games", "Suzanne Collins", "Science-fiction");
    createBookAndAdd(lib, 14, "Divergente", "Veronica Roth", "Science-fiction");
    createBookAndAdd(lib, 15, "L'Attaque des Titans", "Hajime Isayama", "Manga");
    createBookAndAdd(lib, 16, "Dragon Ball", "Akira Toriyama", "Manga");

    createBookAndAdd(lib, 17, "Death Note", "Tsugumi Ohba et Takeshi Obata", "Manga");
    createBookAndAdd(lib, 18, "Sherlock Holmes: Une etude en rouge", "Arthur Conan Doyle", "Policier");
    createBookAndAdd(lib, 19, "Arsene Lupin, gentleman-cambrioleur", "Maurice Leblanc", "Policier");
    createBookAndAdd(lib, 20, "Le Tour du monde en quatre-vingts jours", "Jules Verne", "Aventure");
}

/* Ecrit la liste des livres dans books.txt. */
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

/* Recharge les livres depuis books.txt et valide les champs importants. */
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

        if (id <= 0 || title[0] == '\0' || author[0] == '\0' || category[0] == '\0' || (available != 0 && available != 1)) {
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

/* Ecrit les utilisateurs et leurs emprunts dans users.txt. */
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

/* Recharge les utilisateurs depuis users.txt avec verifications simples. */
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

        if (login[0] == '\0' || password[0] == '\0' || (role != STUDENT && role != PROFESSOR) || nbBorrowed < 0 || nbBorrowed > MAX_BORROWS) {
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

int saveAll(const Library *lib, const User *users, int userCount) {
    return saveBooks(lib) && saveUsers(users, userCount);
}

void loadDataOrInitDefaults(Library *lib, User *users, int *userCount, int maxUsers) {
    if (!loadBooks(lib) || !loadUsers(users, userCount, maxUsers)) {
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

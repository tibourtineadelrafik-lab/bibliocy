#include <stdio.h>
#include <string.h>
#include <time.h>

#include "user.h"

#define DAY_SECONDS (24L * 60L * 60L)
#define BORROW_SECONDS_STUDENT (2L * 60L)
#define BORROW_SECONDS_PROFESSOR (3L * 60L)

static int findBookIndexById(Book *books, int nbBooks, int bookId) {
    int i;

    for (i = 0; i < nbBooks; i++) {
        if (books[i].id == bookId) {
            return i;
        }
    }
    return -1;
}

static int findBorrowedIndex(User *u, int bookId) {
    int i;

    for (i = 0; i < u->nbBorrowed; i++) {
        if (u->borrowedIds[i] == bookId) {
            return i;
        }
    }
    return -1;
}

static void formatDuration(long seconds, char *buffer, int size) {
    long days;
    long hours;
    long minutes;
    long secs;
    long remaining;

    if (seconds < 0) {
        seconds = 0;
    }

    days = seconds / DAY_SECONDS;
    remaining = seconds % DAY_SECONDS;
    hours = remaining / 3600L;
    remaining = remaining % 3600L;
    minutes = remaining / 60L;
    secs = remaining % 60L;

    if (days > 0) {
        snprintf(buffer, size, "%ld j %ld h %ld min", days, hours, minutes);
    } else if (hours > 0) {
        snprintf(buffer, size, "%ld h %ld min", hours, minutes);
    } else {
        snprintf(buffer, size, "%ld min %ld sec", minutes, secs);
    }
}

void initUser(User *u, const char *login, const char *password, int role) {
    int i;

    strcpy(u->login, login);
    strcpy(u->password, password);
    u->role = role;
    u->nbBorrowed = 0;

    for (i = 0; i < MAX_BORROWS; i++) {
        u->borrowedIds[i] = -1;
        u->deadlines[i] = 0;
    }
}

void displayUser(const User *u) {
    if (u == NULL) {
        printf("Erreur : utilisateur invalide.\n");
        return;
    }

    printf("  Login : %s\n", u->login);
    if (u->role == STUDENT) {
        printf("  Role  : Etudiant\n");
    } else {
        printf("  Role  : Professeur\n");
    }
    printf("  Livres empruntes : %d\n", u->nbBorrowed);
}

int isStudent(const User *u) {
    return u->role == STUDENT;
}

const char *getRoleText(const User *u) {
    if (u != NULL && u->role == PROFESSOR) {
        return "Professeur";
    }
    return "Etudiant";
}

User *findUserByLogin(User *users, int userCount, const char *login) {
    int i;

    for (i = 0; i < userCount; i++) {
        if (strcmp(users[i].login, login) == 0) {
            return &users[i];
        }
    }
    return NULL;
}

void initDefaultUsers(User *users, int *userCount) {
    *userCount = 0;
    initUser(&users[*userCount], "etudiant1", "1234", STUDENT);
    (*userCount)++;
    initUser(&users[*userCount], "prof1", "1234", PROFESSOR);
    (*userCount)++;
}

int createUserAccount(User *users, int *userCount, int maxUsers, const char *login, const char *password, int role) {
    if (users == NULL || userCount == NULL || login == NULL || password == NULL) {
        return 0;
    }
    if (*userCount >= maxUsers) {
        return 0;
    }
    if (login[0] == '\0' || password[0] == '\0') {
        return 0;
    }
    if (role != STUDENT && role != PROFESSOR) {
        return 0;
    }
    if (findUserByLogin(users, *userCount, login) != NULL) {
        return 0;
    }

    initUser(&users[*userCount], login, password, role);
    (*userCount)++;
    return 1;
}

User *loginUser(User *users, int userCount, const char *login, const char *password) {
    User *user = findUserByLogin(users, userCount, login);

    if (user == NULL) {
        return NULL;
    }
    if (strcmp(user->password, password) != 0) {
        return NULL;
    }
    return user;
}

int canBorrow(const User *u) {
    if (u == NULL) {
        return 0;
    }

    if (hasLateBooks(u)) {
        return 0;
    }

    if (isStudent(u) && u->nbBorrowed >= 3) {
        return 0;
    }

    if (!isStudent(u) && u->nbBorrowed >= 5) {
        return 0;
    }

    return 1;
}

int hasLateBooks(const User *u) {
    int i;
    long now;

    if (u == NULL) {
        return 0;
    }

    now = (long)time(NULL);
    for (i = 0; i < u->nbBorrowed; i++) {
        if (u->deadlines[i] > 0 && now > u->deadlines[i]) {
            return 1;
        }
    }

    return 0;
}

int borrowBook(User *u, Book *books, int nbBooks, int bookId) {
    int bookIndex;
    long borrowSeconds;
    long now;

    if (u == NULL || books == NULL || bookId <= 0) {
        return 0;
    }
    if (!canBorrow(u) || u->nbBorrowed >= MAX_BORROWS) {
        return 0;
    }

    bookIndex = findBookIndexById(books, nbBooks, bookId);
    if (bookIndex == -1 || books[bookIndex].available == 0) {
        return 0;
    }

    books[bookIndex].available = 0;
    strcpy(books[bookIndex].borrower, u->login);

    u->borrowedIds[u->nbBorrowed] = bookId;
    now = (long)time(NULL);
    borrowSeconds = isStudent(u) ? BORROW_SECONDS_STUDENT : BORROW_SECONDS_PROFESSOR;
    u->deadlines[u->nbBorrowed] = now + borrowSeconds;
    u->nbBorrowed++;
    return 1;
}

int returnBook(User *u, Book *books, int nbBooks, int bookId) {
    int bookIndex;
    int borrowedIndex;
    int i;

    if (u == NULL || books == NULL || bookId <= 0) {
        return 0;
    }

    bookIndex = findBookIndexById(books, nbBooks, bookId);
    if (bookIndex == -1) {
        return 0;
    }
    if (books[bookIndex].available == 1) {
        return 0;
    }
    if (strcmp(books[bookIndex].borrower, u->login) != 0) {
        return 0;
    }

    borrowedIndex = findBorrowedIndex(u, bookId);
    if (borrowedIndex == -1) {
        return 0;
    }

    for (i = borrowedIndex; i < u->nbBorrowed - 1; i++) {
        u->borrowedIds[i] = u->borrowedIds[i + 1];
        u->deadlines[i] = u->deadlines[i + 1];
    }

    u->nbBorrowed--;
    u->borrowedIds[u->nbBorrowed] = -1;
    u->deadlines[u->nbBorrowed] = 0;

    books[bookIndex].available = 1;
    books[bookIndex].borrower[0] = '\0';
    return 1;
}

void displayBorrowedBooks(User *u, Book *books, int nbBooks) {
    int i;
    int bookIndex;
    long now;
    long remainingSeconds;
    char durationText[64];

    if (u == NULL || books == NULL) {
        printf("Erreur : donnees invalides.\n");
        return;
    }

    if (u->nbBorrowed == 0) {
        printf("Aucun livre emprunte.\n");
        return;
    }

    now = (long)time(NULL);
    printf("=== Mes emprunts (%d) ===\n", u->nbBorrowed);
    for (i = 0; i < u->nbBorrowed; i++) {
        bookIndex = findBookIndexById(books, nbBooks, u->borrowedIds[i]);
        if (bookIndex == -1) {
            printf("  [%d] Livre introuvable.\n", u->borrowedIds[i]);
            continue;
        }

        printf("  [%d] %s\n", books[bookIndex].id, books[bookIndex].title);
        printf("     Auteur : %s\n", books[bookIndex].author);

        remainingSeconds = u->deadlines[i] - now;
        if (u->deadlines[i] > 0 && remainingSeconds < 0) {
            formatDuration(-remainingSeconds, durationText, 64);
            printf("     Statut : En retard de %s\n", durationText);
        } else {
            formatDuration(remainingSeconds, durationText, 64);
            printf("     Statut : A rendre dans %s\n", durationText);
        }
    }
}

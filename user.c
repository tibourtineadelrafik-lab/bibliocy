#include <stdio.h>
#include <string.h>
#include <time.h>
#include "user.h"
void initUser(User *u, const char *login, const char *password, int role) {
    int i;
    /* strcpy copie les chaines caractere par caractere */
    strcpy(u->login,    login);
    strcpy(u->password, password);
    u->role = role;
    /* aucun livre emprunte au depart */
    u->nbBorrowed = 0;
     /* on parcourt le tableau pour tout initialiser a vide
     * -1 signifie "aucun livre" pour les IDs
     *  0 signifie "pas de deadline" pour les heures limites */
    for (i = 0; i < MAX_BORROWS; i++) {
        u->borrowedIds[i] = -1;
        u->deadlines[i]   = 0;
    }
}

void displayUser(User *u) {
    if (u == NULL) {
        printf("Erreur : utilisateur invalide.\n");
        return;  /* return sans valeur car la fonction est void */
    }
    printf("  Login : %s\n", u->login);
    if (u->role == STUDENT) {
        printf("  Role  : Etudiant\n");
    } else {
        printf("  Role  : Professeur\n");
    }
 
    printf("  Livres empruntes : %d\n", u->nbBorrowed);
}

int isStudent(User *u) {
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

/* ============================================================
 *  canBorrow
 *  Verifie si l'utilisateur peut emprunter un livre.
 *  Retourne 1 s'il peut, 0 s'il ne peut pas.
 *  Regles du projet :
 *  - etudiant  : max 3 livres, bloque si retard
 *  - professeur: max 5 livres, bloque si retard
 * ============================================================ */
 int canBorrow(User *u) {
    /* regle 1 : bloque si l'utilisateur a des livres en retard */
    if (hasLateBooks(u)) {
        return 0;
    }
 
    /* regle 2 : un etudiant ne peut pas depasser 3 livres */
    if (isStudent(u) && u->nbBorrowed >= 3) {
        return 0;
    }
 
    /* regle 3 : un professeur ne peut pas depasser 5 livres */
    if (!isStudent(u) && u->nbBorrowed >= 5) {
        return 0;
    }
 
    /* toutes les regles sont respectees : il peut emprunter */
    return 1;
}
int hasLateBooks(User *u) {
    int i;
 
    /* on recupere l'heure actuelle en secondes */
    long now = (long)time(NULL);
 
    /* on parcourt tous les livres empruntes */
    for (i = 0; i < u->nbBorrowed; i++) {
        /* deadlines[i] > 0 verifie qu'une deadline existe bien
         * now > deadlines[i] verifie que l'heure est depassee */
        if (u->deadlines[i] > 0 && now > u->deadlines[i]) {
            return 1;  /* au moins un livre est en retard */
        }
    }
 
    return 0;  /* aucun retard */
}

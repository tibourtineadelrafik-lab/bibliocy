#include "book.h"
#define MAX_LOGIN     50
#define MAX_PASSWORD  50
#define MAX_BORROWS    5

typedef struct {
    char login[MAX_LOGIN];          /* identifiant de connexion    */
    char password[MAX_PASSWORD];    /* mot de passe                */
    int  role;                      /* STUDENT ou PROFESSOR        */
    int  borrowedIds[MAX_BORROWS];  /* IDs des livres empruntes    */
    long deadlines[MAX_BORROWS];    /* heures limites en secondes  */
    int  nbBorrowed;                /* nb de livres empruntes      */
} User;

/* Remplit les champs d'un User avec les informations de base */
void initUser(User *u, char *login, char *password, int role);
/* Affiche les informations d'un utilisateur dans le terminal */
void displayUser(User *u);
/* Retourne 1 si l'utilisateur est un etudiant, 0 sinon */
int  isStudent(User *u);
/* Retourne 1 si l'utilisateur peut encore emprunter un livre
 * Verifie les regles : retard, nombre max de livres         */
int  canBorrow(User *u);
/* Retourne 1 si l'utilisateur a au moins un livre en retard */
int  hasLateBooks(User *u);

#ifndef USER_H
#define USER_H

#include "book.h"
#define MAX_LOGIN     50
#define MAX_PASSWORD  50
#define MAX_BORROWS    5

#define STUDENT    1
#define PROFESSOR  2

typedef struct {
    char login[MAX_LOGIN];          /* identifiant de connexion    */
    char password[MAX_PASSWORD];    /* mot de passe                */
    int  role;                      /* STUDENT ou PROFESSOR        */
    int  borrowedIds[MAX_BORROWS];  /* IDs des livres empruntes    */
    long deadlines[MAX_BORROWS];    /* heures limites en secondes  */
    int  nbBorrowed;                /* nb de livres empruntes      */
} User;

/* Remplit les champs d'un User avec les informations de base */
void initUser(User *u, const char *login, const char *password, int role);
/* Affiche les informations d'un utilisateur dans le terminal */
void displayUser(const User *u);
/* Retourne 1 si l'utilisateur est un etudiant, 0 sinon */
int  isStudent(const User *u);
/* Retourne "Etudiant" ou "Professeur" selon le role */
const char *getRoleText(const User *u);
/* Cherche un utilisateur par login */
User *findUserByLogin(User *users, int userCount, const char *login);
/* Cree 2 comptes de test en memoire */
void initDefaultUsers(User *users, int *userCount);
/* Cree un compte utilisateur si les infos sont valides */
int createUserAccount(User *users, int *userCount, int maxUsers, const char *login, const char *password, int role);
/* Verifie login + mot de passe et retourne l'utilisateur */
User *loginUser(User *users, int userCount, const char *login, const char *password);
/* Retourne 1 si l'utilisateur peut encore emprunter un livre
 * Verifie les regles : retard, nombre max de livres         */
int  canBorrow(const User *u);
/* Retourne 1 si l'utilisateur a au moins un livre en retard */
int  hasLateBooks(const User *u);
/* Emprunte un livre si les regles sont respectees */
int borrowBook(User *u, Book *books, int nbBooks, int bookId);
/* Retourne un livre emprunte par l'utilisateur */
int returnBook(User *u, Book *books, int nbBooks, int bookId);
/* Affiche la liste des livres empruntes par l'utilisateur */
void displayBorrowedBooks(User *u, Book *books, int nbBooks);

#endif

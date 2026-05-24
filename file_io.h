#ifndef FILE_IO_H
#define FILE_IO_H

#include "library.h"
#include "user.h"

/* Sauvegarde livres + utilisateurs dans les fichiers texte. */
int saveAll(const Library *lib, const User *users, int userCount);
/* Charge les fichiers si possible, sinon initialise les donnees de test puis sauvegarde. */
void loadDataOrInitDefaults(Library *lib, User *users, int *userCount, int maxUsers);

#endif

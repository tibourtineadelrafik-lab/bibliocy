#ifndef INPUT_HELPER_H
#define INPUT_HELPER_H

/* Vide les caracteres restants dans l'entree standard. */
void clearInputBuffer(void);
/* Lit un entier avec message et recommence tant que la saisie est invalide. */
int readInt(const char *prompt);
/* Lit un entier entre min et max inclus. */
int readIntInRange(const char *prompt, int min, int max);
/* Lit une ligne de texte et retire le retour a la ligne final. */
void readText(const char *prompt, char *buffer, int size);

#endif

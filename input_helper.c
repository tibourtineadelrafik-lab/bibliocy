#include <stdio.h>
#include <string.h>

#include "input_helper.h"

/* Nettoie l'entree pour eviter de garder un '\n' entre deux saisies. */
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

/* Redemande tant que la saisie n'est pas un entier. */
int readInt(const char *prompt) {
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

int readIntInRange(const char *prompt, int min, int max) {
    int value;
    int tmp;

    if (min > max) {
        tmp = min;
        min = max;
        max = tmp;
    }

    while (1) {
        value = readInt(prompt);
        if (value >= min && value <= max) {
            return value;
        }
        printf("Entrez une valeur entre %d et %d.\n", min, max);
    }
}

/* Saisie de texte simple (ligne complete). */
void readText(const char *prompt, char *buffer, int size) {
    printf("%s", prompt);
    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    buffer[strcspn(buffer, "\n")] = '\0';
}

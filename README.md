# bibliocy

Projet C simple de gestion de bibliotheque (niveau etudiant).

## Objectif

GerER une bibliotheque en memoire avec:
- des comptes utilisateur (etudiant/professeur),
- des operations sur les livres,
- un flux d'emprunt/retour.

## Etat actuel

Le projet fonctionne avec des **donnees de test en memoire**.

- Comptes de test:
  - `etudiant1 / 1234`
  - `prof1 / 1234`
- Livres de test charges au demarrage:
  - `[1] Livre 1 - Auteur 1 - Roman`
  - `[2] Livre 2 - Auteur 2 - Informatique`
  - `[3] Livre 3 - Auteur 3 - Histoire`
  - `[4] Livre 4 - Auteur 1 - Science`

<<<<<<< HEAD
Important: **pas d'enregistrement fichier pour le moment**.  
Quand le programme se ferme, les donnees sont perdues et rechargees au prochain lancement.
=======
Au premier lancement, si les fichiers n'existent pas:
- le programme charge les donnees de test,
- puis cree automatiquement `books.txt` et `users.txt`.

Si un fichier est corrompu/invalide, le programme revient aussi aux donnees de test puis recree les fichiers.

Ensuite, les donnees sont rechargees au demarrage et sauvegardees lors des modifications importantes (ajout/suppression/emprunt/retour/compte) et a la fermeture.
>>>>>>> 31a8dcc (Add borrow/return and file save)

## Fonctionnalites disponibles

1. Authentification: se connecter, creer un compte, quitter.
2. Affichage de tous les livres.
3. Ajout de livre (professeur seulement).
4. Suppression de livre (professeur seulement) avec confirmation.
5. Recherche par titre (exacte).
6. Recherche par auteur (exacte).
7. Affichage des livres disponibles.
8. Tri par titre + affichage.
9. Tri par auteur + affichage.
10. Emprunter un livre.
11. Retourner un livre.
12. Afficher mes emprunts (avec deadline en secondes).
13. Recherche par categorie.

Regles metier simples:
- seul un professeur peut ajouter/supprimer,
- suppression refusee si le livre est deja emprunte,
- un etudiant ne peut pas depasser 3 emprunts,
- un professeur ne peut pas depasser 5 emprunts,
- emprunt refuse si l'utilisateur a un retard.

## Structure du projet

- `main.c`: menus, saisie utilisateur, navigation de l'application.
- `book.h` / `book.c`: structure `Book`, affichage, recherche, tri.
- `library.h` / `library.c`: gestion du tableau dynamique de livres.
- `user.h` / `user.c`: comptes, roles, connexion, emprunt/retour.
- `Makefile`: compilation et nettoyage.

<<<<<<< HEAD
=======
## Fichiers de sauvegarde

- `books.txt`: etat des livres (disponible/emprunte, emprunteur).
- `users.txt`: comptes, roles, emprunts utilisateur, deadlines.
- Ces fichiers sont ignores par Git (avec le binaire `bibliocy`) via `.gitignore`.

>>>>>>> 31a8dcc (Add borrow/return and file save)
## Compilation et execution

```bash
make
./bibliocy
```

Nettoyage:

```bash
make clean
```

## Prochaine etape

<<<<<<< HEAD
Ajouter la sauvegarde/chargement des donnees dans des fichiers (livres + etats d'emprunt + utilisateurs).
=======
Ajouter un petit historique des emprunts (date emprunt + date retour) et eventuellement un export simple des donnees.
>>>>>>> 31a8dcc (Add borrow/return and file save)

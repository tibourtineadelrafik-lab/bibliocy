# bibliocy

Application C de gestion de bibliotheque en terminal.

## Objectif

Le programme permet de gerer des livres, des utilisateurs et des emprunts avec sauvegarde dans des fichiers texte.

## Etat du rendu

- Compilation: OK (`make`)
- Execution: OK (`./bibliocy`)
- Donnees persistantes: OK (`books.txt`, `users.txt`)

## Fonctionnalites

### Authentification
- Connexion
- Creation de compte
- Deconnexion

### Gestion des livres
- Afficher la bibliotheque
- Ajouter un livre (professeur uniquement)
- Supprimer un livre (professeur uniquement, avec confirmation)
- Rechercher par titre (partiel, sans casse stricte)
- Rechercher par auteur (partiel, sans casse stricte)
- Rechercher par categorie (partiel, sans casse stricte)
- Afficher les livres disponibles
- Trier par titre
- Trier par auteur

### Emprunts
- Emprunter un livre
- Retourner un livre
- Afficher ses emprunts
- Affichage lisible des delais (temps restant ou retard)

### Tableau de bord
- Nombre total de livres
- Livres disponibles / empruntes
- Nombre total d'utilisateurs
- Repartition etudiants / professeurs
- Etat de l'utilisateur connecte (retard, capacite d'emprunt)

## Conformite cahier des charges

- Connexion + creation de compte
- Roles etudiant/professeur appliques dans les actions
- Emprunt/retour fonctionnels
- Limites d'emprunt respectees (3 etudiant, 5 professeur)
- Delais d'emprunt respectes (2 min etudiant, 3 min professeur)
- Blocage en cas de retard
- Sauvegarde et rechargement automatiques

## Regles metier

- Etudiant: 3 emprunts max
- Professeur: 5 emprunts max
- Duree d'emprunt etudiant: 2 minutes
- Duree d'emprunt professeur: 3 minutes
- Emprunt refuse en cas de retard
- Suppression de livre refusee si le livre est emprunte

## Robustesse

- Saisie entier securisee (`readInt`)
- Saisie entier borne (`readIntInRange`)
- Validation des donnees chargees depuis les fichiers
- Reinitialisation automatique avec donnees par defaut si fichiers absents ou invalides
- Sauvegarde automatique apres les actions importantes

## Structure du projet

| Fichier | Role |
|---|---|
| `main.c` | Boucle principale, menus, orchestration |
| `book.h` / `book.c` | Modele livre, affichage, recherche, tri |
| `library.h` / `library.c` | Tableau dynamique des livres |
| `user.h` / `user.c` | Comptes, roles, regles d'emprunt/retour |
| `file_io.h` / `file_io.c` | Chargement / sauvegarde (`books.txt`, `users.txt`) |
| `input_helper.h` / `input_helper.c` | Saisie clavier securisee |
| `Makefile` | Build, run, clean |
| `.gitignore` | Ignore `bibliocy`, `books.txt`, `users.txt`, `*.o` |

## Commandes

Compiler:

```bash
make
```

Lancer:

```bash
./bibliocy
```

Lancer via Makefile:

```bash
make run
```

Nettoyer:

```bash
make clean
```

Verification rapide avant commit:

```bash
make clean && make
printf '0\n' | ./bibliocy
```

Code entier: 

```bash
cd ~
git clone https://github.com/tibourtineadelrafik-lab/bibliocy.git
cd bibliocy
make
./bibliotech
```
## Fichiers de donnees

- `books.txt`: etat des livres
- `users.txt`: comptes et emprunts

Si ces fichiers sont supprimes, le programme les recrera automatiquement au prochain lancement.

## Comptes de test (premier lancement)

- `etudiant1 / 1234`
- `prof1 / 1234`

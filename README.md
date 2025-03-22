# Tetris

![Platform](https://img.shields.io/badge/platform-Linux-blue)
![Top Language](https://img.shields.io/github/languages/top/IAidenI/Tetris)
![Version](https://img.shields.io/badge/version-0.1-yellow)
![License](https://img.shields.io/badge/license-MIT-green)

Une implémentation basique du jeu Tetris en C, jouable dans un terminal.

## Informations

Ce projet a été développé seul en 2025, dans le but de m'améliorer en langage C. J'ai séparé la logique de l'application dans une "API", et j'ai ajouté une interface visuelle à l'aide de la bibliothèque `ncurses`.

## Présentation

Le jeu fonctionne dans un terminal Linux. La compatibilité avec Windows n’a pas encore été testée. Cette version initiale permet de jouer, mais certaines fonctionnalités comme le score sont encore absentes.

Voici un aperçu de l'application.

<p>
  <img src="./docs/HomePage.png" alt="Page d'accueil" height="400">
  <img src="./docs/GameExemple.png" alt="Exemple de jeu" height="400">
</p>

## Fonctionnalités

- Affichage des blocs avec `ncurses`
- Chute automatique des pièces
- Rotation des blocs
- Alignement de lignes et suppression
- Système de log pour le débogage

## Commandes clavier

- `←` et `→` : déplacer la pièce à gauche/droite
- `↓` : accélérer la descente
- `↑` : faire pivoter la pièce
- `p` : met le jeu en pause
- `q` : quitter le jeu
- `-` et `+` : accélére/décélére le jeu (pas implémenté)

## Installation

Sous Linux :

```bash
git clone https://github.com/IAidenI/Tetris
cd Tetris
make
./Tetris
```

## Architecture

Le projet est organisé en deux grandes parties :
- **API** – Logique métier du jeu ([voir ici](https://github.com/IAidenI/LinuxTools/tree/main/Permissions))
- **IHM** – Affichage dans le terminal via `ncurses` ([voir ici](https://github.com/IAidenI/LinuxTools/tree/main/Permissions))

Il contient également des fichiers secondaires pour :
- la gestion des blocs
- la couleur
- les options de debug
- et autres...

## Débogage

Un fichier de log est disponible ici : `/tmp/debug_tetris.log`  
Il contient des informations utiles pour diagnostiquer l'état de l'application.

## A faire

- [ ] Fix le segfault quand on spam la rotation
- [ ] Spawn d'un bloc pas fluide avec les flèches
- [ ] Ajout d'un outil de debug pour mettre le jeu à un état précis
- [ ] Quitter ingame, demande de confirmation + revenir au menu
- [ ] Ajout du score
- [ ] Système de niveaux
- [ ] Ajout d'un menu de pause
- [ ] Refactoring pour améliorer certaines parties du code
- [ ] Ajout du 7-bag randomizer
- [ ] Vérifier la compatibilité windows

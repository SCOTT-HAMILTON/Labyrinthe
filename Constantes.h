#ifndef CONSTANTES_H
#define CONSTANTES_H

enum {DEVANT, DERRIERE, DROITE, GAUCHE};
enum {NORD, EST, SUD, OUEST};

#define VIDE 0
#define VISITED 1
#define CUL 2
#define CURRENT 3
#define BLOC 4
#define DEPART 5
#define ARRIVER 6
#define PONT 7

#define DIM 19


#define TAILLE_BLOC 30

#define PAUSE 0
#define SOLVERPAUSE 0

#define TROU ((DIM - 1)/2)*((DIM - 1)/2)

struct coordonnee2{
    int ligne;
    int colonne;
};

typedef struct coordonnee2 coordonnee;

// ASSETS
#define FILE_ARRIVER ASSETDIR "arriver.bmp"
#define FILE_DEPART ASSETDIR "depart.bmp"
#define FILE_CHARGEMENT ASSETDIR "MenuAnim/chargement.bmp"
#define FILE_GAGNE ASSETDIR "MenuAnim/Gagne.bmp"
#define FILE_LABYRINTHE ASSETDIR "MenuAnim/Labyrinthe.bmp"
#define DIR_MENU_ANIM ASSETDIR "MenuAnim/"
#define FILE_PLAY ASSETDIR "MenuAnim/play.bmp"
#define FILE_PLAY2 ASSETDIR "MenuAnim/play2.bmp"
#define FILE_SOLVE ASSETDIR "MenuAnim/solve.bmp"
#define FILE_SOLVE2 ASSETDIR "MenuAnim/solve2.bmp"

#endif


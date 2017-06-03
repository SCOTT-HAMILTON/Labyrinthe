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

#define DIM 23


#define TAILLE_BLOC 30

#define PAUSE 0
#define SOLVERPAUSE 0

#define TROU ((DIM - 1)/2)*((DIM - 1)/2)

struct coordonnee2{
    int ligne;
    int colonne;
};

typedef struct coordonnee2 coordonnee;

#endif


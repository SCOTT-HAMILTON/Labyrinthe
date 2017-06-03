#ifndef DEF_LAB_H
#define DEF_LAB_H

void quadrillage(SDL_Surface *fenetre, int **carte);
void getCarte(int **carte);
void getStartedPos(int **carte);
void getRealPos(int *ligne, int *colonne);
int isFinish(int **carte);
void getLab(SDL_Surface *fenetre, int **carte);
void getCurrent(int **carte, int *ligne, int *colonne);
void getNear(int **carte, coordonnee tab[4], int *nbrNear);
void setPont (int **carte, coordonnee choose, coordonnee current, SDL_Surface *fenetre);
void setPont (int **carte, coordonnee choose, coordonnee current, SDL_Surface *fenetre);
void cleen(int **carte, SDL_Surface *fenetre);
void solve(int **carte, SDL_Surface *fenetre);
void getArriver(int **carte, coordonnee *pos);
void getDepart(int **carte, coordonnee *pos);
int Win(coordonnee arriver, coordonnee pos);
void setDirection(int **carte, int direction, int tabDir[4], coordonnee pos);
void setPos(int **carte, int direction, coordonnee tab[4], coordonnee pos);
void initDirection(int **carte, int *direction, coordonnee pos);
void getDirection (int direction);
int containsPos(coordonnee *tab, coordonnee pos, int taille, int *pos2);
coordonnee* delDoublePos(coordonnee *tab, int taille, int *tailleRes);


#endif // DEF_LAB_H

#ifndef DEF_JEU_H
#define DEF_JEU_H

void play(int **carte, SDL_Surface *labyrinthe, SDL_Surface *fenetre);
int getMove(int **carte, coordonnee pos, int direction);
void initCarte(int **carte);

#endif // DEF_JEU_H



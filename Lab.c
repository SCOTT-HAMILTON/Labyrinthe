#include <stdio.h>
#include <SDL.h>
#include "Constantes.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "Lab.h"
#include "Jeu.h"


void generer(SDL_Surface *fenetre, SDL_Surface *labyrinthe, int **carte){
    SDL_Rect posLab, posChargement;
    SDL_Surface *chargement;

    chargement = SDL_LoadBMP(FILE_CHARGEMENT);
    SDL_SetColorKey(chargement, SDL_SRCCOLORKEY, SDL_MapRGB(chargement->format, 255, 255, 255));
    posChargement.x = fenetre->h/2 - chargement->w/2;
    posChargement.y = TAILLE_BLOC * 3;

    SDL_BlitSurface(chargement, NULL, fenetre, &posChargement);
    SDL_Flip(fenetre);
    quadrillage(labyrinthe, carte);
    getStartedPos(carte);
    getLab(labyrinthe, carte);
    cleen(carte, labyrinthe);
    SDL_Flip(labyrinthe);
    posLab.x = fenetre->w/2 - labyrinthe->w/2;
    posLab.y = TAILLE_BLOC;
    SDL_FillRect(fenetre, NULL, SDL_MapRGB(fenetre->format, 15, 115, 236));
    SDL_BlitSurface(labyrinthe, NULL, fenetre, &posLab);
    SDL_Flip(fenetre);
}

void quadrillage(SDL_Surface *fenetre, int **carte){
    SDL_Surface *bloc = NULL;
    SDL_Rect pos;
    bloc = SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLE_BLOC, TAILLE_BLOC, 32, 0, 0, 0, 0);
    SDL_FillRect(bloc, NULL, SDL_MapRGB(bloc->format, 0, 0, 0));
    int i = 0, j = 0;
    pos.x = 0;
    pos.y = 0;
    int ligne = 0, colonne = 0;
    for (i = 0; i < (DIM - 1) * TAILLE_BLOC; i+=TAILLE_BLOC){
        SDL_BlitSurface(bloc, NULL, fenetre, &pos);
        carte[ligne][colonne] = BLOC;
        pos.x += TAILLE_BLOC;
        colonne++;
    }

    for (i = 0; i < (DIM - 1) * TAILLE_BLOC; i+=TAILLE_BLOC){
        SDL_BlitSurface(bloc, NULL, fenetre, &pos);
        carte[ligne][colonne] = BLOC;
        pos.y += TAILLE_BLOC;
        ligne++;
    }

    for (i = 0; i < (DIM - 1) * TAILLE_BLOC; i+=TAILLE_BLOC){
        SDL_BlitSurface(bloc, NULL, fenetre, &pos);
        carte[ligne][colonne] = BLOC;
        pos.x -= TAILLE_BLOC;
        colonne--;
    }

    for (i = 0; i < (DIM - 1) * TAILLE_BLOC; i+=TAILLE_BLOC){
    SDL_BlitSurface(bloc, NULL, fenetre, &pos);
        carte[ligne][colonne] = BLOC;
        pos.y -= TAILLE_BLOC;
        ligne--;
    }
    int paint = 1;
    pos.x = 0;
    pos.y = 0;
    ligne = 0;colonne = 0;
    for (j = 0; j < DIM * TAILLE_BLOC; j+=TAILLE_BLOC){
        for (i = 0; i < DIM * TAILLE_BLOC; i+=TAILLE_BLOC){
            if (paint){
                SDL_BlitSurface(bloc, NULL, fenetre, &pos);
                carte[ligne][colonne] = BLOC;
            }
            paint = 1 - paint;
            pos.x += TAILLE_BLOC;
            colonne++;
        }
        paint = 1;
        pos.y += TAILLE_BLOC;
        pos.x = 0;
        colonne = 0;
        ligne++;
    }
    pos.x = 0;
    pos.y = 0;
    paint = 1;
    ligne = 0;colonne = 0;
    for (j = 0; j < DIM * TAILLE_BLOC; j+=TAILLE_BLOC){
        for (i = 0; i < DIM * TAILLE_BLOC; i+=TAILLE_BLOC){
        if (paint){
            SDL_BlitSurface(bloc, NULL, fenetre, &pos);
            carte[ligne][colonne] = BLOC;
        }
        paint = 1 - paint;
        pos.y += TAILLE_BLOC;
        ligne++;
        }
        paint = 1;
        pos.x += TAILLE_BLOC;
        pos.y = 0;
        ligne = 0;
        colonne++;
    }
    SDL_FreeSurface(bloc);
}

void getCarte(int **carte){
    int ligne = 0, colonne = 0;
    for (ligne = 0; ligne < DIM; ligne++){
        for (colonne = 0; colonne < DIM; colonne++){
            printf("%d|", carte[ligne][colonne]);
        }
        printf("\n");
    }
}

void getStartedPos(int **carte){
    int ligne = 0, colonne = 0;
    srand(time(NULL));
    ligne = rand()%((DIM - 1)-1) +1;

    srand(time(NULL) * 2 / 3);
    colonne = rand()%((DIM - 1)-1) +1;
    while (carte[ligne][colonne] == BLOC){
        if (colonne == 7) ligne++;
        else colonne++;
    }
    carte[ligne][colonne] = CURRENT;
}

void getRealPos(int *ligne, int *colonne){
    *colonne *= TAILLE_BLOC;
    *ligne *= TAILLE_BLOC;
}

int isFinish(int **carte){
    int ligne = 0, colonne = 0;
    for (ligne = 0; ligne < DIM; ligne++){
        for (colonne = 0; colonne < DIM; colonne++){
            if (carte[ligne][colonne] == VIDE) return 0;
        }
    }
    return 1;
}

void getLab(SDL_Surface *fenetre, int **carte){
    SDL_Surface *vide;
    SDL_Rect pos;SDL_Event event;

    vide = SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLE_BLOC, TAILLE_BLOC, 32, 0, 0, 0, 0);

    SDL_FillRect(vide, NULL, SDL_MapRGB(vide->format, 255, 255, 255));

    int ligne = 0;int colonne = 0;int nbrNear = 0, i = 0;int aleatoire = 0;
    int nbrPrec = 0, exec = 0, hasArriver = 0;

    coordonnee depart, arriver;
    getCurrent(carte, &ligne, &colonne);
    depart.colonne = colonne;depart.ligne = ligne;

    coordonnee near1, near2, near3, near4, actuel, precedent[DIM*DIM*DIM];
    coordonnee tab[4] = {near1, near2, near3, near4};
    i = 0;
    while (i < DIM*DIM*DIM){
        coordonnee cord = {0, 0};
        precedent[i] = cord;
        i++;
    }

    while (!isFinish(carte)){
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) break;
        SDL_GetTicks();
        srand(time(NULL) * exec * DIM);
        getNear(carte, tab, &nbrNear);
        getCurrent(carte, &ligne, &colonne);
        actuel.colonne = colonne;actuel.ligne = ligne;
        if (nbrPrec >= (int)TROU/2 && !hasArriver){
            arriver.ligne = actuel.ligne;
            arriver.colonne = actuel.colonne;
            hasArriver = 1;
        }

        if (nbrNear == 0){
            carte[precedent[nbrPrec - 1].ligne][precedent[nbrPrec - 1].colonne] = CURRENT;
            carte[actuel.ligne][actuel.colonne] = CUL;
            nbrPrec-=1;
        }else{
            if (nbrNear == 1) aleatoire = 0;
            else{
            srand(time(NULL) * exec);
            aleatoire = rand() % (nbrNear-0) + 0;
            }
            getCurrent(carte, &ligne, &colonne);
            carte[ligne][colonne] = VISITED;
            ligne = tab[aleatoire].ligne;colonne = tab[aleatoire].colonne;
            carte[ligne][colonne] = CURRENT;
            setPont(carte, tab[aleatoire], actuel, fenetre);
            for (i = 0; i < nbrNear; i++){
                if (i != aleatoire){
                    ligne = tab[i].ligne;colonne = tab[i].colonne;
                    getRealPos(&ligne, &colonne);
                    pos.x = colonne;
                    pos.y = ligne;
                    SDL_BlitSurface(vide, NULL, fenetre, &pos);
                }
            }
        }
        SDL_Flip(fenetre);
        precedent[nbrPrec].colonne = actuel.colonne;
        precedent[nbrPrec].ligne = actuel.ligne;
        if (nbrNear != 0)nbrPrec++;
        exec++;
    }

    carte[arriver.ligne][arriver.colonne] = ARRIVER;
    carte[depart.ligne][depart.colonne] = DEPART;
    SDL_FreeSurface(vide);
}

void getCurrent(int **carte, int *ligne, int *colonne){
    int i = 0, j = 0;
    for (i = 0; i < DIM; i++){
        for (j = 0; j < DIM; j++){
            if (carte[i][j] == CURRENT){
                *ligne = i;*colonne = j;
                i = DIM;j = DIM;
            }
        }
    }
}

void getNear(int **carte, coordonnee tab[4], int *nbrNear){
    int ligne = 0;int colonne = 0;
    getCurrent(carte, &ligne, &colonne);
    *nbrNear = 0;

    if (ligne+2 < DIM){
        if (carte[ligne+2][colonne] == VIDE){
            tab[*nbrNear].ligne = ligne + 2;
            tab[*nbrNear].colonne = colonne;
            *nbrNear+=1;
        }
    }

    if (ligne-2 >= 0){
        if (carte[ligne-2][colonne] == VIDE){
            tab[*nbrNear].ligne = ligne - 2;
            tab[*nbrNear].colonne = colonne;
            *nbrNear+=1;
        }
    }

    if (colonne+2 < DIM){
        if (carte[ligne][colonne+2] == VIDE){
            tab[*nbrNear].ligne = ligne;
            tab[*nbrNear].colonne = colonne + 2;
            *nbrNear+=1;
        }
    }

    if (colonne-2 >= 0){
        if (carte[ligne][colonne-2] == VIDE){
            tab[*nbrNear].ligne = ligne;
            tab[*nbrNear].colonne = colonne - 2;
            *nbrNear+=1;
        }
    }

}

void setPont (int **carte, coordonnee choose, coordonnee current, SDL_Surface *fenetre){
    SDL_Surface *vide = SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLE_BLOC, TAILLE_BLOC, 32, 0, 0, 0, 0);
    SDL_FillRect(vide, NULL, SDL_MapRGB(vide->format, 255, 255, 255));
    SDL_Rect pos;
    int ligne = 0, colonne = 0;
    ligne = current.ligne;
    colonne = current.colonne;

    if (choose.colonne < current.colonne){
        colonne--;
    }
    if (choose.colonne > current.colonne){
        colonne++;
    }
    if (choose.ligne < current.ligne){
        ligne--;
    }
    if (choose.ligne > current.ligne){
        ligne++;
    }
    carte[ligne][colonne] = PONT;
    getRealPos(&ligne, &colonne);
    pos.x = colonne;pos.y = ligne;
    SDL_BlitSurface(vide, NULL, fenetre, &pos);
    SDL_FreeSurface(vide);
}

void cleen(int **carte, SDL_Surface *fenetre){
    SDL_Surface *vide = SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLE_BLOC, TAILLE_BLOC, 32, 0, 0, 0, 0);
    SDL_Surface *arriver, *depart;
    char InsD[50], InsA[50];
    arriver = SDL_LoadBMP(FILE_ARRIVER);
    depart = SDL_LoadBMP(FILE_DEPART);
    SDL_SetColorKey(depart, SDL_SRCCOLORKEY, SDL_MapRGB(depart->format, 255, 255, 255));
    SDL_FillRect(vide, NULL, SDL_MapRGB(vide->format, 255, 255, 255));
    int ligne = 0, colonne = 0, l2 = 0, col2 = 0;
    SDL_Rect pos;
    for (ligne = 0; ligne < DIM; ligne++){
        for (colonne = 0; colonne < DIM; colonne++){
            if (carte[ligne][colonne] != BLOC){
                l2 = ligne;col2 = colonne;
                getRealPos(&l2, &col2);
                pos.x = col2;pos.y = l2;
                SDL_BlitSurface(vide, NULL, fenetre, &pos);
            }
            if (carte[ligne][colonne] == ARRIVER){
                SDL_BlitSurface(arriver, NULL, fenetre, &pos);
            }
            if (carte[ligne][colonne] == DEPART){
                SDL_BlitSurface(depart, NULL, fenetre, &pos);
            }
        }
    }
    SDL_FreeSurface(vide);
    SDL_FreeSurface(arriver);
    SDL_FreeSurface(depart);
}

void solve(int **carte, SDL_Surface *fenetre){
    SDL_Surface *depart, *surfaceTemp, *lab;
    depart = SDL_LoadBMP(FILE_DEPART);
    SDL_SetColorKey(depart, SDL_SRCCOLORKEY, SDL_MapRGB(depart->format, 255, 255, 255));
    SDL_Rect pos, posCur, posPrecedent[2];SDL_Event event;
    SDL_Surface *vide = SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLE_BLOC, TAILLE_BLOC, 32, 0, 0, 0, 0);
    SDL_Surface *trait = SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLE_BLOC / 2, TAILLE_BLOC/2, 32, 0, 0, 0, 0);
    surfaceTemp = SDL_CreateRGBSurface(SDL_HWSURFACE, fenetre->w, fenetre->h, 32, 0, 0, 0, 0);
    lab = SDL_CreateRGBSurface(SDL_HWSURFACE, fenetre->w, fenetre->h, 32, 0, 0, 0, 0);
    SDL_FillRect(vide, NULL, SDL_MapRGB(vide->format, 255, 255, 255));
    SDL_FillRect(trait, NULL, SDL_MapRGB(trait->format, 20, 255, 20));


    int ligne = 0, colonne = 0, precedent = 0, i = 0;
    coordonnee posDepart;
    getDepart(carte, &posDepart);
    ligne = posDepart.ligne;colonne = posDepart.colonne;
    getRealPos(&ligne, &colonne);
    pos.x = colonne;pos.y = ligne;
    SDL_BlitSurface(vide, NULL, fenetre, &pos);
    SDL_Flip(fenetre);
    pos.x = 0;pos.y = 0;
    SDL_BlitSurface(fenetre, NULL, surfaceTemp, &pos);
    SDL_BlitSurface(fenetre, NULL, lab, &pos);
    SDL_Flip(surfaceTemp);
    SDL_Flip(lab);
    int tabEnv[4], direction = NORD;
    coordonnee current = {posDepart.ligne, posDepart.colonne}, arriver, tabPos[4], tabPos2[DIM * DIM];
    getArriver(carte, &arriver);
    int ins = 1;int decompte = 0, trans = 0, exec = 0, nbrPos = 0;
    ligne = current.ligne;
    colonne = current.colonne;
    getRealPos(&ligne, &colonne);
    posCur.x = colonne;
    posCur.y = ligne;
    SDL_BlitSurface(depart, NULL, fenetre, &posCur);
    SDL_Flip(fenetre);
    SDL_Delay(1000);
    initDirection(carte, &direction, current);

    while (!Win(arriver, current)){
        SDL_FillRect(trait, NULL, SDL_MapRGB(trait->format, 0+exec * 5, 255 - exec * 5, exec));
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) break;
        setDirection(carte, direction, tabEnv, current);
        setPos(carte, direction, tabPos, current);

        if (ins == 1){
            if (tabEnv[DEVANT] == BLOC){
                ins = 2;
                trans = 1;
            }
            else{
                current = tabPos[DEVANT];
            }
        }
        if (ins == 2){
            if (trans){
                if (tabEnv[DROITE] == BLOC && tabEnv[GAUCHE] != BLOC){
                    direction--;
                    decompte++;
                }
                else if (tabEnv[GAUCHE] == BLOC && tabEnv[DROITE] != BLOC){
                    direction++;
                    decompte--;
                }
                else{
                    direction -= 2;
                    decompte-=2;
                }
                if (direction < 0) direction+=4;
                if (direction > 3) direction-=4;
                trans = 0;
            }

            else{
            if (decompte == 0)ins = 1;
                else{
                    if (tabEnv[DROITE] != BLOC){
                        direction++;
                        decompte--;
                    }
                    else if (tabEnv[DEVANT] == BLOC){
                        if (tabEnv[GAUCHE] != BLOC){
                            direction--;
                            decompte++;
                        }
                        else {
                            direction -= 2;
                            decompte-=2;
                        }
                    }
                    if (direction < 0) direction+=4;
                    if (direction > 3) direction-=4;
                }
            }
            setPos(carte, direction, tabPos, current);
            current = tabPos[DEVANT];
        }
        tabPos2[nbrPos] = current;

        for  (i = 0; i < precedent; i++){
        posPrecedent[i].x = posPrecedent[i+1].x;
        posPrecedent[i].y = posPrecedent[i+1].y;
        }

        exec++;
        nbrPos++;
        if (precedent < 1)precedent++;
    }

    SDL_Delay(1000);

    int taille;
    coordonnee *plusCourt = delDoublePos(tabPos2, nbrPos, &taille);

    SDL_BlitSurface(lab, NULL, fenetre, &pos);
    ligne = posDepart.ligne;
    colonne = posDepart.colonne;
    getRealPos(&ligne, &colonne);
    pos.x = colonne;pos.y = ligne;
    SDL_BlitSurface(depart, NULL, fenetre, &pos);
    i = 0;
    while (i < taille-1 && !(plusCourt[i].ligne == posDepart.ligne && plusCourt[i].colonne == posDepart.colonne)){
        getRealPos(&plusCourt[i].ligne, &plusCourt[i].colonne);
        pos.x = plusCourt[i].colonne + (TAILLE_BLOC/2 - trait->w/2);
        pos.y = plusCourt[i].ligne + (TAILLE_BLOC/2 - trait->h/2);
        SDL_BlitSurface(trait, NULL, fenetre, &pos);
        SDL_Flip(fenetre);

        i++;
    }
    SDL_FreeSurface(depart);
    SDL_FreeSurface(surfaceTemp);
    SDL_FreeSurface(lab);
    SDL_FreeSurface(trait);
    SDL_FreeSurface(vide);
}

int Win(coordonnee arriver, coordonnee pos){
    if (arriver.colonne == pos.colonne && arriver.ligne == pos.ligne) return 1;
    else return 0;
}

void getArriver(int **carte, coordonnee *pos){
    int i = 0, j = 0;
    for (i = 0; i < DIM; i++){
        for (j = 0; j < DIM; j++){
            if (carte[i][j] == ARRIVER){
                pos->ligne = i;pos->colonne = j;
                i = DIM;j = DIM;
            }
        }
    }
}

void getDepart(int **carte, coordonnee *pos){
    int i = 0, j = 0;
    for (i = 0; i < DIM; i++){
        for (j = 0; j < DIM; j++){
            if (carte[i][j] == DEPART){
                pos->ligne = i;pos->colonne = j;
                i = DIM;j = DIM;
            }
        }
    }
}

void setDirection(int **carte, int direction, int tabDir[4], coordonnee pos){
    int ligne = pos.ligne, colonne = pos.colonne;
    if (direction == NORD){
        tabDir[DEVANT] = carte[ligne-1][colonne];
        tabDir[DERRIERE] = carte[ligne+1][colonne];
        tabDir[DROITE] = carte[ligne][colonne+1];
        tabDir[GAUCHE] = carte[ligne][colonne-1];
    }
    if (direction == SUD){
        tabDir[DEVANT] = carte[ligne+1][colonne];
        tabDir[DERRIERE] = carte[ligne-1][colonne];
        tabDir[DROITE] = carte[ligne][colonne-1];
        tabDir[GAUCHE] = carte[ligne][colonne+1];
    }
    if (direction == OUEST){
        tabDir[DEVANT] = carte[ligne][colonne-1];
        tabDir[DERRIERE] = carte[ligne+1][colonne+1];
        tabDir[DROITE] = carte[ligne-1][colonne];
        tabDir[GAUCHE] = carte[ligne+1][colonne];
    }
    if (direction == EST){
        tabDir[DEVANT] = carte[ligne][colonne+1];
        tabDir[DERRIERE] = carte[ligne+1][colonne-1];
        tabDir[DROITE] = carte[ligne+1][colonne];
        tabDir[GAUCHE] = carte[ligne-1][colonne];
    }
}

void setPos(int **carte, int direction, coordonnee tab[4], coordonnee pos){
    tab[DEVANT].colonne = pos.colonne;
    tab[DEVANT].ligne = pos.ligne;
    tab[DERRIERE].colonne = pos.colonne;
    tab[DERRIERE].ligne = pos.ligne;
    tab[DROITE].colonne = pos.colonne;
    tab[DROITE].ligne = pos.ligne;
    tab[GAUCHE].colonne = pos.colonne;
    tab[GAUCHE].ligne = pos.ligne;

    if (direction == NORD){
        tab[DEVANT].ligne--;
        tab[DERRIERE].ligne++;
        tab[DROITE].colonne++;
        tab[GAUCHE].colonne--;
    }
    if (direction == SUD){
        tab[DEVANT].ligne++;
        tab[DERRIERE].ligne--;
        tab[DROITE].colonne--;
        tab[GAUCHE].colonne++;
    }
    if (direction == OUEST){
        tab[DEVANT].colonne--;
        tab[DERRIERE].colonne++;
        tab[DROITE].ligne++;
        tab[GAUCHE].ligne--;
    }
    if (direction == EST){
        tab[DEVANT].colonne++;
        tab[DERRIERE].colonne--;
        tab[DROITE].ligne--;
        tab[GAUCHE].ligne++;
    }
}

void getDirection (int direction){
    if (direction == NORD) printf("NORD\n");
    if (direction == SUD) printf("SUD\n");
    if (direction == EST) printf("EST\n");
    if (direction == OUEST) printf("OUEST\n");
}

void initDirection(int **carte, int *direction, coordonnee pos){
    int ligne = pos.ligne, colonne = pos.colonne;
    if (carte[ligne][colonne+1] != BLOC) *direction = EST;
    else if (carte[ligne][colonne-1] != BLOC) *direction = OUEST;
    else if (carte[ligne+1][colonne] != BLOC) *direction = SUD;
    else *direction = NORD;
}

int containsPos(coordonnee *tab, coordonnee pos, int taille, int *pos2){
    int i = 0;
    *pos2 = -1;
    while (i < taille){
        if (tab[i].colonne == pos.colonne && tab[i].ligne == pos.ligne){
            *pos2 = i;
            i = taille;
        }
        i++;
    }
    if (*pos2 == -1) return 0;
    else return 1;
}

coordonnee* delDoublePos(coordonnee *tab, int taille, int *tailleRes){
    coordonnee *result = NULL, tab2[taille];
    int i = 0, pos = 0, taille2, j = 0;
    while (i < taille){
        tab2[i].ligne = -1;
        tab2[i].colonne = -1;
        i++;
    }
    i = 0;
    while (i < taille){

        if (containsPos(tab2, tab[i], taille, &pos))j = pos;
        else{
            tab2[j].colonne = tab[i].colonne;
            tab2[j].ligne = tab[i].ligne;
        }
        j++;
        i++;
    }
    i = 0;
    while (i < taille){
        i++;
    }
    taille2 = j;
    *tailleRes = j;
    while (j < taille){
        tab2[j].ligne = -1;
        tab2[j].colonne = -1;
        j++;
    }
    i = 0;

    result = (coordonnee*) malloc(sizeof(coordonnee) * taille2);
    i = 0;
    while (i < taille2){
        result[i].ligne = tab2[i].ligne;
        result[i].colonne = tab2[i].colonne;
        i++;
    }

    return result;

}

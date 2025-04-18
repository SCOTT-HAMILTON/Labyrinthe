#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "Constantes.h"
#include "Jeu.h"
#include "Lab.h"

void play(int **carte, SDL_Surface *labyrinthe, SDL_Surface *fenetre){
    initCarte(carte);
    SDL_Surface *lab, *depart, *vide, *win, *solveImg, *solve1, *solve2;
    SDL_Event event;
    int ligne = 0, colonne = 0, solveIt = 0;
    SDL_Rect posCurrent, pos, posLab, posS, posSolve;
    coordonnee current, arriver;

    getDepart(carte, &current);
    getArriver(carte, &arriver);
    solve1 = SDL_LoadBMP(FILE_SOLVE);
    solve2 = SDL_LoadBMP(FILE_SOLVE2);
    SDL_SetColorKey(solve1, SDL_SRCCOLORKEY, SDL_MapRGB(solve1->format, 255, 255, 255));
    depart = SDL_LoadBMP(FILE_DEPART);
    SDL_SetColorKey(depart, SDL_SRCCOLORKEY, SDL_MapRGB(depart->format, 255, 255, 255));
    vide = SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLE_BLOC, TAILLE_BLOC, 32, 0, 0, 0, 0);
    SDL_FillRect(vide, NULL, SDL_MapRGB(vide->format, 255, 255, 255));
    win = SDL_LoadBMP(FILE_GAGNE);
    ligne = current.ligne;
    colonne = current.colonne;
    getRealPos(&ligne, &colonne);
    posCurrent.x = colonne;
    posCurrent.y = ligne;
    SDL_BlitSurface(vide, NULL, labyrinthe, &posCurrent);
    lab = SDL_CreateRGBSurface(SDL_HWSURFACE, labyrinthe->w, labyrinthe->h, 32, 0, 0, 0, 0);
    pos.x = 0;pos.y = 0;
    SDL_BlitSurface(labyrinthe, NULL, lab, &pos);
    SDL_Flip(lab);
    SDL_BlitSurface(depart, NULL, labyrinthe, &posCurrent);
    posLab.y = TAILLE_BLOC;
    posLab.x = fenetre->w/2 - labyrinthe->w/2;
    SDL_BlitSurface(labyrinthe, NULL, fenetre, &posLab);

    solveImg = solve1;
    posSolve.x = fenetre->w/2 - solveImg->w/2;
    posSolve.y = labyrinthe->h + solveImg->h;
    SDL_BlitSurface(solveImg, NULL, fenetre, &posSolve);

    SDL_Flip(fenetre);
    int continuer = 1;
    SDL_EnableKeyRepeat(10, 50);
    while (!Win(arriver, current) && continuer){
        SDL_WaitEvent(&event);
        switch (event.type){
        case SDL_QUIT:
            continuer = 0;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_UP){
                if (getMove(carte, current, NORD)){
                    current.ligne--;
                }
            }
            if (event.key.keysym.sym == SDLK_DOWN){
                if (getMove(carte, current, SUD)){
                    current.ligne++;
                }
            }
            if (event.key.keysym.sym == SDLK_LEFT){
                if (getMove(carte, current, OUEST)){
                    current.colonne--;
                }
            }
            if (event.key.keysym.sym == SDLK_RIGHT){
                if (getMove(carte, current, EST)){
                    current.colonne++;
                }
            }
            break;
        case SDL_MOUSEMOTION:
            posS.y = event.motion.y;
            posS.x = event.motion.x;
            if (posS.y >= posSolve.y && posS.y <= posSolve.y+solveImg->w){
                if (posS.x >= posSolve.x && posS.x <= posSolve.x+solveImg->h){
                    solveImg = solve2;
                }else{
                    solveImg = solve1;
                }
            }else{
                solveImg = solve1;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            posS.y = event.button.y;
            posS.x = event.button.x;
            if (event.button.button == 1){
                if (posS.y >= posSolve.y && posS.y <= posSolve.y+solveImg->w){
                    if (posS.x >= posSolve.x && posS.x <= posSolve.x+solveImg->h){
                        continuer = 0;
                        solveIt = 1;
                    }
                }
            }
            break;
        }
        ligne = current.ligne;
        colonne = current.colonne;
        getRealPos(&ligne, &colonne);
        posCurrent.x = colonne;posCurrent.y = ligne;
        SDL_FillRect(fenetre, NULL, SDL_MapRGB(fenetre->format, 15, 115, 236));
        SDL_BlitSurface(lab, NULL, labyrinthe, &pos);
        SDL_BlitSurface(depart, NULL, labyrinthe, &posCurrent);
        SDL_Flip(labyrinthe);
        SDL_BlitSurface(labyrinthe, NULL, fenetre, &posLab);
        SDL_BlitSurface(solveImg, NULL, fenetre, &posSolve);
        SDL_Flip(fenetre);
    }
    if (Win(arriver, current)){
        posCurrent.x = labyrinthe->w/2 - win->w/2;
        posCurrent.y = labyrinthe->h/2 - win->h/2;
        SDL_BlitSurface(win, NULL, labyrinthe, &posCurrent);
        SDL_Flip(labyrinthe);
        SDL_BlitSurface(labyrinthe, NULL, fenetre, &posLab);
        SDL_Flip(fenetre);
    }
    if (solveIt){
        SDL_FillRect(fenetre, NULL, SDL_MapRGB(fenetre->format, 15, 115, 236));
        solveImg = solve1;
        SDL_BlitSurface(solveImg, NULL, fenetre, &posSolve);
        SDL_Flip(fenetre);
        cleen(carte, labyrinthe);
        SDL_Flip(labyrinthe);
        SDL_BlitSurface(labyrinthe, NULL, fenetre, &posLab);
        SDL_Flip(fenetre);
        solve(carte, labyrinthe);
        SDL_Flip(labyrinthe);
        SDL_BlitSurface(labyrinthe, NULL, fenetre, &posLab);
        SDL_Flip(fenetre);
    }
    SDL_FreeSurface(vide);
    SDL_FreeSurface(lab);
    SDL_FreeSurface(depart);
    SDL_FreeSurface(win);
    SDL_FreeSurface(solve1);
    SDL_FreeSurface(solve2);
}

int getMove(int **carte, coordonnee pos, int direction){
    printf("%d", carte[pos.ligne][pos.colonne]);
    if (direction == NORD && carte[pos.ligne-1][pos.colonne] != BLOC)return 1;
    if (direction == SUD && carte[pos.ligne+1][pos.colonne] != BLOC)return 1;
    if (direction == OUEST && carte[pos.ligne][pos.colonne-1] != BLOC)return 1;
    if (direction == EST && carte[pos.ligne][pos.colonne+1] != BLOC)return 1;
    return 0;
}

void initCarte(int **carte){
    int ligne = 0, colonne = 0;
    for (ligne = 0; ligne < DIM; ligne++){
        for (colonne = 0; colonne < DIM; colonne++){
            if (carte[ligne][colonne] != BLOC && carte[ligne][colonne] != ARRIVER && carte[ligne][colonne] != DEPART)carte[ligne][colonne] = VIDE;
        }
    }
}


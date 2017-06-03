#include <stdio.h>
#include <SDL.h>
#include "Constantes.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "Lab.h"

int main (int argc, char *argv[]){
    SDL_Surface *fenetre = NULL, *imgMenu;
    SDL_Surface *labyrinthe = NULL;
    SDL_Event event;SDL_Rect posMenu;
    SDL_Init(SDL_INIT_VIDEO);
    fenetre = SDL_SetVideoMode(TAILLE_BLOC * DIM + 100, TAILLE_BLOC * DIM+100, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
    imgMenu = SDL_LoadBMP("MenuAnim/Labyrinthe.bmp");
    posMenu.x = fenetre->w/2 - imgMenu->w/2;
    posMenu.y = fenetre->h/4;
    SDL_FillRect(fenetre, NULL, SDL_MapRGB(fenetre->format, 15, 115, 236));
    SDL_BlitSurface(imgMenu, NULL, fenetre, &posMenu);
    SDL_Flip(fenetre);

    int **carte = NULL;int *carte2 = NULL;
    if (!(carte = (int **) malloc ( sizeof(int *)  *  DIM))){
        return EXIT_FAILURE;
    }
    if (!(carte2 = (int*) malloc(sizeof(int)*DIM*DIM))){
        return EXIT_FAILURE;
    }
    int i = 0;
    for (i = 0; i < DIM; i++){
        SDL_GetTicks();
        carte[i] = &carte2[i * DIM];
    }
    int ligne = 0, colonne = 0;
    for (ligne = 0; ligne < DIM; ligne++){
        for (colonne = 0; colonne < DIM; colonne++){
            carte[ligne][colonne] = VIDE;
            SDL_GetTicks();
        }
    }

    if (carte == NULL)return EXIT_FAILURE;
    labyrinthe = SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLE_BLOC * DIM, TAILLE_BLOC * DIM, 32, 0, 0, 0, 0);
    SDL_FillRect(labyrinthe, NULL, SDL_MapRGB(fenetre->format, 255, 255, 255));
    startMenu(fenetre); //anim di menu
    generer(fenetre, labyrinthe, carte); //genere le lab
    play(carte, labyrinthe, fenetre); //fait jouer
    SDL_Flip(fenetre);


    int continuer = 1;
    while (continuer){
        SDL_GetTicks();
        SDL_WaitEvent(&event);
        switch (event.type){
        case SDL_QUIT:
            continuer = 0;
            break;
        }

        SDL_Flip(fenetre);
    }

    for (i = 0; i < DIM; i++){
        free(carte[i]);
    }

    SDL_Quit();
    return EXIT_SUCCESS;
}





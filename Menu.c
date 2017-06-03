#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include "Menu.h"
#include "Constantes.h"
#include <dirent.h>
#ifndef WIN32
    #include <sys/types.h>
#endif

void startMenu(SDL_Surface *fenetre){
    SDL_Event event;SDL_Rect posAnim, posPlay, posS;
    SDL_Surface *anim, *playButton;

    int continuer = 1, nbrMenu = 0, play = 0;
    char *filename[15][10];
    int i = 0;

    char InsA[50];
    i = 0;
    while (i < 15){
        sprintf(filename[i], "MenuAnim/Men(%d).bmp", i+1);
        i++;
    }

    SDL_Surface *play2 = SDL_LoadBMP("MenuAnim/play2.bmp");
    SDL_Surface *play1 = SDL_LoadBMP("MenuAnim/play.bmp");
    SDL_SetColorKey(play1, SDL_SRCCOLORKEY, SDL_MapRGB(play1->format, 255, 255, 255));
    playButton = play1;

    anim = SDL_LoadBMP(filename[nbrMenu]);
    posAnim.y = TAILLE_BLOC/2;
    posAnim.x = fenetre->w/2 - anim->w/2;
    posPlay.x = fenetre->w/2 - playButton->w/2;
    posPlay.y = anim->h;
    while (nbrMenu<15){
        SDL_GetTicks();
        anim = SDL_LoadBMP(filename[nbrMenu]);
        if (anim == NULL)printf("null!\n");
        SDL_BlitSurface(anim, NULL, fenetre, &posAnim);
        nbrMenu++;
        SDL_Delay(100);
        SDL_Flip(fenetre);
    }
     i = 0;
    SDL_Surface *menu = SDL_CreateRGBSurface(SDL_HWSURFACE, fenetre->w, fenetre->h, 32, 0, 0, 0, 0);
    posAnim.x = 0;
    posAnim.y = 0;
    SDL_BlitSurface(fenetre, NULL, menu, &posAnim);
    SDL_BlitSurface(playButton, NULL, fenetre, &posPlay);
    while (!play && continuer){
        SDL_WaitEvent(&event);
        switch(event.type){
        case SDL_QUIT:
            continuer = 0;
            break;
        case SDL_MOUSEMOTION:
            posS.x = event.motion.x;
            posS.y = event.motion.y;
            if (posS.y >= posPlay.y && posS.y <= posPlay.y+posPlay.w){
                if (posS.x >= posPlay.x && posS.x <= posPlay.x+posPlay.h){
                    playButton = play2;
                }else{
                    playButton = play1;
                }
            }else{
                playButton = play1;
            }
        case SDL_MOUSEBUTTONUP:
            posS.x = event.motion.x;
            posS.y = event.motion.y;
            if (event.button.button == 1){
                if (posS.y >= posPlay.y && posS.y <= posPlay.y+posPlay.w){
                    if (posS.x >= posPlay.x && posS.x <= posPlay.x+posPlay.h){
                        play = 1;
                    }
                }
            }
        }

        SDL_BlitSurface(menu, NULL, fenetre, &posAnim);
        SDL_BlitSurface(playButton, NULL, fenetre, &posPlay);
        SDL_Flip(fenetre);
    }
    SDL_FillRect(fenetre, NULL, SDL_MapRGB(fenetre->format, 15, 115, 236));
    SDL_Flip(fenetre);
}

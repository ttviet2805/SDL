#include "systemProcess.h"

void systemProcess() {
    SDL_Window* gWindow = NULL;
    SDL_Renderer* gRenderer = NULL;
    SDL_Event event;

    if(!init(gWindow, gRenderer, "Test")) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else {
        SDL_RenderClear(gRenderer);

        Button TestButton = {300, 200, 100, 700, 4, BLACK, LIGHTBLUE, RED, BLACK, "a a a a a a a a a a aass a asss as as", 28};

        bool quit = false;
        while(!quit) {
            while(SDL_PollEvent(&event) != 0) {
                if(event.type == SDL_QUIT) {
                    quit = true;
                    break;
                }

//                TestButton.TypeBox = 1;
//                TestButton.isTextBox(gRenderer, &event);

                TestButton.Display(gRenderer);

                SDL_RenderPresent(gRenderer);
            }
        }
    }
}

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

        Button TestButton = {300, 200, 500, 50, 4, BLACK, LIGHTBLUE, RED, BLACK, " ", 28};
        TestButton.TypeBox = 1;
        TestButton.padding_hoz = 10;

        bool quit = false;
        while(!quit) {
            while(SDL_PollEvent(&event) != 0) {
                if(event.type == SDL_QUIT) {
                    quit = true;
                    break;
                }

                TestButton.isTextBox(gRenderer, &event);

                TestButton.Display(gRenderer);

                SDL_RenderPresent(gRenderer);
            }
        }
    }
}

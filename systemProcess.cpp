#include "systemProcess.h"

void systemProcess() {
    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* texture = NULL;
    SDL_Event event;

    if(!init(window, renderer, "Test")) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else {
        //Get window surface
        screenSurface = SDL_GetWindowSurface(window);

        //Fill the surface white
        SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

        //Update the surface
        SDL_UpdateWindowSurface(window);

        SDL_RenderClear(renderer);

        Button TestButton = {SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 20, 100, 40, 4, 4, BLACK, LIGHTBLUE, RED, BLACK, ""};
        TestButton.Display(renderer);

        SDL_RenderPresent(renderer);

        bool quit = false;
        while(!quit) {
            while(SDL_PollEvent(&event) != 0) {
                if(event.type == SDL_QUIT) {
                    quit = true;
                    break;
                }
                int MouseType = isMouseClick(&event, &TestButton.bRect);
                TestButton.FillCol = TestButton.InitCol;
                if(MouseType == 1) {
                    printf("left\n");
                    TestButton.FillCol = TestButton.PressCol;
                }
                else if(MouseType == 2) {
                    printf("hover\n");
                    TestButton.FillCol = TestButton.HoverCol;
                }
                TestButton.Display(renderer);

                SDL_RenderPresent(renderer);
            }
        }
    }

    SDL_FreeSurface(screenSurface);

    //Destroy window
    SDL_DestroyWindow(window);

    //Quit SDL subsystems
    SDL_Quit();
}

#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

#include "Const.h"

/*
//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* gXOut = NULL;

*/


//Starts up SDL and creates window
bool init(SDL_Window* &gWindow, SDL_Renderer* &gRenderer, string Title);

//Loads individual image as texture
SDL_Texture* loadTexture(SDL_Renderer* &gRenderer, string path);

//Loads media
bool loadMedia(SDL_Texture* &gTexture, SDL_Renderer* &gRenderer, string imageFileName);

SDL_Point getMousePos();

//return 0 if no click or right click
//return 1 if left click
//return 2 if hover
int isMouseClick(SDL_Event* event, SDL_Rect* button);

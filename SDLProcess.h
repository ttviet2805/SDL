#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

#include "Const.h"
#include "TextSolve.h"

bool init(SDL_Window* &gWindow, SDL_Renderer* &gRenderer, string Title);

SDL_Texture* loadTexture(SDL_Renderer* &gRenderer, string path );

bool loadImage(SDL_Renderer* &gRenderer, SDL_Texture* &gTexture, string fileName);

bool loadText(SDL_Renderer* &gRenderer, TTF_Font* gFont, TextOutput* &gTextOutput, SDL_Color textColor, string fileName, int textSize);

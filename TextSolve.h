#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

#include "Const.h"

struct TextOutput {
    //The actual hardware texture
    SDL_Texture* mTexture = NULL;
    TTF_Font* mFont;
    SDL_Color textColor;

    //Image dimensions
    int mWidth;
    int mHeight;
    int textSize;

    TextOutput(SDL_Color _textColor, int _textSize) {
        mTexture = NULL;
        textColor = _textColor;
        textSize = _textSize;
        mWidth = 0;
        mHeight = 0;
        mFont = NULL;
    }

    void free();

    //Creates image from font string
    bool loadFromRenderedText(SDL_Renderer* &gRenderer, string textureText);

    //Renders texture at given point
    void Display(SDL_Renderer* &gRenderer, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

    bool loadText(SDL_Renderer* &gRenderer, string curText, string fileName);
};

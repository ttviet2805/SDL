#pragma once

#include <SDL.h>
#include <SDL_image.h>

struct Button {
    SDL_Rect bRect;
    SDL_Texture* bImage;
    int boder, padding;
    SDL_Color OutlineCol;
    SDL_Color FillCol;

    Button() {
        bImage = NULL;
        boder = 0;
        padding = 0;
        OutlineCol = {0, 0, 0, 0};
        FillCol = {0, 0, 0, 0};
    }
    Button(int bx, int by, int bw, int bh, int _boder, int _padding, SDL_Color _OutlineCol, SDL_Color _FillCol) {
        bRect = {bx, by, bw, bh};
        boder = _boder;
        padding = _padding;
        OutlineCol = _OutlineCol;
        FillCol = _FillCol;
    }

    void Display(SDL_Renderer* &renderer) {
        SDL_SetRenderDrawColor(renderer, OutlineCol.r, OutlineCol.g, OutlineCol.b, OutlineCol.a);
        SDL_RenderFillRect(renderer, &bRect);
        SDL_Rect tRect = {bRect.x + boder, bRect.y + boder, bRect.w - 2 * boder, bRect.h - 2 * boder};
        SDL_SetRenderDrawColor(renderer, FillCol.r, FillCol.g, FillCol.b, FillCol.a);
        SDL_RenderFillRect(renderer, &tRect);
    }
};

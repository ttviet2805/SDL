#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include "TextSolve.h"
#include "Const.h"

struct Button {
    SDL_Rect bRect;
    SDL_Texture* bImage;
    int boder, padding_hoz;
    SDL_Color InitCol;
    SDL_Color OutlineCol;
    SDL_Color FillCol;
    SDL_Color HoverCol;
    SDL_Color PressCol;
    string Text;
    int TextSize;
    bool TypeBox;

    Button() {
        bImage = NULL;
        boder = 0;
        padding_hoz = 0;
        InitCol = {0, 0, 0, 0};
        OutlineCol = {0, 0, 0, 0};
        FillCol = {0, 0, 0, 0};
        HoverCol = {0, 0, 0, 0};
        PressCol = {0, 0, 0, 0};
        Text = "";
        TextSize = 0;
        TypeBox = 0;
    }

    Button(int bx, int by, int bw, int bh, int _boder, SDL_Color _OutlineCol, SDL_Color _FillCol, SDL_Color _HoverCol, SDL_Color _PressCol, string _Text, int _TextSize) {
        bRect = {bx, by, bw, bh};
        boder = _boder;
        OutlineCol = _OutlineCol;
        InitCol = _FillCol;
        FillCol = _FillCol;
        HoverCol = _HoverCol;
        PressCol = _PressCol;
        Text = _Text;
        TextSize = _TextSize;
    }

    //return 0 if no click or right click
    //return 1 if left click
    //return 2 if hover
    int isMouseClick(SDL_Event* event) {
        if(event->type != SDL_MOUSEMOTION && event->type != SDL_MOUSEBUTTONDOWN) return 0;
        if(event->button.button == SDL_BUTTON_RIGHT) return 0;
        int type = 2;
        if(event->button.button == SDL_BUTTON_LEFT) type = 1;
        int x, y;
        SDL_GetMouseState(&x, &y);
        if(x < bRect.x || y < bRect.y || x > bRect.x + bRect.w || y > bRect.y + bRect.h) return 0;
        return type;
    }

    void Display(SDL_Renderer* &gRenderer) {
        SDL_SetRenderDrawColor(gRenderer, OutlineCol.r, OutlineCol.g, OutlineCol.b, OutlineCol.a);
        SDL_RenderFillRect(gRenderer, &bRect);
        SDL_Rect tRect = {bRect.x + boder, bRect.y + boder, bRect.w - 2 * boder, bRect.h - 2 * boder};
        SDL_SetRenderDrawColor(gRenderer, FillCol.r, FillCol.g, FillCol.b, FillCol.a);
        SDL_RenderFillRect(gRenderer, &tRect);

        TextOutput tmp = TextOutput(BLACK, TextSize);
        tmp.loadText(gRenderer, Text, FONTDIR);
        if(!TypeBox) {
            tmp.Display(gRenderer, bRect.x + bRect.w / 2 - tmp.mWidth / 2, bRect.y + bRect.h / 2 - tmp.mHeight / 2);
        }
        else {
            tmp.Display(gRenderer, bRect.x + padding_hoz, bRect.y + bRect.h / 2 - tmp.mHeight / 2);
        }
    }

    bool isTextBox(SDL_Renderer* &gRenderer, SDL_Event* event) {
        if(isMouseClick(event) != 1) return false;
        Display(gRenderer);
        //Enable text input
        SDL_StartTextInput();
        bool quit = false;
        while(!quit)
        {
            SDL_Event e;
            while(SDL_PollEvent(&e) != 0) {
                //The rerender text flag
                bool renderText = false;

                if(e.type == SDL_QUIT) {
                    quit = true;
//                    exit(0);
                    break;
                }
                else if(e.type == SDL_KEYDOWN) {
                    if(e.key.keysym.sym == SDLK_BACKSPACE && Text.length() > 0) {
                        Text.pop_back();
                        renderText = true;
                    }
                    else if(e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) {
                        SDL_SetClipboardText(Text.c_str());
                    }
                    else if(e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
                        Text = SDL_GetClipboardText();
                        renderText = true;
                    }
                    else if(e.key.keysym.scancode == SDL_SCANCODE_RETURN) {
                        quit = true;
                        return true;
                    }
                }
                else if(e.type == SDL_TEXTINPUT) {
                    if(!(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V')))
                    {
                        //Append character
                        TextOutput tmp = TextOutput(BLACK, TextSize);
                        tmp.loadText(gRenderer, Text + e.text.text, FONTDIR);
                        if(tmp.mWidth <= bRect.w - padding_hoz * 2) {
                            Text += e.text.text;
                            renderText = true;
                        }
                    }
                }
                if(renderText) {
                    //Text is not empty
                    if(Text != "") {
                        Display(gRenderer);
                    }
                    //Text is empty
                    else {
                        Display(gRenderer);
                    }
                }
                SDL_RenderPresent(gRenderer);
            }
        }

        return true;
    }
};

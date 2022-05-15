#include "TextSolve.h"

void TextOutput::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
	    TTF_CloseFont( mFont );
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
        textColor = {0, 0, 0, 0};
        textSize = 0;
        mWidth = 0;
        mHeight = 0;
        mFont = NULL;
	}
}

bool TextOutput::loadFromRenderedText(SDL_Renderer* &gRenderer, string textureText)
{
    //Get rid of preexisting texture
    free();

    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid( mFont, textureText.c_str(), textColor );
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
        if( mTexture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }

    //Return success
    return mTexture != NULL;
}

void TextOutput::Display( SDL_Renderer* &gRenderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

bool TextOutput::loadText(SDL_Renderer* &gRenderer, string curText, string fileName) {
	//Loading success flag
	bool success = true;

	if(curText == "") return false;

	//Open the font
	mFont = TTF_OpenFont( FONTDIR.c_str(), textSize );
	if( mFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{
		//Render text
		if(!loadFromRenderedText( gRenderer, curText ) )
		{
			printf( "Failed to render text texture!\n" );
			success = false;
		}
	}

	return success;
}

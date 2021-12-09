#pragma once
#include <SDL.h>
#include "Log.h"

struct ChessPiece{
    SDL_Texture* imageTexture = nullptr;
    const char* color;
    const char* type;

    ChessPiece(){}
    ChessPiece(
        SDL_Texture* texture,
        const char* color_,
        const char* type_
    ) : imageTexture(texture), color(color_), type(type_) {}

    ~ChessPiece()
    {
        SDL_DestroyTexture(imageTexture);
    }

};
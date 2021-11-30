#pragma once
#include <SDL.h>
#include "Log.h"

struct ChessPiece{
    SDL_Texture* ImageTexture = nullptr;
    const char* Color;
    const char* Type;

    ChessPiece(){}
    ChessPiece(
        SDL_Texture* Texture_,
        const char* Color_,
        const char* Type_
    ) : ImageTexture(Texture_), Color(Color_), Type(Type_) {}

    ~ChessPiece()
    {
        SDL_DestroyTexture(ImageTexture);
    }

};

#include <SDL.h>
#include "Square.h"

Square::Square()
{

}

void Square::SetGridPosition(int row_, int col_)
{
    row = row_;
    col = col_;
    rect = {col * Square::WIDTH, row * Square::HEIGHT, Square::WIDTH, Square::HEIGHT };
}

void Square::Render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderCopy(renderer, Piece->ImageTexture, NULL, &rect);
}
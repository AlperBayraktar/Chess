#include <SDL.h>
#include <SDL_image.h>

#include "Log.h"
#include "Square.h"
#include "ChessPiece.h"


// Returns image texture of given filename
SDL_Texture* GetImageTexture(std::string filename, SDL_Renderer* renderer)
{
    // Path is set according to Makefile location, not .exe file
    std::string path = "../images/" + filename + ".png";
    return IMG_LoadTexture(renderer, path.c_str());
}


int main(int argc, char* argv[])
{
    //===========================================
    // Variables
    const int ROW_COUNT = 8;
    const int COL_COUNT = 8;

    SDL_Color BLACK = { 168, 108, 59, 255 };
    SDL_Color WHITE = { 255, 255, 255, 1 };
    // ===========================================

    // ===========================================
    // Board

    Square Squares[ROW_COUNT][COL_COUNT];

    const char* currentColor = "WHITE";

    for (int row=0; row<ROW_COUNT; row++)
    {
        for (int col=0; col<COL_COUNT; col++)
        {
            // Get square
            Square& square = Squares[row][col];
            // Set grid position and bg color
            square.SetGridPosition(row, col);
            currentColor == "BLACK" ? square.backgroundColor = BLACK : square.backgroundColor = WHITE;

            // Update currentColor
            currentColor == "BLACK" ? currentColor = "WHITE" : currentColor = "BLACK";
        }
        currentColor == "BLACK" ? currentColor = "WHITE" : currentColor = "BLACK";
    }
    // =========================================== 


    // ===========================================
    // Display
	
    // Initialize SDL and SDL_image

	if (SDL_Init(SDL_INIT_VIDEO) < 0) { ERROR("Couldn't initialize SDL: " << SDL_GetError()); return -1; }
    if (IMG_Init(IMG_INIT_PNG) < 0)   { ERROR("Couldn't initialize SDL_image: " << IMG_GetError()); return -1; }
	
    // Create window, renderer and surface

    SDL_Window* window = SDL_CreateWindow("Chess", 
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    ROW_COUNT * Square::WIDTH, COL_COUNT * Square::HEIGHT, 
    SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (window == NULL)
    { ERROR("Couldn't create Window: " << SDL_GetError()); return -1; }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface* surface =  SDL_GetWindowSurface(window);
    // ===========================================

    //===========================================
    // Create pieces

    SDL_Renderer* r = renderer;

    ChessPiece PAWN_WHITE(GetImageTexture("PawnWhite", r), "WHITE", "PAWN");
    ChessPiece PAWN_BLACK(GetImageTexture("PawnBlack", r), "BLACK", "PAWN");

    ChessPiece ROOK_WHITE(GetImageTexture("RookWhite", r), "WHITE", "ROOK");
    ChessPiece ROOK_BLACK(GetImageTexture("RookBlack", r), "BLACK", "ROOK");

    ChessPiece KNIGHT_WHITE(GetImageTexture("KnightWhite", r), "WHITE", "KNIGHT");
    ChessPiece KNIGHT_BLACK(GetImageTexture("KnightBlack", r), "BLACK", "KNIGHT");
    
    ChessPiece BISHOP_WHITE(GetImageTexture("BishopWhite", r), "WHITE", "BISHOP");
    ChessPiece BISHOP_BLACK(GetImageTexture("BishopBlack", r), "BLACK", "BISHOP");
    
    ChessPiece QUEEN_WHITE(GetImageTexture("QueenWhite", r), "WHITE", "QUEEN");
    ChessPiece QUEEN_BLACK(GetImageTexture("QueenBlack", r), "BLACK", "QUEEN");

    ChessPiece KING_WHITE(GetImageTexture("KingWhite", r), "WHITE", "KING");
    ChessPiece KING_BLACK(GetImageTexture("KingBlack", r), "BLACK", "KING");

    ChessPiece EMPTY(nullptr, "EMPTY", "EMPTY");

    ChessPiece Grid[ROW_COUNT][COL_COUNT] = {
        {   ROOK_BLACK,   KNIGHT_BLACK,  BISHOP_BLACK,  QUEEN_BLACK,  KING_BLACK,  BISHOP_BLACK,  KNIGHT_BLACK,  ROOK_BLACK   },
        {   PAWN_BLACK,   PAWN_BLACK  ,  PAWN_BLACK  ,  PAWN_BLACK ,  PAWN_BLACK,  PAWN_BLACK  ,  PAWN_BLACK  ,  PAWN_BLACK   },
        {   EMPTY     ,   EMPTY       ,  EMPTY       ,  EMPTY      ,  EMPTY     ,  EMPTY       ,  EMPTY       ,  EMPTY        },
        {   EMPTY     ,   EMPTY       ,  EMPTY       ,  EMPTY      ,  EMPTY     ,  EMPTY       ,  EMPTY       ,  EMPTY        },
        {   EMPTY     ,   EMPTY       ,  EMPTY       ,  EMPTY      ,  EMPTY     ,  EMPTY       ,  EMPTY       ,  EMPTY        },
        {   EMPTY     ,   EMPTY       ,  EMPTY       ,  EMPTY      ,  EMPTY     ,  EMPTY       ,  EMPTY       ,  EMPTY        },
        {   PAWN_WHITE,   PAWN_WHITE  ,  PAWN_WHITE  ,  PAWN_WHITE ,  PAWN_WHITE,  PAWN_WHITE  ,  PAWN_WHITE  ,  PAWN_WHITE   },
        {   ROOK_WHITE,   KNIGHT_WHITE,  BISHOP_WHITE,  QUEEN_WHITE,  KING_WHITE,  BISHOP_WHITE,  KNIGHT_WHITE,  ROOK_WHITE   }
    };

    for (int row=0; row<ROW_COUNT; row++)
    {
        for (int col=0; col<COL_COUNT; col++)
        {
            Square& square = Squares[row][col];
            square.Piece = &Grid[row][col];
        }
    }
    // ===========================================



    // ===========================================
    // Game loop

    SDL_Event evt;

    for (int row=0; row<ROW_COUNT; row++)
    {
        for (int col=0; col<COL_COUNT; col++)
        {
            Square& square = Squares[row][col];
            square.Render(renderer);
        }
    }
    SDL_RenderPresent(renderer);

    while (true)
    {
        while (SDL_WaitEvent(&evt))
        {
            switch(evt.type)
            {
                // Close the game
                case SDL_QUIT:  
                    return 0;
            }
        }
    }

    // ===========================================

    return 0;
}
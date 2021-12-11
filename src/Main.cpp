#include <SDL.h>
#include <SDL_image.h>
#include <vector>

#include "Log.h"
#include "Square.h"
#include "ChessPiece.h"
#include "Colors.h"
#include "BoardUtilities.h"


// Returns image texture of given filename
SDL_Texture* GetImageTexture(std::string filename, SDL_Renderer* renderer)
{
    // Path is set according to Makefile location, not .exe file
    std::string path = "../images/" + filename + ".png";
    return IMG_LoadTexture(renderer, path.c_str());
}

// An hash function that return string int int format
constexpr unsigned int hash(const char *s, int off = 0) {                        
    return !s[off] ? 5381 : (hash(s, off+1)*33) ^ s[off];                           
}

#define KNIGHT_MOVE(ROW, COL) HandleSquareSelectMoveableAndTakeables(&Squares[ROW][COL],  &takeableSquares, &moveableSquares, opponent);

int main(int argc, char* argv[])
{
    //===========================================
    // Constant variables
    const int ROW_COUNT = 8;
    const int COL_COUNT = 8;
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

    Square Squares[ROW_COUNT][COL_COUNT];

    for (int row=0; row<ROW_COUNT; row++)
    {
        for (int col=0; col<COL_COUNT; col++)
        {
            Square* square = &Squares[row][col];
            // Set grid position
            square->SetGridPosition(row, col);

            // Set bg color
            ResetSquareBg(square);

            // Render
            square->piece = &Grid[row][col];
            square->Render(renderer);
        }
    }
    // ===========================================



    // ===========================================
    // Game loop

    SDL_Event evt;
    
    const char* turn = "WHITE";
    Square* selectedSquare = nullptr;
    std::vector<Square*> moveableSquares;
    std::vector<Square*> takeableSquares;

    while (true)
    {
        while (SDL_WaitEvent(&evt))
        {
            switch(evt.type)
            {
                // Close the game
                case SDL_QUIT:  
                    return 0;
                
                case SDL_MOUSEBUTTONDOWN:
                    if (evt.button.button == SDL_BUTTON_LEFT)
                    {
                        // Get clicked square
                        int mouseX, mouseY;
                        SDL_GetMouseState(&mouseX, &mouseY);

                        int row = mouseY / Square::HEIGHT;
                        int col = mouseX / Square::WIDTH;

                        Square* clickedSquare = &Squares[row][col];

                        // Select
                        if (!selectedSquare && clickedSquare->piece->type != "EMPTY" && clickedSquare->piece->color == turn)
                        {
                            // Get opponent teams color
                            const char* opponent = turn == "WHITE" ? "BLACK" : "WHITE";

                            // Set moveable and takeable squares
                            switch (hash(clickedSquare->piece->type))
                            {
                                case hash("PAWN"):
                                    if (clickedSquare->piece->color == "WHITE")
                                    {
                                        if (clickedSquare->row == 6)
                                        {
                                            MakeMoveableIfEmpty(&Squares[6 - 2][clickedSquare->col], &moveableSquares);
                                        }

                                        int nextRow = clickedSquare->row - 1;
                                        if (nextRow >= 0)
                                        {
                                            MakeMoveableIfEmpty(&Squares[nextRow][clickedSquare->col], &moveableSquares);
                                            if (clickedSquare->col - 1 >= 0)
                                                MakeTakeableIfOpponent(&Squares[nextRow][clickedSquare->col - 1], &takeableSquares, opponent);
                                            if (clickedSquare->col + 1 <= 7)
                                                MakeTakeableIfOpponent(&Squares[nextRow][clickedSquare->col + 1], &takeableSquares, opponent);
                                        }
                                    }

                                    else if (clickedSquare->piece->color == "BLACK")
                                    {
                                        if (clickedSquare->row == 1)
                                        {
                                            MakeMoveableIfEmpty(&Squares[1 + 2][clickedSquare->col], &moveableSquares);
                                        }

                                        int nextRow = clickedSquare->row + 1;
                                        if (nextRow <= 7)
                                        {
                                            MakeMoveableIfEmpty(&Squares[nextRow][clickedSquare->col], &moveableSquares);
                                            if (clickedSquare->col - 1 >= 0)
                                                MakeTakeableIfOpponent(&Squares[nextRow][clickedSquare->col - 1], &takeableSquares, opponent);
                                            if (clickedSquare->col + 1 <= 7)
                                                MakeTakeableIfOpponent(&Squares[nextRow][clickedSquare->col + 1], &takeableSquares, opponent);
                                        }
                                    }
                                    break;

                                case hash("ROOK"):
                                    FindMoveableSquaresForRookOnAnAxis(Squares, clickedSquare, turn, opponent, &takeableSquares, &moveableSquares, "row", "inc");
                                    FindMoveableSquaresForRookOnAnAxis(Squares, clickedSquare, turn, opponent, &takeableSquares, &moveableSquares, "row", "dec");

                                    FindMoveableSquaresForRookOnAnAxis(Squares, clickedSquare, turn, opponent, &takeableSquares, &moveableSquares, "col", "inc");
                                    FindMoveableSquaresForRookOnAnAxis(Squares, clickedSquare, turn, opponent, &takeableSquares, &moveableSquares, "col", "dec");

                                    break;

                                case hash("KNIGHT"):
                                    if ( clickedSquare->col + 2 <= 7 )
                                    {
                                        if ( clickedSquare->row + 1 <= 7 )
                                            KNIGHT_MOVE(clickedSquare->row + 1, clickedSquare->col + 2);
                                        if ( clickedSquare->row - 1 >= 0 )
                                            KNIGHT_MOVE(clickedSquare->row - 1, clickedSquare->col + 2);
                                    }

                                    if ( clickedSquare->col - 2 >= 0)
                                    {
                                        if ( clickedSquare->row - 1 >= 0)
                                            KNIGHT_MOVE(clickedSquare->row - 1, clickedSquare->col - 2);
                                        if ( clickedSquare->row + 1 <= 7)
                                            KNIGHT_MOVE(clickedSquare->row + 1,clickedSquare->col - 2);
                                    }

                                    if ( clickedSquare->row - 2 >= 0 )
                                    {
                                        if ( clickedSquare->col + 1 <= 7 )
                                            KNIGHT_MOVE(clickedSquare->row - 2,clickedSquare->col + 1);
                                        if ( clickedSquare->col - 1 >= 0 )
                                            KNIGHT_MOVE(clickedSquare->row - 2, clickedSquare->col - 1);
                                    }

                                    if (clickedSquare->row + 2 <= 7)
                                    {
                                        if (clickedSquare->col + 1 <= 7)
                                            KNIGHT_MOVE(clickedSquare->row + 2, clickedSquare->col + 1);
                                        if (clickedSquare->col - 1 >= 0)
                                            KNIGHT_MOVE(clickedSquare->row + 2, clickedSquare->col - 1);
                                    }

                                    break;
                            }

                            for (Square* takeableSquare : takeableSquares)
                            {
                                takeableSquare->backgroundColor = RED;
                                takeableSquare->Render(renderer);
                            }
                            
                            for (Square* moveableSquare : moveableSquares)
                            {
                                moveableSquare->backgroundColor = GREEN;
                                moveableSquare->Render(renderer);
                            }

                            if (selectedSquare)
                            {
                                DeselectSquare(&takeableSquares, &moveableSquares, renderer, selectedSquare);
                            }

                            clickedSquare->backgroundColor = BLUE;
                            clickedSquare->Render(renderer);
                            selectedSquare = clickedSquare;
                            break;
                        }

                        // Deselect
                        if (selectedSquare && selectedSquare->row == clickedSquare->row && selectedSquare->col == clickedSquare->col)
                        {
                            DeselectSquare(&takeableSquares, &moveableSquares, renderer, selectedSquare);
                            selectedSquare = nullptr;
                            break;
                        }

                        // Move
                        if (selectedSquare)
                        {
                            // Combine moveableSquares and takeableSquares into one vector
                            std::vector<Square*> allMoveableSquares;

                            for (Square* moveableSquare : moveableSquares)
                            {
                                allMoveableSquares.push_back(moveableSquare);
                            }

                            for (Square* takeableSquare : takeableSquares)
                            {
                                allMoveableSquares.push_back(takeableSquare);
                            }

                            for (Square* moveableSquare : allMoveableSquares)
                            {
                                if (moveableSquare->row == clickedSquare->row && moveableSquare->col == clickedSquare->col)
                                {
                                    // Move the piece
                                    clickedSquare->piece = selectedSquare->piece;
                                    selectedSquare->piece = &EMPTY;                                
                                    
                                    // Deselect the old one
                                    DeselectSquare(&takeableSquares, &moveableSquares, renderer, selectedSquare);
                                    selectedSquare = nullptr;

                                    // Update turn
                                    turn = turn == "WHITE" ? "BLACK" : "WHITE";
                                    break;
                                }
                            }
                        }
                    }
            }
        }
    }

    // ===========================================

    return 0;
}
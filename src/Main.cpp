#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "Log.h"
#include "ChessPiece.h"
#include "Square.h"
#include "BoardUtilities.h"
#include <SDL_GUI.h>
#include "ChessColors.h"
#include <vector>
#include <string>

// Set path prefix according build type
#ifdef CMD
#define PATH_PREFIX std::string("../")
#endif

#ifdef CLICK_EXE
#define PATH_PREFIX std::string("../../")
#endif


// Returns image texture of given filename
SDL_Texture* GetImageTexture(std::string filename, SDL_Renderer* RENDERER)
{
    // Path is set according to Makefile location, not .exe file
    std::string path = PATH_PREFIX + "images/" + filename + ".png";
    return IMG_LoadTexture(RENDERER, path.c_str());
}

// An hash function that return string int int format
constexpr unsigned int hash(const char *s, int off = 0) {                        
    return !s[off] ? 5381 : (hash(s, off+1)*33) ^ s[off];                           
}

#define KNIGHT_MOVE(ROW, COL) HandleSquareSelectMoveableAndTakeables(&squares[ROW][COL],  &takeableSquares, &moveableSquares, opponent);


int main(int argc, char* argv[])
{
    //===========================================
    // Constant variables
    const int ROW_COUNT = 8;
    const int COL_COUNT = 8;
    const int WINDOW_WIDTH = ROW_COUNT * Square::WIDTH;
    const int WINDOW_HEIGHT = COL_COUNT * Square::HEIGHT; 
    // ===========================================

    // ===========================================
    // Display
	
    // Initialize libraries
	if (SDL_Init(SDL_INIT_VIDEO) < 0) { ERROR("Couldn't initialize SDL: " << SDL_GetError()); return -1; }
    if (IMG_Init(IMG_INIT_PNG) < 0)   { ERROR("Couldn't initialize SDL_image: " << IMG_GetError()); return -1; }
    if (TTF_Init() < 0)   { ERROR("Couldn't initialize SDL_ttf: " << TTF_GetError()); return -1; }
	
    // Create window, renderer and surface
    SDL_Window* WINDOW = SDL_CreateWindow("Chess", 
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    WINDOW_WIDTH, WINDOW_HEIGHT,
    SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (WINDOW == NULL)
    { ERROR("Couldn't create Window: " << SDL_GetError()); return -1; }
    SDL_Renderer* RENDERER = SDL_CreateRenderer(WINDOW, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface* SURFACE =  SDL_GetWindowSurface(WINDOW);
    // ===========================================

    //===========================================
    // Create pieces

    SDL_Renderer* r = RENDERER;

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

    // Default board
    ChessPiece DEFAULT_BOARD[ROW_COUNT][COL_COUNT] = {
        {   ROOK_BLACK,   KNIGHT_BLACK,  BISHOP_BLACK,  QUEEN_BLACK,  KING_BLACK,  BISHOP_BLACK,  KNIGHT_BLACK,  ROOK_BLACK   },
        {   PAWN_BLACK,   PAWN_BLACK  ,  PAWN_BLACK  ,  PAWN_BLACK ,  PAWN_BLACK,  PAWN_BLACK  ,  PAWN_BLACK  ,  PAWN_BLACK   },
        {   EMPTY     ,   EMPTY       ,  EMPTY       ,  EMPTY      ,  EMPTY     ,  EMPTY       ,  EMPTY       ,  EMPTY        },
        {   EMPTY     ,   EMPTY       ,  EMPTY       ,  EMPTY      ,  EMPTY     ,  EMPTY       ,  EMPTY       ,  EMPTY        },
        {   EMPTY     ,   EMPTY       ,  EMPTY       ,  EMPTY      ,  EMPTY     ,  EMPTY       ,  EMPTY       ,  EMPTY        },
        {   EMPTY     ,   EMPTY       ,  EMPTY       ,  EMPTY      ,  EMPTY     ,  EMPTY       ,  EMPTY       ,  EMPTY        },
        {   PAWN_WHITE,   PAWN_WHITE  ,  PAWN_WHITE  ,  PAWN_WHITE ,  PAWN_WHITE,  PAWN_WHITE  ,  PAWN_WHITE  ,  PAWN_WHITE   },
        {   ROOK_WHITE,   KNIGHT_WHITE,  BISHOP_WHITE,  QUEEN_WHITE,  KING_WHITE,  BISHOP_WHITE,  KNIGHT_WHITE,  ROOK_WHITE   }
    };

    // Current state of the board
    Square squares[ROW_COUNT][COL_COUNT];
    bool isPlaying = false;
    // ===========================================

    // Create main menu GUI

    // Font
    TTF_Font* FONT = TTF_OpenFont((PATH_PREFIX + std::string("font/Hack_Regular.ttf")).c_str(), 32);
    TTF_Font* FONT_LITTLE = TTF_OpenFont((PATH_PREFIX  + std::string("font/Hack_Regular.ttf")).c_str(), 24);
    GUI_WidgetBase::FONT = FONT;
    // Set renderer
    GUI_WidgetBase::RENDERER = RENDERER;

    // Grid
    GUI_Grid grid(0,0, GUI_Grid::DEFAULT_SPACE, 35, true);

    // Title
    GUI_Text menuTitle("Chess");
    grid.Add(&menuTitle, 0, 0);

    // Play button
    GUI_Button playButton(new GUI_Text("Play"));
    grid.Add(&playButton, 1, 0);

    // Center and render grid
    grid.UpdateWidgetLocationsAndRectData();
    CenterWidget(&grid,  ROW_COUNT * Square::WIDTH, COL_COUNT * Square::HEIGHT);
    grid.Render();

    // Add made by text
    GUI_WidgetBase::FONT = FONT_LITTLE;
    GUI_Text madeBy("made by Alper Bayraktar");
    // Put it to right bottom and render
    madeBy.SetLocation(ROW_COUNT * Square::WIDTH - madeBy.rect.w - 5, COL_COUNT * Square::HEIGHT - madeBy.rect.h - 5);
    madeBy.Render();

    // ===========================================
    // Game loop

    SDL_Event evt;
    const char* turn = "WHITE";

    // Ptr to currently selected square
    Square* selectedSquare = nullptr;
    // moveable squares of selected square
    std::vector<Square*> moveableSquares;
    // takeable squares of selected square
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
                
                case SDL_MOUSEMOTION:
                    if (!isPlaying)
                    {
                        grid.MouseMotion();
                    }
                
                case SDL_MOUSEBUTTONDOWN:
                    if (evt.button.button == SDL_BUTTON_LEFT)
                    {
                        // Start game
                        if (!isPlaying & playButton.isHovered)
                        {
                            ResetBoard(squares, DEFAULT_BOARD, RENDERER);
                            isPlaying = true;
                            break;
                        }


                        // Get clicked square
                        int mouseX, mouseY;
                        SDL_GetMouseState(&mouseX, &mouseY);
                        int row = mouseY / Square::HEIGHT;
                        int col = mouseX / Square::WIDTH;
                        Square* clickedSquare = &squares[row][col];


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
                                            MakeMoveableIfEmpty(&squares[6 - 2][clickedSquare->col], &moveableSquares);
                                        }

                                        int nextRow = clickedSquare->row - 1;
                                        if (nextRow >= 0)
                                        {
                                            MakeMoveableIfEmpty(&squares[nextRow][clickedSquare->col], &moveableSquares);
                                            if (clickedSquare->col - 1 >= 0)
                                                MakeTakeableIfOpponent(&squares[nextRow][clickedSquare->col - 1], &takeableSquares, opponent);
                                            if (clickedSquare->col + 1 <= 7)
                                                MakeTakeableIfOpponent(&squares[nextRow][clickedSquare->col + 1], &takeableSquares, opponent);
                                        }
                                    }

                                    else if (clickedSquare->piece->color == "BLACK")
                                    {
                                        if (clickedSquare->row == 1)
                                        {
                                            MakeMoveableIfEmpty(&squares[1 + 2][clickedSquare->col], &moveableSquares);
                                        }

                                        int nextRow = clickedSquare->row + 1;
                                        if (nextRow <= 7)
                                        {
                                            MakeMoveableIfEmpty(&squares[nextRow][clickedSquare->col], &moveableSquares);
                                            if (clickedSquare->col - 1 >= 0)
                                                MakeTakeableIfOpponent(&squares[nextRow][clickedSquare->col - 1], &takeableSquares, opponent);
                                            if (clickedSquare->col + 1 <= 7)
                                                MakeTakeableIfOpponent(&squares[nextRow][clickedSquare->col + 1], &takeableSquares, opponent);
                                        }
                                    }
                                    break;

                                case hash("ROOK"):
                                    SelectRook(squares, clickedSquare, turn, opponent, &takeableSquares, &moveableSquares, false);
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

                                case hash("BISHOP"):
                                    SelectBishop(squares, clickedSquare, turn, opponent, &takeableSquares, &moveableSquares, false);
                                    break;

                                case hash("QUEEN"):
                                    SelectRook(squares, clickedSquare, turn, opponent, &takeableSquares, &moveableSquares, false);
                                    SelectBishop(squares, clickedSquare, turn, opponent, &takeableSquares, &moveableSquares, false);
                                    break;

                                case hash("KING"):
                                    SelectRook(squares, clickedSquare, turn, opponent, &takeableSquares, &moveableSquares, true);
                                    SelectBishop(squares, clickedSquare, turn, opponent, &takeableSquares, &moveableSquares, true);
                                    break;
                            }

                            for (Square* takeableSquare : takeableSquares)
                            {
                                takeableSquare->backgroundColor = CHESS_RED;
                                takeableSquare->Render(RENDERER);
                            }
                            
                            for (Square* moveableSquare : moveableSquares)
                            {
                                moveableSquare->backgroundColor = CHESS_GREEN;
                                moveableSquare->Render(RENDERER);
                            }

                            if (selectedSquare)
                            {
                                DeselectSquare(&takeableSquares, &moveableSquares, RENDERER, selectedSquare);
                            }

                            clickedSquare->backgroundColor = CHESS_BLUE;
                            clickedSquare->Render(RENDERER);
                            selectedSquare = clickedSquare;
                            break;
                        }

                        // Deselect
                        if (selectedSquare && selectedSquare->row == clickedSquare->row && selectedSquare->col == clickedSquare->col)
                        {
                            DeselectSquare(&takeableSquares, &moveableSquares, RENDERER, selectedSquare);
                            selectedSquare = nullptr;
                            break;
                        }

                        // Move
                        if (selectedSquare)
                        {
                            // Combine moveableSquares and takeableSquares into one vector
                            std::vector<Square*> allmoveableSquares;

                            for (Square* moveableSquare : moveableSquares)
                            {
                                allmoveableSquares.push_back(moveableSquare);
                            }

                            for (Square* takeableSquare : takeableSquares)
                            {
                                allmoveableSquares.push_back(takeableSquare);
                            }

                            for (Square* moveableSquare : allmoveableSquares)
                            {
                                if (moveableSquare->row == clickedSquare->row && moveableSquare->col == clickedSquare->col)
                                {
                                    // Move the piece
                                    clickedSquare->piece = selectedSquare->piece;
                                    selectedSquare->piece = &EMPTY;                                
                                    
                                    // Deselect the old one
                                    DeselectSquare(&takeableSquares, &moveableSquares, RENDERER, selectedSquare);
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
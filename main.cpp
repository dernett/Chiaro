#include <cstdint>
#include <immintrin.h>
#include <raylib.h>

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
using Bitboard = uint64_t;

enum PieceType
{
    whitePawn,
    whiteKnight,
    whiteBishop,
    whiteRook,
    whiteQueen,
    whiteKing,
    blackPawn,
    blackKnight,
    blackBishop,
    blackRook,
    blackQueen,
    blackKing
};

struct Player
{
    int clickedSquare;
} Player;

struct State
{
    Bitboard pieceMasks[12];
    Bitboard whiteMask;
    Bitboard blackMask;
} State;

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------
const int screenWidth  = 768;
const int screenHeight = 768;

const int squareWidth  = screenWidth / 8;
const int squareHeight = screenHeight / 8;

const char *pieceFileNames[12] = {"resources/whitePawn.png",
                                  "resources/whiteKnight.png",
                                  "resources/whiteBishop.png",
                                  "resources/whiteRook.png",
                                  "resources/whiteQueen.png",
                                  "resources/whiteKing.png",
                                  "resources/blackPawn.png",
                                  "resources/blackKnight.png",
                                  "resources/blackBishop.png",
                                  "resources/blackRook.png",
                                  "resources/blackQueen.png",
                                  "resources/blackKing.png"};

Texture2D pieceTextures[12];

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
void InitGame();   // Initialize game
void UpdateGame(); // Update game (one frame)
void DrawGame();   // Draw game (one frame)
void UnloadGame(); // Unload game variables

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    InitWindow(screenWidth, screenHeight, "Chiaro");

    InitGame();

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        UpdateGame();
        DrawGame();
    }

    UnloadGame();

    CloseWindow();
}

// Initialize game variables
void InitGame()
{
    for (int piece = 0; piece < 12; piece++)
    {
        // Load the piece's image and resize it to fit inside of a square
        Image pieceImage = LoadImage(pieceFileNames[piece]);
        ImageResize(&pieceImage, squareWidth, squareHeight);

        pieceTextures[piece] = LoadTextureFromImage(pieceImage);

        UnloadImage(pieceImage);
    }

    // Represent which squares contain each piece as a bitmask
    State.pieceMasks[whitePawn]   = 0b11111111ULL << 8;
    State.pieceMasks[whiteKnight] = 0b01000010ULL;
    State.pieceMasks[whiteBishop] = 0b00100100ULL;
    State.pieceMasks[whiteRook]   = 0b10000001ULL;
    State.pieceMasks[whiteQueen]  = 0b00010000ULL;
    State.pieceMasks[whiteKing]   = 0b00001000ULL;
    State.pieceMasks[blackPawn]   = 0b11111111ULL << 48;
    State.pieceMasks[blackKnight] = 0b01000010ULL << 56;
    State.pieceMasks[blackBishop] = 0b00100100ULL << 56;
    State.pieceMasks[blackRook]   = 0b10000001ULL << 56;
    State.pieceMasks[blackQueen]  = 0b00010000ULL << 56;
    State.pieceMasks[blackKing]   = 0b00001000ULL << 56;

    for (int piece = 0; piece < 6; piece++)
        State.whiteMask |= State.pieceMasks[piece];

    for (int piece = 6; piece < 12; piece++)
        State.blackMask |= State.pieceMasks[piece];

    Player.clickedSquare = -1;
}

// Update game (one frame)
void UpdateGame()
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        int square = 8 * (GetMouseY() / squareHeight) + GetMouseX() / squareWidth;

        // Check if the square we clicked contains a white piece
        if (State.whiteMask & (1 << (63 - square)))
            Player.clickedSquare = square;
    }
    else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        Player.clickedSquare = -1;
    }
}

// Draw game (one frame)
void DrawGame()
{
    BeginDrawing();
    {
        ClearBackground(RAYWHITE);

        // Draw chessboard squares
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if ((i & 1) ^ (j & 1))
                    DrawRectangle(j * squareWidth, i * squareHeight, squareWidth, squareHeight, BROWN);
                else
                    DrawRectangle(j * squareWidth, i * squareHeight, squareWidth, squareHeight, BEIGE);
            }
        }

        int clickedPiece = -1;

        // Draw pieces
        for (int piece = 0; piece < 12; piece++)
        {
            for (Bitboard mask = State.pieceMasks[piece]; mask; mask = _blsr_u64(mask))
            {
                int square = 63 - _tzcnt_u64(mask);
                int i = square / 8, j = square % 8;

                if (square == Player.clickedSquare)
                    clickedPiece = piece;
                else
                    DrawTexture(pieceTextures[piece], j * squareWidth, i * squareHeight, WHITE);
            }
        }

        // The clicked piece is drawn last so it is infront of all the other pieces
        if (clickedPiece != -1)
            DrawTexture(pieceTextures[clickedPiece], GetMouseX() - squareWidth / 2, GetMouseY() - squareHeight / 2, WHITE);
    }
    EndDrawing();
}

// Unload game variables
void UnloadGame()
{
    for (int piece = 0; piece < 12; piece++)
    {
        UnloadTexture(pieceTextures[piece]);
    }
}
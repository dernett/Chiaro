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

Bitboard pieceMasks[12];

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
void InitGame();   // Initialize game
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
    pieceMasks[whitePawn]   = 0b11111111ULL << 8;
    pieceMasks[whiteKnight] = 0b01000010ULL;
    pieceMasks[whiteBishop] = 0b00100100ULL;
    pieceMasks[whiteRook]   = 0b10000001ULL;
    pieceMasks[whiteQueen]  = 0b00010000ULL;
    pieceMasks[whiteKing]   = 0b00001000ULL;
    pieceMasks[blackPawn]   = 0b11111111ULL << 48;
    pieceMasks[blackKnight] = 0b01000010ULL << 56;
    pieceMasks[blackBishop] = 0b00100100ULL << 56;
    pieceMasks[blackRook]   = 0b10000001ULL << 56;
    pieceMasks[blackQueen]  = 0b00010000ULL << 56;
    pieceMasks[blackKing]   = 0b00001000ULL << 56;
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
                    DrawRectangle(j * squareWidth, i * squareHeight,
                                  squareWidth, squareHeight, BROWN);
                else
                    DrawRectangle(j * squareWidth, i * squareHeight,
                                  squareWidth, squareHeight, BEIGE);
            }
        }

        // Draw pieces
        for (int piece = 0; piece < 12; piece++)
        {
            for (Bitboard mask = pieceMasks[piece]; mask; mask = _blsr_u64(mask))
            {
                int square = 63 - _tzcnt_u64(mask);
                int i = square / 8, j = square % 8;
                DrawTexture(pieceTextures[piece], j * squareWidth, i * squareHeight, WHITE);
            }
        }
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
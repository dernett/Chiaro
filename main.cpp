#include <raylib.h>

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------
const int screenWidth = 768;
const int screenHeight = 768;

const int squareWidth = screenWidth / 8;
const int squareHeight = screenHeight / 8;

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
void DrawGame(); // Draw game (one frame)

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    InitWindow(screenWidth, screenHeight, "Chiaro");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        DrawGame();
    }

    CloseWindow();
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
    }
    EndDrawing();
}
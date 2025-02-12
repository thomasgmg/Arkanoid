#include "raylib.h"

int main(void) {
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "raylib [shapes] example - basic ball");

    Vector2 ballPosition = {GetScreenWidth()/2.0f, GetScreenHeight()/2.0f};
    Vector2 ballSpeed = {5.0f, 4.0f};
    int ballRadius = 20;

    bool pause = 0;
    int framesCounter = 0;

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        if (IsKeyPressed(KEY_SPACE)) pause = !pause;
        if (!pause){
            ballPosition.x += ballSpeed.x;
            ballPosition.y += ballSpeed.y;

            if ((ballPosition.x >= (GetScreenWidth() - ballRadius)) || (ballPosition.x <= ballRadius)) ballSpeed.x *= -1.0f;
            if ((ballPosition.y >= (GetScreenHeight() - ballRadius)) || (ballPosition.y <= ballRadius)) ballSpeed.y *= -1.0f;
        }
        else framesCounter++;

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(DARKBLUE);

        DrawCircleV(ballPosition, (float)ballRadius, MAROON);
        DrawText("Press space to pause ball movement", 10, GetScreenHeight() - 25, 20, BLACK);

        if (pause && ((framesCounter/30%2))) DrawText("Paused", 350, 200, 20, BLACK);

        DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();
    //--------------------------------------------------------------------------------------

    return 0;
}

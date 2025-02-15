#include "raylib.h"

#include <stdlib.h>
#include <math.h>


Font font;
        
int const PLAYER_MAX_LIFE = 3;
int const LINES_OF_BRICKS = 6;
int const BRICKS_PER_LINE = 10;
#define BG CLITERAL(Color) { 0, 4, 53, 255}

struct Player {
    Vector2 position;
    Vector2 size;
    int life;
};

struct Ball {
    Vector2 position;
    Vector2 speed;
    int radius;
    bool active;
};

struct Brick {
    Vector2 position;
    bool active;
};

int const screenWidth = 1150;
int const screenHeight = 650;

bool isMenu = true;
bool gameOver = false;
bool pause = false;
int score = 0;
Player player = {0};
Ball ball = {0};
Brick brick[LINES_OF_BRICKS][BRICKS_PER_LINE] = {0};
Vector2 brickSize = {0};

void InitGame();
void UpdateGame();
void DrawGame();
void UnloadBricks();
void UpdateDrawFrame();

int main() {
    InitWindow(screenWidth, screenHeight, "classic game: arkanoid");
    InitGame();
    SetTargetFPS(60);
    font = LoadFontEx("resources/font.ttf", 96, 0, 0);
    while (!WindowShouldClose()) {
        if (isMenu) {
            if (IsKeyPressed(KEY_ENTER))
                isMenu = false;
        } else {
            if (IsKeyPressed(KEY_M))
                isMenu = true;
        }
        UpdateDrawFrame();
    }
    CloseWindow();
    return 0;
}

void InitGame(void) {
    brickSize = (Vector2){GetScreenWidth() / (float)BRICKS_PER_LINE, 40};

    player.position =
        (Vector2){(float)screenWidth / 2, (float)screenHeight * 7 / 8};
    player.size = (Vector2){(float)screenWidth / 10, 20};
    player.life = PLAYER_MAX_LIFE;

    ball.position =
        (Vector2){player.position.x, player.position.y - 30};
    ball.speed = (Vector2){0, 0};
    ball.radius = 10;
    ball.active = false;

    for (int i = 0; i < LINES_OF_BRICKS; i++) {
        for (int j = 0; j < BRICKS_PER_LINE; j++) {
            brick[i][j].position =
                (Vector2){j * brickSize.x + brickSize.x / 2, i * brickSize.y + brickSize.y / 2};
            brick[i][j].active = true;
        }
    }
}

void UpdateGame() {
    if (IsKeyPressed('P'))
        pause = !pause;

    // ball launching logic
    if (IsKeyPressed(KEY_SPACE) && !ball.active)
    {
        ball.active = true;
        ball.speed = (Vector2){0, -5};
    }

    if (pause || !ball.active) return;

    if (IsKeyDown(KEY_LEFT))
        player.position.x -= 9;

    if ((player.position.x - player.size.x / 2) <= 0)
        player.position.x = player.size.x / 2;

    if (IsKeyDown(KEY_RIGHT))
        player.position.x += 9;

    if ((player.position.x + player.size.x / 2) >= screenWidth)
        player.position.x = screenWidth - player.size.x / 2;


    // ball movement logic
    if (ball.active) {
        ball.position.x += ball.speed.x;
        ball.position.y += ball.speed.y;
    } else {
        ball.position = (Vector2){player.position.x, (float)screenHeight * 7 / 8 - 30};
    }

    //Collision: ball vs walls
    if (((ball.position.x + ball.radius) >= screenWidth) ||
        ((ball.position.x - ball.radius) <= 0))
        ball.speed.x *= -1;
    if ((ball.position.y - ball.radius) <= 0)
        ball.speed.y *= -1;
    if ((ball.position.y + ball.radius) >= screenHeight) 
    {
        ball.speed = (Vector2){0, 0};
        ball.position = (Vector2){player.position.x, player.position.y - 30};
        ball.active = false;
        player.life--;
    }

    // Check collision between ball and player
    if (CheckCollisionCircleRec(ball.position, ball.radius, (Rectangle) {player.position.x - player.size.x / 2, player.position.y - player.size.y / 2, player.size.x, player.size.y}))
    {
        if (ball.speed.y > 0)
        {
            ball.speed.y *= -1;
            ball.speed.x = (ball.position.x - player.position.x) / (player.size.x / 2) * 5;
        }
    }

    // Collision logic: ball vs bricks
    for (int i = 0; i < LINES_OF_BRICKS; i++)
    {
        for (int j = 0; j < BRICKS_PER_LINE; j++)
        {
            if (brick[i][j].active)
            {
                // Hit below
                if (((ball.position.y - ball.radius) <= (brick[i][j].position.y + brickSize.y / 2))
                    && ((ball.position.y - ball.radius) > (brick[i][j].position.y + brickSize.y / 2 + ball.speed.y)) 
                    && ((fabs(ball.position.x - brick[i][j].position.x)) < (brickSize.x / 2 + ball.radius * 2 / 3)) 
                    && (ball.speed.y < 0))
                {
                    brick[i][j].active = false;
                    ball.speed.y *= -1;
                    score++;
                }
                // Hit above
                else if (((ball.position.y + ball.radius) >= (brick[i][j].position.y - brickSize.y / 2)) 
                    && ((ball.position.y + ball.radius) < (brick[i][j].position.y - brickSize.y / 2 + ball.speed.y)) 
                    && ((fabs(ball.position.x - brick[i][j].position.x)) < (brickSize.x / 2 + ball.radius * 2 / 3)) 
                    && (ball.speed.y > 0))
                {
                    brick[i][j].active = false;
                    ball.speed.y *= -1;
                    score++;
                }
                // Hit left
                else if (((ball.position.x + ball.radius) >= (brick[i][j].position.x - brickSize.x / 2)) 
                    && ((ball.position.x + ball.radius) < (brick[i][j].position.x - brickSize.x / 2 + ball.speed.x)) 
                    && ((fabs(ball.position.y - brick[i][j].position.y)) < (brickSize.y / 2 + ball.radius * 2 / 3)) 
                    && (ball.speed.x > 0))
                {
                    brick[i][j].active = false;
                    ball.speed.x *= -1;
                    score++;
                }
                // Hit right
                else if (((ball.position.x - ball.radius) <= (brick[i][j].position.x + brickSize.x / 2)) 
                    && ((ball.position.x - ball.radius) > (brick[i][j].position.x + brickSize.x / 2 + ball.speed.x)) 
                    && ((fabs(ball.position.y - brick[i][j].position.y)) < (brickSize.y / 2 + ball.radius * 2 / 3)) 
                    && (ball.speed.x < 0))
                {
                    brick[i][j].active = false;
                    ball.speed.x *= -1;
                    score++;
                }
            }
        }
    }

    if (player.life <= 0)
        gameOver = true;
    else
    {
        gameOver = true;

        for (int i = 0; i < LINES_OF_BRICKS; i++)
        {
            for (int j = 0; j < BRICKS_PER_LINE; j++)
            {
                if (brick[i][j].active)
                    gameOver = false;
            }
        }
    }

    if (IsKeyPressed(KEY_ENTER))
    {
        InitGame();
        gameOver = false;
    }
}


// Draw game (one frame)
void DrawGame()
{
    BeginDrawing();
    if (isMenu)
    {
        ClearBackground(BLUE);
        // RLAPI void DrawTextEx(Font font, const char *text, Vector2 position, float fontSize, float spacing, Color tint); // Draw text using font and additional parameters
        DrawTextEx(font, "ARKAN0ID: 2D Classic GAME", (Vector2){(float)GetScreenWidth() / 2 - 300, (float)GetScreenHeight() / 2 - 50}, 50, 0, BLACK);
        DrawTextEx(font, "Press ENTER to start the GAME", (Vector2){(float)GetScreenWidth() / 2 - 140, screenHeight - 170}, 20, 0, BLACK);
        DrawTextEx(font, "Press P to pause the GAME", (Vector2){(float)GetScreenWidth() / 2 - 140, screenHeight - 145}, 20, 0, BLACK);
        DrawTextEx(font, "Press M to return to MENU", (Vector2){(float)GetScreenWidth() / 2 - 140, screenHeight - 120}, 20, 0, BLACK);
        DrawTextEx(font, "Press ESC to exit the GAME", (Vector2){(float)GetScreenWidth() / 2 - 140, screenHeight - 95}, 20, 0, BLACK);
    }
    else
{

        ClearBackground(BG);

        if (!gameOver)
        {
            // Draw player bar
            Rectangle paddle = {player.position.x - player.size.x / 2, player.position.y - player.size.y / 2, player.size.x, player.size.y / 2};
            DrawRectangleRounded(paddle, 10.0, 4, WHITE);

            // Draw player lives
            for (int i = 0; i < player.life; i++)
            {
                Rectangle life = {(float)20 + 40 * i, screenHeight - 30, 35, 10};
                DrawRectangleRounded(life, 10.4, 4, LIGHTGRAY);
            }

            // Draw ball
            DrawCircleV(ball.position, ball.radius, MAROON);

            // Draw bricks
            UnloadBricks();

            if (pause)
                DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, screenHeight / 2 - 40, 40, WHITE);
        }
        else
    {
            if (score != (LINES_OF_BRICKS * BRICKS_PER_LINE))
            {
                DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ESC..] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 4 * 3 - 50, 20, WHITE);
                DrawText("PRESS [ESC] TO EXIT the GAME", GetScreenWidth() / 2 - MeasureText("PRESS [ESC..] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 4 * 3, 20, WHITE);
                // Draw Scoreboard
                DrawText(TextFormat("SCORE%4i", score), GetScreenWidth() / 2 - MeasureText("SCORE", 40), 50, 50, WHITE);
            }
            else
        { 
                DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ESC..] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 4 * 3 - 50, 20, WHITE);
                DrawText("PRESS [ESC] TO EXIT the GAME", GetScreenWidth() / 2 - MeasureText("PRESS [ESC..] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 4 * 3, 20, WHITE);
                // Draw Scoreboard
                DrawText(TextFormat("CONGRATULATIONS"), GetScreenWidth() / 2 - 250, 50, 50, WHITE);
                DrawText("You Earned the Highest Score!", GetScreenWidth() / 2 - 230, 100, 30, WHITE);
            }
        }
    }
    EndDrawing();
}

// Update and Draw (one frame)
void UpdateDrawFrame()
{
    UpdateGame();
    DrawGame();
}

void UnloadBricks()
{
    for (int i = 0; i < LINES_OF_BRICKS; i++)
    {
        for (int j = 0; j < BRICKS_PER_LINE; j++)
        {
            if (brick[i][j].active)
            {
                if (i == 0)
                {
                    DrawRectangle(brick[i][j].position.x - brickSize.x / 2, brick[i][j].position.y - brickSize.y / 2, brickSize.x, brickSize.y, YELLOW);
                }
                else if (i == 1)
                {
                    DrawRectangle(brick[i][j].position.x - brickSize.x / 2, brick[i][j].position.y - brickSize.y / 2, brickSize.x, brickSize.y, PINK);
                }
                else if (i == 2)
                {
                    DrawRectangle(brick[i][j].position.x - brickSize.x / 2, brick[i][j].position.y - brickSize.y / 2, brickSize.x, brickSize.y, GREEN);
                }
                else if (i == 3)
                {
                    DrawRectangle(brick[i][j].position.x - brickSize.x / 2, brick[i][j].position.y - brickSize.y / 2, brickSize.x, brickSize.y, BLUE);
                }
                else if (i == 4)
                {
                    DrawRectangle(brick[i][j].position.x - brickSize.x / 2, brick[i][j].position.y - brickSize.y / 2, brickSize.x, brickSize.y, PURPLE);
                }
                else if (i == 5)
                {
                    DrawRectangle(brick[i][j].position.x - brickSize.x / 2, brick[i][j].position.y - brickSize.y / 2, brickSize.x, brickSize.y, BROWN);
                }
                else if (i == 6)
                {
                    DrawRectangle(brick[i][j].position.x - brickSize.x / 2, brick[i][j].position.y - brickSize.y / 2, brickSize.x, brickSize.y, RED);
                }
                DrawRectangleLines(brick[i][j].position.x - brickSize.x / 2, brick[i][j].position.y - brickSize.y / 2, brickSize.x, brickSize.y, BG);
            }
        }
    }
}

#include "raylib.h"
#include <iostream>
#include <vector>
#include "Paddle.h"

using namespace std;

int main() {

    const int x = 900;
    const int y = 900;
    const float PADDLE_WIDTH = 10.0f;
    const float PADDLE_HEIGHT = 150.0f;
    const float PADDLE_SPEED = 10.0f;
    const float PADDLE_DECELERATION = 0.95f;

    InitWindow(x, y, "Gregs Raylib");
    SetTargetFPS(60);

    Font ft = LoadFont("resources/fonts/alagard.png");

    int ballX = x / 2;
    int ballY = y / 2;

    Paddle leftPaddle;
    Paddle rightPaddle;

    int movementX = -10;
    int movementY = -10;

    Vector2 position = { x / 4.0f, 0 };

    leftPaddle.Init(
        Vector2{ 50.0f, (y / 2.0f) - (PADDLE_HEIGHT / 2.0f) },
        Vector2{ 0.0f, 0.0f },
        PADDLE_WIDTH,
        PADDLE_HEIGHT,
        WHITE);

    rightPaddle.Init(
        Vector2{ x - 50.0f - PADDLE_WIDTH, (y / 2.0f) - (PADDLE_HEIGHT / 2.0f) },
        Vector2{ 0.0f, 0.0f },
        PADDLE_WIDTH,
        PADDLE_HEIGHT,
        WHITE);

    while (!WindowShouldClose()) {

        ballX += movementX;
        ballY += movementY;

        Vector2 ballPos = { static_cast<float>(ballX), static_cast<float>(ballY) };
        float ballRadius = 10.0f;

        if (CheckCollisionCircleRec(ballPos, ballRadius, leftPaddle.GetPaddleRectangle()))
        {
            movementX = abs(movementX);
            ballX = leftPaddle.GetPosition().x + leftPaddle.GetWidth() + ballRadius;
        }
        if (CheckCollisionCircleRec(ballPos, ballRadius, rightPaddle.GetPaddleRectangle()))
        {
            movementX = -abs(movementX);
            ballX = rightPaddle.GetPosition().x - ballRadius;
        }

        if (ballY - ballRadius <= 0 || ballY + ballRadius >= y)
        {
            movementY = -movementY;
        }

        if (ballX - ballRadius <= 0 || ballX + ballRadius >= x)
        {
            ballX = x / 2;
            ballY = y / 2;
        }

        leftPaddle.Update();
        if (IsKeyDown('S'))
            leftPaddle.SetSpeed(Vector2{ 0.0f, PADDLE_SPEED });
        else if (IsKeyDown('W'))
            leftPaddle.SetSpeed(Vector2{ 0.0f, -PADDLE_SPEED });
        else
            leftPaddle.SetSpeed(Vector2{ 0.0f, leftPaddle.GetSpeed().y * PADDLE_DECELERATION });

        rightPaddle.Update();
        if (IsKeyDown(KEY_DOWN))
            rightPaddle.SetSpeed(Vector2{ 0.0f, PADDLE_SPEED });
        else if (IsKeyDown(KEY_UP))
            rightPaddle.SetSpeed(Vector2{ 0.0f, -PADDLE_SPEED });
        else
            rightPaddle.SetSpeed(Vector2{ 0.0f, rightPaddle.GetSpeed().y * PADDLE_DECELERATION });

        if (leftPaddle.GetPosition().y <= 0)
            leftPaddle.SetPosition(Vector2{ leftPaddle.GetPosition().x, 0 });
        else if (leftPaddle.GetPosition().y + leftPaddle.GetHeight() >= y)
            leftPaddle.SetPosition(Vector2{ leftPaddle.GetPosition().x, y - leftPaddle.GetHeight() });

        if (rightPaddle.GetPosition().y <= 0)
            rightPaddle.SetPosition(Vector2{ rightPaddle.GetPosition().x, 0 });
        else if (rightPaddle.GetPosition().y + rightPaddle.GetHeight() >= y)
            rightPaddle.SetPosition(Vector2{ rightPaddle.GetPosition().x, y - rightPaddle.GetHeight() });



        BeginDrawing();



        ClearBackground(DARKGREEN);
        DrawText("HitMe", ballX - 10, ballY - 40, 15, RED);
        DrawTextEx(ft, "PONG", position, 65, 2, DARKPURPLE);
        DrawCircle(ballX, ballY, ballRadius, Color{ 2,222,233,242 });
        leftPaddle.Draw();
        rightPaddle.Draw();
        DrawRectangle(x / 2, 0, 4, y, WHITE);




        EndDrawing();
    }

    CloseWindow();
    return 0;
}

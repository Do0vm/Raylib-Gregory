#include "raylib.h"
#include <iostream>
#include <vector>
#include "Paddle.h"
#include <string>

using namespace std;
void ScoreGoal(bool ScoredLeft);


int lScore=0;
int rScore=0;
const int WINDOW_WIDTH = 900;
const int WINDOW_HEIGHT = 900;
const float PADDLE_WIDTH = 10.0f;
const float PADDLE_HEIGHT = 150.0f;
const float PADDLE_SPEED = 15.0f;
const float PADDLE_DECELERATION = 0.95f;
Vector2 lLeftPosition = { WINDOW_WIDTH / 4.0f, 0 };
Vector2 lRightPosition = { (WINDOW_WIDTH / 4.0f+ WINDOW_WIDTH/2), 0 };


int main() {



    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Gregs Raylib");
    SetTargetFPS(60);

    Font ft = LoadFont("resources/fonts/alagard.png");

    int ballX = WINDOW_WIDTH / 2;
    int ballY = WINDOW_HEIGHT / 2;


    Paddle leftPaddle;
    Paddle rightPaddle;

    int movementX = -10;
    int movementY = -10;



    leftPaddle.Init(
        Vector2{ 50.0f, (WINDOW_HEIGHT / 2.0f) - (PADDLE_HEIGHT / 2.0f) },
        Vector2{ 0.0f, 0.0f },
        PADDLE_WIDTH,
        PADDLE_HEIGHT,
        WHITE);

    rightPaddle.Init(
        Vector2{ WINDOW_WIDTH - 50.0f - PADDLE_WIDTH, (WINDOW_HEIGHT / 2.0f) - (PADDLE_HEIGHT / 2.0f) },
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

        if (ballY - ballRadius <= 0 || ballY + ballRadius >= WINDOW_HEIGHT)
        {
            movementY = -movementY;
        }
        
        //if (ballX - ballRadius <= 0 || ballX + ballRadius >= x)
        //{

        //    ballX = x / 12;
        //    ballY = y / 12;
        //    movementX = abs(movementX);
        //    movementY= abs(movementY);
        //}
        //Scoring logic
        if (ballX - ballRadius <= 0)
        {       
            ScoreGoal(false);
            ballX = WINDOW_WIDTH / 12;
            ballY = WINDOW_HEIGHT / 12;
            movementX = abs(movementX);
            movementY = abs(movementY);
        }
        else if (ballX + ballRadius >= WINDOW_WIDTH)
        {
            ScoreGoal(true);     
            ballX = WINDOW_WIDTH / 12;
            ballY = WINDOW_HEIGHT / 12;
            movementX = abs(movementX);
            movementY = abs(movementY);
        }

        
        //Left Paddle
        leftPaddle.Update();
        if (IsKeyDown('S'))
            leftPaddle.SetSpeed(Vector2{ 0.0f, PADDLE_SPEED });
        else if (IsKeyDown('W'))
            leftPaddle.SetSpeed(Vector2{ 0.0f, -PADDLE_SPEED });
        else
            leftPaddle.SetSpeed(Vector2{ 0.0f, leftPaddle.GetSpeed().y * PADDLE_DECELERATION });
        //Right Paddle
        rightPaddle.Update();
        if (IsKeyDown(KEY_DOWN))
            rightPaddle.SetSpeed(Vector2{ 0.0f, PADDLE_SPEED });
        else if (IsKeyDown(KEY_UP))
            rightPaddle.SetSpeed(Vector2{ 0.0f, -PADDLE_SPEED });
        else
            rightPaddle.SetSpeed(Vector2{ 0.0f, rightPaddle.GetSpeed().y * PADDLE_DECELERATION });
        //collision left paddle
        if (leftPaddle.GetPosition().y <= 0)
            leftPaddle.SetPosition(Vector2{ leftPaddle.GetPosition().x, 0 });
        else if (leftPaddle.GetPosition().y + leftPaddle.GetHeight() >= WINDOW_HEIGHT)
            leftPaddle.SetPosition(Vector2{ leftPaddle.GetPosition().x, WINDOW_HEIGHT - leftPaddle.GetHeight() });
        //collision right paddle
        if (rightPaddle.GetPosition().y <= 0)
            rightPaddle.SetPosition(Vector2{ rightPaddle.GetPosition().x, 0 });
        else if (rightPaddle.GetPosition().y + rightPaddle.GetHeight() >= WINDOW_HEIGHT)
            rightPaddle.SetPosition(Vector2{ rightPaddle.GetPosition().x, WINDOW_HEIGHT - rightPaddle.GetHeight() });



        BeginDrawing();



        ClearBackground(DARKGREEN);
        DrawText("HitMe", ballX - 10, ballY - 40, 15, RED);
        DrawCircle(ballX, ballY, ballRadius, Color{ 2,222,233,242 });
        leftPaddle.Draw();
        rightPaddle.Draw();
        DrawRectangle(WINDOW_WIDTH / 2, 0, 4, WINDOW_HEIGHT, WHITE);
        DrawTextEx(ft, TextFormat("%i",lScore), lLeftPosition, 65, 2, DARKPURPLE);
        DrawTextEx(ft, TextFormat("%i", rScore), lRightPosition, 65, 2, DARKPURPLE);


        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void ScoreGoal(bool ScoredLeft)
{

    if (ScoredLeft) { lScore++; }
    else { rScore++; }
    
        
}

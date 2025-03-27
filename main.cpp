#include "raylib.h"
#include <iostream>
#include <vector>
#include "Paddle.h"

using namespace std;

int main() {

    //CONST

     const  int x = 900;
     const  int y = 900;
     const float PADDLE_WIDTH = 10.0f;
     const float PADDLE_HEIGHT = 60.0f;
     const float PADDLE_SPEED = 6.0f;
     const float PADDLE_DECELERATION = 0.95f;


    //VARIABLES


    InitWindow(x, y, "Gregs Raylib");
    SetTargetFPS(60);

    //init
    Font ft = LoadFont("resources/fonts/alagard.png");
 

    int ballX = 0;
    int ballY = 0;

    Paddle leftPaddle;

    leftPaddle.Init(
        Vector2{ 50.0f, (y / 2.0f) - (PADDLE_HEIGHT / 2.0f) }, 
        Vector2{ 0.0f, 0.0f },                                  
        PADDLE_WIDTH,                                      
        PADDLE_HEIGHT,                                      
        WHITE);


    int movementX = -10;
    int movementY = -10;

    Vector2 position = {x /4};


    while (!WindowShouldClose()) {
        // UPdate

        ballX += movementX;
        ballY += movementY;


        // Create ball position vector and set radius
        Vector2 ballPos = { static_cast<float>(ballX), static_cast<float>(ballY) };
        float ballRadius = 10.0f;

        if (CheckCollisionCircleRec(ballPos, ballRadius, leftPaddle.GetPaddleRectangle()))
        {
            movementX = -movementX;
            movementY += leftPaddle.GetSpeed().y;
            ballX = leftPaddle.GetPosition().x + leftPaddle.GetWidth() + 1;
        }


        if (ballX >= x || ballX <= 0 || ballY >= y || ballY <= 0)
        {
            ballX = x / 2;
            ballY = y / 2;



        }
        // Left Paddle

        leftPaddle.Update();

        if (IsKeyDown('S'))
        {
            leftPaddle.SetSpeed(Vector2{ 0.0f,PADDLE_SPEED });
        }


        if (IsKeyDown('W'))
        {
            leftPaddle.SetSpeed(Vector2{ 0.0f,-PADDLE_SPEED });
        }

        else {

            leftPaddle.SetSpeed(Vector2{ 0.0f,leftPaddle.GetSpeed().y * PADDLE_DECELERATION });

        }

        //Collisions left

        if (leftPaddle.GetPosition().y <= 0)
        {
            leftPaddle.SetPosition(Vector2{ leftPaddle.GetPosition().x,0 });
        }


        else if (leftPaddle.GetPosition().y + leftPaddle.GetHeight() >= y) 
        {

            leftPaddle.SetPosition(Vector2{ leftPaddle.GetPosition().x, y - leftPaddle.GetHeight() });

        }

        BeginDrawing();

        ClearBackground(DARKGREEN);
        DrawText("Player",ballX -10 , ballY-40, 15, RED);
        DrawTextEx(ft,"PONG",position , 65,2,DARKPURPLE);


        //Draw
        DrawCircle(ballX, ballY, 10, Color{ 2,222,233, 242 });
        leftPaddle.Draw();
        DrawRectangle(x / 2.0f, 0,4, y, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
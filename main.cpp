#include "raylib.h"
#include <iostream>
#include <vector>
#include "Paddle.h"

using namespace std;

int main() {


    cout << "Hello World" << endl;

    int x = 300;
    int y = 300;

    InitWindow(x, y, "Gregs Raylib");
    SetTargetFPS(60);

    //init
    Font ft = LoadFont("resources/fonts/alagard.png");
 

    int ballX = 0;
    int ballY = 0;

    int movementX = 10;
    int movementY = 10;

    Vector2 position = {x / 4};


    while (!WindowShouldClose()) {
        // UPdate

        ballX += movementX;
        ballY += movementY;

        if (ballX >= x  )
        {
            movementX = -10;



        }
        if (ballY > y)
        {
            movementY = -10;

        }       
 
        if (ballX <= 0)
        {
            movementX = 10;

        }
        if (ballY < 0)
        {
            movementY = 10;

        }


        BeginDrawing();

        ClearBackground(DARKGREEN);
        DrawText("Player",ballX -10 , ballY-40, 15, RED);
        DrawTextEx(ft,"PONG",position , 65,2,DARKPURPLE);


        //Draw
        DrawCircle(ballX, ballY, 10, Color{ 2,222,233, 242 });

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
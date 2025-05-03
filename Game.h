#pragma once
#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "Paddle.h" // Need Paddle definition
#include "Ball.h"   // Need Ball definition

// --- Constants ---
extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;
extern const char* WINDOW_TITLE;
extern const float PADDLE_WIDTH;
extern const float PADDLE_HEIGHT;
extern const float PADDLE_SPEED;
extern const float PADDLE_DECELERATION;
extern const float BALL_RADIUS;
extern const float INITIAL_BALL_SPEED;
extern const int WINNING_SCORE;

// --- Game States ---
typedef enum GameScreen {
    MENU,
    PLAYING,
    PAUSE,
    GAME_OVER
} GameScreen;

// --- Global Variable Declarations 
extern GameScreen currentScreen;
extern int leftScore;
extern int rightScore;
extern Paddle leftPaddle;
extern Paddle rightPaddle;
extern Ball gameBall; 
extern Font gameFont;
extern Music backgroundMusic;   
extern Texture2D backgroundTexture;

// --- Function Declarations ---
void InitGame();
// ResetBall is now part of Ball class
void UpdateMenu();
void UpdatePlaying();
void UpdatePause();
void UpdateGameOver();
void DrawMenu();
void DrawPlaying();
void DrawPause();
void DrawGameOver();
void DrawTextCentered(const char* text, int posY, int fontSize, Color color); 


#endif // GAME_H
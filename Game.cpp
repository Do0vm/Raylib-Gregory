#include "Game.h" // Include the header file for declarations
#include "Paddle.h"
#include "Ball.h"
#include "raylib.h"
#include <string>  // For std::to_string
#include <cmath>   // For fabs
#include <cstdlib> // For rand() - needed for initial ball reset call


// --- Constant Definitions ---
const int WINDOW_WIDTH = 900;
const int WINDOW_HEIGHT = 900;
const char* WINDOW_TITLE = "GREGS Enhanced Pong";
const float PADDLE_WIDTH = 10.0f;
const float PADDLE_HEIGHT = 150.0f;
const float PADDLE_SPEED = 10.0f;
const float PADDLE_DECELERATION = 0.90f;
const float BALL_RADIUS = 10.0f;
const float INITIAL_BALL_SPEED = 7.0f;
const int WINNING_SCORE = 5;

// --- Global Variable Definitions ---
GameScreen currentScreen = MENU;
int leftScore = 0;
int rightScore = 0;
Paddle leftPaddle;
Paddle rightPaddle;
Ball gameBall; // Define the global Ball object
Font gameFont;
Music backgroundMusic;   
Texture2D backgroundTexture;

// --- Function Implementations ---

void InitGame() {
    leftScore = 0;
    rightScore = 0;

    leftPaddle.Init(
        Vector2{ 50.0f, (WINDOW_HEIGHT / 2.0f) - (PADDLE_HEIGHT / 2.0f) },
        Vector2{ 0.0f, 0.0f },
        PADDLE_WIDTH,
        PADDLE_HEIGHT,
        PADDLE_DECELERATION, // Pass deceleration
        WHITE);

    rightPaddle.Init(
        Vector2{ WINDOW_WIDTH - 50.0f - PADDLE_WIDTH, (WINDOW_HEIGHT / 2.0f) - (PADDLE_HEIGHT / 2.0f) },
        Vector2{ 0.0f, 0.0f },
        PADDLE_WIDTH,
        PADDLE_HEIGHT,
        PADDLE_DECELERATION, // Pass deceleration
        WHITE);

    // Initialize Ball using its Init method (could also use Reset directly)
    gameBall.Init(
        { WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f },
        { 0, 0 }, // Initial speed set by Reset
        BALL_RADIUS,
        Color{ 2, 222, 233, 242 },
        INITIAL_BALL_SPEED
    );
    // Reset ball position and give it initial velocity
    gameBall.Reset(WINDOW_WIDTH, WINDOW_HEIGHT, rand() % 2 == 0);
}

// ResetBall is now Ball::Reset()

void UpdateMenu() {
    if (IsKeyPressed(KEY_ENTER)) {
        InitGame();
        currentScreen = PLAYING;
    }
}

void UpdatePlaying() {
    if (IsKeyPressed(KEY_P)) {
        currentScreen = PAUSE;
        return;
    }

    // --- Ball Update (Movement & Wall Collision) ---
    gameBall.Update(WINDOW_HEIGHT);

    // --- Ball Collision: Paddles ---
    if (gameBall.CheckCollisionWithPaddle(leftPaddle)) {
        gameBall.HandlePaddleCollision(leftPaddle);
    }
    if (gameBall.CheckCollisionWithPaddle(rightPaddle)) {
        gameBall.HandlePaddleCollision(rightPaddle);
    }

    // --- Scoring ---
    Vector2 currentBallPos = gameBall.GetPosition();
    float currentBallRadius = gameBall.GetRadius();

    if (currentBallPos.x + currentBallRadius < 0) { // Right player scores
        rightScore++;
        if (rightScore >= WINNING_SCORE) {
            currentScreen = GAME_OVER;
        }
        else {
            gameBall.Reset(WINDOW_WIDTH, WINDOW_HEIGHT, true); // Reset towards left player
        }
    }
    else if (currentBallPos.x - currentBallRadius > WINDOW_WIDTH) { // Left player scores
        leftScore++;
        if (leftScore >= WINNING_SCORE) {
            currentScreen = GAME_OVER;
        }
        else {
            gameBall.Reset(WINDOW_WIDTH, WINDOW_HEIGHT, false); // Reset towards right player
        }
    }

    // --- Paddle Movement & Input ---
    // Left Paddle
    if (IsKeyDown(KEY_S))
        leftPaddle.SetSpeed(Vector2{ 0.0f, PADDLE_SPEED });
    else if (IsKeyDown(KEY_W))
        leftPaddle.SetSpeed(Vector2{ 0.0f, -PADDLE_SPEED });
    else
        leftPaddle.ApplyDeceleration(); // Apply deceleration if no key is pressed

    // Right Paddle
    if (IsKeyDown(KEY_DOWN))
        rightPaddle.SetSpeed(Vector2{ 0.0f, PADDLE_SPEED });
    else if (IsKeyDown(KEY_UP))
        rightPaddle.SetSpeed(Vector2{ 0.0f, -PADDLE_SPEED });
    else
        rightPaddle.ApplyDeceleration(); // Apply deceleration if no key is pressed

    // Update Paddle Positions & Handle Bounds
    leftPaddle.Update(WINDOW_HEIGHT);
    rightPaddle.Update(WINDOW_HEIGHT);

}

void UpdatePause() {
    if (IsKeyPressed(KEY_P)) {
        currentScreen = PLAYING;
    }
    if (IsKeyPressed(KEY_ENTER)) {
        currentScreen = MENU;
    }
}

void UpdateGameOver() {
    if (IsKeyPressed(KEY_ENTER)) {
        currentScreen = MENU;
    }
}

void DrawTextCentered(const char* text, int posY, int fontSize, Color color) {
    if (gameFont.texture.id == 0) return; // Avoid crash if font not loaded
    int textWidth = MeasureTextEx(gameFont, text, (float)fontSize, 2).x;
    DrawTextEx(gameFont, text, Vector2{ (float)WINDOW_WIDTH / 2 - textWidth / 2, (float)posY }, (float)fontSize, 2, color);
}

void DrawMenu() {
    DrawTextCentered("PONG", WINDOW_HEIGHT / 4, 100, BLUE);
    DrawTextCentered("Press ENTER to Start", WINDOW_HEIGHT / 2, 40, RED);
    DrawTextCentered("W/S for Left Paddle", WINDOW_HEIGHT / 2 + 50, 20, GOLD);
    DrawTextCentered("UP/DOWN for Right Paddle", WINDOW_HEIGHT / 2 + 80, 20, GOLD);
    DrawTextCentered("P to Pause during game", WINDOW_HEIGHT / 2 + 110, 20, GOLD);
}

void DrawPlaying() {
    DrawRectangle(WINDOW_WIDTH / 2 - 2, 0, 4, WINDOW_HEIGHT, WHITE); // Center Line
    leftPaddle.Draw();
    rightPaddle.Draw();
    gameBall.Draw();

    // Draw Scores
    std::string lScoreStr = std::to_string(leftScore);
    std::string rScoreStr = std::to_string(rightScore);
    if (gameFont.texture.id != 0) { // Check if font loaded before drawing text
        DrawTextEx(gameFont, lScoreStr.c_str(), Vector2{ WINDOW_WIDTH / 4.0f, 20.0f }, 65, 2, GOLD);
        DrawTextEx(gameFont, rScoreStr.c_str(), Vector2{ (WINDOW_WIDTH / 4.0f + WINDOW_WIDTH / 2.0f) - 30.0f, 20.0f }, 65, 2, GOLD);
    }
}

void DrawPause() {
    DrawPlaying(); // Draw game state underneath
    DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Fade(BLACK, 0.7f));
    DrawTextCentered("PAUSED", WINDOW_HEIGHT / 2 - 60, 80, RED);
    DrawTextCentered("Press P to Resume", WINDOW_HEIGHT / 2 + 20, 30, GOLD);
    DrawTextCentered("Press ENTER for Menu", WINDOW_HEIGHT / 2 + 60, 30, GOLD);
}

void DrawGameOver() {
    DrawPlaying(); // Draw final game state underneath
    DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Fade(BLACK, 0.8f));
    DrawTextCentered("GAME OVER", WINDOW_HEIGHT / 3, 80, RED);

    const char* winnerText = (leftScore > rightScore) ? "Left Player Wins!" : "Right Player Wins!";
    DrawTextCentered(winnerText, WINDOW_HEIGHT / 2 - 20, 50, GOLD);

    std::string finalScoreText = std::to_string(leftScore) + " - " + std::to_string(rightScore);
    DrawTextCentered(finalScoreText.c_str(), WINDOW_HEIGHT / 2 + 40, 40, RED);

    DrawTextCentered("Press ENTER for Menu", WINDOW_HEIGHT / 2 + 100, 30, RED);
}
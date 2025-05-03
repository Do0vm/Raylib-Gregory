#include "raylib.h"
#include "Paddle.h"
#include <string>
#include <cmath> // For abs() with floats/doubles
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()

// --- Constants ---
const int WINDOW_WIDTH = 900;
const int WINDOW_HEIGHT = 900;
const char* WINDOW_TITLE = "GREGS Enhanced Pong";
const float PADDLE_WIDTH = 10.0f;
const float PADDLE_HEIGHT = 150.0f;
const float PADDLE_SPEED = 10.0f; 
const float PADDLE_DECELERATION = 0.90f; 
const float BALL_RADIUS = 10.0f;
const float INITIAL_BALL_SPEED = 7.0f; 
const int WINNING_SCORE = 5; // Score needed to win

// --- Game States ---
typedef enum GameScreen {
    MENU,
    PLAYING,
    PAUSE,
    GAME_OVER
} GameScreen;

// --- Global Variables ---
// Game State
GameScreen currentScreen = MENU;

// Score
int leftScore = 0;
int rightScore = 0;

// Game Objects
Paddle leftPaddle;
Paddle rightPaddle;
Vector2 ballPosition;
Vector2 ballSpeed;

// Textures / Fonts
Font gameFont; // Using a single font

// --- Function Declarations ---
void InitGame();          
void ResetBall(bool toLeftPlayer); 
void UpdateMenu();        // Handle input and logic for the Menu screen
void UpdatePlaying();     // Handle input and logic for the Playing screen
void UpdatePause();       // Handle input and logic for the Pause screen
void UpdateGameOver();    
void DrawMenu();          
void DrawPlaying();       
void DrawPause();         
void DrawGameOver();     

// --- Main Function ---
int main() {
    // Initialization
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);
    srand(time(NULL)); // Seed random number generator

    // Load Resources
    // Ensure the font file path is correct relative to your executable
    gameFont = LoadFont("resources/fonts/alagard.png");
    if (gameFont.texture.id == 0) {
        // Fallback to default font if loading fails
        gameFont = GetFontDefault();
        TraceLog(LOG_WARNING, "Failed to load font 'alagard.png'. Using default font.");
    }

    InitGame(); // Initialize game variables

    // Main game loop
    while (!WindowShouldClose()) {
        // --- Update ---
        switch (currentScreen) {
        case MENU:      UpdateMenu(); break;
        case PLAYING:   UpdatePlaying(); break;
        case PAUSE:     UpdatePause(); break;
        case GAME_OVER: UpdateGameOver(); break;
        default: break;
        }

        // --- Draw ---
        BeginDrawing();
        ClearBackground(DARKGREEN); // Common background

        switch (currentScreen) {
        case MENU:      DrawMenu(); break;
        case PLAYING:   DrawPlaying(); break;
        case PAUSE:     DrawPause(); break; // DrawPause will often draw Playing underneath
        case GAME_OVER: DrawGameOver(); break; // DrawGameOver might draw final state underneath
        default: break;
        }

        EndDrawing();
    }

    // De-Initialization
    UnloadFont(gameFont);
    CloseWindow();

    return 0;
}

// --- Function Definitions ---

void InitGame() {
    // Reset scores
    leftScore = 0;
    rightScore = 0;

    // Initialize Paddles
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

    // Initialize Ball
    ResetBall(rand() % 2 == 0); // Start towards a random player
}

void ResetBall(bool toLeftPlayer) {
    ballPosition = { WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f };

    // Randomize vertical direction slightly
    float randomY = (float)(rand() % 100 - 50) / 50.0f; // Range -1.0 to 1.0
    ballSpeed.y = INITIAL_BALL_SPEED * randomY * 0.5f; // Less steep initial angle

    // Horizontal direction based on who scored (or random on initial start)
    ballSpeed.x = toLeftPlayer ? -INITIAL_BALL_SPEED : INITIAL_BALL_SPEED;

    // Ensure it's not perfectly horizontal (optional, prevents boring volleys)
    if (fabs(ballSpeed.y) < 1.0f) {
        ballSpeed.y = (ballSpeed.y >= 0) ? 1.0f : -1.0f;
    }
}

void UpdateMenu() {
    if (IsKeyPressed(KEY_ENTER)) {
        InitGame(); // Reset scores and positions
        currentScreen = PLAYING;
    }
}

void UpdatePlaying() {
    // Pause Check
    if (IsKeyPressed(KEY_P)) {
        currentScreen = PAUSE;
        return; // Skip the rest of the update for this frame
    }

    // --- Ball Movement ---
    ballPosition.x += ballSpeed.x;
    ballPosition.y += ballSpeed.y;

    // Ball Collision: Top/Bottom Walls
    if (ballPosition.y - BALL_RADIUS <= 0 || ballPosition.y + BALL_RADIUS >= WINDOW_HEIGHT) {
        ballSpeed.y *= -1.0f;
        // Prevent sticking: Adjust position slightly if stuck
        if (ballPosition.y - BALL_RADIUS < 0) ballPosition.y = BALL_RADIUS;
        if (ballPosition.y + BALL_RADIUS > WINDOW_HEIGHT) ballPosition.y = WINDOW_HEIGHT - BALL_RADIUS;
    }

    // Ball Collision: Paddles
    if (CheckCollisionCircleRec(ballPosition, BALL_RADIUS, leftPaddle.GetPaddleRectangle())) {
        if (ballSpeed.x < 0) { // Only reflect if moving towards paddle
            ballSpeed.x *= -1.05f; // Reverse direction and slightly increase speed
            // Adjust Y speed based on where it hits the paddle
            float deltaY = (ballPosition.y - (leftPaddle.GetPosition().y + leftPaddle.GetHeight() / 2.0f)) / (leftPaddle.GetHeight() / 2.0f);
            ballSpeed.y = deltaY * fabs(ballSpeed.x); // More angle further from center

            ballPosition.x = leftPaddle.GetPosition().x + leftPaddle.GetWidth() + BALL_RADIUS; // Prevent sticking
        }
    }
    if (CheckCollisionCircleRec(ballPosition, BALL_RADIUS, rightPaddle.GetPaddleRectangle())) {
        if (ballSpeed.x > 0) { // Only reflect if moving towards paddle
            ballSpeed.x *= -1.05f; // Reverse direction and slightly increase speed
            // Adjust Y speed based on where it hits the paddle
            float deltaY = (ballPosition.y - (rightPaddle.GetPosition().y + rightPaddle.GetHeight() / 2.0f)) / (rightPaddle.GetHeight() / 2.0f);
            ballSpeed.y = deltaY * fabs(ballSpeed.x); // More angle further from center

            ballPosition.x = rightPaddle.GetPosition().x - BALL_RADIUS; // Prevent sticking
        }
    }


    // --- Scoring ---
    if (ballPosition.x + BALL_RADIUS < 0) { // Right player scores
        rightScore++;
        if (rightScore >= WINNING_SCORE) {
            currentScreen = GAME_OVER;
        }
        else {
            ResetBall(true); // Reset ball towards left player
        }
    }
    else if (ballPosition.x - BALL_RADIUS > WINDOW_WIDTH) { // Left player scores
        leftScore++;
        if (leftScore >= WINNING_SCORE) {
            currentScreen = GAME_OVER;
        }
        else {
            ResetBall(false); // Reset ball towards right player
        }
    }

    // --- Paddle Movement & Input ---
    // Left Paddle (W/S)
    if (IsKeyDown(KEY_S))
        leftPaddle.SetSpeed(Vector2{ 0.0f, PADDLE_SPEED });
    else if (IsKeyDown(KEY_W))
        leftPaddle.SetSpeed(Vector2{ 0.0f, -PADDLE_SPEED });
    else
        leftPaddle.SetSpeed(Vector2{ 0.0f, leftPaddle.GetSpeed().y * PADDLE_DECELERATION });

    // Right Paddle (Up/Down Arrows)
    if (IsKeyDown(KEY_DOWN))
        rightPaddle.SetSpeed(Vector2{ 0.0f, PADDLE_SPEED });
    else if (IsKeyDown(KEY_UP))
        rightPaddle.SetSpeed(Vector2{ 0.0f, -PADDLE_SPEED });
    else
        rightPaddle.SetSpeed(Vector2{ 0.0f, rightPaddle.GetSpeed().y * PADDLE_DECELERATION });

    // Update Paddle Positions
    leftPaddle.Update();
    rightPaddle.Update();

    // --- Paddle Boundary Collision ---
    // Left Paddle
    if (leftPaddle.GetPosition().y <= 0) {
        leftPaddle.SetPosition(Vector2{ leftPaddle.GetPosition().x, 0 });
        leftPaddle.SetSpeed(Vector2{ 0, 0 }); // Stop movement at boundary
    }
    else if (leftPaddle.GetPosition().y + leftPaddle.GetHeight() >= WINDOW_HEIGHT) {
        leftPaddle.SetPosition(Vector2{ leftPaddle.GetPosition().x, WINDOW_HEIGHT - leftPaddle.GetHeight() });
        leftPaddle.SetSpeed(Vector2{ 0, 0 }); // Stop movement at boundary
    }
    // Right Paddle
    if (rightPaddle.GetPosition().y <= 0) {
        rightPaddle.SetPosition(Vector2{ rightPaddle.GetPosition().x, 0 });
        rightPaddle.SetSpeed(Vector2{ 0, 0 }); // Stop movement at boundary
    }
    else if (rightPaddle.GetPosition().y + rightPaddle.GetHeight() >= WINDOW_HEIGHT) {
        rightPaddle.SetPosition(Vector2{ rightPaddle.GetPosition().x, WINDOW_HEIGHT - rightPaddle.GetHeight() });
        rightPaddle.SetSpeed(Vector2{ 0, 0 }); // Stop movement at boundary
    }
}

void UpdatePause() {
    if (IsKeyPressed(KEY_P)) {
        currentScreen = PLAYING; // Resume game
    }
    if (IsKeyPressed(KEY_ENTER)) {
        currentScreen = MENU; // Go back to menu
    }
}

void UpdateGameOver() {
    if (IsKeyPressed(KEY_ENTER)) {
        currentScreen = MENU; // Go back to the menu
    }
}

void DrawTextCentered(const char* text, int posY, int fontSize, Color color) {
    int textWidth = MeasureTextEx(gameFont, text, (float)fontSize, 2).x;
    DrawTextEx(gameFont, text, Vector2{ (float)WINDOW_WIDTH / 2 - textWidth / 2, (float)posY }, (float)fontSize, 2, color);
}

void DrawMenu() {
    DrawTextCentered("PONG", WINDOW_HEIGHT / 4, 100, WHITE);
    DrawTextCentered("Press ENTER to Start", WINDOW_HEIGHT / 2, 40, LIGHTGRAY);
    DrawTextCentered("W/S for Left Paddle", WINDOW_HEIGHT / 2 + 50, 20, LIGHTGRAY);
    DrawTextCentered("UP/DOWN for Right Paddle", WINDOW_HEIGHT / 2 + 80, 20, LIGHTGRAY);
    DrawTextCentered("P to Pause during game", WINDOW_HEIGHT / 2 + 110, 20, LIGHTGRAY);
}

void DrawPlaying() {
    // Draw Center Line
    DrawRectangle(WINDOW_WIDTH / 2 - 2, 0, 4, WINDOW_HEIGHT, WHITE);

    // Draw Paddles
    leftPaddle.Draw();
    rightPaddle.Draw();

    // Draw Ball
    DrawCircleV(ballPosition, BALL_RADIUS, Color{ 2, 222, 233, 242 }); // Using your original color

    // Draw Scores
    std::string lScoreStr = std::to_string(leftScore);
    std::string rScoreStr = std::to_string(rightScore);
    DrawTextEx(gameFont, lScoreStr.c_str(), Vector2{ WINDOW_WIDTH / 4.0f, 20.0f }, 65, 2, DARKPURPLE);
    DrawTextEx(gameFont, rScoreStr.c_str(), Vector2{ (WINDOW_WIDTH / 4.0f + WINDOW_WIDTH / 2.0f) - 30.0f, 20.0f }, 65, 2, DARKPURPLE);
}

void DrawPause() {
    // Draw the underlying game state so players see where they left off
    DrawPlaying();

    // Draw pause overlay message
    DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Fade(BLACK, 0.7f)); // Semi-transparent overlay
    DrawTextCentered("PAUSED", WINDOW_HEIGHT / 2 - 60, 80, WHITE);
    DrawTextCentered("Press P to Resume", WINDOW_HEIGHT / 2 + 20, 30, LIGHTGRAY);
    DrawTextCentered("Press ENTER for Menu", WINDOW_HEIGHT / 2 + 60, 30, LIGHTGRAY);
}

void DrawGameOver() {
    // Draw the final game state
    DrawPlaying();

    // Draw Game Over overlay message
    DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Fade(BLACK, 0.8f)); // Darker overlay
    DrawTextCentered("GAME OVER", WINDOW_HEIGHT / 3, 80, RED);

    const char* winnerText = (leftScore > rightScore) ? "Left Player Wins!" : "Right Player Wins!";
    DrawTextCentered(winnerText, WINDOW_HEIGHT / 2 - 20, 50, WHITE);

    std::string finalScoreText = std::to_string(leftScore) + " - " + std::to_string(rightScore);
    DrawTextCentered(finalScoreText.c_str(), WINDOW_HEIGHT / 2 + 40, 40, LIGHTGRAY);

    DrawTextCentered("Press ENTER for Menu", WINDOW_HEIGHT / 2 + 100, 30, LIGHTGRAY);
}
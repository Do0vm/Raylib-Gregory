#include "raylib.h"
#include "Game.h"   
#include <cstdlib> 
#include <ctime>    

int main() {
    // Initialization
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);
    srand(time(NULL)); 

    // Load Resources 
    gameFont = LoadFont("resources/fonts/alagard.png");
    if (gameFont.texture.id == 0) {
        gameFont = GetFontDefault();
        TraceLog(LOG_WARNING, "Failed to load font 'alagard.png'. Using default font.");
    }

    InitGame(); // Initialize game state

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
        ClearBackground(DARKGREEN);

        switch (currentScreen) {
        case MENU:      DrawMenu(); break;
        case PLAYING:   DrawPlaying(); break;
        case PAUSE:     DrawPause(); break;
        case GAME_OVER: DrawGameOver(); break;
        default: break;
        }

        EndDrawing();
    }

    // De-Initialization
    UnloadFont(gameFont);
    CloseWindow();

    return 0;
}
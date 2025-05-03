#include "raylib.h"
#include "Game.h"   
#include <cstdlib> 
#include <ctime>    

int main() {
    // Initialization
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    InitAudioDevice();
    SetTargetFPS(60);
    srand(time(NULL)); 

    // Load Resources 
    gameFont = LoadFont("resources/fonts/alagard.png");
    if (gameFont.texture.id == 0) {
        gameFont = GetFontDefault();
        TraceLog(LOG_WARNING, "Failed to load font 'alagard.png'. Using default font.");
    }

    // Background Music

    backgroundMusic = LoadMusicStream("resources/audio/music.mp3");
    if (backgroundMusic.stream.buffer == NULL) {
        TraceLog(LOG_WARNING, "AUDIO: Failed to load music stream 'resources/audio/music.mp3'");
        // Handle error appropriately, maybe game continues without music
    }
    else {
        PlayMusicStream(backgroundMusic); 
        SetMusicVolume(backgroundMusic, 0.2f); 
    }


    // Background Texture
   
    backgroundTexture = LoadTexture("resources/textures/background.png");
    if (backgroundTexture.id == 0) {
        TraceLog(LOG_WARNING, "TEXTURE: Failed to load texture 'resources/textures/background.png'");
       
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

        if (backgroundTexture.id > 0) { 
            
            DrawTexture(backgroundTexture, 0, 0, WHITE);
        }
        else {
            
            ClearBackground(DARKGREEN);
        }


        ClearBackground(DARKGREEN);

        switch (currentScreen) {
        case MENU:      DrawMenu(); break;
        case PLAYING:   DrawPlaying(); break;
        case PAUSE:     DrawPause(); break;
        case GAME_OVER: DrawGameOver(); break;
        default: break;
        }

        if (backgroundMusic.stream.buffer != NULL) { 
            UpdateMusicStream(backgroundMusic);
        }

        EndDrawing();
    }

    // De-Initialization
    UnloadFont(gameFont);
    // Unload Music Stream
    if (backgroundMusic.stream.buffer != NULL) {
        UnloadMusicStream(backgroundMusic);
    }

    // Unload Texture
    if (backgroundTexture.id > 0) {
        UnloadTexture(backgroundTexture);
    }


    CloseAudioDevice();


    CloseWindow();

    return 0;
}
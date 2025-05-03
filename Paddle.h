#ifndef PADDLE_H
#define PADDLE_H

#include "raylib.h"

class Paddle {
private:
    Vector2 position;
    Vector2 speed;
    float width;
    float height;
    Color color;
    float deceleration; 

public:
    Paddle(); 

    void Init(Vector2 pos, Vector2 spd, float w, float h, float decel, Color col);
    void Update(float screenHeight); // Pass screenHeight for bounds checking
    void ApplyDeceleration();
    void Draw();

    // Getters
    Rectangle GetPaddleRectangle() const;
    Vector2 GetPosition() const;
    Vector2 GetSpeed() const;
    float GetWidth() const;
    float GetHeight() const;

    // Setters
    void SetPosition(Vector2 pos);
    void SetSpeed(Vector2 spd);
};

#endif 
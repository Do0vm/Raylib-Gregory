#include "Paddle.h"
#include "raylib.h" 
#include <cmath>

Paddle::Paddle() : position{ 0, 0 }, speed{ 0, 0 }, width(0), height(0), deceleration(0.9f), color(WHITE) {}

void Paddle::Init(Vector2 pos, Vector2 spd, float w, float h, float decel, Color col) {
    position = pos;
    speed = spd;
    width = w;
    height = h;
    deceleration = decel;
    color = col;
}

void Paddle::Update(float screenHeight) {
    position.y += speed.y;

    // Boundary Collision Check
    if (position.y <= 0) {
        position.y = 0;
        speed.y = 0; 
    }
    else if (position.y + height >= screenHeight) {
        position.y = screenHeight - height;
        speed.y = 0; 
    }
}

void Paddle::ApplyDeceleration() {
    speed.y *= deceleration;
    //This prevents driftss
    if (fabs(speed.y) < 0.1f) {
        speed.y = 0.0f;
    }
}

void Paddle::Draw() {
    DrawRectangleV(position, Vector2{ width, height }, color);
}

Rectangle Paddle::GetPaddleRectangle() const {
    return Rectangle{ position.x, position.y, width, height };
}

Vector2 Paddle::GetPosition() const {
    return position;
}

Vector2 Paddle::GetSpeed() const {
    return speed;
}

float Paddle::GetWidth() const {
    return width;
}

float Paddle::GetHeight() const {
    return height;
}

void Paddle::SetPosition(Vector2 pos) {
    position = pos;
}

void Paddle::SetSpeed(Vector2 spd) {
    speed = spd;
}
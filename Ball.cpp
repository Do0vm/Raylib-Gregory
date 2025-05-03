#include "Ball.h"
#include "raylib.h"
#include <cmath>   
#include <cstdlib> 

Ball::Ball() : position{ 0, 0 }, speed{ 0, 0 }, radius(0), color(WHITE), initialSpeedMagnitude(5.0f) {}

void Ball::Init(Vector2 pos, Vector2 initialSpeed, float rad, Color col, float initialMag) {
    position = pos;
    speed = initialSpeed;
    radius = rad;
    color = col;
    initialSpeedMagnitude = initialMag;
}

void Ball::Reset(float screenWidth, float screenHeight, bool toLeftPlayer) {
    position = { screenWidth / 2.0f, screenHeight / 2.0f };

    float randomY = (float)(rand() % 100 - 50) / 50.0f; // Range -1.0 to 1.0
    speed.y = initialSpeedMagnitude * randomY * 0.5f; 
    speed.x = toLeftPlayer ? -initialSpeedMagnitude : initialSpeedMagnitude;

    if (fabs(speed.y) < 1.0f) {
        speed.y = (speed.y >= 0) ? 1.0f : -1.0f;
    }
}

void Ball::Update(float screenHeight) {
    position.x += speed.x;
    position.y += speed.y;

    // Ball Collision: Top/Bottom Walls
    if (position.y - radius <= 0 || position.y + radius >= screenHeight) {
        speed.y *= -1.0f;
        // Prevent sticking
        if (position.y - radius < 0) position.y = radius;
        if (position.y + radius > screenHeight) position.y = screenHeight - radius;
    }
}

void Ball::Draw() {
    DrawCircleV(position, radius, color);
}

bool Ball::CheckCollisionWithPaddle(const Paddle& paddle) {
    return CheckCollisionCircleRec(position, radius, paddle.GetPaddleRectangle());
}

void Ball::HandlePaddleCollision(const Paddle& paddle) {
    // Only reflect if moving towards the paddle that was hit
    bool movingTowardsLeft = speed.x < 0;
    bool isLeftPaddle = paddle.GetPosition().x < GetScreenWidth() / 2.0f; // Basic check

    if ((movingTowardsLeft && isLeftPaddle) || (!movingTowardsLeft && !isLeftPaddle))
    {
        speed.x *= -1.05f; // Reverse direction and slightly increase speed

        // Adjust Y speed based on where it hits the paddle
        float deltaY = (position.y - (paddle.GetPosition().y + paddle.GetHeight() / 2.0f)) / (paddle.GetHeight() / 2.0f);
        speed.y = deltaY * fabs(speed.x); // More angle further from center

        
        if (isLeftPaddle) {
            position.x = paddle.GetPosition().x + paddle.GetWidth() + radius;
        }
        else {
            position.x = paddle.GetPosition().x - radius;
        }
    }
}


Vector2 Ball::GetPosition() const {
    return position;
}

float Ball::GetRadius() const {
    return radius;
}

void Ball::SetPosition(Vector2 pos) {
    position = pos;
}

void Ball::SetSpeed(Vector2 spd) {
    speed = spd;
}

void Ball::ReverseXSpeed() {
    speed.x *= -1.0f;
}

void Ball::ReverseYSpeed() {
    speed.y *= -1.0f;
}
#pragma once
#ifndef BALL_H
#define BALL_H

#include "raylib.h"
#include "Paddle.h" 

class Ball {
private:
    Vector2 position;
    Vector2 speed;
    float radius;
    Color color;
    float initialSpeedMagnitude;

public:
    Ball();

    void Init(Vector2 pos, Vector2 initialSpeed, float rad, Color col, float initialMag);
    void Reset(float screenWidth, float screenHeight, bool toLeftPlayer);
    void Update(float screenHeight); // Handles movement and wall collision
    void Draw();
    bool CheckCollisionWithPaddle(const Paddle& paddle);
    void HandlePaddleCollision(const Paddle& paddle);    // Handle reflection logic

    // Getters
    Vector2 GetPosition() const;
    float GetRadius() const;

    // Setters 
    void SetPosition(Vector2 pos);
    void SetSpeed(Vector2 spd);
    void ReverseXSpeed();
    void ReverseYSpeed();

};

#endif 
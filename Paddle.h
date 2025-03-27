#pragma once
#include "raylib.h"
#include <iostream>
#include <vector>

using namespace std;

class Paddle {
	

	Paddle();
	Paddle(float x, float y, float width, float height, int speedDown, int speedUp);

public:



	

	int GetSpeedDown() const;
	int GetSpeedUp() const;

protected:





};
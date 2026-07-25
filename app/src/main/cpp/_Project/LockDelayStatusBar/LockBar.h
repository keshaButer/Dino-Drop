#pragma once
#include "SmartPointers.h"
#include "../Core/Config.h"

class ActiveTetromino;
class SpriteRenderer;

class LockBar
{
public:
    LockBar(ActiveTetromino* tetromino);
    void OnContextRestored();

    void Update(float deltaTime);
    void Draw();

private:
    ActiveTetromino* activeTetromino; 
    Unique<SpriteRenderer> sprite;

    float progress = 0.0f;
    float yOffset = -1.0f * (Config::Gameplay::CELL_SIZE * static_cast<int>(Config::Gameplay::BOARD_HEIGHT * 0.5f)) - 0.0125f;

    float lerp(float a, float b, float accel)
    {
        return a + (b - a) * accel;
    }
};

#pragma once
#include "../Board/board.h"
#include "draw_tetromino.h"
#include "../Camera/camera.h"
#include "../Engine/engine.h"

class ActiveTetromino
{
public:
    ActiveTetromino(Board* board, DrawTetromino* renderer, Camera* camera);

    void Update(float deltaTime);
    void SetCurrentLevel();
    void HandleInput(int32_t action, float deltaX, float deltaY);
    void HandleHandledInput(float deltaX, float deltaY, bool isDraggingX, bool isDraggingY);
    void SaveStartGesturePosition();
    void CancelHorizontalMove();
    void Rotate();
    void HardDrop();
    void Draw();
    float GetLockDelayTimer() const { return lockDelayTimer; }

private:
    Board* const board;
    DrawTetromino* const tetrominoRenderer;
    Camera* const camera;

    int type = 0;
    int nextType = 0;
    int rotation;
    int row;
    int col;
    int ghostRow;
    int lastCol;
    int lastRotation;
    bool wasFreeze;
    float lockDelayTimer = 0.0f;
    bool isInput = false;

    int currentLevel = 0;
    int countClearedLines = 0;

    float spawnInterval = Config::Gameplay::DROP_INTERVAL;
    float sensitivityX = Config::Gameplay::CELL_SIZE * Engine::Get().GetWindowWidth() * Config::Control::DRAG_SENSITIVITY_X;
    float sensitivityY = Config::Gameplay::CELL_SIZE * Engine::Get().GetWindowWidth() * Config::Control::DRAG_SENSITIVITY_Y;
    float accumulatedDx = 0.0f;
    float accumulatedDy = 0.0f;
    int startColOfGesture = 0;
    int mixX = 5;
    int maxY = -1;

    float dropTimer = 0.0f;

    void SpawnPiece();
    void FreezePiece();
    void DrawGhost();
    void DrawNext();
    void SetLean();
    int GetPieceWidth();
    bool IsPositionValid(int nextRow, int nextCol, int nextRotation);
    bool IsTraped();
    void ResetLockDelay();

    Shader* const ghostShader;

    float startTouchX;
    float startTouchY;
    uint64_t startTouchTime;
    bool isDragging;
    bool isDraggingX = false;
    bool isFirstHorizontalStep = true;
    bool canRotateThisTouch = true;
    bool isDraggingY = false;
    bool isTraped = false;
    int lockMoveCount = 0;

    float squashX = 1.0f;
    float squashY = 1.0f;
    float squashVelocity = 0.0f;

    float targetLeanAngle = 0.0f;
    float currentLeanAngle = 0.0f;
};

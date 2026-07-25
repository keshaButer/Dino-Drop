#pragma once
#include "../Camera/camera.h"
#include "GameState.h"


class GameStateManager
{
public:
    static GameStateManager& Get() { static GameStateManager instance; return instance; }

    void SetState(GameState* gameState);
    void UpdateCurrentState(float deltaTime);
    GameState* GetCurrentState() { return currentState; }

    Camera* mainCamera;

private:
    GameStateManager() = default;

    void ChangeState();

    bool isChangeState = false;
    GameState* currentState = nullptr;
    GameState* changingState = nullptr;
};

#include "GameState.h"
#include "GameStateManager.h"
#include "../Engine/engine.h"
#include "../Pause/PauseManager.h"
#include "../Score/ScoreManager.h"

void GameStateManager::SetState(GameState* gameState)
{
    changingState = gameState;
    isChangeState = true;
}

void GameStateManager::ChangeState()
{
    mainCamera->ClearViewMatrix();
    Engine::Get().OnUIInput.Clear();
    Engine::Get().OnGameplayInput.Clear();
    ScoreManager::Get().Clear();
    PauseManager::Get().SetPaused(false);

    if (currentState != nullptr)
    {
        currentState->Exit();
        delete currentState;
    }

    currentState = changingState;
    changingState = nullptr;

    if (currentState != nullptr)
    {
        currentState->Enter();
    }
}

void GameStateManager::UpdateCurrentState(float deltaTime)
{
    if (currentState != nullptr)
        currentState->Update(deltaTime);
    else
        Engine::Get().PrintError("GameStateManager: currentState is null!");

    if (isChangeState)
    {
        Engine::Get().PrintError("GameStateManager: changing state...");
        isChangeState = false;
        ChangeState();
    }
}

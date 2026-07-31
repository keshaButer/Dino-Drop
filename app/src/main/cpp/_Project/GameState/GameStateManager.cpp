#include "GameState.h"
#include "GameStateManager.h"
#include "../Engine/engine.h"
#include "../Pause/PauseManager.h"
#include "../Score/HighScoreManager.h"
#include "../Score/ScoreManager.h"
#include "../Audio/AudioManager.h"

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
    AudioManager::Get().Reset();
    PauseManager::Get().SetPaused(false);

    if (currentState != nullptr)
    {
        currentState->Exit();
        HighScoreManager::Get().WriteToFile();
        ScoreManager::Get().Reset();

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

#pragma once
#include "../GameState.h"
#include "../../FontRenderer/FontRenderer.h"
#include "../Camera/camera.h"
#include "SmartPointers.h"
#include "../Button/Button.h"

class GameOverState : public GameState
{
public:
    GameOverState(Camera* camera);
    void Enter() override;
    void Update(float deltaTime) override;
    void Exit() override;

    void HandleInput(int32_t action, float x, float y);

private:
    Unique<SpriteRenderer> spriteRenderer;
    Unique<FontRenderer> fontRenderer;
    Unique<Button> restartButton;
    Camera* const mainCamera;
};

#pragma once
#include "../GameState.h"
#include "../../FontRenderer/FontRenderer.h"
#include "../Camera/camera.h"
#include "../Background/moving_background.h"
#include "SmartPointers.h"
#include "../Button/Button.h"

class GameOverState : public GameState
{
public:
    GameOverState(Camera* camera, StateData data);
    void Enter() override;
    void Update(float deltaTime) override;
    void Exit() override;

    void HandleInput(int32_t action, float x, float y);

private:
    Unique<SpriteRenderer> spriteRenderer;
    Unique<FontRenderer> fontRenderer;
    Unique<Button> restartButton;
    Unique<Button> enterMenuButton;
    Unique<Background> background;
    Camera* const mainCamera;
    Texture* BG = nullptr;
    StateData data;
};

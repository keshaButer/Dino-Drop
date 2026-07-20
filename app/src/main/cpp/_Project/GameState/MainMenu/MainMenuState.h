#pragma once
#include "../GameState.h"
#include "../Background/moving_background.h"
#include "../FontRenderer/FontRenderer.h"
#include "../Button/Button.h"
#include "../Camera/camera.h"
#include "SmartPointers.h"

class MainMenuState : public GameState
{
public:
    MainMenuState(Camera* camera);

    void Enter() override;
    void Update(float deltaTime) override;
    void Exit() override;

    void UpdateButtons(float deltaTime);
    void DrawButtons();

private:
    Unique<Background> background;
    Unique<FontRenderer> textRenderer;
    Unique<SpriteRenderer> spriteRenderer;
    Unique<Button> startButton;
    Unique<Button> exitButton;
    Unique<Button> settingsButton;
    Camera* const mainCamera;
};

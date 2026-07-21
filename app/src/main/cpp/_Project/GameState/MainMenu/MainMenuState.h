#pragma once
#include "../GameState.h"
#include "../Background/moving_background.h"
#include "../FontRenderer/FontRenderer.h"
#include "../Button/Button.h"
#include "../Camera/camera.h"
#include <SmartPointers.h>

class MainMenuState : public GameState
{
public:
    MainMenuState(Camera* camera);

    void Enter() override;
    void Update(float deltaTime) override;
    void Exit() override;

    enum class MenuScreen
    {
        Main,
        Scores,
        Settings
    };

private:
    void UpdateButtons(float deltaTime);
    void DrawButtons();
    void InitializeButtons();
    void SubscribeButtons();

    MenuScreen currentScreen = MenuScreen::Main;

    Unique<Background> background;
    Unique<FontRenderer> textRenderer;
    Unique<SpriteRenderer> spriteRenderer;
    Unique<Button> startButton;
    Unique<Button> exitButton;
    Unique<Button> settingsButton;
    Unique<Button> scoresButton;
    Unique<Button> backButton;
    Camera* const mainCamera;
};

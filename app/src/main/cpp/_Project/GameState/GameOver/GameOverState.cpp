#include "GameOverState.h"
#include "../../Engine/engine.h"
#include "../Core/Config.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../GameStateManager.h"
#include "../Gameplay/GameplayState.h"
#include "../Audio/AudioManager.h"
#include "../Core/Config.h"

GameOverState::GameOverState(Camera* camera) : mainCamera(camera)
{}

void GameOverState::Enter()
{
    Engine::Get().PrintInfo("GameOverState: Enter");

    fontRenderer = std::make_unique<FontRenderer>(Config::GetFontPathOTF("Base").c_str(), 128);
    spriteRenderer = std::make_unique<SpriteRenderer>();

    restartButton = std::make_unique<Button>(
        glm::vec2(0.0f, -0.6f),
        1.0f,
        0.5f,
        "Restart",
        spriteRenderer.get(),
        fontRenderer.get(),
        0.6f
    );

    restartButton->onPress.Subscribe([this]() { GameStateManager::Get().SetState(new GameplayState(mainCamera)); } );

    AudioManager::Get().PlayAudioClip(Config::Sound::GAME_OVER_SOUND);
}

void GameOverState::Update(float deltaTime)
{
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    fontRenderer->RenderText("Game Over", glm::vec2(0.0f), Config::UI::GAME_OVER_TEXT_SIZE, glm::vec4(0, 0, 0, 1));
    restartButton->Update(deltaTime);
    restartButton->Draw();
}

void GameOverState::Exit()
{

}

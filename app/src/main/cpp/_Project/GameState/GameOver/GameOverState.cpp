#include "GameOverState.h"
#include "../../Engine/engine.h"
#include "../Core/Config.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include "../GameStateManager.h"
#include "../Shaders/ShaderManager.h"
#include "../Pause/PauseManager.h"
#include "../Gameplay/GameplayState.h"
#include "../Audio/AudioManager.h"
#include "../MainMenu/MainMenuState.h"
#include "../Core/Config.h"

GameOverState::GameOverState(Camera* camera, StateData _data) : mainCamera(camera), data(_data)
{}

void GameOverState::OnContextRestored()
{
    mainCamera->ClearViewMatrix();
    PauseManager::Get().SetPaused(false);

    BG = ResourceManager::Get().GetTexture(Config::TextureNames::GAME_OVER_BG);
    background->texture = ResourceManager::Get().GetTexture(Config::TextureNames::GAMEPLAY_BACKGROUND);
    fontRenderer->Initialize(Config::GetFontPathOTF("Base").c_str(), 128);

    Shader* shader = ShaderManager::Get().GetShader(Config::ShaderNames::DEFAULT_TEXTURE);
    spriteRenderer->shader = shader;
    spriteRenderer->SetTextureID(shader->textureID);

    enterMenuButton->OnContextRestore();
    restartButton->OnContextRestore();
}

void GameOverState::Enter()
{
    Engine::Get().PrintInfo("GameOverState: Enter");

    background = std::make_unique<Background>(ResourceManager::Get().GetTexture(Config::TextureNames::GAMEPLAY_BACKGROUND));

    BG = ResourceManager::Get().GetTexture(Config::TextureNames::GAME_OVER_BG);
    fontRenderer = std::make_unique<FontRenderer>(Config::GetFontPathOTF("Base").c_str(), 128);
    spriteRenderer = std::make_unique<SpriteRenderer>();

    glm::vec2 size = glm::vec2(0.9f, 0.4f);
    enterMenuButton = std::make_unique<Button>(
        glm::vec2(0.0f, -1.2f),
        size,
        "Menu",
        spriteRenderer.get(),
        fontRenderer.get(),
        0.6f
    );

    restartButton = std::make_unique<Button>(
        glm::vec2(0.0f, -0.7f),
        size,
        "Restart",
        spriteRenderer.get(),
        fontRenderer.get(),
        0.6f
    );

    enterMenuButton->onPress.Subscribe([this]() { GameStateManager::Get().SetState(new MainMenuState(mainCamera)); } );
    restartButton->onPress.Subscribe([this]() { GameStateManager::Get().SetState(new GameplayState(mainCamera)); } );

    AudioManager::Get().PlayAudioClip(Config::Sound::GAME_OVER_SOUND);
}

void GameOverState::Update(float deltaTime)
{
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    background->Draw();
    spriteRenderer->Draw(glm::vec2(0.0f, 0.85f), glm::vec2(0.9f, 0.4f), 0, glm::vec4(1.0f), BG);
    fontRenderer->RenderText("Game Over", glm::vec2(0.0f, 1.0f), Config::UI::GAME_OVER_TEXT_SIZE, glm::vec4(1.0f));
    fontRenderer->RenderText("Score: " + std::to_string(data.Score), glm::vec2(0.0f, 0.0f), Config::UI::SCORE_GAME_OVER_TEXT_SIZE, glm::vec4(1.0f));

    enterMenuButton->Update(deltaTime);
    enterMenuButton->Draw();

    restartButton->Update(deltaTime);
    restartButton->Draw();
}

void GameOverState::Exit()
{

}

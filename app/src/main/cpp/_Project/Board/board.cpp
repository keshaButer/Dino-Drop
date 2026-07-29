#include "board.h"
#include <memory>
#include "../ParticleSystem/FXSystem.h"
#include "../Grid/grid.h"
#include "../ParticleSystem/LineNeonFlash/line_neon_flash.h"
#include "../GameSettings/SettingsManager.h"
#include "../ParticleSystem/LineNeonFlash/particle_explosion.h"
#include "../Invoke/InvokeSystem.h"
#include "../Shaders/ShaderManager.h"
#include "../Audio/AudioManager.h"

Board::Board(DrawTetromino* tetrominoRenderer, Camera* _camera, Grid* _grid) : 
    tetrominoRenderer(tetrominoRenderer), camera(_camera),
    flashShader(ShaderManager::Get().GetShader("Flash")), 
    particlesShader(ShaderManager::Get().GetShader("Particles")),
    grid(_grid),
    outlineRenderer(ShaderManager::Get().GetShader(Config::ShaderNames::DEFAULT_NO_TEXTURE))
{
    Clear();
}

void Board::OnContextRestored()
{
    flashShader = ShaderManager::Get().GetShader("Flash");
    particlesShader = ShaderManager::Get().GetShader("Particles");
    outlineRenderer.shader = ShaderManager::Get().GetShader(Config::ShaderNames::DEFAULT_NO_TEXTURE);
}

void Board::Draw()
{
    if (SettingsManager::Get().GetSettings().isOutlineEnabled)
    {
        for (int r = 0; r < Config::Gameplay::BOARD_HEIGHT; r++)
        {
            for (int c = 0; c < Config::Gameplay::BOARD_WIDTH; c++)
            {
                if (board[r][c] > 0)
                {
                    tetrominoRenderer->DrawOutLine(r, c, glm::vec4(1.0f), Config::Gameplay::OUTLINE_THICKNESS, outlineRenderer);
                }
            }
        }
    }

    for (int r = 0; r < Config::Gameplay::BOARD_HEIGHT; r++)
    {
        for (int c = 0; c < Config::Gameplay::BOARD_WIDTH; c++)
        {
            int blockType = board[r][c];

            if (blockType > 0)
            {
                int colorIndex = blockType - 1;

                tetrominoRenderer->Draw(r, c, colorIndex);
            }
        }
    }
}

void Board::Clear()
{
    for (int r = 0; r < Config::Gameplay::BOARD_HEIGHT; r++)
    {
        for (int c = 0; c < Config::Gameplay::BOARD_WIDTH; c++)
        {
            board[r][c] = 0;
        }
    }
}

int Board::GetCell(int row, int col) const
{
    if (row >= 0 && row < Config::Gameplay::BOARD_HEIGHT && col >= 0 && col < Config::Gameplay::BOARD_WIDTH)
    {
        return board[row][col];
    }
    return 0;
}

void Board::SetCell(int row, int col, int value)
{
    if (row >= 0 && row < Config::Gameplay::BOARD_HEIGHT && col >= 0 && col < Config::Gameplay::BOARD_WIDTH)
    {
        board[row][col] = value;
    }
}

int Board::ClearFullLines()
{
    int fullLines[4];
    int fullLinesCount = 0;

    for (int r = 0; r < Config::Gameplay::BOARD_HEIGHT; r++)
    {
        bool isLineFull = true;

        for (int c = 0; c < Config::Gameplay::BOARD_WIDTH; c++)
        {
            if (board[r][c] == 0)
            {
                isLineFull = false;
                break;
            }
        }

        if (isLineFull)
        {
            FXSystem::Get().AddEffect(std::make_unique<LineNeonFlash>(r, glm::vec3(1), flashShader));
            FXSystem::Get().AddLateEffect(std::make_unique<ParticleExplosion>(r, glm::vec3(1), particlesShader));

            grid->TriggerWave(r);

            fullLines[fullLinesCount] = r;
            fullLinesCount++;

            for(int i = 0; i < Config::Gameplay::BOARD_WIDTH; i++)
            {
                board[r][i] = 0;
            }
        }
    }

    InvokeSystem::Get().Add(Config::Gameplay::SHAKE_DURATION - 0.1f, [=]() {
    for (int i = fullLinesCount - 1; i >= 0; i--)
    {
        int row = fullLines[i];

        for (int nextRow = row; nextRow < Config::Gameplay::BOARD_HEIGHT - 1; nextRow++)
        {
            for (int c = 0; c < Config::Gameplay::BOARD_WIDTH; c++)
            {
                board[nextRow][c] = board[nextRow + 1][c];
            }
        }

        for (int c = 0; c < Config::Gameplay::BOARD_WIDTH; c++)
        {
            board[Config::Gameplay::BOARD_HEIGHT - 1][c] = 0;
        }
    }
    });

    if (fullLinesCount > 0)
    {
        PauseManager::Get().SetPausedGameplay(true);
        InvokeSystem::Get().Add(Config::Gameplay::PAUSE_DELAY_ON_CLEAR_LINE, [this]() { PauseManager::Get().SetPausedGameplay(false); });
        OnLinesCleard.Invoke(fullLinesCount);
        camera->TriggerShake(Config::Gameplay::SHAKE_INTENSY * fullLinesCount, Config::Gameplay::SHAKE_DURATION);
        AudioManager::Get().PlayAudioClip(Config::Sound::CLEAR_LINE_SOUND, false, 1.0f, Engine::Get().RandomRange(0.85f, 0.95f));
    }
    
    return fullLinesCount;
}

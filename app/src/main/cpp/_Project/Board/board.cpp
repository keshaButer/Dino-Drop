#include "board.h"
#include <memory>
#include "../ParticleSystem/FXSystem.h"
#include "../ParticleSystem/LineNeonFlash/line_neon_flash.h"
#include "../ParticleSystem/LineNeonFlash/particle_explosion.h"
#include "../Invoke/InvokeSystem.h"
#include "../Shaders/ShaderManager.h"
#include "../Audio/AudioManager.h"

Board::Board(DrawTetromino* tetrominoRenderer, Camera* _camera)
 : tetrominoRenderer(tetrominoRenderer), camera(_camera),
   flashShader(ShaderManager::Get().GetShader("Flash")), particlesShader(ShaderManager::Get().GetShader("Particles"))
{
    Clear();

}

void Board::Draw()
{
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
        OnLinesCleard.Invoke(fullLinesCount);
        camera->TriggerShake(Config::Gameplay::SHAKE_INTENSY * fullLinesCount, Config::Gameplay::SHAKE_DURATION);
        AudioManager::Get().PlayAudioClip(Config::Sound::CLEAR_LINE_SOUND, false, 1.0f, Engine::Get().RandomRange(0.85f, 0.95f));
    }
    
    return fullLinesCount;
}

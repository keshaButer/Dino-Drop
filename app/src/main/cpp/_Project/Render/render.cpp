#include <GLES3/gl3.h>
#include "render.h"
#include "../Grid/grid.h"
#include "../Board/board.h"
#include "../Tetrominos/activeTetromino.h"
#include "../ParticleSystem/FXSystem.h"
#include "../Background/moving_background.h"
#include "../Score/DrawScore.h"
#include "../Button/Button.h"


Render::Render(Grid* _grid, Board* board, ActiveTetromino* activeTetromino, Background* background, DrawScore* _drawScore, Button* _pauseButton) : 
grid(_grid), board(board), activeTetromino(activeTetromino), background(background), drawScore(_drawScore), pauseButton(_pauseButton)
{ }

void Render::RenderFrame()
{
    background->Draw();
    grid->Draw();

    FXSystem::Get().Render();

    board->Draw();
    activeTetromino->Draw();

    FXSystem::Get().LateRender();

    pauseButton->Draw();
    drawScore->Draw();
}

#pragma once
#include "../Core/Config.h"
#include "../Tetrominos/draw_tetromino.h"
#include "../Camera/camera.h"
#include "../Event/Event.h"

class Board
{
public:
    Board(DrawTetromino* tetrominoRenderer, Camera* _camera);

    void Draw();
    void Clear();

    int GetCell(int row, int col) const;
    void SetCell(int row, int col, int value);

    int ClearFullLines();

    Event<int> OnLinesCleard;
    bool wasClearLine = false;

private:
    int board[Config::Gameplay::BOARD_HEIGHT][Config::Gameplay::BOARD_WIDTH] = {0};
    DrawTetromino* const tetrominoRenderer;
    Camera* const camera;
    Shader* const flashShader;
    Shader* const particlesShader;
};

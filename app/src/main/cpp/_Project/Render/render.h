#pragma once

class Board;
class Grid;
class ActiveTetromino;
class Camera;
class Background;
class DrawScore;
class Button;

class Render
{
public:
    Render(Grid* grid, Board* board, ActiveTetromino* activeTetromino, Background* background, DrawScore* _drawScore, Button* pauseButton);

    void RenderFrame();

private:
    Button* pauseButton;
    Grid* const grid;
    Board* const board;
    ActiveTetromino* const activeTetromino;
    Background* const background;
    DrawScore* const drawScore;
};

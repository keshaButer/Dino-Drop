#pragma once
#include "../GameState.h"
#include "SmartPointers.h"

class Render;
class Grid;
class Board;
class DrawTetromino;
class ActiveTetromino;
class Camera;
class Background;
class DrawScore;
class Button;
class SpriteRenderer;
class FontRenderer;
class LockBar;
class Texture;

class GameplayState : public GameState
{
public:
    GameplayState(Camera* camera);

    void Enter() override;
    void Update(float deltaTime) override;
    void Exit() override;

    void OnContextRestored() override;

private:
    bool isPaused = false;

    Unique<SpriteRenderer> spriteRenderer;
    Unique<FontRenderer> fontRenderer;
    Unique<Render> render;
    Unique<Button> pauseButton;
    Unique<Grid> grid;
    Unique<Board> board;
    Unique<DrawTetromino> drawTetromino;
    Unique<ActiveTetromino> activeTetromino;
    Unique<Background> background;
    Unique<DrawScore> drawScore;
    Unique<LockBar> lockBar;
    Unique<Button> restartButton;
    Unique<Button> enterMenuButton;
    Camera* const mainCamera; 
    Texture* pauseBG;
};

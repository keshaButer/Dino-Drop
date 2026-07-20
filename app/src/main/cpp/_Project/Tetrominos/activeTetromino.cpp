#include "activeTetromino.h"
#include "data_tetromino.h"
#include <cstdlib>
#include "../ParticleSystem/FXSystem.h"
#include "../ParticleSystem/GhostTrail/ghost_trail.h"
#include "../Shaders/ShaderManager.h"
#include "../GameState/GameStateManager.h"
#include "../GameState/GameOver/GameOverState.h"
#include "../Score/ScoreManager.h"
#include "../Audio/AudioManager.h"
#include "../Invoke/InvokeSystem.h"
#include "../Pause/PauseManager.h"

ActiveTetromino::ActiveTetromino(Board* board, DrawTetromino* renderer, Camera* camera) : 
board(board), tetrominoRenderer(renderer),
camera(camera), ghostShader(ShaderManager::Get().GetShader("GhostFX"))
{
    Engine::Get().OnGameplayInput.Subscribe(
        [this](int32_t action, float x, float y) { this->HandleInput(action, x, y); }
    );

    nextType = rand() % 7;
    SpawnPiece();
}

void ActiveTetromino::SpawnPiece()
{
    Engine::Get().PrintDebug("Spawn Piece");
    type = nextType;
    rotation = 0;
    row = Config::Gameplay::BOARD_HEIGHT - 2;
    col = (Config::Gameplay::BOARD_WIDTH / 2) - 2;

    while (nextType == type)
    {
        nextType = rand() % 7;
    }

    if (!IsPositionValid(row - 1, col, rotation))
    {
        board->Clear();
        Engine::Get().PrintInfo("===GAME OVER==="); 
        StateData data { .Score = ScoreManager::Get().GetCurrentScore() };
        GameStateManager::Get().SetState(new GameOverState(camera, data));
        return;
    }

    Engine::Get().PrintInfo("Next type is: %i.", nextType); 
}

void ActiveTetromino::Update(float deltaTime)
{
    dropTimer += deltaTime;

    if (dropTimer >= spawnInterval)
    {
        dropTimer -= spawnInterval;
        
        if (IsPositionValid(row - 1, col, rotation))
        {
            row--;
        }
        else
        {
            FreezePiece();
            SetCurrentLevel();
            SpawnPiece();
        }
    }
}

void ActiveTetromino::FreezePiece()
{
    AudioManager::Get().PlayAudioClip(Config::Sound::TETROMINO_FREEZE_SOUND, false, 1.0f, Engine::Get().RandomRange(0.8f, 1.2f));

    for (int i = 0; i < 4; i++)
    {
        Point localPoint = TETROMINO_SHAPES[type][rotation][i];
        int worldRow = row + localPoint.y;
        int worldCol = col + localPoint.x;
        board->SetCell(worldRow, worldCol, type + 1);
    }

    InvokeSystem::Get().Add(Config::Gameplay::SHAKE_DURATION - 0.09f, [=]() { wasFreeze = true; });

    wasFreeze = true;
}

void ActiveTetromino::Draw()
{
    DrawNext();
    DrawGhost();

    for (int i = 0; i < 4; i++)
    {
        Point localPoint = TETROMINO_SHAPES[type][rotation][i];
        tetrominoRenderer->Draw(row + localPoint.y, col + localPoint.x, type);
    }
}

void ActiveTetromino::DrawNext()
{
    float rowNext = Config::Gameplay::BOARD_HEIGHT + 0.3;
    float colNext = (Config::Gameplay::BOARD_WIDTH / 2.0) + 2.5;

    for (int i = 0; i < 4; i++)
    {
        Point localPoint = TETROMINO_SHAPES[nextType][0][i];
        tetrominoRenderer->Draw(
            rowNext + localPoint.y * Config::UI::NEXT_TETROMINO_SCALE,
            colNext + localPoint.x * Config::UI::NEXT_TETROMINO_SCALE,
            nextType,
            Config::UI::NEXT_TETROMINO_SCALE
        );
    }
}

void ActiveTetromino::DrawGhost()
{
    if (lastCol != col || lastRotation != rotation || wasFreeze || board->wasClearLine)
    {
        ghostRow = row;

        while (IsPositionValid(ghostRow - 1, col, rotation))
        {
            ghostRow--;
        }

        lastCol = col;
        lastRotation = rotation;
        wasFreeze = false;
        board->wasClearLine = false;
    }

    for (int i = 0; i < 4; i++)
    {
        Point localPoint = TETROMINO_SHAPES[type][rotation][i];
        tetrominoRenderer->Draw(ghostRow + localPoint.y, col + localPoint.x, type, true);
    }
}

bool ActiveTetromino::IsPositionValid(int nextRow, int nextCol, int nextRotation)
{
    for (int i = 0; i < 4; i++)
    {
        Point localPos = TETROMINO_SHAPES[type][nextRotation][i];
        
        int absRow = nextRow + localPos.y;
        int absCol = nextCol + localPos.x;

        if (absCol < 0 || absCol >= Config::Gameplay::BOARD_WIDTH) return false;
        
        if (absRow < 0) return false;

        if (absRow < Config::Gameplay::BOARD_HEIGHT)
        {
            if (board->GetCell(absRow, absCol) > 0) return false;
        }
    }
    return true;
}

void ActiveTetromino::HandleInput(int32_t action, float touchX, float touchY)
{
    if (PauseManager::Get().IsPaused()) return;

    uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();


    static float lastFrameX = 0.0f;
    static float lastFrameY = 0.0f;

    switch (action)
    {
        case AMOTION_EVENT_ACTION_DOWN:
        {
            startTouchX = touchX;
            startTouchY = touchY;
            lastFrameX = touchX;
            lastFrameY = touchY;
            startTouchTime = now;
            isDraggingX = false;
            isDraggingY = false;

            isFirstHorizontalStep = true;
            canRotateThisTouch = true;

            SaveStartGesturePosition();

        } break;

        case AMOTION_EVENT_ACTION_MOVE:
        {
            float dx = touchX - lastFrameX;
            float dy = touchY - lastFrameY;

            float totalDistX = touchX - startTouchX;
            float totalDistY = touchY - startTouchY;
            float totalDistSq = totalDistX * totalDistX + totalDistY * totalDistY;

            if (canRotateThisTouch && totalDistSq > (Config::Control::ROTATE_CANCEL_DISTANCE * Config::Control::ROTATE_CANCEL_DISTANCE))
            {
                canRotateThisTouch = false;
            }

            if (!isDraggingX && !isDraggingY)
            {
                if (totalDistSq > Config::Control::THRESHOLD_DRAGGING)
                {
                    if (std::abs(totalDistY) > std::abs(totalDistX))
                    {
                        isDraggingY = true;
                        isDraggingX = false;
                    }
                    else
                    {
                        isDraggingX = true;
                        isDraggingY = false;
                    }
                }
            }

            HandleHandledInput(dx, dy, isDraggingX, isDraggingY);

            lastFrameX = touchX;
            lastFrameY = touchY;

        } break;

        case AMOTION_EVENT_ACTION_UP:
        {    
            float totalDistY = touchY - startTouchY;
            float totalDistX = touchX - startTouchX;
            uint64_t duration = now - startTouchTime;

            if (totalDistY > Config::Control::HARD_DROP_DISTANCE &&
                std::abs(totalDistY) > std::abs(totalDistX) &&
                duration < Config::Control::HARD_DROP_INTERVAL)
            {
                CancelHorizontalMove();
                HardDrop();
            }
            else
            {
                if (canRotateThisTouch && duration < Config::Control::TOUCH_UP_INTERVAL)
                {
                    Rotate();
                    AudioManager::Get().PlayAudioClip(Config::Sound::ROTATE_TETROMINO, false, 1.0f, Engine::Get().RandomRange(0.9f, 1.1f));
                }
            }

            isDraggingX = false;

        } break;
    }
}

void ActiveTetromino::HandleHandledInput(float deltaX, float deltaY, bool isDraggingX, bool isDraggingY)
{
    if (isDraggingX)
    {
        accumulatedDx += deltaX;

        float currentSensX = Config::Control::DRAG_SENSITIVITY_X;
        if (isFirstHorizontalStep)
        {
            currentSensX *= Config::Control::FIRST_STEP_SENSITIVITY_SCALE;
        }

        while (accumulatedDx >= currentSensX)
        { 
            int nextCol = col + 1;
            if (IsPositionValid(row, nextCol, rotation))
            {
                col = nextCol;
                AudioManager::Get().PlayAudioClip(Config::Sound::MOVE_TETROMINO, false, 1.0f, Engine::Get().RandomRange(0.95f, 1.05f));
                isFirstHorizontalStep = false;
            }
            accumulatedDx -= currentSensX;
            currentSensX = Config::Control::DRAG_SENSITIVITY_X;
        }

        while (accumulatedDx <= -currentSensX)
        {
            int nextCol = col - 1;
            if (IsPositionValid(row, nextCol, rotation))
            {
                col = nextCol;
                AudioManager::Get().PlayAudioClip(Config::Sound::MOVE_TETROMINO, false, 1.0f, Engine::Get().RandomRange(0.95f, 1.05f));
                isFirstHorizontalStep = false;
            }
            accumulatedDx += currentSensX;
            currentSensX = Config::Control::DRAG_SENSITIVITY_X;
        }

        accumulatedDy = 0.0f;
    }

    if (isDraggingY)
    {
        accumulatedDy += deltaY;

        float currentSensY = Config::Control::DRAG_SENSITIVITY_Y;

        while (accumulatedDy >= currentSensY)
        {
            if (IsPositionValid(row - 1, col, rotation))
            {
                row--;
                dropTimer = 0.0f; 
            }
            accumulatedDy -= currentSensY;
        }

        if (accumulatedDy < 0.0f)
        {
            accumulatedDy = 0.0f;
        }

        accumulatedDx = 0.0f;
    }
}

void ActiveTetromino::SaveStartGesturePosition()
{
    startColOfGesture = col;
    accumulatedDx = 0.0f;
    accumulatedDy = 0.0f;
}

void ActiveTetromino::CancelHorizontalMove()
{
    col = startColOfGesture; 
}

void ActiveTetromino::Rotate()
{
    if (accumulatedDy >= sensitivityY / 2) return;
    if (type == 1) return;

    int nextRotation = (rotation + 1) % 4;

    if (IsPositionValid(row, col, nextRotation))
    {
        rotation = nextRotation;
        return;
    }

    if (IsPositionValid(row, col - 1, nextRotation))
    {
        col--;
        rotation = nextRotation;
    }
    else if (IsPositionValid(row, col + 1, nextRotation))
    {
        col++;
        rotation = nextRotation;
    }
}

int ActiveTetromino::GetPieceWidth()
{
    if (type < 0 || type >= 7 || rotation < 0 || rotation >= 4) return 1;

    mixX = 5;
    int maxX = -1;
    maxY = -1;

    for (int i = 0; i < 4; i++)
    {
        int currentX = TETROMINO_SHAPES[type][rotation][i].x;
        int currentY = TETROMINO_SHAPES[type][rotation][i].y;

        if (currentX > maxX) maxX = currentX;
        if (currentX < mixX) mixX = currentX;
        if (currentX > maxY) maxY = currentY;
    }

    return maxX - mixX + 1;
}

void ActiveTetromino::HardDrop()
{
    camera->TriggerShakeY();

    int rowStart = 20;
    while (IsPositionValid(row - 1, col, rotation))
    {
        row--;
    }

    int pieceWidth = GetPieceWidth();

    int baseColor = type * 3;
    glm::vec3 color = glm::vec3( 
        Config::Color::TETROMINO_COLORS[baseColor], 
        Config::Color::TETROMINO_COLORS[baseColor + 1], 
        Config::Color::TETROMINO_COLORS[baseColor + 2]
    );

    FXSystem::Get().AddEffect(std::make_unique<GhostTrail>(
        col + mixX, rowStart, row + maxY,
        glm::vec3(0.0f, 0.8f, 1.0f),
        ghostShader,
        pieceWidth,
        color
    ));


    SetLean();
    FreezePiece();
    SetCurrentLevel();
    SpawnPiece();
}

void ActiveTetromino::SetLean()
{
    int limitLeft = 2;
    int limitRight = 5;

    if (col < limitLeft)
        camera->LeanLeft();
    else if (col > limitRight)
        camera->LeanRight();
    else
        camera->LeanCenter();
}

void ActiveTetromino::SetCurrentLevel()
{
    int clearedLinesNow = board->ClearFullLines();
    countClearedLines += clearedLinesNow;

    int targetLevel = countClearedLines / 10;

    if (targetLevel > currentLevel)
    {
        int levelGained = targetLevel - currentLevel;
        for (int i = 0; i < levelGained; i++)
        {
            spawnInterval *= Config::Gameplay::DIFFICULTY_MULTIPLIER;
        }

        currentLevel = targetLevel;
        Engine::Get().PrintInfo("Level up! Current level: %d, Interval: %f", currentLevel, spawnInterval);
    }
}

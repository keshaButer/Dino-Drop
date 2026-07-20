#pragma once
#include "../Engine/engine.h"
#include "../Board/board.h"

class ScoreManager
{
public:

    static ScoreManager& Get() { static ScoreManager instance; return instance; };

    void Initialize(Board* board)
    {
        Reset();

        board->OnLinesCleard.Subscribe([this](int lines) { this->AddScore(lines); });
    }

    int GetCurrentScore() const { return currentScore; }

    void Reset() { currentScore = 0; }

private:

    ScoreManager() = default;

    uint currentScore = 0;

    void AddScore(int fullLines)
    {
        int baseAddition = 0;
        switch (fullLines)
        {
            case 1: baseAddition = 100;  break;
            case 2: baseAddition = 300;  break;
            case 3: baseAddition = 500;  break;
            case 4: baseAddition = 1200; break;
            default:
                Engine::Get().PrintError("Score Manager: too many full lines: %i", fullLines);
                return;
        }

        int scoreFromLines = baseAddition;

        currentScore += scoreFromLines;
        Engine::Get().PrintInfo("CURRENT SCORE HAS INCREASED: %i (Lines cleared: %i)", currentScore, fullLines);
    }

};

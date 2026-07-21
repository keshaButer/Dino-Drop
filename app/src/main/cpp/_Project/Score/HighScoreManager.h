#pragma once
#include <string>
#include <fstream>
#include <vector>
#include "../Engine/engine.h"

class HighScoreManager
{
public:
    static HighScoreManager& Get() { static HighScoreManager instance; return instance; }

    void Initialize()
    {
        scores = LoadHighScores();

        Engine::Get().PrintInfo("Score:\n");
        Engine::Get().PrintInfo("-----------------");
        for (auto score : scores)
        {
            std::string message = "\n" + std::to_string(score);
            Engine::Get().PrintInfo(message.c_str());
        }
        Engine::Get().PrintInfo("-----------------");
    }

    std::vector<int> LoadHighScores()
    {
        std::string path = std::string(Engine::Get().GetInternalDataPath()) + "/highscores.txt";
        std::vector<int> scores;
        int tempScore = 0;

        std::ifstream file(path);
        if (file.is_open())
        {
            while (file >> tempScore)
            {
                scores.push_back(tempScore);
            }
            file.close();
        }
        return scores;
    }

    void AddNewScore(int newScore)
    {
        if (!scores.empty() && newScore <= scores.back()) return;

        scores.push_back(newScore);

        std::string path = std::string(Engine::Get().GetInternalDataPath()) + "/highscores.txt";
        std::ofstream file(path);
        
        if (file.is_open())
        {
            for (int score : scores)
            {
                file << score << "\n";
            }
            file.close();
        }
    }

    int GetBestScore()
    {
        if (scores.empty()) return 0;
        
        return scores.back();
    }

private:
    HighScoreManager() = default;

    std::vector<int> scores;
};

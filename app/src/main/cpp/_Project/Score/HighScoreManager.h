#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include "../Engine/engine.h"

class HighScoreManager
{
public:
    static HighScoreManager& Get() { static HighScoreManager instance; return instance; }

    void Initialize()
    {
        scores = LoadHighScores();
    }

    const std::vector<int>& GetHighScores() const
    {
        return scores;
    }

    void AddNewScore(int newScore)
    {
        if (newScore <= 0)
        {
            Engine::Get().PrintInfo("HighScoreManager: can not add new score, it is too low: %i", newScore);
            return;
        }

        scores.push_back(newScore);
        std::sort(scores.begin(), scores.end(), std::greater<int>());
    }

    void WriteToFile()
    {
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
        
        return scores.front();
    }

private:
    HighScoreManager() = default;

    std::vector<int> LoadHighScores()
    {
        std::string path = std::string(Engine::Get().GetInternalDataPath()) + "/highscores.txt";

        std::vector<int> loadedScores;
        int tempScore = 0;

        std::ifstream file(path);
        if (file.is_open())
        {
            while (file >> tempScore)
            {
                loadedScores.push_back(tempScore);
            }
            file.close();
        }
        return loadedScores;
    }

    std::vector<int> scores;
};

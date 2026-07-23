#pragma once
#include <string>
#include <fstream>
#include "../Engine/engine.h"

struct GameSettings
{
    bool isMusicEnabled = true;
    bool isSFXEnabled = true;
    bool isGhostEnabled = true;
    bool isOutlineEnabled = true;
};

class SettingsManager
{
public:
    static SettingsManager& Get() { static SettingsManager instance; return instance; }

    const GameSettings& GetSettings() const { return currentSettings; }

    GameSettings& GetSettingsMutable() { return currentSettings; }

    void SaveSettings(const GameSettings& newSettings)
    {
        currentSettings = newSettings;
        SaveSettings();
    }

    void SaveSettings()
    {
        std::string path = std::string(Engine::Get().GetInternalDataPath()) + "/settings.bin";
        
        std::ofstream file(path, std::ios::binary);
        if (file.is_open())
        {
            file.write(reinterpret_cast<const char*>(&currentSettings), sizeof(GameSettings));
            file.close();
            Engine::Get().PrintInfo("SettingsManager: Settings saved successfully.");
        }
    }

    void LoadSettings()
    {
        std::string path = std::string(Engine::Get().GetInternalDataPath()) + "/settings.bin";
        std::ifstream file(path, std::ios::binary);
        
        if (file.is_open())
        {
            file.read(reinterpret_cast<char*>(&currentSettings), sizeof(GameSettings));
            file.close();
            Engine::Get().PrintInfo("SettingsManager: Settings loaded from disk.");
        }
        else
        {
            Engine::Get().PrintInfo("SettingsManager: No settings file found, using defaults");
        }
    }

private:
    SettingsManager() = default;
    GameSettings currentSettings;
};

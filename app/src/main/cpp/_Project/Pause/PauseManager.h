#pragma once

class PauseManager
{
public:
    static PauseManager& Get() { static PauseManager instance; return instance; }

    bool IsPaused() const { return isPaused; }
    void SetPaused(bool paused) { isPaused = paused; }
    void TogglePause() { isPaused = !isPaused; }

private:
    bool isPaused = false;
    PauseManager() = default;
};

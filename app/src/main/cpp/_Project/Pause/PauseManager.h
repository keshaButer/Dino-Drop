#pragma once

class PauseManager
{
public:
    static PauseManager& Get() { static PauseManager instance; return instance; }

    bool IsPaused() const { return isPaused; }
    bool IsPausedUI() const { return isPausedUI; }
    void SetPaused(bool paused) { isPaused = paused; isPausedUI = paused; }
    void SetPausedGameplay(bool paused) { isPaused = paused; }
    void TogglePause() { isPaused = !isPaused; isPausedUI = !isPausedUI; }

private:
    bool isPaused = false;
    bool isPausedUI = false;
    PauseManager() = default;
};

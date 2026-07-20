#pragma once
#include "../Camera/camera.h"

class Game
{
public:
    static Game& Get() { static Game instance; return instance; }    

    void Start();
    void Update();
private:
    void LoadShaders();
    void LoadSounds();
    void LoadTextures();

    Camera* const mainCamera = new Camera();
};

#pragma once
#include "../Camera/camera.h"

class Game
{
public:
    static Game& Get() { static Game instance; return instance; }    

    void Start();
    void Update();

    void LoadShaders();
    void LoadSounds();
    void LoadTextures();
private:

    Camera* const mainCamera = new Camera();
};

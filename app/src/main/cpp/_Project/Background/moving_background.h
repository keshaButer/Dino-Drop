#pragma once
#include "../SpriteRenderer/sprite_renderer.h"
#include <GLES3/gl3.h>

class Background
{
public:
    Background();

    void Draw();

private:
    SpriteRenderer mainBG;
    float cycledTime;

    Shader* const shader;
    GLuint timeLocation;
};

#pragma once
#include "../SpriteRenderer/sprite_renderer.h"
#include <GLES3/gl3.h>

class Background
{
public:
    Background();
    Background(Texture* _texture);

    void Draw();

private:
    SpriteRenderer mainBG;
    float cycledTime;
    Texture* texture =  nullptr;

    Shader* const shader;
    GLuint timeLocation;
};

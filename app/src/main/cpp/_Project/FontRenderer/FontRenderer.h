#pragma once
#include <ft2build.h> 
#include FT_FREETYPE_H 
#include <vector>
#include <GLES3/gl3.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include "SmartPointers.h"
#include "../SpriteRenderer/sprite_renderer.h"

struct Character 
{
    Unique<Texture> texture;     // текстура глифа
    glm::vec2 Size;      // Размеры глифа
    glm::vec2 Bearing;   // Смещение верхней левой точки глифа
    float     Advance;   // Горизонтальное смещение до начала следующего глифа
};

class FontRenderer
{
public:
    FontRenderer(const char* fontPath, int fontSize);

    void Initialize(const char* fontPath, int fontSize);
    void RenderText(const std::string& text, glm::vec2 position, float scale, glm::vec4 color, bool centered = true, float shadow = 0.02f);
    float CalculateTextWidth(const std::string& text, float scale);

private:
    FT_Library ft;
    FT_Face face;

    std::vector<unsigned char> fontBuffer; 
    std::unordered_map<GLchar, Character> characters;
    SpriteRenderer spriteRenderer;
};

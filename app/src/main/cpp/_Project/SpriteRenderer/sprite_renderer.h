#pragma once
#include "../Shaders/Shader.h"
#include <glm/glm.hpp>
#include <GLES3/gl3.h>
#include "../Texture/Texture.h"

class SpriteRenderer
{
public:
    SpriteRenderer();
    SpriteRenderer(Shader* _shader);

    ~SpriteRenderer() = default;

    void Draw(glm::vec2 position, glm::vec2 scale, float rotationDegrees, glm::vec4 color, float progress = -1.0f);
    void Draw(glm::vec2 position, glm::vec2 scale, float rotationDegrees, glm::vec4 color, const Texture* texture, float progress = -1.0f);

    void UseProgram() const;
    GLuint GetProgramID();
    void SetTextureID(GLuint id);

private:
    Shader* const shader;
    GLuint textureID = 0;
    GLuint colorPos, progressPos, modelPos;
};

#include "sprite_renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Shaders/ShaderManager.h"
#include "../Resources/resource_manager.h"
#include "../Core/Config.h"

SpriteRenderer::SpriteRenderer() : shader(ShaderManager::Get().GetShader(Config::ShaderNames::DEFAULT_TEXTURE))
{
    shader->UseProgram();
    colorPos = glGetUniformLocation(shader->IDprogram, "uColor");
    progressPos = glGetUniformLocation(shader->IDprogram, "uProgress");
    modelPos = glGetUniformLocation(shader->IDprogram, "model");
    textureID = shader->textureID;
}

SpriteRenderer::SpriteRenderer(Shader* _shader)
 : shader(_shader) 
{
    shader->UseProgram();
    colorPos = glGetUniformLocation(shader->IDprogram, "uColor");
    progressPos = glGetUniformLocation(shader->IDprogram, "uProgress");
    modelPos = glGetUniformLocation(shader->IDprogram, "model");

    textureID = _shader->textureID;
}

void SpriteRenderer::Draw(glm::vec2 position, glm::vec2 scale, float rotationDegrees, glm::vec4 color, float progress)
{
    shader->UseProgram();

    glActiveTexture(GL_TEXTURE0);
    ShaderManager::Get().BindTexture(textureID);

    glm::mat4 model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(position.x, position.y, 0));
    if (rotationDegrees != 0)
    {
        model = glm::translate(model, glm::vec3(0.5f * scale.x, 0.5f * scale.y, 0));
        model = glm::rotate(model, glm::radians(rotationDegrees), glm::vec3(0, 0, 1));
        model = glm::translate(model, glm::vec3(-0.5f * scale.x, -0.5f * scale.y, 0));
    }

    model = glm::scale(model, glm::vec3(scale.x, scale.y, 1.0f));

    glUniformMatrix4fv(modelPos, 1, GL_FALSE, glm::value_ptr(model));
    glUniform4f(colorPos, color.r, color.g, color.b, color.a);
    if (progress >= 0)
        glUniform1f(progressPos, progress);

    glBindVertexArray(ResourceManager::Get().GetQuadVAO()); 
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
}

void SpriteRenderer::Draw(glm::vec2 position, glm::vec2 scale, float rotationDegrees, glm::vec4 color, const Texture* texture, float progress)
{
    shader->UseProgram();
    glActiveTexture(GL_TEXTURE0);
    ShaderManager::Get().BindTexture(texture->id);

    glm::mat4 model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(position.x, position.y, 0));

    if (rotationDegrees != 0)
    {
        model = glm::rotate(model, glm::radians(rotationDegrees), glm::vec3(0, 0, 1));
    }

    model = glm::scale(model, glm::vec3(scale.x, scale.y, 1.0f));

    glUniformMatrix4fv(modelPos, 1, GL_FALSE, glm::value_ptr(model));
    glUniform4f(colorPos, color.r, color.g, color.b, color.a);

    if (progress >= 0) glUniform1f(progressPos, progress);

    glBindVertexArray(ResourceManager::Get().GetQuadVAO()); 
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
}

void SpriteRenderer::UseProgram() const
{
    shader->UseProgram();
}

GLuint SpriteRenderer::GetProgramID()
{
    return shader->IDprogram;
}

void SpriteRenderer::SetTextureID(GLuint id)
{
    shader->textureID = id;
    textureID = id;
}

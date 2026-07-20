#include "ShaderManager.h"
#include "../Core/Config.h"
#include <GLES3/gl3.h>
#include "../Engine/engine.h"

GLuint ShaderManager::currentActiveProgramID = 0;
GLuint ShaderManager::currentlyBoundTextureID = 999999;

void ShaderManager::BindTexture(GLuint id)
{
    if (currentlyBoundTextureID != id)
    {
        glBindTexture(GL_TEXTURE_2D, id);
        currentlyBoundTextureID = id;
    }
}

Shader* ShaderManager::LoadShader(const std::string_view& viewName, bool viewUpdate)
{
    std::string name = static_cast<std::string>(viewName);
    auto shader = std::make_unique<Shader>();
    shader->Initialize(
        Config::GetVertexShaderPath(name).c_str(), 
        Config::GetFragmentShaderPath(name).c_str()
    );
    shader->viewUpdate = viewUpdate;
    Shader* ptr = shader.get();
    shaders[name] = std::move(shader);
    return ptr;
}

Shader* ShaderManager::GetShader(const std::string_view& viewName)
{
    std::string name = static_cast<std::string>(viewName);
    auto it = shaders.find(name);
    if (it != shaders.end())
        return it->second.get();
    
    Engine::Get().PrintError("Shader not found in manager: %s", name.c_str());
    return nullptr;
}

void ShaderManager::AttachAllToCamera(Camera* camera)
{
    for (const auto& [name, shaderPtr] : shaders)
    {
        if (shaderPtr->viewUpdate)
            camera->AddShader(shaderPtr.get());
        else
            camera->SetStaticViewMatrix(shaderPtr.get());
    }
}

void ShaderManager::SetTextureSlotZeroToAll()
{
    for (const auto& [name, shaderPtr] : shaders)
    {
        shaderPtr->UseProgram();
        glUniform1i(glGetUniformLocation(shaderPtr->IDprogram, "uTexture"), 0);
    }
}


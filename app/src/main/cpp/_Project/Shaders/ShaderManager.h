#pragma once
#include <unordered_map>
#include <string>
#include "SmartPointers.h"
#include "Shader.h"
#include <../Camera/camera.h>
#include <GLES3/gl3.h>

class ShaderManager
{
public:
    static ShaderManager& Get() { static ShaderManager instance; return instance; }

    Shader* LoadShader(const std::string_view& name, bool viewUpdate = true);
    Shader* GetShader(const std::string_view& viewName);

    void AttachAllToCamera(Camera* camera);
    void SetTextureSlotZeroToAll();
    void BindTexture(GLuint id);
    static GLuint currentActiveProgramID;

private:
    std::unordered_map<std::string, Unique<Shader>> shaders;
    ShaderManager() = default;
    static GLuint currentlyBoundTextureID;
};

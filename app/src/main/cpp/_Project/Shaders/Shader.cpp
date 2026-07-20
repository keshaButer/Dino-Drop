#define STB_IMAGE_IMPLEMENTATION
#include <GLES2/gl2.h>
#include <android/asset_manager.h>
#include "../Plugins/stb_image.h"
#include "../Engine/engine.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Shader.h"
#include "../Shaders/ShaderManager.h"

void Shader::UseProgram() 
{ 
    if (IDprogram == 0)
    {
        Engine::Get().PrintError("There is no program in this shader");
        return;
    }

    if (ShaderManager::Get().currentActiveProgramID != IDprogram)
    {
        glUseProgram(IDprogram); 
        ShaderManager::Get().currentActiveProgramID = IDprogram;
    }
}

void Shader::Initialize(const char *vertexPath, const char *fragmentPath) 
{
    stbi_set_flip_vertically_on_load(true);
    IDprogram = createShaderProgram(vertexPath, fragmentPath);

    UseProgram();

    int width = Engine::Get().GetWindowWidth();
    int height = Engine::Get().GetWindowHeight();
    float aspect = (float)width / (float)height;
    
    glm::mat4 projection = glm::mat4(1);
    projection = glm::ortho(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect); 

    GLuint projectionPos = glGetUniformLocation(IDprogram, "projection");
    glUniformMatrix4fv(projectionPos, 1, GL_FALSE, glm::value_ptr(projection));
}

void Shader::InitializeFromSource(const char* vertexSource, const char* fragmentSource) 
{
    IDprogram = createShaderProgramFromSource(vertexSource, fragmentSource);
}

unsigned int Shader::createShaderProgramFromSource(const char* vertexSource, const char* fragmentSource) 
{
    unsigned int vertexShader = createCompileShader(GL_VERTEX_SHADER, vertexSource);
    unsigned int fragmentShader = createCompileShader(GL_FRAGMENT_SHADER, fragmentSource);
    
    if (vertexShader == 0 || fragmentShader == 0) return 0;

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) 
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        Engine::Get().PrintError("Program linking failed: %s", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

void Shader::DeleteProgram() 
{
    if (IDprogram != 0) 
    {
        glDeleteProgram(IDprogram);
    } 
    else 
    {
        Engine::Get().PrintError("Failed to delete program. It does not exist");
    }
}

std::string Shader::getShaderSource(const char *filePath) 
{
    AAssetManager* mgr = Engine::Get().GetAssetManager();
    AAsset* asset = AAssetManager_open(mgr, filePath, AASSET_MODE_BUFFER);
    
    if (!asset)
    {
        Engine::Get().PrintError("Could not open asset: %s", filePath);
        return "";
    }
    
    size_t size = AAsset_getLength(asset);
    char* buffer = new char[size + 1];
    AAsset_read(asset, buffer, size);
    buffer[size] = '\0';

    std::string content(buffer);
    // Engine::Get().PrintDebug("Shader source (%s):\n%s", filePath, content.c_str());
    delete[] buffer;
    AAsset_close(asset);
    
    return content;
}

unsigned int Shader::createCompileShader(unsigned int type, const char *shaderSource) 
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    
    // Всегда выводим инфо лог, даже если успех (там могут быть предупреждения)
    char infoLog[512];
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    if (infoLog[0] != '\0') 
    {
        Engine::Get().PrintDebug("Shader info log (%s): %s", 
            type == GL_VERTEX_SHADER ? "vertex" : "fragment", infoLog);
    }
    
    if (!success) 
    {
        Engine::Get().PrintError("Shader compilation failed (%s)", 
            type == GL_VERTEX_SHADER ? "vertex" : "fragment");
        return 0;
    }
    return shader;
}

unsigned int Shader::createShaderProgram(const char *vertexPath, const char *fragmentPath) 
{
    if (!Engine::Get().IsContextReady()) 
    {
        Engine::Get().PrintError("Cannot create shader: no OpenGL context");
        return 0;
    }

    std::string vertexString = getShaderSource(vertexPath);
    std::string fragmentString = getShaderSource(fragmentPath);

    if (vertexString.empty() || fragmentString.empty()) 
    {
        Engine::Get().PrintError("Failed to get shader code from files");
        return 0;
    }

    const char *vertexSource = vertexString.c_str();
    const char *fragmentSource = fragmentString.c_str();

    unsigned int vertexShader = createCompileShader(GL_VERTEX_SHADER, vertexSource);
    unsigned int fragmentShader = createCompileShader(GL_FRAGMENT_SHADER, fragmentSource);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) 
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        Engine::Get().PrintError("Failed to link shader program: %s", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void Shader::LoadTexture(const char *texturePath) 
{
    AAssetManager* assetManager = Engine::Get().GetAssetManager();
    if (!assetManager)
    {
        Engine::Get().PrintError("Asset manager is null");
        return;
    }

    AAsset* asset = AAssetManager_open(assetManager, texturePath, AASSET_MODE_BUFFER);
    if (!asset) 
    {
        Engine::Get().PrintError("Failed to open asset from APK: %s", texturePath);
        return;
    }

    size_t size = AAsset_getLength(asset);
    std::vector<unsigned char> fileBuffer(size);

    int readBytes = AAsset_read(asset, fileBuffer.data(), size);
    AAsset_close(asset);

    if (readBytes <= 0)
    {
        Engine::Get().PrintError("Failed to read bytes from asset: %s", texturePath);
        return;
    }

    int width, height, nrChannels;
    unsigned char *data = stbi_load_from_memory(fileBuffer.data(), size, &width, &height, &nrChannels, 0);

    if (data == nullptr) 
    {
        Engine::Get().PrintError("stbi_load_from_memory FAILED for: %s! Data pointer is NULL", texturePath);
        return;
    }

    GLint internalFormat;
    GLenum format;

    if (nrChannels == 4) 
    {
        internalFormat = 0x8058;
        format = GL_RGBA;
    } 
    else 
    {
        internalFormat = 0x8051;
        format = GL_RGB;
    }

    glGenTextures(1, &textureID);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    UseProgram();
    glUniform1i(glGetUniformLocation(IDprogram, "uTexture"), 0);

    glBindTexture(GL_TEXTURE_2D, 0); 
}

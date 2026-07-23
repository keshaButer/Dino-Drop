#include "camera.h"
#include <GLES3/gl3.h>
#include <cstdlib>
#include "../Core/Config.h"
#include "../Engine/engine.h"

Camera::Camera() {}

void Camera::AddShader(Shader* shader)
{
    shaders.push_back(shader);
}

void Camera::TriggerShake(float intensity, float duration)
{
    shakeIntensity = intensity;
    shakeDuration = duration;
}

void Camera::TriggerShakeY()
{
    isShakeY = true;
    posY = 0.0f;
    isCameraOnTop = true;
}

void Camera::LeanRight()
{
    rotZ = -Config::Gameplay::LEAN_INTENSY;
}

void Camera::LeanLeft()
{
    rotZ = Config::Gameplay::LEAN_INTENSY;
}

void Camera::LeanCenter()
{
    rotZ = 0.0f;
}

void Camera::CalculateLean(float deltaTime)
{
    currentRotZ = Lerp(currentRotZ, rotZ, Config::Gameplay::LEAN_ACCEL * deltaTime);
}

void Camera::Move(float deltaTime)
{
    posX = Lerp(posX, 0.0f, Config::Gameplay::ACCEL_MOVE_BOARD_TO_ZERO_X);

    CalculateShake(deltaTime);
    CalculateShakeY(deltaTime);
    CalculateLean(deltaTime);

    view = glm::mat4(1);
    view = glm::translate(view, glm::vec3(posX + offsetX, posY + offsetY, 0));
    view = glm::rotate(view, glm::radians(currentRotZ), glm::vec3(0, 0, 1));

    for (const auto& shader : shaders)
    {
        shader->UseProgram();

        GLuint viewPos = glGetUniformLocation(shader->IDprogram, "view");
        glUniformMatrix4fv(viewPos, 1, GL_FALSE, glm::value_ptr(view));
    }
}

void Camera::MoveSide(float deltaX)
{
    posX += (deltaX / Engine::Get().GetWindowWidth()) * Config::Gameplay::STREGTH_MOVE_BOARD_SIDE;
}

glm::mat4& Camera::GetViewMatrix()
{
    return view;
}

void Camera::SetStaticViewMatrix(Shader* shader)
{
    glm::mat4 view = glm::mat4(1);

    shader->UseProgram();

    GLuint viewPos = glGetUniformLocation(shader->IDprogram, "view");
    glUniformMatrix4fv(viewPos, 1, GL_FALSE, glm::value_ptr(view));
}

void Camera::CalculateShake(float deltaTime)
{
    if (shakeDuration > 0)
    {
        shakeDuration -= deltaTime;

        float randomX = ((float)rand() / RAND_MAX) * 2.0f - 1;       
        float randomY = ((float)rand() / RAND_MAX) * 2.0f - 1;       

        offsetX = randomX * shakeIntensity;
        offsetY = randomY * shakeIntensity;
    }
    else
    {
        shakeIntensity = 0.0f;
    }
}

void Camera::ClearViewMatrix()
{
    shakeIntensity = 0.0f;
    shakeDuration = 0.0f;

    posX = 0.0f;
    posY = 0.0f;
    currentRotZ = 0.0f;
    rotZ = 0.0f;
    offsetX = 0.0f;
    offsetY = 0.0f;
    isShakeY = false;
    isCameraOnTop = true;

    for (const auto& shader : shaders)
    {
        SetStaticViewMatrix(shader);
    }
}

void Camera::CalculateShakeY(float deltaTime)
{
    if (!isShakeY) return;

    if (isCameraOnTop)
    {
        posY = Lerp(posY, shakeYdepth, shakeYaccelaration * deltaTime);

        if (std::abs(shakeYdepth - posY) < 0.05f)
        {
            isCameraOnTop = false;
        }
    }
    else
    {
        posY = Lerp(posY, 0.0f, shakeYaccelaration * deltaTime);

        if (std::abs(posY) < 0.05f)
        {
            isShakeY = false;
            isCameraOnTop = true;
        }
    }
}

float Camera::Lerp(float start, float end, float accleration)
{
    return start + (end - start) * accleration;
}

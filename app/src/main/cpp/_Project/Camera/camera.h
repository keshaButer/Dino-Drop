#pragma once
#include "../Shaders/Shader.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
    Camera();

    void ClearViewMatrix();
    void SetStaticViewMatrix(Shader* shader);
    void AddShader(Shader* shader);
    void TriggerShake(float intensity, float duration);
    void TriggerShakeY();
    void LeanRight();
    void LeanLeft();
    void LeanCenter();
    void Move(float deltaTime);
    void MoveSide(float deltaX);
    float GetCurrentRotation() const { return currentRotZ; }
    glm::mat4& GetViewMatrix();

private:
    void CalculateShake(float deltaTime);
    void CalculateShakeY(float deltaTime);
    void CalculateLean(float deltaTime);
    float Lerp(float start, float end, float accleration);

    float shakeIntensity = 0.0f;
    float shakeDuration = 0.0f;
    float shakeYdepth = -0.2f;
    float shakeYaccelaration = 10.0f;

    float posX = 0.0f;
    float posY = 0.0f;
    float currentRotZ = 0.0f;
    float rotZ = 0.0f;
    float offsetX = 0.0f;
    float offsetY = 0.0f;
    bool isShakeY = false;
    bool isCameraOnTop = true;

    glm::mat4 view = glm::mat4(1);

    std::vector<Shader*> shaders;
};

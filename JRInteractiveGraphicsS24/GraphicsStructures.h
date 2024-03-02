#pragma once
#include <glm/glm.hpp>

struct SphericalCoordinate {
    float phi = 0.0f, theta = 0.0f, rho = 1.0f;

    glm::mat4 ToMat4() {
        float thetaRadians = glm::radians(theta);
        float phiRadians = glm::radians(phi);
        float sinPhi = sin(phiRadians);
        glm::vec3 zAxis{};
        zAxis.x = rho * sin(thetaRadians) * sinPhi;
        zAxis.y = rho * cos(phiRadians);
        zAxis.z = rho * cos(thetaRadians) * sinPhi;

        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 xAxis = glm::normalize(glm::cross(up, zAxis));
        glm::vec3 yAxis = glm::cross(zAxis, xAxis);
        glm::mat4 orientation(1.0f);
        orientation[0] = glm::vec4(xAxis, 0.0f);
        orientation[1] = glm::vec4(yAxis, 0.0f);
        orientation[2] = glm::vec4(zAxis, 0.0f);
        return orientation;
    }
};

struct MouseParams {
    SphericalCoordinate spherical{};
    double x = 0, y = 0;
    double windowX = 0, windowY = 0;
    int windowWidth = 0, windowHeight = 0;
    float fieldOfView = 60.0f;
};

struct Material {
    float ambientIntensity;  // 0 to 1
    float specularIntensity; // 0 to 1
    float shininess;         // 0 to infinity
};

struct Light {
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
    float attenuationCoef;
};
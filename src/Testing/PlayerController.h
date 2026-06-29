#pragma once
#include "Core/Camera.h"
#include <GLFW/glfw3.h>

struct PlayerController
{
    float moveSpeed = 5.0f;
    float mouseSensitivity = 0.0025f;

    double lastMouseX = 0.0;
    double lastMouseY = 0.0;
    bool firstMouse = true;

    void Update(Camera& cam, GLFWwindow* window, float deltaTime)
    {
        Vec3 moveDir{0,0,0};

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            moveDir = moveDir + cam.forward;

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            moveDir = moveDir - cam.forward;

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            moveDir = moveDir + cam.right;

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            moveDir = moveDir - cam.right;

        if (Length(moveDir) > 0.0f)
            cam.position = cam.position + Normalize(moveDir) * moveSpeed * deltaTime;

        double x, y;
        glfwGetCursorPos(window, &x, &y);

        if (firstMouse)
        {
            lastMouseX = x;
            lastMouseY = y;
            firstMouse = false;
        }

        double xoffset = x - lastMouseX;
        double yoffset = lastMouseY - y;

        lastMouseX = x;
        lastMouseY = y;

        cam.rotation.y += (float)xoffset * mouseSensitivity;
        cam.rotation.x += (float)yoffset * mouseSensitivity;

        if (cam.rotation.x > 1.5f) cam.rotation.x = 1.5f;
        if (cam.rotation.x < -1.5f) cam.rotation.x = -1.5f;

        UpdateCameraVectors(cam);
    }
};
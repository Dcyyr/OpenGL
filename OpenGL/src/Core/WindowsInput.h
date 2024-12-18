#pragma once
#include <GLFW/glfw3.h>
#include "OpenGLRender/Camera.h"

Camera camera(glm::vec3(0.0f,0.0f,3.0f));
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

const uint32_t width = 1600;
const uint32_t height = 900;

float lastX = width / 2.0f;
float lastY = height / 2.0f;

bool firstMouse = true;


class WindowsInput
{
public:
	WindowsInput(){}

   

    void KeyInput(GLFWwindow* window) 
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, 1);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.CameraInput(CameraMoveDirection::FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.CameraInput(CameraMoveDirection::BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.CameraInput(CameraMoveDirection::LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.CameraInput(CameraMoveDirection::RIGHT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            camera.CameraInput(CameraMoveDirection::UP, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            camera.CameraInput(CameraMoveDirection::DOWN, deltaTime);
    }

	void MouseCallback(GLFWwindow* window, double xpos, double ypos)
	{
        float Xpos = static_cast<float>(xpos);
        float Ypos = static_cast<float>(ypos);

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = ypos - lastY;

        lastX = xpos;
        lastY = ypos;

        camera.MouseMovement(xoffset, yoffset);
	}

    void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        camera.MouseScroll(static_cast<float>(yoffset));
    }


	
};

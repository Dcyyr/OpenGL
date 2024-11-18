#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

int main(void)
{

    glfwInit();
    glfwWindowHint(GLFW_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_VERSION_MINOR, 3);

    GLFWwindow* window = glfwCreateWindow(1600, 900, "OpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (!window)
    {
        std::cout << "GLFW窗口创建失败" << std::endl;
        glfwTerminate();
        return -1;
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "初始化GLAD失败！" << std::endl;
        return -1;
    }

    if (!glfwInit())
    {
        return -1;
    }
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
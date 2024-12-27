#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>

#include <imgui.h>

#include "OpenGLRender/Buffers.h"
#include "OpenGLRender/Shader.h"
#include "OpenGLRender/Texture2D.h"
#include "OpenGLRender/VertexArray.h"
#include "OpenGLRender/Renderer.h"
#include "OpenGLRender/Camera.h"
#include "OpenGLRender/Model.h"

//#include "Core/WindowsInput.h"
void KeyInput(GLFWwindow* window);
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);


Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

const uint32_t width = 1600;
const uint32_t height = 900;

float lastX = width / 2.0f;
float lastY = height / 2.0f;

bool firstMouse = true;

int main()
{

    if (!glfwInit()) return -1;

   
    GLFWwindow* window = glfwCreateWindow(1600, 900, "Hello World", NULL, NULL);

    if (!window) 
    {
        glfwTerminate();
        return -1;
    }
    
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    glfwSetCursorPosCallback(window,MouseCallback);
    glfwSetScrollCallback(window,MouseScrollCallback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "初始化GLAD失败！" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);//启用深度测试
    glEnable(GL_BLEND);//启用混合
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   
    glEnable(GL_CULL_FACE);//启用面剔除
    glCullFace(GL_FRONT);//GL_FRONT剔除正向面，GL_BACK剔除背向面，GL_FRONT_AND_BACK
    glFrontFace(GL_CCW);//默认值是GL_CCW，它代表的是逆时针的环绕顺序，另一个选项是GL_CW，代表的是顺时针顺序。

    float cubeVertices[] = {
        // Back face
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
    // Front face
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
    // Left face
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
    // Right face
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
     // Bottom face
     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
      0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
     // Top face
     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
     -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
    };
    float planeVertices[] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f
    };
    
    float transparentVertices[] = {
        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  0.0f
    };
    // cube VAO
    VertexArray cubeVa;
    VertexBuffer cubeVb(cubeVertices,sizeof(cubeVertices));
    
    VertexBufferLayout cubeLayer;
    cubeLayer.Push<float>(3);
    cubeVa.AddVertexBuffer(cubeVb,cubeLayer);
    cubeLayer.Push<float>(2);
    cubeVa.AddVertexBuffer(cubeVb, cubeLayer);


    // plane VAO
    VertexArray planeVa;
    VertexBuffer planeVb(planeVertices, sizeof(planeVertices));
    
    VertexBufferLayout planeLayer;
    planeLayer.Push<float>(3);
    planeVa.AddVertexBuffer(planeVb, planeLayer);
    planeLayer.Push<float>(2);
    planeVa.AddVertexBuffer(planeVb, planeLayer);

    
    //transparent VAO
    VertexArray transparentVa;
    VertexBuffer transparentVb(transparentVertices,sizeof(transparentVertices));

    VertexBufferLayout transparentLayer;
    transparentLayer.Push<float>(3);
    transparentVa.AddVertexBuffer(transparentVb, transparentLayer);
    transparentLayer.Push<float>(2);
    transparentVa.AddVertexBuffer(transparentVb, transparentLayer);

    transparentVa.Unbind();

    Shader Blend("res/shader/blending.shader");
    
    Texture2D CubeTexture("res/texture/marble.jpg");
    Texture2D FloorTexture("res/texture/metal.png");
    Texture2D TransparentTexture("res/texture/window.png");
    
    std::vector<glm::vec3> windows
    {
        glm::vec3(-1.5f, 0.0f, -0.48f),
        glm::vec3(1.5f, 0.0f, 0.51f),
        glm::vec3(0.0f, 0.0f, 0.7f),
        glm::vec3(-0.3f, 0.0f, -2.3f),
        glm::vec3(0.5f, 0.0f, -0.6f)
    };

    Blend.Bind();
    Blend.SetUniformInt("texture1", 0);

    Renderer render;


    while (!glfwWindowShouldClose(window))
    {
    
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        KeyInput(window);
        
        std::map<float, glm::vec3> sorted;
        for (uint32_t i = 0; i < windows.size(); i++)
        {
            float distance = glm::length(camera.m_Position - windows[i]);
            sorted[distance] = windows[i];
        }

        /* Render here */
        render.Clear();


        Blend.Bind();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.m_Zoom), (float)width / (float)height, 0.1f, 100.0f);
        Blend.SetUniformMat4("view",view);
        Blend.SetUniformMat4("projection", projection);

   
        // cubes
        cubeVa.Bind();
        CubeTexture.Bind();
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        Blend.SetUniformMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        Blend.SetUniformMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // floor
        planeVa.Bind();
        FloorTexture.Bind();
        Blend.SetUniformMat4("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        //transparent windows
        transparentVa.Bind();
        TransparentTexture.Bind();
        for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); it++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, it->second);
            Blend.SetUniformMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
      
     

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//GL_LINE线框，GL_FILL恢复默认
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
   


    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


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



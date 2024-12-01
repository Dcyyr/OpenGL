#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>

#include "OpenGLRender/Buffers.h"
#include "OpenGLRender/Shader.h"
#include "OpenGLRender/Texture2D.h"
#include "OpenGLRender/VertexArray.h"
#include "OpenGLRender/Renderer.h"
#include "OpenGLRender/OrthoCamera.h"

void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void Input(GLFWwindow* window);

const uint32_t width = 1600;
const uint32_t height = 900;

OrthoCamera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = width / 2.0f;
float lastY = height / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{

    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(1600, 900, "Hello World", NULL, NULL);

    if (!window) 
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, MouseScrollCallback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "初始化GLAD失败！" << std::endl;
        return -1;
    }
   
    glEnable(GL_DEPTH_TEST);

    float vertices[] = {

    -0.5f, -0.5f, -0.5f,     0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,     1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,     1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,     1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,     0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,     0.0f, 0.0f,
                             
    -0.5f, -0.5f,  0.5f,     0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,     1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,     1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,     1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,     0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,     0.0f, 0.0f,
                             
    -0.5f,  0.5f,  0.5f,     1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,     1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,     0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,     0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,     0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,     1.0f, 0.0f,
                             
     0.5f,  0.5f,  0.5f,     1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,     1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,     0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,     0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,     0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,     1.0f, 0.0f,
                             
    -0.5f, -0.5f, -0.5f,     0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,     1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,     1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,     1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,     0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,     0.0f, 1.0f,
                             
    -0.5f,  0.5f, -0.5f,     0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,     1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,     1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,     1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,     0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,     0.0f, 1.0f
           
          
    };

    glm::vec3 cubePositions[] = {
       glm::vec3(0.0f,  0.0f,  0.0f),
       glm::vec3(2.0f,  5.0f, -15.0f),
       glm::vec3(-1.5f, -2.2f, -2.5f),
       glm::vec3(-3.8f, -2.0f, -12.3f),
       glm::vec3(2.4f, -0.4f, -3.5f),
       glm::vec3(-1.7f,  3.0f, -7.5f),
       glm::vec3(1.3f, -2.0f, -2.5f),
       glm::vec3(1.5f,  2.0f, -2.5f),
       glm::vec3(1.5f,  0.2f, -1.5f),
       glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    uint32_t indices[] = {
        0,1,2,//第一个三角形
        2,3,0 //second
    };
   
   

    VertexArray va;
    VertexBuffer vb(vertices, sizeof(vertices));
    
    VertexBufferLayout layout;
    layout.Push<float>(3);//glVertexAttribPointer
    layout.Push<float>(2);
    va.AddVertexBuffer(vb, layout);

    IndexBuffer ib(indices, 6);

    glm::vec4 color = glm::vec4(0.9f, 0.5f, 0.8f, 1.0f);
    Shader shader("res/shader/Shader.glsl");
    shader.Bind();
    shader.SetUniformFloat4("u_Color", color);

    Texture2D tex("res/texture/2.jpg");
    tex.Bind();
    shader.SetUniformInt("u_Texture", 0);
    //颜色属性
   /* glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);*/

    
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();
    va.Unbind();

    Renderer render;


    while (!glfwWindowShouldClose(window))
    {
    
        double currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        Input(window);

        /* Render here */
        render.Clear();

        shader.Bind();

        //transform
        //glm::mat4 model = glm::mat4(1.0f);
        //glm::mat4 view  = glm::mat4(1.0f);
        //glm::mat4 projection = glm::mat4(1.0f);

      /*  model = glm::rotate(model, (float)glfwGetTime() * glm::radians(45.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        shader.SetUniformMat4("model", model);*/

        glm::mat4 view = camera.GetViewMatrix();
        shader.SetUniformMat4("view", view);

        glm::mat4 projection = glm::perspective(glm::radians(camera.m_Zoom), (float)width / (float)height, 0.1f, 100.0f);
        shader.SetUniformMat4("projection", projection);
        //glm::mat4 mvp = projection * view * model;
        //shader.SetUniformMat4("mvp", mvp);

        va.Bind();
        for (uint32_t i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.SetUniformMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //glDrawArrays(GL_TRIANGLES, 0, 36);//
        //render.Draw(vb,ib,shader);
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

void Input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window,1);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.CameraInput(CameraMoveDirection::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.CameraInput(CameraMoveDirection::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.CameraInput(CameraMoveDirection::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.CameraInput(CameraMoveDirection::RIGHT, deltaTime);
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
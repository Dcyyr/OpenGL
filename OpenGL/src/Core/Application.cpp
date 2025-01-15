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
#include "OpenGLRender/FrameBuffers.h"

//#include "Core/WindowsInput.h"
void KeyInput(GLFWwindow* window);
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
uint32_t loadCubeMap(std::vector<std::string> faces);

Camera camera(glm::vec3(0.0f, 3.0f, 15.0f));
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

const uint32_t width = 1600;
const uint32_t height = 900;

float lastX = width / 2.0f;
float lastY = height / 2.0f;

bool firstMouse = true;

bool blinn = false;
bool blinnKeyPressed = false;

bool GammaEnabled = false;
bool GammaKeyPressed = false;

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

    Renderer render;


    glEnable(GL_DEPTH_TEST);//启用深度测试
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_FRAMEBUFFER_SRGB);
    
    float planeVertices[] = {
        // positions            // normals         // texcoords
         10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
        -10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

         10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
        -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
         10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
    };

    Shader LightShader("res/shader/Advance_Lighting/GammaCorrection.shader");
    //plane
    VertexArray planeVa;
    planeVa.Bind();
    VertexBuffer planeVb(planeVertices, sizeof(planeVertices));

    VertexBufferLayout layout;
    layout.Push<float>(3);
    planeVa.AddVertexBuffer(planeVb, layout);
    layout.Push<float>(3);
    planeVa.AddVertexBuffer(planeVb, layout);
    layout.Push<float>(2);
    planeVa.AddVertexBuffer(planeVb, layout);
    planeVa.Unbind();


    Texture2D texture1("res/texture/wood.png");
    Texture2D gammatexture("res/texture/wood.png");

    LightShader.Bind();
    LightShader.SetUniformInt("texture1", 0);

    glm::vec3 lightPositions[] = 
    {
     glm::vec3(-3.0f, 0.0f, 0.0f),
     glm::vec3(-1.0f, 0.0f, 0.0f),
     glm::vec3(1.0f, 0.0f, 0.0f),
     glm::vec3(3.0f, 0.0f, 0.0f)
    };

    glm::vec3 lightColors[] = 
    {
        glm::vec3(0.25),
        glm::vec3(0.50),
        glm::vec3(0.75),
        glm::vec3(1.00)
    };
   

    while (!glfwWindowShouldClose(window))
    {
    
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        KeyInput(window);

       
        /* Render here */
        render.Clear();

        LightShader.Bind();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        LightShader.SetUniformMat4("projection", projection);
        LightShader.SetUniformMat4("view", view);

        LightShader.SetUniformFloat3("viewPos",camera.m_Position);
        glUniform3fv(glGetUniformLocation(LightShader.m_RendererID, "lightPositions"), 4, &lightPositions[0][0]);
        glUniform3fv(glGetUniformLocation(LightShader.m_RendererID, "lightColors"), 4, &lightColors[0][0]);
        LightShader.SetUniformInt("gamma", GammaEnabled);


        planeVa.Bind();
        if (GammaEnabled)
        {
            gammatexture.Bind();
        }
        else
        {
             texture1.Bind();
        }
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //std::cout << (blinn ? "BlinnPhong" : "Phong") << std::endl;
        std::cout << (GammaEnabled ? "Gamma enabled" : "Gamma disabled") << std::endl;

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

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !blinnKeyPressed)
    {
        blinn = !blinn;
        blinnKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE)
    {
        blinnKeyPressed = false;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !GammaKeyPressed)
    {
        GammaEnabled = !GammaEnabled;
        GammaKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
    {
        GammaKeyPressed = false;
    }
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

uint32_t loadCubeMap(std::vector<std::string> faces)
{
    uint32_t textureId;
    glCreateTextures(GL_TEXTURE_2D, 1, &textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    //stbi_set_flip_vertically_on_load(1);

    int width, height,Channels;
    for (uint32_t i = 0; i < faces.size(); i++)
    {
        uint8_t* data = stbi_load(faces[i].c_str(), &width, &height, &Channels, 0);

        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }

        else
        {
            std::cout << "立方贴图加载失败：" << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return textureId;

}


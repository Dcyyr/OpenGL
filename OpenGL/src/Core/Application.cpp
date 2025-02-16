#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <imgui.h>

//#include "Core/WindowsInput.cpp"
#include "Core/Timestep.h"
//#include "OpenGLRender/CameraController.h"
#include "OpenGLRender/Buffers.h"
#include "OpenGLRender/Shader.h"
#include "OpenGLRender/Texture2D.h"
#include "OpenGLRender/VertexArray.h"
#include "OpenGLRender/RenderObjects.h"
#include "OpenGLRender/Camera.h"
#include "OpenGLRender/Model.h"
#include "OpenGLRender/FrameBuffers.h"

//#include "Core/WindowsInput.h"
void KeyInput(GLFWwindow* window,Timestep ts);
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
uint32_t loadCubeMap(std::vector<std::string> faces);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

const uint32_t width = 1600;
const uint32_t height = 900;

float lastX = width / 2.0f;
float lastY = height / 2.0f;

bool firstMouse = true;

bool blinn = false;
bool blinnKeyPressed = false;

bool GammaEnabled = false;
bool GammaKeyPressed = false;

bool shadows = true;


int main()
{

    if (!glfwInit()) 
        return -1;
   
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
        std::cout << "³õÊ¼»¯GLADÊ§°Ü£¡" << std::endl;
        return -1;
    }

    Renderer render;

    glEnable(GL_DEPTH_TEST);//ÆôÓÃÉî¶È²âÊÔ
   
    //glEnable(GL_FRAMEBUFFER_SRGB);
 
    //setup shader
    Shader Shader("res/shader/Advance_Lighting/NormalMapping.shader");

    //setup texture
    Texture2D Texture("res/texture/brickwall.jpg");
    Texture2D NormalTexture("res/texture/brickwall_normal.jpg");


    Shader.Bind();
    Shader.SetUniformInt("u_DiffuseMap", 0);
    Shader.SetUniformInt("u_NormalMap", 1);

    glm::vec3 lightPos(0.5f, 1.0f, 0.3f);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = (float)glfwGetTime();
        float lastFrameTime = 0.0f;
        Timestep timestep = currentFrame - lastFrameTime;
        lastFrameTime = currentFrame;

        
        KeyInput(window, timestep);
        
       
        /* Render here */
        render.Clear();

        //----------------------------------------------------------------------------
        glm::mat4 projection = glm::perspective(glm::radians(camera.m_Zoom), (float)width / (float)height, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        Shader.Bind();
        Shader.SetUniformMat4("projection", projection);
        Shader.SetUniformMat4("view", view);

        //äÖÈ¾·¨ÏßÌùÍ¼ quad
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians((float)glfwGetTime() * -10.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));
        Shader.SetUniformMat4("model", model);
        Shader.SetUniformFloat3("viewPos", camera.m_Position);
        Shader.SetUniformFloat3("lightPos", lightPos);
        //
        Texture.Bind();
        NormalTexture.Bind(1);
        render.RenderQuad();

        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.1f));
        Shader.SetUniformMat4("model", model);
        render.RenderQuad();

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//GL_LINEÏß¿ò£¬GL_FILL»Ö¸´Ä¬ÈÏ
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }
   

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


void KeyInput(GLFWwindow* window,Timestep ts)
{

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.CameraInput(CameraMoveDirection::FORWARD, ts);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.CameraInput(CameraMoveDirection::BACKWARD, ts);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.CameraInput(CameraMoveDirection::LEFT, ts);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.CameraInput(CameraMoveDirection::RIGHT, ts);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.CameraInput(CameraMoveDirection::UP, ts);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.CameraInput(CameraMoveDirection::DOWN, ts);

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
            std::cout << "Á¢·½ÌùÍ¼¼ÓÔØÊ§°Ü£º" << faces[i] << std::endl;
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


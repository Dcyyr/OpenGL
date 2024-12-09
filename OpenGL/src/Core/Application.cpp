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
#include "OpenGLRender/Camera.h"

void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void Input(GLFWwindow* window);

const uint32_t width = 1600;
const uint32_t height = 900;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = width / 2.0f;
float lastY = height / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

glm::vec3 LightPos(1.2f, 1.0f, 2.0f);

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

         // positions          // normals           // texture coords
         -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
          0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
          0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
          0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
         -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        
         -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
          0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
         -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        
         -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        
          0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
          0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
          0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
          0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
          0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
          0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        
         -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
          0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
          0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
          0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
         -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        
         -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
          0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
         -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f

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

    glm::vec3 pointLightPositions[] = {
       glm::vec3(0.7f,  0.2f,  2.0f),
       glm::vec3(2.3f, -3.3f, -4.0f),
       glm::vec3(-4.0f,  2.0f, -12.0f),
       glm::vec3(0.0f,  0.0f, -3.0f)
    };

    Shader lightShader("res/shader/LightShader.shader");
    Shader lightCubeshader("res/shader/LightCubeShader.shader");

    Texture2D tex("res/texture/box.png");
    Texture2D tex2("res/texture/iron.png");

    //正方体1
    VertexArray va;
    VertexBuffer vb(vertices,sizeof(vertices));
    
    va.Bind();
    VertexBufferLayout layout1;
    layout1.Push<float>(3);
    layout1.Push<float>(3);
    layout1.Push<float>(2);
    va.AddVertexBuffer(vb, layout1);

    //正方体2
    VertexArray Lightva;
    Lightva.Bind();

    vb.Bind();
    VertexBufferLayout layout2;
    layout2.Push<float>(3);
    layout2.Push<float>(3);
    layout2.Push<float>(2);
    Lightva.AddVertexBuffer(vb, layout2);


     
    vb.Unbind();
    va.Unbind();
    Lightva.Unbind();
    Renderer render;

   

    while (!glfwWindowShouldClose(window))
    {
    
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        Input(window);

        /* Render here */
        render.Clear();

        //texture
        lightShader.Bind();
        lightShader.SetUniformInt("material.diffuse", 0);
        lightShader.SetUniformInt("material.specular", 1);
        //
        lightShader.Bind();
        lightShader.SetUniformFloat3("viewPos", camera.m_Position);
        lightShader.SetUniformFloat("material.shininess",32.0f);

        // directional light
        lightShader.SetUniformFloat3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
        lightShader.SetUniformFloat3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        lightShader.SetUniformFloat3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
        lightShader.SetUniformFloat3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        // point light 1
        lightShader.SetUniformFloat3("pointLights[0].position", pointLightPositions[0]);
        lightShader.SetUniformFloat3("pointLights[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        lightShader.SetUniformFloat3("pointLights[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        lightShader.SetUniformFloat3("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightShader.SetUniformFloat("pointLights[0].constant", 1.0f);
        lightShader.SetUniformFloat("pointLights[0].linear", 0.09f);
        lightShader.SetUniformFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        lightShader.SetUniformFloat3("pointLights[1].position", pointLightPositions[1]);
        lightShader.SetUniformFloat3("pointLights[1].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        lightShader.SetUniformFloat3("pointLights[1].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        lightShader.SetUniformFloat3("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightShader.SetUniformFloat("pointLights[1].constant", 1.0f);
        lightShader.SetUniformFloat("pointLights[1].linear", 0.09f);
        lightShader.SetUniformFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        lightShader.SetUniformFloat3("pointLights[2].position", pointLightPositions[2]);
        lightShader.SetUniformFloat3("pointLights[2].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        lightShader.SetUniformFloat3("pointLights[2].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        lightShader.SetUniformFloat3("pointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightShader.SetUniformFloat("pointLights[2].constant", 1.0f);
        lightShader.SetUniformFloat("pointLights[2].linear", 0.09f);
        lightShader.SetUniformFloat("pointLights[2].quadratic", 0.032f);
        // point light 4
        lightShader.SetUniformFloat3("pointLights[3].position", pointLightPositions[3]);
        lightShader.SetUniformFloat3("pointLights[3].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        lightShader.SetUniformFloat3("pointLights[3].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        lightShader.SetUniformFloat3("pointLights[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightShader.SetUniformFloat("pointLights[3].constant", 1.0f);
        lightShader.SetUniformFloat("pointLights[3].linear", 0.09f);
        lightShader.SetUniformFloat("pointLights[3].quadratic", 0.032f);
        // spotLight
        lightShader.SetUniformFloat3("spotLight.position", camera.m_Position);
        lightShader.SetUniformFloat3("spotLight.direction", camera.m_Front);
        lightShader.SetUniformFloat3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        lightShader.SetUniformFloat3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
        lightShader.SetUniformFloat3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightShader.SetUniformFloat("spotLight.constant", 1.0f);
        lightShader.SetUniformFloat("spotLight.linear", 0.09f);
        lightShader.SetUniformFloat("spotLight.quadratic", 0.032f);
        lightShader.SetUniformFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        lightShader.SetUniformFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.0f)));


        glm::mat4 view = camera.GetViewMatrix();
        lightShader.SetUniformMat4("view", view);

        glm::mat4 projection = glm::perspective(glm::radians(camera.m_Zoom), (float)width / (float)height, 0.1f, 100.0f);
        lightShader.SetUniformMat4("projection", projection);
    

        tex.Bind();
        tex2.Bind(1);
        va.Bind();

        for (uint32_t i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightShader.SetUniformMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        //光源矩形
        lightCubeshader.Bind();
        lightCubeshader.SetUniformMat4("projection", projection);
        lightCubeshader.SetUniformMat4("view", view);
        Lightva.Bind();
        for (uint32_t i = 0; i < 4; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);

            model = glm::translate(model,pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.1f));
            lightCubeshader.SetUniformMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
      
        //glm::mat4 mvp = projection * view * model;
        //shader.SetUniformMat4("mvp", mvp);  
     
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
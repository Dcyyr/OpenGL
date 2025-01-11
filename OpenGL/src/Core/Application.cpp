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

Camera camera(glm::vec3(0.0f, 0.0f, 100.0f));
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

    Renderer render;


    glEnable(GL_DEPTH_TEST);//启用深度测试

    Shader Asteroidshader("res/shader/Asteroid.shader");
    Shader Planetshader("res/shader/Planet.shader");
    //Model  Loadmodel("res/model/back/backpack.obj");
    //Model  RenModel("res/model/ren/nanosuit.obj");
    Model  PlantModel("res/model/planet/planet.obj");
    Model  AsteroidModel("res/model/rock/rock.obj");


    uint32_t amount = 10000;
    glm::mat4* modelMatrices;
    modelMatrices = new glm::mat4[amount];
    srand(static_cast<uint32_t>(glfwGetTime())); //  初始化随机种子
    float radius = 50.0f;
    float offset = 25.0f;
    for (uint32_t i = 0; i < amount; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        // 1. 位移：分布在半径为 'radius' 的圆形上，偏移的范围是 [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // 让行星带的高度比x和z的宽度要小
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        // 2. 缩放：在 0.05 和 0.25f 之间缩放
        float scale = static_cast<float>((rand() % 20) / 100.0 + 0.05);
        model = glm::scale(model, glm::vec3(scale));
         
        // 3. 旋转：绕着一个（半）随机选择的旋转轴向量进行随机的旋转
        float rotAngle = static_cast<float>((rand() % 360));
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. 添加到矩阵的数组中
        modelMatrices[i] = model;
    }


    uint32_t buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

    // set transformation matrices as an instance vertex attribute (with divisor 1)
    // note: we're cheating a little by taking the, now publicly declared, VAO of the model's mesh(es) and adding new vertexAttribPointers
    // normally you'd want to do this in a more organized fashion, but for learning purposes this will do.
    // -----------------------------------------------------------------------------------------------------------------------------------
    for (uint32_t i = 0; i < AsteroidModel.m_Meshes.size(); i++)
    {
        uint32_t VAO = AsteroidModel.m_Meshes[i].VAO;
        glBindVertexArray(VAO);
        // set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }

    while (!glfwWindowShouldClose(window))
    {
    
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        KeyInput(window);

       
        /* Render here */
        render.Clear();

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        Asteroidshader.Bind();
        Asteroidshader.SetUniformMat4("projection", projection);
        Asteroidshader.SetUniformMat4("view", view);
        Planetshader.Bind();
        Planetshader.SetUniformMat4("projection", projection);
        Planetshader.SetUniformMat4("view", view);


        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
        Planetshader.SetUniformMat4("model", model);
        PlantModel.Draw(Planetshader);
        
        Asteroidshader.Bind();

        Asteroidshader.SetUniformInt("texture_diffuse1", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, AsteroidModel.m_Textures[0].Id);

        for (uint32_t i = 0; i < AsteroidModel.m_Meshes.size(); i++)
        {
            glBindVertexArray(AsteroidModel.m_Meshes[i].VAO);
            //绘制一组元素的多个实例
            glDrawElementsInstanced(GL_TRIANGLES, static_cast<uint32_t>(AsteroidModel.m_Meshes[i].m_Indices.size()), GL_UNSIGNED_INT, 0, amount);
            glBindVertexArray(0);
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


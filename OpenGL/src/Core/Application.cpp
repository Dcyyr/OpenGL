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

Camera camera(glm::vec3(0.0f, 3.0f, 15.0f));

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
    
    //glEnable(GL_FRAMEBUFFER_SRGB);
 
    //setup shader
    Shader ShadowMappingShader("res/shader/Advance_Lighting/ShadowMapping/ShadowMapping.shader");
    Shader ShadowMappingDepthShader("res/shader/Advance_Lighting/ShadowMapping/ShadowMappingDepth.shader");
    Shader QuadShader("res/shader/Advance_Lighting/ShadowMapping/Quad.shader");

    //setup texture
    Texture2D WoodTexture("res/texture/wood.png");
    //Texture2D gammatexture("res/texture/wood.png");


    //配置深度图帧缓冲
    const uint32_t ShadowWidth = 1024, ShadowHeight = 1024;
    uint32_t depthMapFB;
    glCreateFramebuffers(1, &depthMapFB);
    //创建深度纹理
    uint32_t depthMap;
    glCreateTextures(GL_TEXTURE_2D, 1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, ShadowWidth, ShadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTextureParameteri(depthMap, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(depthMap, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
    //我们宁可让所有超出深度贴图的坐标的深度范围是1.0，这样超出的坐标将永远不在阴影之中。我们可以储存一个边框颜色，
    //然后把深度贴图的纹理环绕选项设置为GL_CLAMP_TO_BORDER：
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    //附加深度纹理作为帧缓冲的深度缓冲区
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFB);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    ShadowMappingShader.Bind();
    ShadowMappingShader.SetUniformInt("diffuseTexture", 0);
    ShadowMappingShader.SetUniformInt("shadowMap", 1);

    QuadShader.Bind();
    QuadShader.SetUniformInt("depthMap", 0);

    glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = (float)glfwGetTime();
        float lastFrameTime = 0.0f;
        Timestep timestep = currentFrame - lastFrameTime;
        lastFrameTime = currentFrame;

        
        KeyInput(window, timestep);
        
       
        /* Render here */
        render.Clear();

        
        //将场景深度渲染为纹理（从光线的角度看）
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float NearPlane = 1.0f, FarPlane = 7.5f;
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, NearPlane, FarPlane);
        lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        //从光线的角度渲染场景
        ShadowMappingDepthShader.Bind();
        ShadowMappingDepthShader.SetUniformMat4("lightSpaceMatrix", lightSpaceMatrix);

        glViewport(0, 0, ShadowWidth, ShadowHeight);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFB);
        glClear(GL_DEPTH_BUFFER_BIT);
        WoodTexture.Bind();
        glCullFace(GL_FRONT);
        render.RenderScene(ShadowMappingDepthShader);
        glCullFace(GL_BACK);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        //使用生成的深度/阴影贴图将场景渲染为正常效果

        ShadowMappingShader.Bind();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ShadowMappingShader.SetUniformMat4("projection", projection);
        ShadowMappingShader.SetUniformMat4("view", view);
        //设置light uniform
        ShadowMappingShader.SetUniformFloat3("viewPos",camera.m_Position);
        ShadowMappingShader.SetUniformFloat3("lightPos", lightPos);
        ShadowMappingShader.SetUniformMat4("lightSpaceMatrix", lightSpaceMatrix);
        WoodTexture.Bind();
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        render.RenderScene(ShadowMappingShader);


        //将深度图渲染为四边形，以便进行可视化调试
        QuadShader.Bind();
        QuadShader.SetUniformFloat("near_plane", NearPlane);
        QuadShader.SetUniformFloat("far_plane", FarPlane);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        

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


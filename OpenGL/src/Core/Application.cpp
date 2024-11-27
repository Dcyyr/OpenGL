#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <imgui.h>
#include <stb_image.h>


#include "OpenGLRender/Buffers.h"
#include "OpenGLRender/Shader.h"
#include "OpenGLRender/Texture2D.h"
#include "OpenGLRender/VertexArray.h"
#include "OpenGLRender/Renderer.h"

int main()
{

    glfwInit();
    glfwWindowHint(GLFW_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_VERSION_MINOR, 3);

    GLFWwindow* window = glfwCreateWindow(1600, 900, "OpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (!window)
    {
        std::cout << "GLFW���ڴ���ʧ��" << std::endl;
        glfwTerminate();
        return -1;
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "��ʼ��GLADʧ�ܣ�" << std::endl;
        return -1;
    }

    if (!glfwInit())
    {
        return -1;
    }

   

    float vertices[] = {
        0.5f,  0.5f, 1.0f,1.0f,                 // ���Ͻ�
        0.5f, -0.5f, 1.0f,0.0f,                  // ���½�
       -0.5f, -0.5f, 0.0f,0.0f,                   // ���½�
       -0.5f,  0.5f, 0.0f,1.0f                   // ���Ͻ�
           
          
    };

    uint32_t indices[] = {
        0,1,2,//��һ��������
        2,3,0 //second
    };
   
    VertexArray va;
    VertexBuffer vb(vertices, 4 * 4 * sizeof(float));
    
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddVertexBuffer(vb, layout);

    IndexBuffer ib(indices, 6);

    glm::vec4 color = glm::vec4(0.9f, 0.5f, 0.8f, 1.0f);
    Shader shader("res/shader/Shader.glsl");
    shader.Bind();
    shader.SetFloat4("u_Color", color);

    Texture2D tex("res/texture/1.jpg");
    tex.Bind();
    shader.SetInt("u_Texture", 0);
    //��ɫ����
   /* glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);*/

    
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();
    va.Unbind();

    Renderer render;


    while (!glfwWindowShouldClose(window))
    {
        
        /* Render here */
        render.Clear();
        
        shader.Bind();
        shader.SetFloat4("u_Color",color);

        va.Bind();
        ib.Bind();
        //glDrawArrays(GL_TRIANGLES, 0, 3);//��һ��������
        render.Draw(vb,ib,shader);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//GL_LINE�߿�GL_FILL�ָ�Ĭ��
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}
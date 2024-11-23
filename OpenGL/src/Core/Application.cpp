#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <imgui.h>

#include "OpenGLRender/Buffers.h"
#include "OpenGLRender/Shader.h"


int main()
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

    glm::vec4 color = glm::vec4(0.9f,0.2f,0.8f,1.0f);
    Shader shader("shader/Shader.glsl");
    shader.Bind();
    shader.SetFloat4("u_Color", color);

    float vertices[] = {
         0.5f, -0.5f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   // bottom left
         0.0f,  0.5f, 0.0f    // top 
          
    };

    uint32_t indices[] = {
        0,1,2,//第一个三角形
        2,3,0 //second
    };
   
    uint32_t vao;
    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);

    VertexBuffer vb(vertices, 3 * 3 * sizeof(float));
    IndexBuffer ib(indices, 6);

    //位置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //颜色属性
   /* glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);*/

    vb.Unbind();
    ib.Unbind();
    shader.Unbind();

    while (!glfwWindowShouldClose(window))
    {
        
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
        
        shader.Bind();
        shader.SetFloat4("u_Color",color);

        glBindVertexArray(vao);
        
 
        glDrawArrays(GL_TRIANGLES, 0, 3);//画一个三角形
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//画一个四边形
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//GL_LINE线框，GL_FILL恢复默认
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    

    glfwTerminate();
    return 0;
}
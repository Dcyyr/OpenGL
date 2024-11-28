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

int main()
{

    glfwInit();
   

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
   

    float vertices[] = {
        0.5f,  0.5f, 1.0f,1.0f,                 // 右上角
        0.5f, -0.5f, 1.0f,0.0f,                 // 右下角
       -0.5f, -0.5f, 0.0f,0.0f,                 // 左下角
       -0.5f,  0.5f, 0.0f,1.0f                  // 左上角
           
          
    };

    uint32_t indices[] = {
        0,1,2,//第一个三角形
        2,3,0 //second
    };
   
    VertexArray va;
    VertexBuffer vb(vertices, 4 * 4 * sizeof(float));
    
    VertexBufferLayout layout;
    layout.Push<float>(2);//glVertexAttribPointer
    layout.Push<float>(2);
    va.AddVertexBuffer(vb, layout);

    IndexBuffer ib(indices, 6);

    glm::vec4 color = glm::vec4(0.9f, 0.5f, 0.8f, 1.0f);
    Shader shader("res/shader/Shader.glsl");
    shader.Bind();
    shader.SetUniformFloat4("u_Color", color);

    Texture2D tex("res/texture/1.jpg");
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
        
        /* Render here */
        render.Clear();
        
        shader.Bind();
        shader.SetUniformFloat4("u_Color",color);

        glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 1.0f));

        shader.SetUniformMat4("transform",transform);

        va.Bind();
        ib.Bind();
        //glDrawArrays(GL_TRIANGLES, 0, 3);//画一个三角形
        render.Draw(vb,ib,shader);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//GL_LINE线框，GL_FILL恢复默认
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}
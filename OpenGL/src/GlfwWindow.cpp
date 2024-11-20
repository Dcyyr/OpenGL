#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>


const char* vertexshaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 a_Pos;
    void main()
    {
       gl_Position = vec4(a_Pos.x, a_Pos.y, a_Pos.z, 1.0);
    }
)";

const char* fragmentshaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main()
    {
       FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    };
   
)";


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

    uint32_t vertexshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexshader, 1, &vertexshaderSource, NULL);
    glCompileShader(vertexshader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexshader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    uint32_t fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentshader, 1, &fragmentshaderSource, NULL);
    glCompileShader(fragmentshader);
    // check for shader compile errors
    glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentshader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    uint32_t shaderprogram = glCreateProgram();
    glAttachShader(shaderprogram, vertexshader);
    glAttachShader(shaderprogram, fragmentshader);
    glLinkProgram(shaderprogram);
    // check for linking errors
    glGetProgramiv(shaderprogram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderprogram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexshader);
    glDeleteShader(fragmentshader);


    float vertices[] = {
        -0.5f,-0.5f,0.0f,//左下
         0.5f,-0.5f,0.0f,//右下
         0.5f, 0.5f,0.0f,//右上
        -0.5f, 0.5f,0.0f //左上
    };

    uint32_t indices[] = {
        0,1,2,//第一个三角形
        2,3,0 //second
    };
   
    uint32_t vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    uint32_t vb;
    glCreateBuffers(1, &vb);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    uint32_t ib;
    glCreateBuffers(1, &ib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
        
        glUseProgram(shaderprogram);
        glBindVertexArray(vao);
        //glDrawArrays(GL_TRIANGLES, 0, 3);//画一个三角形
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//画一个四边形

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &ib);
    glDeleteProgram(shaderprogram);

    glfwTerminate();
    return 0;
}
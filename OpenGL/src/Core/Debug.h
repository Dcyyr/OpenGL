#pragma once

#include <iostream>
#include <glad/glad.h>

#define ASSERT(x, ...) if(!x) __debugbreak()

#define GLCall(x) do{\
    GLClearError();\
    x;\
    ASSERT(GLLogCall(#x,__FILE__, __LINE__));\
}while(0)



void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);


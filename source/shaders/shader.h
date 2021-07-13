#ifndef _SHADER_H
#define _SHADER_H

#include <iostream>
#include <cstring>
#include <fstream>

#include <glad/glad.h>

class shader { 
    public:
        shader(const char*, GLenum);
        ~shader();
        
        GLuint get_handle();
    protected:
    private:
        std::string name;
        GLuint shader_handle;
};

#endif
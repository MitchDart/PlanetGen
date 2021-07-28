#ifndef _SHADER_H
#define _SHADER_H

#include <iostream>
//Macro for checking if using Microsoft Visual C++
#ifdef _MSC_VER
#include <string>
#elif !_MSC_VER
#include <cstring>
#endif
#include <fstream>

#include <glad/glad.h>

class shader { 
    public:
        shader(const char*, GLenum);
        ~shader();
        
        GLuint get_handle();
    protected:
    private:
        std::string load_file(std::string path);

        std::string name;
        GLuint shader_handle;
};
#endif
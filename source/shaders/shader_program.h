#ifndef _SHADER_PROGRAM_H
#define _SHADER_PROGRAM_H

#include <glad/glad.h>

#include "shader.h"

class shader_program {
    public:
        shader_program() {};
        shader_program(const char* vertex_shader, const char* fragment_shader, const char* geometry_shader, const char* tcp_shader, const char* tes_shader);
        shader_program(const shader_program&);

        void use_program();
        GLuint get_shader_program_handle();
    protected:
    private:
        GLuint shader_program_handle;
};

#endif
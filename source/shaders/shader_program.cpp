#include "shader_program.h"        
        
shader_program::shader_program(const char* vertex_shader, const char* fragment_shader) {
    shader vertex(vertex_shader, GL_VERTEX_SHADER);
    shader fragment(fragment_shader , GL_FRAGMENT_SHADER);

    shader_program_handle = glCreateProgram();

    glAttachShader(shader_program_handle, vertex.get_handle());
    glAttachShader(shader_program_handle, fragment.get_handle());

    glLinkProgram(shader_program_handle);

    int  success;
    char infoLog[512];
    glGetProgramiv(shader_program_handle, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shader_program_handle, 512, NULL, infoLog);
        std::cout << "Linking shader program failed: " << std::endl << infoLog << std::endl;
        exit(EXIT_FAILURE);
    }
}

shader_program::shader_program(const shader_program& other) {
    shader_program_handle = other.shader_program_handle;
}

void shader_program::use_program() {
    glUseProgram(shader_program_handle);
}

GLuint shader_program::get_shader_program_handle() {
    return shader_program_handle;
}
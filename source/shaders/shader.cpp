#include "shader.h"

shader::shader(const char* file_name, GLenum shaderType) {
    name = std::string(file_name);
    std::ifstream in(std::string("glsl/") + name);
    std::string contents((std::istreambuf_iterator<char>(in)),std::istreambuf_iterator<char>());

    if(contents.empty()) {
        std::cout << "Shader: " << name << " empty"; 
        exit(EXIT_FAILURE);
    }

    shader_handle = glCreateShader(shaderType);

    auto char_source = contents.c_str();

    glShaderSource(shader_handle, 1, &char_source, NULL);
    glCompileShader(shader_handle);

    int  success;
    char infoLog[512];
    glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(shader_handle, 512, NULL, infoLog);
        std::cout << "Compilation failed shader: " << name << std::endl << infoLog << std::endl;
        exit(EXIT_FAILURE);
    }
}

shader::~shader() {
    glDeleteShader(shader_handle);
    shader_handle = 0;

    std::cout << "Shader destroyed: " << name << std::endl;
}

GLuint shader::get_handle() {
    return shader_handle;
}
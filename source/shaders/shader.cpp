#include "shader.h"

shader::shader(const char* file_name, GLenum shaderType) {
    name = std::string(file_name);

    std::string contents = load_file(std::string("glsl/") + name);

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

std::string shader::load_file(std::string path) {
    std::string include_identifier = "#include ";
    static bool is_recursive = false;

    std::string source = "";
    std::ifstream file(path);

    if (!file.is_open())
    {
        std::cerr << "ERROR: could not open the shader at: " << path << "\n" << std::endl;
        return source;
    }

    std::string line_buffer;
    while (std::getline(file, line_buffer))
    {
        if (line_buffer.find(include_identifier) != line_buffer.npos)
        {
            line_buffer.erase(0, include_identifier.size());

            size_t found = path.find_last_of("/\\");
            auto path_without_file = path.substr(0, found + 1);
            line_buffer.insert(0, path_without_file);

            is_recursive = true;
            source += load_file(line_buffer);

            continue;
        }

        source += line_buffer + '\n';
    }

    if (!is_recursive)
        source += '\0';

    file.close();

    return source;
}

GLuint shader::get_handle() {
    return shader_handle;
}
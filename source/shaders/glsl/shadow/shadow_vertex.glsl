#version 410

layout (location = 0) in vec3 vertex_position_in;

void main()
{
    gl_Position = vec4(vertex_position_in, 1.0f);
}
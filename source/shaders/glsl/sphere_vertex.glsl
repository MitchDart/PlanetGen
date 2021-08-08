#version 410

layout (location = 0) in vec3 vertex_position_in;
layout (location = 1) in vec3 color;

out vec3 position_v;

void main()
{
    position_v = vertex_position_in;
    gl_Position = vec4(vertex_position_in, 1.0f);
}
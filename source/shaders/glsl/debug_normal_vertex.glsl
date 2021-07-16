#version 460

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;

out vec3 normal_v;

void main()
{
    normal_v = vertex_normal;

    gl_Position = vec4(vertex_position, 1.0f);
}
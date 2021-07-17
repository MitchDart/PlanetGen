#version 460

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;

out vec3 position;
out vec3 normal;

uniform mat4 matrix_mvp;
uniform mat4 matrix_m;

void main()
{
    position = (matrix_m * vec4(vertex_position, 1.0f)).xyz;
    normal = normalize(matrix_m * vec4(vertex_normal, 0.0f)).xyz;
    gl_Position = matrix_mvp * vec4(vertex_position, 1.0f);
}
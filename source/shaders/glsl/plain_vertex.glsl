#version 460
layout (location = 0) in vec3 vertex_position;

out vec3 position;

uniform mat4 mvp;

void main()
{
    position = vertex_position;
    gl_Position = mvp * vec4(vertex_position.x, vertex_position.y, vertex_position.z, 1.0);
}
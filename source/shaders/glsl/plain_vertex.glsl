#version 460
layout (location = 0) in vec3 vertex_position;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(vertex_position.x, vertex_position.y, vertex_position.z, 1.0);
}
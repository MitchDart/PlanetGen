#version 460
out vec4 fragment_color;
in vec3 position;

void main()
{
    vec3 offset_pos = (position + 1.0f)/2.0f;
    fragment_color = vec4(offset_pos.x, offset_pos.y, offset_pos.z, 1.0f);
} 
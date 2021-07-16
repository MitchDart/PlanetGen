#version 460

in vec3 position;
in vec3 normal;

out vec4 fragment_color;

void main()
{
    vec3 offset_pos = (position + 1.0f)/2.0f;
    float diffuse = clamp(dot(normal, vec3(0.0f,1.0f,1.0f)), 0.0f, 1.0f);

    fragment_color = vec4(normal * diffuse, 1.0f);
} 
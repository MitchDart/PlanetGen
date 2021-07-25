#version 410

in vec3 position;
in vec3 normal;
in vec3 barycentric_coordinate;

out vec4 fragment_color;

uniform float transparent;

void main()
{
    float closest_edge = min(barycentric_coordinate.x, min(barycentric_coordinate.y, barycentric_coordinate.z));
    float f_width = fwidth(closest_edge);
    float side = (smoothstep(0.0f, f_width ,closest_edge) - 1.0f) * -1.0;

    float alpha = max(side, transparent);

    fragment_color = vec4(side * barycentric_coordinate, alpha);
}
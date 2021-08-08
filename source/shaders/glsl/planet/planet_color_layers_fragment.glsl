#version 410

in vec3 position_v;
in vec3 normal;

out vec4 fragment_color;

uniform vec4 diffuse_color;

void main()
{
    vec4 deep_ocean = vec4(0.0, 0.0, 1.0, 1.0);
    vec4 color = deep_ocean;
    vec3 origin = vec3(0.0, 0.0, 0.0);
    float calc_distance = distance(origin, position_v);
    vec4 shallow_ocean = vec4(0.2, 0.2, 0.9, 1.0);
    vec4 grass = vec4(0.0, 1.0, 0.0, 1.0);
    vec4 mountain = vec4(0.3, 0.13, 0.13, 1.0);

    if(calc_distance > 11.0) {
        color = mountain;
    }

    if(calc_distance > 9.0 && calc_distance < 11.0) {
        float percentage = (11.0 - calc_distance);
        color = (percentage * grass) + (1.0 - percentage) * mountain;
    }

    if(calc_distance > 7.0 && calc_distance < 9.0) {
        float percentage = (10.0 - calc_distance);
        color = (percentage * shallow_ocean) + (1.0 - percentage) * grass;
    }

    fragment_color = color;
}
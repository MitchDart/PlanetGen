#version 410

layout(triangles, equal_spacing, ccw) in;

in vec3 position_raw_tcs[];

out vec3 position_v;

uniform mat4 matrix_m;
uniform mat4 matrix_light;

uniform float height_noise_strength;

uniform float noise_scale;
uniform float noise_position;

#define PI 3.1415926538

#include ../util/noise.glsl

vec3 point_height(vec3 unit_sphere_vector) {
    vec3 position = normalize(unit_sphere_vector);
    float height = height_noise_strength * snoise((position + vec3(noise_position, noise_position, noise_position)) * noise_scale);
    position = position + (position * height);
    return position;
}

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}

void main()
{
    position_v = interpolate3D(position_raw_tcs[0], position_raw_tcs[1], position_raw_tcs[2]);

    position_v = point_height(position_v);
    position_v = (matrix_m * vec4(position_v, 1.0f)).xyz;

    gl_Position = matrix_light * vec4(position_v, 1.0f);
}
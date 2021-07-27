#version 410

layout(triangles, equal_spacing, ccw) in;

in vec3 position_raw_tcs[];

out vec3 position_v;
out vec3 normal_v;
out vec3 position_raw_v;
out vec3 normal_raw_v;
out vec4 light_v;

uniform mat4 matrix_m;
uniform mat4 matrix_mvp;
uniform mat4 matrix_light;
uniform float height_noise_strength;
uniform float normal_sample_distance;

uniform float noise_scale;
uniform float noise_position;

#define PI 3.1415926538

#include util/noise.glsl

vec3 point_height(vec3 unit_sphere_vector) {
    vec3 position = normalize(unit_sphere_vector);
    float height = height_noise_strength * snoise((position + vec3(noise_position, noise_position, noise_position)) * noise_scale);
    position = position + (position * height);
    return position;
}

float atan2(in float y, in float x)
{
    bool s = (abs(x) > abs(y));
    return mix(PI/2.0 - atan(x,y), atan(y,x), s);
}

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}

vec3 calc_normal(vec3 vertex_position_in) {
    float radius = 1.0f;
    float inclination = atan2(sqrt((vertex_position_in.x * vertex_position_in.x) + (vertex_position_in.z * vertex_position_in.z)), vertex_position_in.y);
    float azimuth = atan2(vertex_position_in.z, vertex_position_in.x);

    float sample_incl_top = max(0.0f, inclination - normal_sample_distance);
    float sample_incl_bottom = min(PI, inclination + normal_sample_distance);

    float sample_azi_left = azimuth - normal_sample_distance;
    float sample_azi_right = azimuth + normal_sample_distance;

    vec3 sample_top = point_height(vec3(radius * cos(azimuth) * sin(sample_incl_top), radius * cos(sample_incl_top), radius * sin(azimuth) * sin(sample_incl_top)));
    vec3 sample_bottom = point_height(vec3(radius * cos(azimuth) * sin(sample_incl_bottom), radius * cos(sample_incl_bottom), radius * sin(azimuth) * sin(sample_incl_bottom)));
    vec3 sample_left = point_height(vec3(radius * cos(sample_azi_left) * sin(inclination), radius * cos(inclination), radius * sin(sample_azi_left) * sin(inclination)));
    vec3 sample_right = point_height(vec3(radius * cos(sample_azi_right) * sin(inclination), radius * cos(inclination), radius * sin(sample_azi_right) * sin(inclination)));

    vec3 normal_sample_top_left = cross(sample_left - vertex_position_in, sample_top - vertex_position_in);
    vec3 normal_sample_top_right = cross(sample_top - vertex_position_in, sample_right - vertex_position_in);
    vec3 normal_sample_bottom_left = cross(sample_bottom - vertex_position_in, sample_left - vertex_position_in);
    vec3 normal_sample_bottom_right = cross(sample_right - vertex_position_in, sample_bottom - vertex_position_in);

    vec3 normal_average = (normal_sample_top_left + normal_sample_top_right + normal_sample_bottom_left + normal_sample_bottom_right)/4.0f;

    return normal_average;
}

void main()
{
    position_raw_v = interpolate3D(position_raw_tcs[0], position_raw_tcs[1], position_raw_tcs[2]);

    vec3 normal_average = calc_normal(position_raw_v);
    normal_raw_v = normalize(normal_average);
    normal_v = normalize(matrix_m * vec4(normal_raw_v, 0.0f)).xyz;

    position_raw_v = point_height(position_raw_v);
    position_v = (matrix_m * vec4(position_raw_v, 1.0f)).xyz;
    
    light_v = matrix_light * vec4(position_v, 1.0f);

    gl_Position = matrix_mvp * vec4(position_raw_v, 1.0f);
}
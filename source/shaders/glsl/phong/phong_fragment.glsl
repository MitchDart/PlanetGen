#version 410

in vec3 position_v;
in vec3 normal_v;
in vec4 light_v;

out vec4 fragment_color;

uniform vec3 camera_position;
uniform vec3 light_direction;

uniform float specular_strength;
uniform float ambient_strength;

uniform vec4 diffuse_color;
uniform uint specular_exponent;

uniform sampler2D shadow_map;

float calc_shadow(vec4 position) {
    //This is some funky stuff. Need more reading. NVM not needed for ortho
    vec3 projected_coords = position.xyz / position.w;

    //[-1,1] -> [0,1]
    projected_coords = projected_coords * 0.5f + 0.5f;

    //r get first value (ie depth)
    float closest_depth = texture(shadow_map, projected_coords.xy).r;
    float current_depth = projected_coords.z;

    float shadow = current_depth > closest_depth  ? 1.0 : 0.0;  
    return shadow;
}

void main()
{
    vec3 view_direction = normalize(camera_position - position_v);
    vec3 reflect_direction = reflect(-light_direction, normal_v);

    float specular_ratio = min(pow(max(dot(view_direction, reflect_direction), 0.0f), specular_exponent), 1.0f);
    float diffuse_ratio = max(dot(normal_v, light_direction), 0.0f);

    vec3 ambient = diffuse_color.xyz * ambient_strength;
    vec3 diffuse = diffuse_color.xyz * diffuse_ratio;
    vec3 specular = specular_ratio * specular_strength * vec3(1.0f,1.0f,1.0f);
    float shadow = 0.0f;//calc_shadow(light_v);

    vec3 result = ambient + (1 - shadow) * (diffuse + specular);
    fragment_color = vec4(result, diffuse_color.a);
}
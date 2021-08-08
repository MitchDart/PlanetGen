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
uniform float shadow_bias_min;
uniform float shadow_bias_max;

uniform int show_layers;
uniform sampler2D planet_layer_map;

float calc_shadow(vec4 position) {
    //This is some funky stuff. Need more reading. NVM not needed for ortho
    vec3 projected_coords = position.xyz / position.w;

    //[-1,1] -> [0,1]
    projected_coords = projected_coords * 0.5f + 0.5f;

    float bias = max(shadow_bias_max * (1.0 - dot(normal_v, light_direction)), shadow_bias_min);  

    float current_depth = projected_coords.z;

    float shadow = 0.0f;
    vec2 texel_size = 1.0 / textureSize(shadow_map, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcf = texture(shadow_map, projected_coords.xy + vec2(x, y) * texel_size).r; 
            shadow += current_depth - bias > pcf ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    return shadow;
}

vec4 calc_color_from_planet_layer(vec4 position) {
    vec2 position_in_texture = vec2(position.x/3840, position.y/2160);
    float red = texture(planet_layer_map, position_in_texture).r;
    float green = texture(planet_layer_map, position_in_texture).g;
    float blue = texture(planet_layer_map, position_in_texture).b;
    if(red < 0 || green < 0 || blue < 0 || red > 1.0 || green > 1.0 || blue > 1.0) {
        return vec4(0.0, 0.0, 0.0, 1.0);
    }
    return vec4(red, green, blue, 1.0f);
}

void main()
{
    vec3 view_direction = normalize(camera_position - position_v);
    vec3 reflect_direction = reflect(-light_direction, normal_v);

    float specular_ratio = min(pow(max(dot(view_direction, reflect_direction), 0.0f), specular_exponent), 1.0f);
    float diffuse_ratio = max(dot(normal_v, light_direction), 0.0f);

    vec4 color = vec4(diffuse_color.xyz * ambient_strength, 1.0);
    if(show_layers == 1) {
        color = calc_color_from_planet_layer(gl_FragCoord);
    }
    vec3 ambient = color.xyz * ambient_strength;
    vec3 diffuse = diffuse_color.xyz * diffuse_ratio;
    vec3 specular = specular_ratio * specular_strength * vec3(1.0f,1.0f,1.0f);
    float shadow = calc_shadow(light_v);

    vec3 result =  ambient + (1 - shadow) * (diffuse + specular);
    fragment_color = vec4(result, 1.0f);
}
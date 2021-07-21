#version 410

in vec3 position;
in vec3 normal;

out vec4 fragment_color;

uniform vec3 camera_position;
uniform vec3 light_direction;

uniform float specular_strength;
uniform float ambient_strength;

uniform vec4 diffuse_color;
uniform uint specular_exponent;

void main()
{
    vec3 view_direction = normalize(camera_position - position);
    vec3 reflect_direction = reflect(-light_direction, normal);

    float specular_ratio = min(pow(max(dot(view_direction, reflect_direction), 0.0f), specular_exponent), 1.0f);
    float diffuse_ratio = max(dot(normal, light_direction), 0.0f);

    vec3 ambient = diffuse_color.xyz * ambient_strength;
    vec3 diffuse = diffuse_color.xyz * diffuse_ratio;
    vec3 specular = specular_ratio * specular_strength * vec3(1.0f,1.0f,1.0f);  

    vec3 result = ambient + diffuse + specular;
    fragment_color = vec4(result, diffuse_color.a);
}
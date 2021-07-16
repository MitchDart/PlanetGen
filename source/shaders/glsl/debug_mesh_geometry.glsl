#version 460

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 position_v[];
in vec3 normal_v[];

out vec3 position;
out vec3 normal;
out vec3 barycentric_coordinate;

void main()
{
    vec3 barycentric_coordinates[3] = vec3[]( 
        vec3(1.0, 0.0, 0.0),
        vec3(0.0, 1.0, 0.0),
        vec3(0.0, 0.0, 1.0)
    );

    for(int i = 0; i < 3; i++ ) {
        position = position_v[i];
        normal = normal_v[i];
        barycentric_coordinate = barycentric_coordinates[i];

        gl_Position = gl_in[i].gl_Position;

        EmitVertex();
    }

    EndPrimitive();
}
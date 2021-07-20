#version 460

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 position_v[];
in vec3 normal_v[];

out vec3 position;
out vec3 normal;

void main()
{
    for(int i = 0; i < 3; i++ ) {
        position = position_v[i];
        normal =  normal_v[i];
        //normal = normalize(cross(position_v[2] - position_v[0],position_v[1] - position_v[0]));

        gl_Position = gl_in[i].gl_Position;

        EmitVertex();
    }

    EndPrimitive();
}
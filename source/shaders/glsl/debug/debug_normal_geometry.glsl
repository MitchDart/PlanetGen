#version 410

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in vec3 normal_raw_v[];
in vec3 position_raw_v[];

out vec4 color_f;

uniform mat4 matrix_mvp;

uniform vec4 color;
uniform float normal_length;

void main()
{
    for(int i = 0; i < 3; i++ ) {

        vec3 original_pos = position_raw_v[i];
        
        gl_Position = matrix_mvp * vec4(original_pos, 1.0f);
        color_f = color;

        EmitVertex();

        gl_Position = matrix_mvp * vec4(original_pos.xyz + (normal_raw_v[i] * normal_length),1.0f);

        EmitVertex();

        EndPrimitive();
    }
}
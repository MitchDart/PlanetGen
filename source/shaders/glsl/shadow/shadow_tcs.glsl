#version 410

layout (vertices = 3) out;

in vec3 position_v[];

out vec3 position_tcs[];

uniform uint tessellation_level;

void main() 
{
    uint tlv = tessellation_level;

    position_tcs[gl_InvocationID] = position_v[gl_InvocationID];

    gl_TessLevelOuter[0] = tlv;
    gl_TessLevelOuter[1] = tlv;
    gl_TessLevelOuter[2] = tlv;
    gl_TessLevelInner[0] = gl_TessLevelOuter[2];
}
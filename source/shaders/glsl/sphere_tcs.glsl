#version 410

layout (vertices = 3) out;

in vec3 position_v[];

out vec3 position_tcs[];

uniform uint tessellation_level;
uniform vec3 camera_position;
uniform mat4 matrix_m;
uniform int camera_tlv;

void main() 
{
    vec3 pos = (matrix_m * vec4(position_v[0], 1.0f)).xyz;
    uint tlv = tessellation_level;

    if(camera_tlv > 0) {
        float dist = distance(camera_position,pos);
        if( dist <= 10.0f) {
            tlv = 4;
        } else if( dist <= 20.0f) {
            tlv = 3;
        } else if( dist <= 30.0f) {
            tlv = 2;
        } else {
            tlv = 1;
        }
    }

    position_tcs[gl_InvocationID] = position_v[gl_InvocationID];

    gl_TessLevelOuter[0] = tlv;
    gl_TessLevelOuter[1] = tlv;
    gl_TessLevelOuter[2] = tlv;
    gl_TessLevelInner[0] = gl_TessLevelOuter[2];
}
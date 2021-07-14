#include "icosphere.h"

icosphere::icosphere(std::shared_ptr<camera> camera) : object(camera) {
    const float t = (1.f + sqrtf(5.f)) * 0.5f; 
    const float i = 1.0f/sqrtf(t * t + 1.f);  
    const float a = t * i;                    

    using glm::vec3;

    vec3 verts[12] = {
        vec3(a, i, 0), 
        vec3(-a, i, 0),
        vec3( a,-i, 0),
        vec3(-a,-i, 0),
        vec3( i, 0, a),
        vec3( i, 0,-a),
        vec3(-i, 0, a),
        vec3(-i, 0,-a),
        vec3( 0, a, i),
        vec3( 0,-a, i),
        vec3( 0, a,-i),
        vec3( 0,-a,-i)
    };


    unsigned int index[] = {
        0,  4,  8,
        0,  10, 5,
        2,  9,  4,
        2,  5, 11,
        1,  8,  6,

        1,  7, 10,
        3,  6,  9,
        3, 11,  7,
        0,  8, 10,
        1, 10,  8,

        2, 11,  9,
        3,  9,  11,
        4,  0,  2,

        5,  2,  0,
        6,  3,  1,
        7,  1,  3,
        8,  4,  6,
        9,  6,  4,
        10, 7,  5,
        11, 5,  7
    };


    std::shared_ptr<float[]> array_vert(new float[12 * 3], [](const float* other) { delete[] other; });

    for (int i = 0; i < 12; i++) {
        array_vert.get()[i * 3 + 0] = verts[i].x;
        array_vert.get()[i * 3 + 1] = verts[i].y;
        array_vert.get()[i * 3 + 2] = verts[i].z;
    }

    vertices = array_vert;

    std::shared_ptr<unsigned int[]> array_indices(new unsigned int[20*3], [](const unsigned int* other) { delete[] other; });
    
    for (int i = 0; i < 20*3; i++) {
        array_indices.get()[i] = index[i];
    }

    indices = array_indices;

    vertex_count = 12;
    index_count = 20*3;

    calc_norms();
}

std::shared_ptr<float[]> icosphere::get_vertices() {
    return vertices;
}
std::shared_ptr<unsigned int[]> icosphere::get_indices() {
    return indices;
}
std::shared_ptr<float[]> icosphere::get_normals() {
    return normals;
}

void icosphere::calc_norms() {

    std::shared_ptr<float[]> array_normals(new float[vertex_count * 3], [](const float* other) { delete[] other; });

    for (int i = 0; i < index_count; i += 3) {
        unsigned int a_index = indices.get()[i] * 3;
        unsigned int b_index = indices.get()[i + 1] * 3;
        unsigned int c_index = indices.get()[i + 2] * 3;

        glm::vec3 a(vertices.get()[a_index], vertices.get()[a_index + 1], vertices.get()[a_index + 2]);
        glm::vec3 b(vertices.get()[b_index], vertices.get()[b_index + 1], vertices.get()[b_index + 2]);
        glm::vec3 c(vertices.get()[c_index], vertices.get()[c_index + 1], vertices.get()[c_index + 2]);

        auto normal = glm::normalize(glm::cross(c - a, b - a));
        
        array_normals.get()[a_index] = normal.x;
        array_normals.get()[a_index + 1] = normal.y;
        array_normals.get()[a_index + 2] = normal.z;

        array_normals.get()[b_index] = normal.x;
        array_normals.get()[b_index + 1] = normal.y;
        array_normals.get()[b_index + 2] = normal.z;

        array_normals.get()[c_index] = normal.x;
        array_normals.get()[c_index + 1] = normal.y;
        array_normals.get()[c_index + 2] = normal.z;
    }

    normals = array_normals;
}
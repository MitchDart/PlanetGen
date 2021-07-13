#include "color_cube.h"

color_cube::color_cube(std::shared_ptr<camera> camera) : object(camera) {

    using glm::vec3;

    vec3 verts[8] = {
        vec3(-1.0f, 1.0f, 1.0f),
        vec3(1.0f, 1.0f, 1.0f),
        
        vec3(1.0f, -1.0f, 1.0f),
        vec3(-1.0f, -1.0f, 1.0f),

        vec3(-1.0f, 1.0f, -1.0f),
        vec3(1.0f, 1.0f, -1.0f),
        
        vec3(1.0f, -1.0f, -1.0f),
        vec3(-1.0f, -1.0f, -1.0f)
    };


    unsigned int index[36] = {
        //f1
            0, 1, 3,
        //f2
            3, 1, 2,
        //b1
            4, 7, 5,
        //b2
            5, 7, 6,
        //r1
            1, 5, 6,
        //r2
            6, 2, 1,
        //l1
            4, 0, 7,
        //l2
            7, 0, 3,
        //t1
            1, 0, 5,
        //t2
            5, 0, 4,
        //b1
            3, 2, 6,
        //b2
            6, 7, 3,
    };

    std::shared_ptr<float[]> array_vert(new float[8], [](const float* other) { delete[] other; });

    for (int i = 0; i < 8; i++) {
        array_vert.get()[i * 3 + 0] = verts[i].x;
        array_vert.get()[i * 3 + 1] = verts[i].y;
        array_vert.get()[i * 3 + 2] = verts[i].z;
    }

    vertices = array_vert;

    std::shared_ptr<unsigned int[]> array_indices(new unsigned int[36], [](const unsigned int* other) { delete[] other; });
    
    for (int i = 0; i < 36; i++) {
        array_indices.get()[i] = index[i];
    }

    indices = array_indices;

    vertex_count = 8;
    index_count = 36;
}

std::shared_ptr<float[]> color_cube::get_vertices() {
    return vertices;
}

std::shared_ptr<unsigned int[]> color_cube::get_indices() {
    return indices;
}
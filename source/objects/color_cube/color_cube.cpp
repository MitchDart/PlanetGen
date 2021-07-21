#include "color_cube.h"

color_cube::color_cube(std::shared_ptr<camera> camera) : object(camera) {

    using glm::vec3;

    vec3 verts[24] = {
        //Front
        vec3(-1.0f, 1.0f, 1.0f),  //ltf 
        vec3(1.0f, 1.0f, 1.0f),   //rtf
        vec3(1.0f, -1.0f, 1.0f),  //rbf
        vec3(-1.0f, -1.0f, 1.0f), //lbf

        //Back
        vec3(-1.0f, 1.0f, -1.0f), //ltb  4 
        vec3(1.0f, 1.0f, -1.0f),  //rtb  5
        vec3(1.0f, -1.0f, -1.0f), //rbb  6
        vec3(-1.0f, -1.0f, -1.0f),//lbb  7

        //Top
        vec3(-1.0f, 1.0f, -1.0f), //ltb  8  
        vec3(1.0f, 1.0f, -1.0f),  //rtb  9    
        vec3(1.0f, 1.0f, 1.0f),   //rtf 10
        vec3(-1.0f, 1.0f, 1.0f),  //ltf 11 

        //Bottom
        vec3(-1.0f, -1.0f, -1.0f),//lbb 12
        vec3(1.0f, -1.0f, -1.0f), //rbb 13
        vec3(1.0f, -1.0f, 1.0f),  //rbf 14
        vec3(-1.0f, -1.0f, 1.0f), //lbf 15

        //Left
        vec3(-1.0f, 1.0f, -1.0f), //ltb 16
        vec3(-1.0f, -1.0f, -1.0f),//lbb 17
        vec3(-1.0f, -1.0f, 1.0f), //lbf 18
        vec3(-1.0f, 1.0f, 1.0f),  //ltf 19

        //Left
        vec3(1.0f, 1.0f, -1.0f), //rtb 20
        vec3(1.0f, -1.0f, -1.0f),//rbb 21
        vec3(1.0f, -1.0f, 1.0f), //rbf 22
        vec3(1.0f, 1.0f, 1.0f),  //rtf 23
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
        //t1
            11, 8, 9,
        //t2
            9, 10, 11,
        //b1
            12, 15, 13,
        //b2
            13, 15, 14,
        //l1
            16, 19, 18,
        //l2
            18, 17, 16,
        //r1
            20, 21, 22,
        //r2
            22, 23, 20,
    };

    std::shared_ptr<float[]> array_vert(new float[24 * 3], [](const float* other) { delete[] other; });

    for (int i = 0; i < 24; i++) {
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

    calc_norms();
}

std::shared_ptr<float[]> color_cube::get_vertices() {
    return vertices;
}

std::shared_ptr<unsigned int[]> color_cube::get_indices() {
    return indices;
}

void color_cube::on_draw_ui() 
{
    ImGui::SliderFloat("x", &x, -10.0f, 10.0f);
}

const char* color_cube::window_name()
{
    return "Color cube";
}

void color_cube::on_update(double delta) 
{
    rotation_matrix = glm::mat4(1.0f);
    rotate(x, glm::vec3(1.0f, 0, 0));
}

std::shared_ptr<float[]> color_cube::get_normals() {
    return normals;
}

void color_cube::calc_norms() {

    std::shared_ptr<float[]> array_normals(new float[vertex_count * 3], [](const float* other) { delete[] other; });

    for (unsigned int i = 0; i < index_count; i += 3) {
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

unsigned int color_cube::get_start_vertex() {
    return 0;
}
unsigned int color_cube::get_vertex_count() {
    return vertex_count;
}
unsigned int color_cube::get_start_index() {
    return 0;
}
unsigned int color_cube::get_index_count() {
    return index_count;
}
#include "icosphere.h"


icosphere::icosphere(std::shared_ptr<camera> camera) : object(camera) {
    const float t = (1.f + sqrtf(5.f)) * 0.5f; 
    const float i = 1.0f/sqrtf(t * t + 1.f);  
    const float a = t * i;                    

    using glm::vec3;

    verts = {
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

    index = {
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

    faces = std::vector<face>(20);

    for (int i = 0; i < 20 * 3; i += 3) {
        faces[i/3] = face {
            { verts[index[i]], verts[index[i+1]], verts[index[i+2]] },
            { index[i], index[i+1], index[i+2] }
        };
    }

    subdivision_index_count = std::vector<unsigned int>(0);
    subdivision_vertex_count = std::vector<unsigned int>(0);

    subdivision_index_count.push_back(index.size());
    subdivision_vertex_count.push_back(verts.size());

    for (int r = 0; r < max_subdivision; r++) {
        subdivide();
        subdivision_index_count.push_back(20 * pow(4,r + 1) * 3);
        subdivision_vertex_count.push_back(verts.size());
    }

    std::shared_ptr<float[]> array_vert(new float[verts.size() * 3], [](const float* other) { delete[] other; });

    for (int i = 0; i < verts.size(); i++) {
        array_vert.get()[i * 3 + 0] = verts[i].x;
        array_vert.get()[i * 3 + 1] = verts[i].y;
        array_vert.get()[i * 3 + 2] = verts[i].z;
    }

    vertices = array_vert;

    std::shared_ptr<unsigned int[]> array_indices(new unsigned int[index.size()], [](const unsigned int* other) { delete[] other; });
    
    for (int i = 0; i < index.size(); i++) {
        array_indices.get()[i] = index[i];
    }

    indices = array_indices;

    scale_matrix = glm::scale(scale_matrix, glm::vec3(10.0f,10.0f,10.0f));
}

void icosphere::subdivide() {
    std::vector<face> new_faces;

    for (int i = 0; i < faces.size(); i++) {
        auto current_face = faces[i];
        auto first = current_face.points[0];
        auto second = current_face.points[1];
        auto third = current_face.points[2];

        auto first_second = glm::normalize((first + second) / 2.0f); 
        auto second_third = glm::normalize((second + third) / 2.0f); 
        auto third_first = glm::normalize((third + first) / 2.0f); 

        unsigned int latest_index = verts.size();
        verts.push_back(first_second);
        verts.push_back(second_third);
        verts.push_back(third_first);

        auto first_second_index = latest_index;
        auto second_third_index = latest_index + 1;
        auto third_first_index = latest_index + 2;

        //t1
        index.push_back(current_face.indicies[0]);
        index.push_back(first_second_index);
        index.push_back(third_first_index);

        new_faces.push_back(face {
            { first, first_second, third_first },
            { current_face.indicies[0], first_second_index, third_first_index }
        });

        //t2
        index.push_back(first_second_index);
        index.push_back(second_third_index);
        index.push_back(third_first_index);

        new_faces.push_back(face {
            { first_second, second_third, third_first },
            { first_second_index, second_third_index, third_first_index }
        });

        //t3
        index.push_back(third_first_index);
        index.push_back(second_third_index);
        index.push_back(current_face.indicies[2]);

        new_faces.push_back(face {
            { third_first, second_third, third },
            { third_first_index, second_third_index, current_face.indicies[2] }
        });

        //t4
        index.push_back(first_second_index);
        index.push_back(current_face.indicies[1]);
        index.push_back(second_third_index);

        new_faces.push_back(face {
            { first_second, second, second_third },
            { first_second_index, current_face.indicies[1], second_third_index }
        });
    }

    faces.clear();
    faces = new_faces;
}

std::shared_ptr<float[]> icosphere::get_vertices() {
    return vertices;
}
std::shared_ptr<unsigned int[]> icosphere::get_indices() {
    return indices;
}
std::shared_ptr<float[]> icosphere::get_normals() {
    return vertices;
}

void icosphere::on_draw_ui() {
    ImGui::Text("Debug");
    ImGui::Checkbox("Debug mesh", &debug_mesh);
    ImGui::Checkbox("Debug normals", &debug_normals);
    ImGui::Checkbox("Debug transparent", &debug_transparent);
    ImGui::ColorEdit4("Debug normal start color", debug_normal_start_color);
    ImGui::ColorEdit4("Debug normal end color", debug_normal_end_color);
    ImGui::SliderFloat("Debug normal length", &debug_normal_length, 0.0f, 1.0f);

    ImGui::Text("Lighting");
    ImGui::SliderFloat("Ambient strength", &ambient_strength, 0.0f, 1.0f);
    ImGui::SliderFloat("Specular strength", &specular_strength, 0.0f, 1.0f);
    unsigned int min = 0;
    unsigned int max = 64;
    ImGui::SliderScalar("Specular exponent",ImGuiDataType_U32 ,&specular_exponent, &min, &max);
    ImGui::SliderFloat3("Lighting direction", glm::value_ptr(light_direction), -1.0f, 1.0f);
    
    ImGui::Text("Tessellation");
    unsigned int sub_div_min = 1;
    unsigned int sub_div_max = max_subdivision;
    ImGui::SliderScalar("CPU subdivision", ImGuiDataType_U32, &subdivision, &sub_div_min, &sub_div_max);

    unsigned int tess_min = 1;
    unsigned int tess_max = max_tessellation_level;
    ImGui::SliderScalar("GPU subdivision", ImGuiDataType_U32, &tessellation_level, &tess_min, &tess_max);
    ImGui::Checkbox("Camera TLV", &camera_tlv);

    ImGui::Text("Height");
    ImGui::SliderFloat("Noise strength", &height_noise_strength, 0.0f, 1.0f);
    ImGui::SliderFloat("Noise scale", &noise_scale, 1.0f, 10.0f);
    ImGui::SliderFloat("Noise position", &noise_position, 0.0f, 1.0f);
    ImGui::SliderFloat("Normal sample distance", &normal_sample_distance, 0.0f, 0.3f);
}

const char* icosphere::window_name() {
    return "Icosphere";
}

unsigned int icosphere::get_max_index_count() {
    return subdivision_index_count.at(max_subdivision);
}

unsigned int icosphere::get_max_vertex_count() {
    return subdivision_vertex_count.at(max_subdivision);
}

unsigned int icosphere::get_start_index() {
    unsigned int start_index = 0;

    for(int i = 0; i < subdivision - 1; i++) {
        start_index += subdivision_index_count.at(i);
    }
    
    return start_index;
}

unsigned int icosphere::get_index_count() {
    return subdivision_index_count.at(subdivision - 1);
}

#include "color_cube.h"

std::shared_ptr<float[]> color_cube::get_mesh() {

    auto point1 = glm::vec3(0.0f,1.0f,0.0f);
    auto point2 = glm::vec3(1.0f,-1.0f,0.0f);
    auto point3 = glm::vec3(-1.0f,-1.0f,0.0f);

    std::shared_ptr<float[]> array(new float[9], [](const float* other) { delete[] other; });
    memcpy(array.get(), glm::value_ptr(point1), 3 * sizeof(float));
    memcpy(array.get() + 3, glm::value_ptr(point2), 3 * sizeof(float));
    memcpy(array.get() + 6, glm::value_ptr(point3), 3 * sizeof(float));

    vertex_count = 3;
    
    return array;
}
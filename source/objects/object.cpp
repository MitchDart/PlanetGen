#include "object.h"

object::object(std::shared_ptr<camera> _main_camera) {
    main_camera = _main_camera;
}

void object::on_create() {
    initilize_vao();
    program = shader_program("plain_vertex.glsl","plain_fragment.glsl");
}

void object::on_draw() {
    program.use_program();

    glm::mat4 mvp = main_camera->get_camera_matrix() * get_model_matrix();
    GLuint mvp_id = glGetUniformLocation(program.get_shader_program_handle(), "mvp");

    glUniformMatrix4fv(mvp_id, 1, GL_FALSE, glm::value_ptr(mvp));

    glBindVertexArray(vao_handle);
    glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
}

void object::on_destroy() {}

void object::on_update(double delta) {}

void object::initilize_vao() {
    auto vertices = get_vertices();
    auto indices = get_indices();
    
    glGenVertexArrays(1, &vao_handle);
    glBindVertexArray(vao_handle);

    GLuint position_vbo_handle;

    glGenBuffers(1, &position_vbo_handle);
    
    glBindBuffer(GL_ARRAY_BUFFER, position_vbo_handle);
    glBufferData(GL_ARRAY_BUFFER, vertex_count * 3 * sizeof(float), vertices.get(), GL_STATIC_DRAW);

    GLuint element_vbo_handle;

    glGenBuffers(1, &element_vbo_handle);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_vbo_handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(unsigned int), indices.get(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);  
}

void object::translate(glm::vec3 offset) {
    translation_matrix = glm::translate(translation_matrix, offset);
}

void object::rotate(float angle, glm::vec3 axis) {
    rotation_matrix = glm::rotate(rotation_matrix, angle, axis);
}

glm::mat4 object::get_model_matrix() {
    return translation_matrix * rotation_matrix * scale_matrix;
}
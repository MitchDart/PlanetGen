#include "object.h"

object::object(std::shared_ptr<camera> _main_camera) {
    main_camera = _main_camera;
}

void object::on_create() {
    initilize_vao();
    debug_mesh = shader_program("debug_mesh_vertex.glsl","debug_mesh_fragment.glsl", "debug_mesh_geometry.glsl");
    debug_normals = shader_program("debug_normal_vertex.glsl","debug_normal_fragment.glsl", "debug_normal_geometry.glsl");
}

void object::on_draw() {
    draw_debug_mesh();
    draw_debug_normals();
}

void object::draw_debug_mesh() {
    debug_mesh.use_program();

    glm::mat4 mvp = main_camera->get_camera_matrix() * get_model_matrix();
    GLuint mvp_id = glGetUniformLocation(debug_mesh.get_shader_program_handle(), "matrix_mvp");
    GLuint v_id = glGetUniformLocation(debug_mesh.get_shader_program_handle(), "matrix_v");
    GLuint m_id = glGetUniformLocation(debug_mesh.get_shader_program_handle(), "matrix_m");

    GLuint transparent = glGetUniformLocation(debug_mesh.get_shader_program_handle(), "transparent");
    glUniform1f(transparent, !debug_transparent);

    glUniformMatrix4fv(mvp_id, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniformMatrix4fv(v_id, 1, GL_FALSE, glm::value_ptr(main_camera->get_look_matrix()));
    glUniformMatrix4fv(m_id, 1, GL_FALSE, glm::value_ptr(get_model_matrix()));

    glBindVertexArray(vao_handle);

    if(debug_transparent) {
        glDisable(GL_CULL_FACE); 
        glDisable(GL_DEPTH_TEST);
    }
    
    glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
    
    glEnable(GL_CULL_FACE);  
    glEnable(GL_DEPTH_TEST);
}

void object::draw_debug_normals() {
    debug_normals.use_program();

    glm::mat4 mvp = main_camera->get_camera_matrix() * get_model_matrix();
    GLuint mvp_id = glGetUniformLocation(debug_normals.get_shader_program_handle(), "matrix_mvp");

    GLuint color = glGetUniformLocation(debug_normals.get_shader_program_handle(), "color");
    glUniform4fv(color,1,glm::value_ptr(glm::vec4(1.0f,0.0f,0.0f,1.0f)));

    GLuint normal_length = glGetUniformLocation(debug_normals.get_shader_program_handle(), "normal_length");
    glUniform1f(normal_length, 0.1f);

    glUniformMatrix4fv(mvp_id, 1, GL_FALSE, glm::value_ptr(mvp));

    glBindVertexArray(vao_handle);
    glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
}
        


void object::on_destroy() {}

void object::on_update(double delta) {}

void object::initilize_vao() {
    auto vertices = get_vertices();
    auto indices = get_indices();
    auto normals = get_normals();
    
    glGenVertexArrays(1, &vao_handle);
    glBindVertexArray(vao_handle);

    GLuint position_vbo_handle;

    glGenBuffers(1, &position_vbo_handle);
    
    glBindBuffer(GL_ARRAY_BUFFER, position_vbo_handle);
    glBufferData(GL_ARRAY_BUFFER, vertex_count * 3 * sizeof(float), vertices.get(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);  

    GLuint normal_vbo_handle;

    glGenBuffers(1, &normal_vbo_handle);

    glBindBuffer(GL_ARRAY_BUFFER, normal_vbo_handle);
    glBufferData(GL_ARRAY_BUFFER, vertex_count * 3 * sizeof(float), normals.get(), GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);  

    GLuint element_vbo_handle;

    glGenBuffers(1, &element_vbo_handle);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_vbo_handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(unsigned int), indices.get(), GL_STATIC_DRAW);

    glBindVertexArray(0);
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
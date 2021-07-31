#include "object.h"

object::object(std::shared_ptr<camera> _main_camera) {
    main_camera = _main_camera;
}

void object::on_create() {
    initilize_vao();
    initilize_shadow_map();
    debug_mesh_shader = shader_program("sphere_vertex.glsl","debug/debug_mesh_fragment.glsl", "debug/debug_mesh_geometry.glsl", "sphere_tcs.glsl",  "sphere_tes.glsl");
    debug_normals_shader = shader_program("sphere_vertex.glsl","debug/debug_normal_fragment.glsl", "debug/debug_normal_geometry.glsl", "sphere_tcs.glsl",  "sphere_tes.glsl");
    phong_shader = shader_program("sphere_vertex.glsl", "phong/phong_fragment.glsl", nullptr, "sphere_tcs.glsl",  "sphere_tes.glsl");
    shadow_shader = shader_program("shadow/shadow_vertex.glsl", "shadow/shadow_fragment.glsl", nullptr, "shadow/shadow_tcs.glsl",  "shadow/shadow_tes.glsl");
}

void object::on_draw() {
    draw_shadow_map();

    if (debug_mesh) {
        draw_debug_mesh();
    } else {
        draw_phong();
    }
    
    if (debug_normals) {
        draw_debug_normals();
    }
}


void object::draw_phong() {
    
    phong_shader.use_program();
    
    glPatchParameteri(GL_PATCH_VERTICES, 3);

    bind_uniforms(phong_shader);

    glBindVertexArray(vao_handle);

    glBindTexture(GL_TEXTURE_2D, depth_texture_handle);

    glDrawElements(GL_PATCHES, get_index_count(), GL_UNSIGNED_INT, (void*)(get_start_index() *sizeof(unsigned int)));
}


void object::draw_debug_mesh() {
    debug_mesh_shader.use_program();
    
    glPatchParameteri(GL_PATCH_VERTICES, 3);

    bind_uniforms(debug_mesh_shader);

    glBindVertexArray(vao_handle);

    if(debug_transparent) {
        glDisable(GL_CULL_FACE); 
        glDisable(GL_DEPTH_TEST);
    }
    
    glDrawElements(GL_PATCHES, get_index_count(), GL_UNSIGNED_INT,  (void*)(get_start_index() *sizeof(unsigned int)));
    
    glEnable(GL_CULL_FACE);  
    glEnable(GL_DEPTH_TEST);
}

void object::draw_debug_normals() {
    debug_normals_shader.use_program();

    glPatchParameteri(GL_PATCH_VERTICES, 3);

    bind_uniforms(debug_normals_shader);

    glBindVertexArray(vao_handle);
    glDrawElements(GL_PATCHES, get_index_count(), GL_UNSIGNED_INT,  (void*)(get_start_index() * sizeof(unsigned int)));
}

void object::bind_uniforms(shader_program shader) {

    glm::vec3 camera_position = main_camera->get_camera_position();

    glm::mat4 mvp = main_camera->get_camera_matrix() * get_model_matrix();
    GLuint mvp_id = glGetUniformLocation(shader.get_shader_program_handle(), "matrix_mvp");
    GLuint m_id = glGetUniformLocation(shader.get_shader_program_handle(), "matrix_m");
    GLuint v_id = glGetUniformLocation(shader.get_shader_program_handle(), "matrix_v");

    GLuint camera_position_handle = glGetUniformLocation(shader.get_shader_program_handle(), "camera_position");
    glUniform3fv(camera_position_handle, 1, glm::value_ptr(camera_position));

    GLuint light_direction_handle = glGetUniformLocation(shader.get_shader_program_handle(), "light_direction");
    glUniform3fv(light_direction_handle, 1, glm::value_ptr(glm::normalize(light_direction)));

    GLuint ambient_strength_handle = glGetUniformLocation(shader.get_shader_program_handle(), "ambient_strength");
    glUniform1f(ambient_strength_handle, ambient_strength);

    GLuint specular_strength_handle = glGetUniformLocation(shader.get_shader_program_handle(), "specular_strength");
    glUniform1f(specular_strength_handle, specular_strength);

    GLuint specular_exponent_handle = glGetUniformLocation(shader.get_shader_program_handle(), "specular_exponent");
    glUniform1ui(specular_exponent_handle, specular_exponent);

    GLuint diffuse_color_handle = glGetUniformLocation(shader.get_shader_program_handle(), "diffuse_color");
    glUniform4fv(diffuse_color_handle, 1, glm::value_ptr(diffuse_color));

    GLuint height_noise_strength_handle = glGetUniformLocation(shader.get_shader_program_handle(), "height_noise_strength");
    glUniform1f(height_noise_strength_handle, height_noise_strength);

    GLuint normal_length = glGetUniformLocation(shader.get_shader_program_handle(), "normal_length");
    glUniform1f(normal_length, debug_normal_length);

    GLuint normal_sample_distance_handle = glGetUniformLocation(shader.get_shader_program_handle(), "normal_sample_distance");
    glUniform1f(normal_sample_distance_handle, normal_sample_distance);

    GLuint transparent_handle = glGetUniformLocation(shader.get_shader_program_handle(), "transparent");
    glUniform1f(transparent_handle, !debug_transparent);

    GLuint debug_normal_start_color_handle = glGetUniformLocation(shader.get_shader_program_handle(), "debug_normal_start_color");
    glUniform4fv(debug_normal_start_color_handle, 1, glm::value_ptr(glm::vec4(debug_normal_start_color[0], debug_normal_start_color[1], debug_normal_start_color[2], debug_normal_start_color[3])));
    
    GLuint debug_normal_end_color_handle = glGetUniformLocation(shader.get_shader_program_handle(), "debug_normal_end_color");
    glUniform4fv(debug_normal_end_color_handle, 1, glm::value_ptr(glm::vec4(debug_normal_end_color[0], debug_normal_end_color[1], debug_normal_end_color[2], debug_normal_end_color[3])));

    GLuint tessellation_level_handle = glGetUniformLocation(shader.get_shader_program_handle(), "tessellation_level");
    glUniform1ui(tessellation_level_handle, tessellation_level);

    GLuint camera_tlv_handle = glGetUniformLocation(shader.get_shader_program_handle(), "camera_tlv");
    glUniform1i(camera_tlv_handle, camera_tlv);

    GLuint noise_scale_handle = glGetUniformLocation(shader.get_shader_program_handle(), "noise_scale");
    glUniform1f(noise_scale_handle, noise_scale);

    GLuint noise_position_handle = glGetUniformLocation(shader.get_shader_program_handle(), "noise_position");
    glUniform1f(noise_position_handle, noise_position);

    GLuint light_matrix_handle = glGetUniformLocation(shader.get_shader_program_handle(), "matrix_light");
    glUniformMatrix4fv(light_matrix_handle, 1, GL_FALSE, glm::value_ptr(light_matrix));

    GLuint shadow_bias_min_handle = glGetUniformLocation(shader.get_shader_program_handle(), "shadow_bias_min");
    glUniform1f(shadow_bias_min_handle, shadow_bias_min);

    GLuint shadow_bias_max_handle = glGetUniformLocation(shader.get_shader_program_handle(), "shadow_bias_max");
    glUniform1f(shadow_bias_max_handle, shadow_bias_max);

    glUniformMatrix4fv(mvp_id, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniformMatrix4fv(v_id, 1, GL_FALSE, glm::value_ptr(main_camera->get_look_matrix()));
    glUniformMatrix4fv(m_id, 1, GL_FALSE, glm::value_ptr(get_model_matrix()));
}
        
void object::on_destroy() {}

void object::on_update(double delta) {
    //We update the light matrix on each frame update so that the shadows are recalculated correctly
    update_light_matrix();
}

void object::initilize_vao() {
    auto vertices = get_vertices();
    auto indices = get_indices();
    auto normals = get_normals();
    
    glGenVertexArrays(1, &vao_handle);
    glBindVertexArray(vao_handle);

    GLuint position_vbo_handle;

    glGenBuffers(1, &position_vbo_handle);
    
    glBindBuffer(GL_ARRAY_BUFFER, position_vbo_handle);
    glBufferData(GL_ARRAY_BUFFER, get_max_vertex_count() * 3 * sizeof(float), vertices.get(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);  

    GLuint normal_vbo_handle;

    glGenBuffers(1, &normal_vbo_handle);

    glBindBuffer(GL_ARRAY_BUFFER, normal_vbo_handle);
    glBufferData(GL_ARRAY_BUFFER, get_max_vertex_count() * 3 * sizeof(float), normals.get(), GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);  

    GLuint element_vbo_handle;

    glGenBuffers(1, &element_vbo_handle);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_vbo_handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, get_max_index_count() * sizeof(unsigned int), indices.get(), GL_STATIC_DRAW);

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

void object::initilize_shadow_map() {
    glGenFramebuffers(1, &fbo_depth_handle);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_depth_handle);

    glGenTextures(1, &depth_texture_handle);
    glBindTexture(GL_TEXTURE_2D, depth_texture_handle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadow_map_width, shadow_map_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture_handle, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    update_light_matrix();
}

void object::update_light_matrix() {
    auto normalized_light_direction = glm::normalize(light_direction);
    auto light_look_matrix = glm::lookAt(normalized_light_direction, glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
    auto light_projection_matrix = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, -15.0f, 15.0f);
    light_matrix = light_projection_matrix * light_look_matrix;
}

void object::draw_shadow_map() {
    GLint original_viewport[4];
    glGetIntegerv(GL_VIEWPORT, original_viewport);

    glViewport(0,0,shadow_map_width, shadow_map_height);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_depth_handle);
    glClear(GL_DEPTH_BUFFER_BIT);

    shadow_shader.use_program();
    
    glPatchParameteri(GL_PATCH_VERTICES, 3);

    bind_uniforms(shadow_shader);

    glBindVertexArray(vao_handle);

    glDrawElements(GL_PATCHES, get_index_count(), GL_UNSIGNED_INT,  (void*)(get_start_index() * sizeof(unsigned int)));
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(original_viewport[0], original_viewport[1], original_viewport[2], original_viewport[3]);
}
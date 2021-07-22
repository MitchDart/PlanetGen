#ifndef _OBJECT_H
#define _OBJECT_H

#include <memory>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../common/drawable.h"
#include "../shaders/shader_program.h"
#include "../camera/camera.h"

class object : public drawable {
    public: 
        object(std::shared_ptr<camera> camera);

        void on_create();
        void on_draw();
        void on_destroy();
        virtual void on_update(double delta);

        void translate(glm::vec3);
        void rotate(float angle, glm::vec3 axis);

        virtual std::shared_ptr<float[]> get_vertices() = 0;
        virtual std::shared_ptr<unsigned int[]> get_indices() = 0;
        virtual std::shared_ptr<float[]> get_normals() = 0;
        glm::mat4 get_model_matrix();
    protected:
        virtual unsigned int get_max_index_count() = 0;
        virtual unsigned int get_max_vertex_count() = 0;
        virtual unsigned int get_start_index() = 0;
        virtual unsigned int get_index_count() = 0;
        
        std::shared_ptr<camera> main_camera;

        glm::mat4 scale_matrix = glm::mat4(1.0f);
        glm::mat4 translation_matrix = glm::mat4(1.0f);
        glm::mat4 rotation_matrix = glm::mat4(1.0f);

        glm::vec4 diffuse_color = glm::vec4(1.0f,0.0f,0.0f,1.0f);
        unsigned int specular_exponent = 2;
        float specular_strength = 0.4f;
        float ambient_strength = 0.3f;
        glm::vec3 light_direction = glm::vec3(1.0f,0.3f,0.4f);

        bool debug_mesh = false;
        bool debug_normals = false;
        bool debug_transparent = false;

        float debug_normal_color[4] = {1.0f, 0, 0, 1.0f};
        float debug_normal_color_end[4] = {0, 1.0f, 1.0f, 1.0f};
        float debug_normal_length = 0.03f;

        float height_noise_strength = 0.2f;

        float normal_sample_distance = 0.1f;
    private:
        void initilize_vao(); 

        void draw_phong();
        void draw_debug_mesh();
        void draw_debug_normals();
        
        GLuint vao_handle;

        shader_program phong_shader;
        shader_program debug_mesh_shader;
        shader_program debug_normals_shader;
};

#endif


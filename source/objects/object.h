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
        int vertex_count;
        int index_count;
        std::shared_ptr<camera> main_camera;

        glm::mat4 scale_matrix = glm::mat4(1.0f);
        glm::mat4 translation_matrix = glm::mat4(1.0f);
        glm::mat4 rotation_matrix = glm::mat4(1.0f);
    private:
        void initilize_vao(); 
        
        GLuint vao_handle;
        shader_program program;

};

#endif
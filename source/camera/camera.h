#ifndef _CAMERA_H
#define _CAMERA_H

#define _USE_MATH_DEFINES

#include <memory>
#include <iostream>

#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "../common/drawable.h"
#include "../common/inputable.h"
#include "../common/windowable.h"
#include "../ui/ui.h"

class camera : public drawable, public inputable, public windowable {
    public:
        void on_create();
        void on_draw();
        void on_destroy();
        void on_update(double delta);

        void on_draw_ui();
        const char* window_name();
        
        void on_input_init(GLFWwindow*);
        void on_scroll(double,double);

        glm::mat4 get_camera_matrix();
        void look_at(glm::vec3, glm::vec3, glm::vec3);
        void update_window_size(int, int);
    protected:
    private:
        GLFWwindow* window;
        glm::mat4 look_matrix = glm::mat4(1.0f);

        int width = 1.0f;
        int height = 1.0f;
        float fov = glm::radians(60.0f);
        float near_clip = 0.1f;
        float far_clip = 100.0f;

        float orbit_start_angle_x = 0.0f;
        float orbit_angle_x = 0.01f;
        float orbit_start_angle_y = 0.0f;
        float orbit_angle_y = 0.01f;
        
        float orbit_speed = 1.0f;

        float zoom = -10.0f;
        float zoom_speed = 1.0f;

        bool is_panning = false;
        double pan_start_mouse_x, pan_start_mouse_y;
};

#endif
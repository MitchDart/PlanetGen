#include "camera.h"

glm::mat4 camera::get_camera_matrix() {
    glm::mat4 projection = glm::perspective(fov, (float) width / (float)height, near_clip, far_clip);

    return projection * look_matrix;
}

glm::mat4 camera::get_look_matrix() {
    return look_matrix;
}

void camera::update_window_size(int _width, int _height) {
    width = _width;
    height = _height;
}

void camera::on_create() {
    camera_position = glm::vec3(0.0f,0.0f,-zoom);
}

void camera::on_draw() {}

void camera::on_update(double delta) {
    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (state == GLFW_PRESS && !ImGui::GetIO().WantCaptureMouse)
    {
        //Is clicking
        //get x distance from start of mouse click
        if(!is_panning) {
            is_panning = true;
            glfwGetCursorPos(window, &pan_start_mouse_x, &pan_start_mouse_y);
            orbit_start_angle_x = orbit_angle_x;
            orbit_start_angle_y = orbit_angle_y;
        }

        double mouse_x, mouse_y;
        glfwGetCursorPos(window, &mouse_x, &mouse_y);
        
        double diff_mouse_x = mouse_x - pan_start_mouse_x;
        double diff_mouse_y = mouse_y - pan_start_mouse_y;

        //Get movement as ratio of screen width/height
        diff_mouse_x = diff_mouse_x/width;
        diff_mouse_y = diff_mouse_y/width;

        orbit_angle_x = orbit_start_angle_x + (2.0f * M_PI * diff_mouse_x * orbit_speed);
        orbit_angle_y = std::fmin(M_PI/2.0f - 0.1f, std::fmax(-M_PI/2.0f + 0.1f ,orbit_start_angle_y + (2.0f * M_PI * diff_mouse_y * orbit_speed)));
    } else {
        is_panning = false;
    }

    auto x_rotation_matrix = glm::rotate(glm::mat4(1.0f), orbit_angle_x, glm::vec3(0.0f,1.0f,0.0f));

    auto y_rotation_axis = glm::vec4(1.0f,0.0f,0.0f,0.0f) * x_rotation_matrix;
    auto y_rotation_matrix = glm::rotate(glm::mat4(1.0f), orbit_angle_y, glm::vec3(y_rotation_axis.x, y_rotation_axis.y, y_rotation_axis.z));

    camera_position = glm::vec4(0.0f,0.0f,-zoom,0.0f) * x_rotation_matrix * y_rotation_matrix;

    look_matrix = glm::lookAt(camera_position, glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
}

glm::vec3 camera::get_camera_position() {
    return camera_position;
}

void camera::on_destroy() {}

const char* camera::window_name() {
    return "Camera";
}

void camera::on_draw_ui() {
    ImGui::Text("Camera");
    ImGui::SliderAngle("FOV", &fov, 0.0f, 100.0f);
    ImGui::SliderFloat("Orbit speed", &orbit_speed, 0.0f, 5.0f);
    ImGui::SliderFloat("Near clip", &near_clip, 0.0f, 10.0f);
    ImGui::SliderFloat("Far clip", &far_clip, 10.0f, 100.0f);
    ImGui::SliderFloat("Zoom speed", &zoom_speed, 0.0f, 5.0f);

}


void camera::on_input_init(GLFWwindow* _window) {
    window = _window;
}

void camera::on_scroll(double x ,double y) {
    zoom = std::min(zoom + ((float)y*zoom_speed), -1.0f);
}
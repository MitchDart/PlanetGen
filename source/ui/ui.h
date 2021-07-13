#ifndef _UI_H
#define _UI_H

#include <memory>
#include <vector>

#include "../common/windowable.h"
#include "../common/drawable.h"
#include "../deps/imgui/imgui_impl_glfw.h"
#include "../deps/imgui/imgui_impl_opengl3.h"

class ui : public drawable {
    public:
        ui(GLFWwindow* window, std::vector<std::shared_ptr<windowable>> drawables);

        void on_create();
        void on_draw();
        void on_destroy();
        void on_update(double delta);
    protected:
    private:
        std::vector<std::shared_ptr<windowable>> windowables;
        GLFWwindow* window;
        
        bool showDemo = false;
        bool showDebug = false;
};

#endif
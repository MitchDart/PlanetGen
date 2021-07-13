#ifndef _MODULES_H
#define _MODULES_H

#include <vector>
#include <memory>
#include <GLFW/glfw3.h>

#include "common/drawable.h"
#include "common/inputable.h"
#include "objects/color_cube/color_cube.h"
#include "camera/camera.h"
#include "ui/ui.h"

class modules
{
    public:
        static modules& getInstance()
        {
            static modules instance;
            return instance;
        }

        //----------WINDOW------------
        GLFWwindow* main_window;

        //----------CAMERA------------
        std::shared_ptr<camera> main_camera;

        //----------DRAWABLES----------
        std::vector<std::shared_ptr<drawable>> drawables;

        //----------INPUTABLES---------
        std::vector<std::shared_ptr<inputable>> inputables;

        //----------WINDOWABLES---------
        std::vector<std::shared_ptr<windowable>> windowables;

    private:
        modules() {}
    public:
        modules(modules const&) = delete;
        void operator=(modules const&) = delete;
        void resolve(GLFWwindow* window) {
            main_window = window;
            main_camera = std::make_shared<camera>();
            windowables = {
                main_camera
            };
            drawables = {
                //-------CAMERA-------
                main_camera,
                
                //-------Objects-------
                std::make_shared<color_cube>(main_camera),
                
                //-------UI-------
                std::make_shared<ui>(window, windowables)
            };
            inputables = {
                main_camera
            };
        }
        void destroy() {
            main_camera.reset();
        }
};

#endif
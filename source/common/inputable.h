#ifndef _SCROLLABLE_H
#define _SCROLLABLE_H

#include <GLFW/glfw3.h>

class inputable {
    public:
        virtual void on_input_init(GLFWwindow* window) = 0;
        virtual void on_scroll(double x, double y) = 0;
};

#endif
#ifndef _WINDOWABLE_H
#define _WINDOWABLE_H

#include <GLFW/glfw3.h>

class windowable {
    public:
        virtual void on_draw_ui() = 0;
        virtual const char* window_name() = 0;
};

#endif
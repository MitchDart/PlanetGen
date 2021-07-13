#ifndef _DRAWABLE_H
#define _DRAWABLE_H

class drawable {
    public:
        virtual void on_create() = 0;
        virtual void on_update(double delta) = 0;
        virtual void on_draw() = 0;
        virtual void on_destroy() = 0;
};

#endif
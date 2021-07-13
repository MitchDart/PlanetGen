#ifndef _COLOR_CUBE_H
#define _COLOR_CUBE_H

#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../object.h"

class color_cube : public object {
    public:
        color_cube(std::shared_ptr<camera> camera) : object(camera) {};
        std::shared_ptr<float[]> get_mesh();
    protected:
    private:
};

#endif
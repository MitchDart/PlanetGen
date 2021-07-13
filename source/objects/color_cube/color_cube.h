#ifndef _COLOR_CUBE_H
#define _COLOR_CUBE_H

#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../object.h"

class color_cube : public object {
    public:
        color_cube(std::shared_ptr<camera> camera);
        std::shared_ptr<float[]> get_vertices();
        std::shared_ptr<unsigned int[]> get_indices();
    protected:
    private:
        std::shared_ptr<float[]> vertices;
        std::shared_ptr<unsigned int[]> indices;
};

#endif
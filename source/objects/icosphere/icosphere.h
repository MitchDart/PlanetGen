#ifndef _ICOSPHERE_H
#define _ICOSPHERE_H

#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../object.h"

class icosphere : public object {
    public:
        icosphere(std::shared_ptr<camera> camera) : object(camera) {};
        std::shared_ptr<float[]> get_vertices();
        std::shared_ptr<unsigned int[]> get_indices();
    protected:
    private:
};

#endif
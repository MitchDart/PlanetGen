#ifndef _ICOSPHERE_H
#define _ICOSPHERE_H

#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../object.h"

class icosphere : public object {
    public:
        icosphere(std::shared_ptr<camera> camera);
        std::shared_ptr<float[]> get_vertices();
        std::shared_ptr<unsigned int[]> get_indices();
        std::shared_ptr<float[]> get_normals();
    protected:
    private:
        std::shared_ptr<float[]> vertices;
        std::shared_ptr<unsigned int[]> indices;
        std::shared_ptr<float[]> normals;

        void calc_norms();
};

#endif
#ifndef _ICOSPHERE_H
#define _ICOSPHERE_H

#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../object.h"

struct face {
    glm::vec3 points[3];
    unsigned int indicies[3];
};


class icosphere : public object, public windowable {
    public:
        icosphere(std::shared_ptr<camera> camera);
        std::shared_ptr<float[]> get_vertices();
        std::shared_ptr<unsigned int[]> get_indices();
        std::shared_ptr<float[]> get_normals();
        
        unsigned int get_max_index_count();
        unsigned int get_max_vertex_count();
        unsigned int get_start_index();
        unsigned int get_index_count();

        void on_draw_ui();
        const char* window_name();
    protected:
    private:
        std::shared_ptr<float[]> vertices;
        std::shared_ptr<unsigned int[]> indices;

        std::vector<glm::vec3> verts;
        std::vector<unsigned int> index;
        std::vector<face> faces;

        unsigned int subdivision = 2;
        unsigned int max_subdivision = 8;

        std::vector<unsigned int> subdivision_vertex_count;
        std::vector<unsigned int> subdivision_index_count;

        void subdivide();
};


#endif

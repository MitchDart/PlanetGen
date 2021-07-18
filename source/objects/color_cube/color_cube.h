#ifndef _COLOR_CUBE_H
#define _COLOR_CUBE_H

#include <memory>
#include <vector>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../object.h"

class color_cube : public object, public windowable {
    public:
        color_cube(std::shared_ptr<camera> camera);
        std::shared_ptr<float[]> get_vertices();
        std::shared_ptr<unsigned int[]> get_indices();
        std::shared_ptr<float[]> get_normals();
        void on_draw_ui();
        const char* window_name();
        void on_update(double delta) override;
    protected:
        unsigned int get_start_vertex();
        unsigned int get_vertex_count();
        unsigned int get_start_index();
        unsigned int get_index_count();
    private:
        unsigned int vertex_count = 24;
        unsigned int index_count = 36;

        std::shared_ptr<float[]> vertices;
        std::shared_ptr<unsigned int[]> indices;
        std::shared_ptr<float[]> normals;
        float x = 0;

        void calc_norms();
};

#endif
#include "color_cube.h"

std::shared_ptr<float[]> color_cube::get_mesh() {

    using glm::vec3;

    vec3 ftl(-1.0f, 1.0f, 1.0f);
    vec3 ftr(1.0f, 1.0f, 1.0f);
    
    vec3 fbr(1.0f, -1.0f, 1.0f);
    vec3 fbl(-1.0f, -1.0f, 1.0f);

    vec3 btl(-1.0f, 1.0f, -1.0f);
    vec3 btr(1.0f, 1.0f, -1.0f);
    
    vec3 bbr(1.0f, -1.0f, -1.0f);
    vec3 bbl(-1.0f, -1.0f, -1.0f);

    vec3 cube[36] = {
        //f1
            ftl, ftr, fbl,
        //f2
            fbl, ftr, fbr,
        //b1
            btl, bbl, btr,
        //b2
            btr, bbl, bbr,
        //r1
            ftr, btr, bbr,
        //r2
            bbr, fbr, ftr,
        //l1
            btl, ftl, bbl,
        //l2
            bbl, ftl, fbl,
        //t1
            ftr, ftl, btr,
        //t2
            btr, ftl, btl,
        //b1
            fbl, fbr, bbr,
        //b2
            bbr, bbl, fbl,
    };

    std::shared_ptr<float[]> array(new float[108], [](const float* other) { delete[] other; });

    for (int i = 0; i < 36; i++) {
        array.get()[i * 3 + 0] = cube[i].x;
        array.get()[i * 3 + 1] = cube[i].y;
        array.get()[i * 3 + 2] = cube[i].z;
    }
    
    vertex_count = 36;
    
    return array;
}
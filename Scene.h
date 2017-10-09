//
// Created by Adam on 2017-10-08.
//

#ifndef RENDERER_MIASGK_SCENE_H
#define RENDERER_MIASGK_SCENE_H


#include <list>
#include "Triangle.h"
#include "Rasterizer.h"

using namespace std;

class Scene {

public:
    list<Triangle *> triangles;

    void render(Rasterizer *pRasterizer);

    void add(Triangle *triangle);

    void
    setColorFromTriangle(unsigned char *colorBuffer, Color color, int index) const;
};

#endif //RENDERER_MIASGK_SCENE_H

//
// Created by Adam on 2017-10-08.
//

#ifndef RENDERER_MIASGK_SCENE_H
#define RENDERER_MIASGK_SCENE_H


#include <list>
#include "Triangle.h"

using namespace std;

class Scene {

public:
    explicit Scene(list <Triangle> triangles);

    list <Triangle> triangles;
};

#endif //RENDERER_MIASGK_SCENE_H

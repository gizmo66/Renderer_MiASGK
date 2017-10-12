//
// Created by Adam on 2017-10-08.
//

#ifndef RENDERER_MIASGK_SCENE_H
#define RENDERER_MIASGK_SCENE_H


#include <list>
#include <vector>
#include "Triangle.h"

using namespace std;

class Scene {

public:
    explicit Scene(vector<Triangle> triangles);

    vector<Triangle> triangles;
};

#endif //RENDERER_MIASGK_SCENE_H

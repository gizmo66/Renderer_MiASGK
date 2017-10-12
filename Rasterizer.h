//
// Created by Adam on 2017-10-08.
//

#ifndef RENDERER_MIASGK_RASTERIZER_H
#define RENDERER_MIASGK_RASTERIZER_H

#include <iostream>
#include "Rasterizer.h"
#include "ModelLoader.h"
#include "Scene.h"
#include "Renderer.h"

class Rasterizer {

public:
    void render();

    vector<Triangle> getTriangles(Model3D &model3D) const;

    void openFile(const char *fileName);
};


#endif //RENDERER_MIASGK_RASTERIZER_H

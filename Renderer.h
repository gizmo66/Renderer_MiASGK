//
// Created by Adam on 2017-10-11.
//

#ifndef RENDERER_MIASGK_RENDERER_H
#define RENDERER_MIASGK_RENDERER_H

#include <map>
#include <iostream>
#include "Scene.h"

class Renderer {
private:
    Scene *scene;
    int imageWidth = 100;
    int imageHeight = 100;
    double width_2;
public:
    virtual ~Renderer();

    double render(unsigned char *colorBuffer);

    void setImageSize(int imageWidth, int imageHeight);

    explicit Renderer(Scene *scene);

    void setColor(const Color &color, int index, unsigned char *colorBuffer, double intensity) const;
};


#endif //RENDERER_MIASGK_RENDERER_H

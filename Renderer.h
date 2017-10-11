//
// Created by Adam on 2017-10-11.
//

#ifndef RENDERER_MIASGK_RENDERER_H
#define RENDERER_MIASGK_RENDERER_H

#include <map>
#include <iostream>
#include "Scene.h"
#include "ColorBuffer.h"

class Renderer {
private:
    Scene *scene;
    ColorBuffer colorBuffer{};
    int imageWidth = 100;
    int imageHeight = 100;
public:
    virtual ~Renderer();

    void initColorBuffer();

    void render();

    void setImageSize(int imageWidth, int imageHeight);

    void setColor(unsigned char *colorBuffer, Color color, int index) const;

    explicit Renderer(Scene *scene);

    void saveImageToTga(const char *fileName);
};


#endif //RENDERER_MIASGK_RENDERER_H

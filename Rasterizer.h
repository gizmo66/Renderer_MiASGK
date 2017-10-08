//
// Created by Adam on 2017-10-08.
//

#ifndef RENDERER_MIASGK_RASTERIZER_H
#define RENDERER_MIASGK_RASTERIZER_H


#include "ColorBuffer.h"

class Rasterizer {

public:
    void render();

    void saveToTga();

    unsigned char *getColorBuffer();

    int getImageWidth() const;

    int getImageHeight() const;

private:
    ColorBuffer colorBuffer{};
    int imageWidth = 100;
    int imageHeight = 100;

    void setImageSize(int imageWidth, int imageHeight);

    void initColorBuffer();
};


#endif //RENDERER_MIASGK_RASTERIZER_H

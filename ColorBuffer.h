//
// Created by Adam on 2017-10-08.
//

#ifndef RENDERER_MIASGK_COLORBUFFER_H
#define RENDERER_MIASGK_COLORBUFFER_H


#include "Color.h"

class ColorBuffer {

public:
    void init(int width, int height);

    unsigned char *getBuffer() const;

private:
    unsigned char *buffer;
};


#endif //RENDERER_MIASGK_COLORBUFFER_H

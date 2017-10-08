//
// Created by Adam on 2017-10-08.
//

#include "ColorBuffer.h"

void ColorBuffer::init(int w, int h) {
    buffer = new unsigned char[4 * w * h];
}

unsigned char *ColorBuffer::getBuffer() const {
    return buffer;
}

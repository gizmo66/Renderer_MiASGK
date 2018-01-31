//
// Created by Adam on 2017-10-08.
//

#include "Color.h"

Color::Color(unsigned char r, unsigned char g, unsigned char b) {
    this->r = r;
    this->g = g;
    this->b = b;
}

Color::Color(Color *color) {
    this->r = color->r;
    this->g = color->g;
    this->b = color->b;
}

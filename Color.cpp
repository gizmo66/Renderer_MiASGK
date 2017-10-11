//
// Created by Adam on 2017-10-08.
//

#include "Color.h"

const Color Color::Black(0, 0, 0);
const Color Color::Red(255, 0, 0);
const Color Color::Green(0, 255, 0);
const Color Color::LightBlue(0, 255, 255);

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

//
// Created by Adam on 2017-10-08.
//

#include "Color.h"

const Color Color::Black(0, 0, 0);
const Color Color::Red(255, 0, 0);
const Color Color::Green(0, 255, 0);

Color::Color(int r, int g, int b) {
    this->r = r;
    this->g = g;
    this->b = b;
}

Color::Color(Color *color) {
    this->r = color->r;
    this->g = color->g;
    this->b = color->b;
}

Color::Color() {
    Color(Color::Red);
}

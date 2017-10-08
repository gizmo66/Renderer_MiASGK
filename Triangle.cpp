//
// Created by Adam on 2017-10-08.
//

#include "Triangle.h"

Triangle::Triangle(Point3 *a, Point3 *b, Point3 *c) {
    this->a = a;
    this->b = b;
    this->c = c;
}

const Color &Triangle::getColor() const {
    return color;
}

void Triangle::setColor(const Color &color) {
    Triangle::color = color;
}

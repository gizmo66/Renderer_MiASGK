//
// Created by Adam on 2017-10-08.
//

#include "Triangle.h"

Triangle::Triangle(Vector3 a, Vector3 b, Vector3 c, Vector3 normalVector, Color color, bool isLightMarker) : color(color) {
    this->color = color;
    this->a = a;
    this->b = b;
    this->c = c;
    this->normalVector = normalVector;
    this->isLightMarker = isLightMarker;
}

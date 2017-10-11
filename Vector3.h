//
// Created by Adam on 2017-10-11.
//

#ifndef RENDERER_MIASGK_VECTOR3_H
#define RENDERER_MIASGK_VECTOR3_H

#include <cmath>

class Vector3 {
public:
    float x, y, z;

    static Vector3 vectorFromTwoPoints(Vector3 point1, Vector3 point2);

    Vector3 add(Vector3 vector2);

    Vector3 divideByNumber(float number);

    Vector3 multiply(Vector3 vector2);

    Vector3 normalize();
};

#endif //RENDERER_MIASGK_VECTOR3_H
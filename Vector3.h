//
// Created by Adam on 2017-10-11.
//

#ifndef RENDERER_MIASGK_VECTOR3_H
#define RENDERER_MIASGK_VECTOR3_H

#include "Vector3.h"

class Vector3 {
public:
    Vector3();

    Vector3(double x, double y, double z);

    double x, y, z;

    static Vector3 vectorFromTwoPoints(Vector3 point1, Vector3 point2);

    Vector3 add(Vector3 vector2);

    Vector3 divideByNumber(double number);

    Vector3 multiply(Vector3 vector2);

    Vector3 normalize();

    static double dot(double x1, double y1, double z1, Vector3 vector);

    static Vector3 cross(Vector3 vector1, Vector3 vector2);

    static Vector3 subtract(Vector3 point1, Vector3 point2);
};

#endif //RENDERER_MIASGK_VECTOR3_H
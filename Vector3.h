//
// Created by Adam on 2017-10-11.
//

#ifndef RENDERER_MIASGK_VECTOR3_H
#define RENDERER_MIASGK_VECTOR3_H

class Vector3 {
public:
    Vector3();

    Vector3(float x, float y, float z);

    float x, y, z;

    static Vector3 vectorFromTwoPoints(Vector3 point1, Vector3 point2);

    Vector3 add(Vector3 vector2);

    Vector3 divideByNumber(float number);

    Vector3 multiply(Vector3 vector2);

    Vector3 normalize();

    static float dot(float x1, float y1, float z1, Vector3 vector);

    static Vector3 cross(Vector3 vector1, Vector3 vector2);

    static Vector3 subtract(Vector3 point1, Vector3 point2);
};

#endif //RENDERER_MIASGK_VECTOR3_H
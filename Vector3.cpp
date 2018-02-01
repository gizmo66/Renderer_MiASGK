//
// Created by Adam on 2017-10-11.
//

#include "Vector3.h"
#include "Math.h"

Vector3::Vector3() = default;

Vector3::Vector3(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3 Vector3::vectorFromTwoPoints(Vector3 point1, Vector3 point2) {
    Vector3 vector{};
    vector.x = point1.x - point2.x;
    vector.y = point1.y - point2.y;
    vector.z = point1.z - point2.z;
    return vector;
}

Vector3 Vector3::add(Vector3 vector2) {
    Vector3 result{};
    result.x = vector2.x + this->x;
    result.y = vector2.y + this->y;
    result.z = vector2.z + this->z;
    return result;
}

Vector3 Vector3::divideByNumber(float number) {
    Vector3 result{};
    result.x = this->x / number;
    result.y = this->y / number;
    result.z = this->z / number;
    return result;
}

Vector3 Vector3::multiply(Vector3 vector2) {
    Vector3 result{};
    result.x = ((this->y * vector2.z) - (this->z * vector2.y));
    result.y = ((this->z * vector2.x) - (this->x * vector2.z));
    result.z = ((this->x * vector2.y) - (this->y * vector2.x));
    return result;
}

Vector3 Vector3::normalize() {
    double size;
    size = Math::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
    Vector3 result = *this;
    result.x /= (float) size;
    result.y /= (float) size;
    result.z /= (float) size;
    return result;
}

float Vector3::dot(float x1, float y1, float z1, Vector3 vector) {
    return x1 * vector.x + y1 * vector.y + z1 * vector.z;
}

Vector3 Vector3::cross(Vector3 vector1, Vector3 vector2) {
    return {vector1.y * vector2.z - vector1.z * vector2.y, vector1.z * vector2.x - vector1.x * vector2.z,
            vector1.x * vector2.y - vector1.y * vector2.x};
}

Vector3 Vector3::subtract(Vector3 point1, Vector3 point2) {
    return {point1.x - point2.x, point1.y - point2.y, point1.z - point2.z};
}

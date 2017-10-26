//
// Created by Adam on 2017-10-08.
//

#ifndef RENDERER_MIASGK_TRIANGLE_H
#define RENDERER_MIASGK_TRIANGLE_H


#include "Vector3.h"
#include "Color.h"
#include "Vector3.h"

class Triangle {

public:
    Triangle(Vector3 a, Vector3 b, Vector3 c, Vector3 normalVector);

    const Color &getColor() const;

    void setColor(const Color &color);

    Vector3 a;
    Vector3 b;
    Vector3 c;
    Vector3 normalVector;
private:
    Color color = Color::Red;
};


#endif //RENDERER_MIASGK_TRIANGLE_H

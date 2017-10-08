//
// Created by Adam on 2017-10-08.
//

#ifndef RENDERER_MIASGK_TRIANGLE_H
#define RENDERER_MIASGK_TRIANGLE_H


#include "Point3.h"
#include "Color.h"

class Triangle {

public:
    Triangle(Point3 *a, Point3 *b, Point3 *c);

    const Color &getColor() const;

    void setColor(const Color &color);

    Point3 *a;
    Point3 *b;
    Point3 *c;
private:
    Color color = Color::Red;
};


#endif //RENDERER_MIASGK_TRIANGLE_H

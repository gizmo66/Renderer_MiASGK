//
// Created by Adam on 2017-10-08.
//

#ifndef RENDERER_MIASGK_ENTITY_H
#define RENDERER_MIASGK_ENTITY_H

#include <list>
#include "Color.h"

using namespace std;

class Entity {
public:
    const Color &getColor() const;

    void setColor(const Color &color);

private:
    Color color;
};


#endif //RENDERER_MIASGK_ENTITY_H

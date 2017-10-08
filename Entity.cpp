//
// Created by Adam on 2017-10-08.
//

#include "Entity.h"

const Color &Entity::getColor() const {
    return color;
}

void Entity::setColor(const Color &color) {
    Entity::color = color;
}
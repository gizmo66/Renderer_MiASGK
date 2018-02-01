//
// Created by Adam on 01.02.2018.
//

#ifndef RASTERYZER_LIGHT_H
#define RASTERYZER_LIGHT_H


#include "Vector3.h"

class Light {

public:

    explicit Light(Vector3 position);

    Vector3 position;
};


#endif //RASTERYZER_LIGHT_H

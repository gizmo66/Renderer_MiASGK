//
// Created by Adam on 2017-10-08.
//

#include <map>
#include "Scene.h"

Scene::Scene(list<Triangle> triangles) : triangles(triangles) {
    this->triangles = triangles;
}

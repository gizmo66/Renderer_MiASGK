//
// Created by Adam on 2017-10-08.
//

#include "Scene.h"

void Scene::add(Triangle *triangle) {
    this->triangles.push_back(triangle);
}

void Scene::render(Rasterizer *pRasterizer) {

    int width = pRasterizer->getImageWidth();
    int height = pRasterizer->getImageHeight();
    unsigned char *colorBuffer = pRasterizer->getColorBuffer();

    for (auto triangle : triangles) {

        float width_2 = width * .5f;

        float x1 = (triangle->a->x + 1) * width_2;
        float y1 = (triangle->a->y + 1) * width_2;

        float x2 = (triangle->b->x + 1) * width_2;
        float y2 = (triangle->b->y + 1) * width_2;

        float x3 = (triangle->c->x + 1) * width_2;
        float y3 = (triangle->c->y + 1) * width_2;

        float dx12 = x1 - x2;
        float dx23 = x2 - x3;
        float dx31 = x3 - x1;
        float dy12 = y1 - y2;
        float dy23 = y2 - y3;
        float dy31 = y3 - y1;

        Color color = triangle->getColor();

        //TODO akolodziejek: optymalizacja1 - przeszukiwanie
        //TODO akolodziejek: culling
        //TODO akolodziejek: obcinanie
        //TODO akolodziejek: interpolacja
        //TODO akolodziejek: bufor głębokości
        //TODO akolodziejek: krawędzie top left
        for (int x = 0; x < width; x++) {

            float dy12_dx01 = dy12 * (x - x1);
            float dy23_dx02 = dy23 * (x - x2);
            float dy31_dx03 = dy31 * (x - x3);

            for (int y = 0; y < height; y++) {
                bool isPointInsideTriangle = (dx12) * (y - y1) - dy12_dx01 > 0 && (dx23) * (y - y2) - dy23_dx02 > 0 &&
                                             (dx31) * (y - y3) - dy31_dx03 > 0;
                if (isPointInsideTriangle) {
                    int index = 4 * (x * height + y);
                    setColorFromTriangle(colorBuffer, color, index);
                }
            }
        }
    }
}

void Scene::setColorFromTriangle(unsigned char *colorBuffer, Color color, int index) const {
    //.tga format:
    //The 4 byte entry contains 1 byte for blue, 1 byte
    //for green, 1 byte for red, and 1 byte of attribute
    //information, in that order. -> BGRA

    colorBuffer[index] = color.b;
    colorBuffer[index + 1] = color.g;
    colorBuffer[index + 2] = color.r;
    colorBuffer[index + 3] = 255;
}

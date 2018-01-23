//
// Created by Adam on 2017-10-11.
//

#include <chrono>
#include "Renderer.h"

chrono::duration<double> elapsedSeconds;
int rendered = 0, index;
float x1, y1, z1, x2, y2, z2, x3, y3, z3, dx12, dx13, dx23, dx31, dx32, dy12, dy13, dy23, dy31, dy12_dx01, dy23_dx02,
        dy31_dx03, dy23_dx03, lambda1, lambda2, lambda3, depth, minX, minY, maxX, maxY;
map<int, float>::iterator i;

Renderer::Renderer(Scene *scene) {
    this->scene = scene;
}

void Renderer::setImageSize(int imageWidth, int imageHeight) {
    this->imageWidth = imageWidth;
    this->imageHeight = imageHeight;
    this->width_2 = imageWidth * .5f;
}

double Renderer::render(unsigned char *colorBuffer) {
    //cout << "Rendering started" << endl;
    auto start = chrono::system_clock::now();

    map<int, float> pixelsDepthMap;
    for (const auto &triangle : scene->triangles) {

        x1 = (triangle.a.y + 1) * width_2;
        y1 = (triangle.a.x + 1) * width_2;
        z1 = (triangle.a.z + 1) * width_2;

        if (Vector3::dot(x1, y1, z1, triangle.normalVector) >= 0) {

            x2 = (triangle.b.y + 1) * width_2;
            y2 = (triangle.b.x + 1) * width_2;
            z2 = (triangle.b.z + 1) * width_2;

            x3 = (triangle.c.y + 1) * width_2;
            y3 = (triangle.c.x + 1) * width_2;
            z3 = (triangle.c.z + 1) * width_2;

            dx12 = x1 - x2;
            dx13 = x1 - x3;
            dx23 = x2 - x3;
            dx31 = x3 - x1;
            dx32 = x3 - x2;
            dy12 = y1 - y2;
            dy13 = y1 - y3;
            dy23 = y2 - y3;
            dy31 = y3 - y1;

            //TODO akolodziejek: interpolacja koloru
            //TODO akolodziejek: oświetlenie w wierzchołkach

            minX = std::min(std::min(x1, x2), x3);
            maxX = std::max(std::max(x1, x2), x3);

            minX = std::max(minX, 0.0f);
            maxX = std::min(maxX, imageWidth - 1.0f);

            for (int x = minX; x < maxX; x++) {

                dy12_dx01 = dy12 * (x - x1);
                dy23_dx02 = dy23 * (x - x2);
                dy31_dx03 = dy31 * (x - x3);
                dy23_dx03 = dy23 * (x - x3);

                minY = std::min(std::min(y1, y2), y3);
                maxY = std::max(std::max(y1, y2), y3);

                minY = std::max(minY, 0.0f);
                maxY = std::min(maxY, imageHeight - 1.0f);

                for (int y = minY; y < maxY; y++) {
                    if (dx12 * (y - y1) - dy12_dx01 > 0 && dx23 * (y - y2) - dy23_dx02 > 0 &&
                        dx31 * (y - y3) - dy31_dx03 > 0) {
                        index = 3 * (x * imageHeight + y);
                        lambda1 = (dy23_dx03 + dx32 * (y - y3)) / (dy23 * dx13 + dx32 * dy13);
                        lambda2 = (dy31_dx03 + dx13 * y - y3) / (dy31 * dx23 + dx13 * dy23);
                        lambda3 = 1 - lambda1 - lambda2;
                        i = pixelsDepthMap.find(index);
                        if (i == pixelsDepthMap.end() || i->second < lambda1 * z1 + lambda2 * z2 + lambda3 * z3) {
                            pixelsDepthMap[index] = depth;
                            setColor(triangle.getColor(), index, colorBuffer);
                        }
                    }
                }
            }
            rendered++;
        }
    }

    auto end = chrono::system_clock::now();
    elapsedSeconds = end - start;

    //cout << "Elapsed time: " << elapsed_seconds.count() << "s\n";
    //cout << "Rendered triangles: " << rendered << endl;
    return elapsedSeconds.count();
}

void Renderer::setColor(const Color &color, int index, unsigned char *colorBuffer) const {
    colorBuffer[index] = color.b;
    colorBuffer[index + 1] = color.g;
    colorBuffer[index + 2] = color.r;
}

Renderer::~Renderer() {
    delete scene;
}

//
// Created by Adam on 2017-10-11.
//

#include <chrono>
#include <cmath>
#include "Renderer.h"
#include "Math.h"

chrono::duration<double> elapsedSeconds;
int rendered = 0, index;
float tx1, ty1, tz1, tx2, ty2, tz2, tx3, ty3, tz3, dx12, dx13, dx23, dx31, dx32, dy12, dy13, dy23, dy31, dy12_dx01, dy23_dx02,
        dy31_dx03, dy23_dx03, lambda1, lambda2, lambda3, depth, minX, minY, maxX, maxY;
map<int, float>::iterator i;
float lightX = 100, lightY = 100, lightZ = 100, tCenterX, tCenterY, tCenterZ, distanceFromLight, intensity;

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

        tx1 = (triangle.a.y + 1) * width_2;
        ty1 = (triangle.a.x + 1) * width_2;
        tz1 = (triangle.a.z + 1) * width_2;

        if (Vector3::dot(tx1, ty1, tz1, triangle.normalVector) >= 0) {

            tx2 = (triangle.b.y + 1) * width_2;
            ty2 = (triangle.b.x + 1) * width_2;
            tz2 = (triangle.b.z + 1) * width_2;

            tx3 = (triangle.c.y + 1) * width_2;
            ty3 = (triangle.c.x + 1) * width_2;
            tz3 = (triangle.c.z + 1) * width_2;

            tCenterX = (tx1 + tx2 + tx3) / 3;
            tCenterY = (ty1 + ty2 + ty3) / 3;
            tCenterZ = (tz1 + tz2 + tz3) / 3;

            dx12 = tx1 - tx2;
            dx13 = tx1 - tx3;
            dx23 = tx2 - tx3;
            dx31 = tx3 - tx1;
            dx32 = tx3 - tx2;
            dy12 = ty1 - ty2;
            dy13 = ty1 - ty3;
            dy23 = ty2 - ty3;
            dy31 = ty3 - ty1;

            minX = std::min(std::min(tx1, tx2), tx3);
            maxX = std::max(std::max(tx1, tx2), tx3);

            minX = std::max(minX, 0.0f);
            maxX = std::min(maxX, imageWidth - 1.0f);

            for (auto x = static_cast<int>(minX); x < maxX; x++) {

                dy12_dx01 = dy12 * (x - tx1);
                dy23_dx02 = dy23 * (x - tx2);
                dy31_dx03 = dy31 * (x - tx3);
                dy23_dx03 = dy23 * (x - tx3);

                minY = std::min(std::min(ty1, ty2), ty3);
                maxY = std::max(std::max(ty1, ty2), ty3);

                minY = std::max(minY, 0.0f);
                maxY = std::min(maxY, imageHeight - 1.0f);

                for (int y = minY; y < maxY; y++) {
                    if (dx12 * (y - ty1) - dy12_dx01 > 0 && dx23 * (y - ty2) - dy23_dx02 > 0 &&
                        dx31 * (y - ty3) - dy31_dx03 > 0) {
                        index = 3 * (x * imageHeight + y);
                        lambda1 = (dy23_dx03 + dx32 * (y - ty3)) / (dy23 * dx13 + dx32 * dy13);
                        lambda2 = (dy31_dx03 + dx13 * y - ty3) / (dy31 * dx23 + dx13 * dy23);
                        lambda3 = 1 - lambda1 - lambda2;
                        i = pixelsDepthMap.find(index);
                        if (i == pixelsDepthMap.end() || i->second < lambda1 * tz1 + lambda2 * tz2 + lambda3 * tz3) {
                            pixelsDepthMap[index] = depth;

                            distanceFromLight = Math::sqrt((lightX - tCenterX) * (lightX - tCenterX)
                                                           + (lightY - tCenterY) * (lightY - tCenterY)
                                                           + (lightZ - tCenterZ) * (lightZ - tCenterZ));
                            intensity = (1 / (distanceFromLight * distanceFromLight)) * 5000;
                            if (intensity > 1) {
                                intensity = 1;
                            }
                            setColor(Color(triangle.color.r, triangle.color.g, triangle.color.b), index, colorBuffer,
                                     intensity);
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

void Renderer::setColor(const Color &color, int index, unsigned char *colorBuffer, float intensity) const {
    colorBuffer[index] = color.b * intensity;
    colorBuffer[index + 1] = color.g * intensity;
    colorBuffer[index + 2] = color.r * intensity;
}

Renderer::~Renderer() {
    delete scene;
}

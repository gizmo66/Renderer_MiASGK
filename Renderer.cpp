//
// Created by Adam on 2017-10-11.
//

#include <chrono>
#include <cmath>
#include "Renderer.h"
#include "Math.h"

chrono::duration<float> elapsedSeconds;
int rendered = 0, index;
float tx1, ty1, tz1, tx2, ty2, tz2, tx3, ty3, tz3, dx12, dx13, dx23, dx31, dx32, dy12, dy13, dy23, dy31, dy12_dx01, dy23_dx02,
        dy31_dx03, dy23_dx03, dy23_dx13_A_dx32_dy13, dy31_dx23_A_dx13_dy23, lambda1, lambda2, lambda3, depth, minX, minY, maxX, maxY;
map<int, float>::iterator i;
float lightX, lightY, lightZ, tCenterX, tCenterY, tCenterZ, distanceFromLight1, distanceFromLight2,
        distanceFromLight3, intensity, intensity1, intensity2, intensity3;

Renderer::Renderer(Scene *scene) {
    this->scene = scene;
}

void Renderer::setImageSize(int imageWidth, int imageHeight) {
    this->imageWidth = imageWidth;
    this->imageHeight = imageHeight;
    this->width_2 = imageWidth * 0.5f;
}

float Renderer::render(unsigned char *colorBuffer) {
    //cout << "Rendering started" << endl;
    auto start = chrono::system_clock::now();

    map<int, float> pixelsDepthMap;
    for (const auto &triangle : scene->triangles) {

        tCenterX = (triangle.a.y + triangle.b.y + triangle.c.y) / 3.0f;
        tCenterY = (triangle.a.x + triangle.b.x + triangle.c.x) / 3.0f;
        tCenterZ = (triangle.a.z + triangle.b.z + triangle.c.z) / 3.0f;

        if (Vector3::dot(tCenterX, tCenterY, tCenterZ, triangle.normalVector) >= 0.0f) {

            tx1 = (triangle.a.y + 1.0f) * width_2;
            ty1 = (triangle.a.x + 1.0f) * width_2;
            tz1 = (triangle.a.z + 1.0f) * width_2;

            tx2 = (triangle.b.y + 1.0f) * width_2;
            ty2 = (triangle.b.x + 1.0f) * width_2;
            tz2 = (triangle.b.z + 1.0f) * width_2;

            tx3 = (triangle.c.y + 1.0f) * width_2;
            ty3 = (triangle.c.x + 1.0f) * width_2;
            tz3 = (triangle.c.z + 1.0f) * width_2;

            dx12 = tx1 - tx2;
            dx13 = tx1 - tx3;
            dx23 = tx2 - tx3;
            dx31 = tx3 - tx1;
            dx32 = tx3 - tx2;
            dy12 = ty1 - ty2;
            dy13 = ty1 - ty3;
            dy23 = ty2 - ty3;
            dy31 = ty3 - ty1;

            dy23_dx13_A_dx32_dy13 = 1 / (dy23 * dx13 + dx32 * dy13);
            dy31_dx23_A_dx13_dy23 = 1 / (dy31 * dx23 + dx13 * dy23);

            minX = std::min(std::min(tx1, tx2), tx3);
            maxX = std::max(std::max(tx1, tx2), tx3);

            minX = std::max(minX, 0.0f);
            maxX = std::min(maxX, imageWidth - 1.0f);

            intensity1 = 0;
            intensity2 = 0;
            intensity3 = 0;

            if (!triangle.isLightMarker) {
                for (const auto &light : scene->lights) {
                    lightX = light.position.x;
                    lightY = light.position.y;
                    lightZ = light.position.z;

                    distanceFromLight1 = static_cast<float>(Math::sqrt((lightX - triangle.a.x) * (lightX - triangle.a.x)
                                                                       +
                                                                       (lightY - triangle.a.y) * (lightY - triangle.a.y)
                                                                       +
                                                                       (lightZ - triangle.a.z) *
                                                                       (lightZ - triangle.a.z)));

                    distanceFromLight2 = static_cast<float>(Math::sqrt((lightX - triangle.b.x) * (lightX - triangle.b.x)
                                                                       +
                                                                       (lightY - triangle.b.y) * (lightY - triangle.b.y)
                                                                       +
                                                                       (lightZ - triangle.b.z) *
                                                                       (lightZ - triangle.b.z)));

                    distanceFromLight3 = static_cast<float>(Math::sqrt((lightX - triangle.c.x) * (lightX - triangle.c.x)
                                                                       +
                                                                       (lightY - triangle.c.y) * (lightY - triangle.c.y)
                                                                       +
                                                                       (lightZ - triangle.c.z) *
                                                                       (lightZ - triangle.c.z)));

                    intensity1 += (1.0f / (distanceFromLight1)) * 0.1f;
                    intensity2 += (1.0f / (distanceFromLight2)) * 0.1f;
                    intensity3 += (1.0f / (distanceFromLight3)) * 0.1f;
                }
            }

            minY = std::min(std::min(ty1, ty2), ty3);
            maxY = std::max(std::max(ty1, ty2), ty3);

            minY = std::max(minY, 0.0f);
            maxY = std::min(maxY, imageHeight - 1.0f);

            for (auto x = static_cast<int>(minX) - 1; x < maxX + 1; x++) {

                dy12_dx01 = dy12 * (x - tx1);
                dy23_dx02 = dy23 * (x - tx2);
                dy31_dx03 = dy31 * (x - tx3);
                dy23_dx03 = dy23 * (x - tx3);

                for (auto y = static_cast<int>(minY); y < maxY + 1; y++) {

                    if (dx12 * (y - ty1) - dy12_dx01 > 0.0f && dx23 * (y - ty2) - dy23_dx02 > 0.0f &&
                        dx31 * (y - ty3) - dy31_dx03 > 0.0f) {

                        index = 3 * (x * imageHeight + y);
                        i = pixelsDepthMap.find(index);

                        lambda1 = (dy23_dx03 + dx32 * (y - ty3)) * dy23_dx13_A_dx32_dy13;
                        lambda2 = (dy31_dx03 + dx13 * (y - ty3)) * dy31_dx23_A_dx13_dy23;
                        lambda3 = 1.0f - lambda1 - lambda2;

                        if (i == pixelsDepthMap.end() || i->second < lambda1 * tz1 + lambda2 * tz2 + lambda3 * tz3) {
                            pixelsDepthMap[index] = depth;

                            if (!triangle.isLightMarker) {
                                intensity = lambda1 * intensity1 + lambda2 * intensity2 + lambda3 * intensity3;
                                if (intensity > 1.0f) {
                                    intensity = 1.0f;
                                }
                            } else {
                                intensity = 1.0f;
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
    colorBuffer[index] = static_cast<unsigned char>(color.b * intensity);
    colorBuffer[index + 1] = static_cast<unsigned char>(color.g * intensity);
    colorBuffer[index + 2] = static_cast<unsigned char>(color.r * intensity);
}

Renderer::~Renderer() {
    delete scene;
}

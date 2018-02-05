//
// Created by Adam on 2017-10-11.
//

#include <chrono>
#include <cmath>
#include "Renderer.h"
#include "Math.h"

chrono::duration<float> elapsedSeconds;
map<int, float>::iterator i;

Renderer::Renderer(Scene *scene) {
    this->scene = scene;
}

void Renderer::setImageSize(int imageWidth, int imageHeight) {
    this->imageWidth = imageWidth;
    this->imageHeight = imageHeight;
}

void Renderer::render(unsigned char *colorBuffer) {

    auto start = chrono::system_clock::now();

    int index = 0;
    int depth = 0;

    map<int, float> pixelsDepthMap;
    for (const auto &triangle : scene->triangles) {

        //float tCenterX = (triangle.a.y + triangle.b.y + triangle.c.y) / 3.0f;
        //float tCenterY = (triangle.a.x + triangle.b.x + triangle.c.x) / 3.0f;
        //float tCenterZ = (triangle.a.z + triangle.b.z + triangle.c.z) / 3.0f;

        //if (Vector3::dot(tCenterX, tCenterY, tCenterZ, triangle.normalVector) >= 0.0f) {

            float x1 = (triangle.a.y + 1.0f) * imageWidth / 2;
            float y1 = (triangle.a.x + 1.0f) * imageWidth / 2;
            float z1 = (triangle.a.z + 1.0f) * imageWidth / 2;

            float x2 = (triangle.b.y + 1.0f) * imageWidth / 2;
            float y2 = (triangle.b.x + 1.0f) * imageWidth / 2;
            float z2 = (triangle.b.z + 1.0f) * imageWidth / 2;

            float x3 = (triangle.c.y + 1.0f) * imageWidth / 2;
            float y3 = (triangle.c.x + 1.0f) * imageWidth / 2;
            float z3 = (triangle.c.z + 1.0f) * imageWidth / 2;


            float intensity1 = 0;
            float intensity2 = 0;
            float intensity3 = 0;

            if (!triangle.isLightMarker) {
                for (const auto &light : scene->lights) {
                    float lightX = light.position.x;
                    float lightY = light.position.y;
                    float lightZ = light.position.z;

                    float distanceFromLight1 = static_cast<float>(Math::sqrt((lightX - triangle.a.x) * (lightX - triangle.a.x)
                                                                       +
                                                                       (lightY - triangle.a.y) * (lightY - triangle.a.y)
                                                                       +
                                                                       (lightZ - triangle.a.z) *
                                                                       (lightZ - triangle.a.z)));

                    float distanceFromLight2 = static_cast<float>(Math::sqrt((lightX - triangle.b.x) * (lightX - triangle.b.x)
                                                                       +
                                                                       (lightY - triangle.b.y) * (lightY - triangle.b.y)
                                                                       +
                                                                       (lightZ - triangle.b.z) *
                                                                       (lightZ - triangle.b.z)));

                    float distanceFromLight3 = static_cast<float>(Math::sqrt((lightX - triangle.c.x) * (lightX - triangle.c.x)
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

            for (auto x = 0; x < imageWidth; x++) {



                for (auto y = 0; y < imageHeight; y++) {

                    bool inTriangle = (x1 - x2) * (y - y1) - ((y1 - y2) * (x - x1)) > 0 && (x2 - x3) * (y - y2) - ((y2 - y3) * (x - x2)) > 0 &&
                                      (x3 - x1) * (y - y3) - ((y3 - y1) * (x - x3)) > 0;
                    if (inTriangle) {

                        index = 3 * (x * imageHeight + y);
                        i = pixelsDepthMap.find(index);

                        float lambda1 = (((y2 - y3) * (x - x3)) + (x3 - x2) * (y - y3)) / ((y2 - y3)*(x1 - x3)+(x3 - x2)*(y1 - y3));
                        float lambda2 = (((y3 - y1) * (x - x3)) + (x1 - x3) * (y - y3)) / ((y3 - y1)*(x2 - x3)+(x1 - x3)*(y2 - y3));
                        float lambda3 = 1.0f - lambda1 - lambda2;

                        if (i == pixelsDepthMap.end() || i->second < lambda1 * z1 + lambda2 * z2 + lambda3 * z3) {
                            pixelsDepthMap[index] = depth;

                            float intensity;
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
        //}
    }

    auto end = chrono::system_clock::now();
    elapsedSeconds = end - start;

    printf("%f\n", elapsedSeconds.count());

    //cout << "Rendered triangles: " << rendered << endl;
}

void Renderer::setColor(const Color &color, int index, unsigned char *colorBuffer, float intensity) const {
    colorBuffer[index] = static_cast<unsigned char>(color.b * intensity);
    colorBuffer[index + 1] = static_cast<unsigned char>(color.g * intensity);
    colorBuffer[index + 2] = static_cast<unsigned char>(color.r * intensity);
}

Renderer::~Renderer() {
    delete scene;
}

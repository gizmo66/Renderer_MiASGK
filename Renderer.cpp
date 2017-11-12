//
// Created by Adam on 2017-10-11.
//

#include <chrono>
#include "Renderer.h"

Renderer::Renderer(Scene *scene) {
    this->scene = scene;
}

void Renderer::setImageSize(int imageWidth, int imageHeight) {
    this->imageWidth = imageWidth;
    this->imageHeight = imageHeight;
}

void Renderer::initColorBuffer() {
    colorBuffer = new unsigned char[4 * imageWidth * imageHeight];

    for (int x = 0; x < imageWidth; x++) {
        for (int y = 0; y < imageHeight; y++) {
            int index = 4 * (x * imageHeight + y);
            setColor(Color::LightBlue, index);
        }
    }
}

void Renderer::render() {
    cout << "Rendering started" << endl;
    auto start = chrono::system_clock::now();

    map<int, float> pixelsDepthMap;
    float width_2 = imageWidth * .5f;
    int rendered = 0;
    for (const auto &triangle : scene->triangles) {

        float x1 = (triangle.a.y + 1) * width_2;
        float y1 = (triangle.a.x + 1) * width_2;
        float z1 = (triangle.a.z + 1) * width_2;

        if (Vector3::dot(x1, y1, z1, triangle.normalVector) >= 0) {

            float x2 = (triangle.b.y + 1) * width_2;
            float y2 = (triangle.b.x + 1) * width_2;
            float z2 = (triangle.b.z + 1) * width_2;

            float x3 = (triangle.c.y + 1) * width_2;
            float y3 = (triangle.c.x + 1) * width_2;
            float z3 = (triangle.c.z + 1) * width_2;

            float dx12 = x1 - x2;
            float dx13 = x1 - x3;
            float dx23 = x2 - x3;
            float dx31 = x3 - x1;
            float dx32 = x3 - x2;
            float dy12 = y1 - y2;
            float dy13 = y1 - y3;
            float dy23 = y2 - y3;
            float dy31 = y3 - y1;

            //TODO akolodziejek: optymalizacja1 - przeszukiwanie
            //TODO akolodziejek: obcinanie
            //TODO akolodziejek: interpolacja
            //TODO akolodziejek: krawędzie top left
            for (int x = 0; x < imageWidth; x++) {

                float dy12_dx01 = dy12 * (x - x1);
                float dy23_dx02 = dy23 * (x - x2);
                float dy31_dx03 = dy31 * (x - x3);
                float dy23_dx03 = dy23 * (x - x3);

                for (int y = 0; y < imageHeight; y++) {
                    if (dx12 * (y - y1) - dy12_dx01 > 0 && dx23 * (y - y2) - dy23_dx02 > 0 &&
                        dx31 * (y - y3) - dy31_dx03 > 0) {
                        int index = 4 * (x * imageHeight + y);
                        float lambda1 = (dy23_dx03 + dx32 * (y - y3)) / (dy23 * dx13 + dx32 * dy13);
                        float lambda2 = (dy31_dx03 + dx13 * y - y3) / (dy31 * dx23 + dx13 * dy23);
                        float lambda3 = 1 - lambda1 - lambda2;
                        float depth = lambda1 * z1 + lambda2 * z2 + lambda3 * z3;
                        auto i = pixelsDepthMap.find(index);
                        if (i == pixelsDepthMap.end() || i->second < depth) {
                            pixelsDepthMap[index] = depth;
                            setColor(triangle.getColor(), index);
                        }
                    }
                }
            }
            rendered++;
        }
    }

    auto end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end - start;

    cout << "Elapsed time: " << elapsed_seconds.count() << "s\n";
    cout << "Rendered triangles: " << rendered << endl;
}

void Renderer::setColor(const Color &color, int index) const {
    //format .tga -> BGRA, 1 bajt na kazdy kolor, wartosci 0-255
    colorBuffer[index] = color.b;
    colorBuffer[index + 1] = color.g;
    colorBuffer[index + 2] = color.r;
    colorBuffer[index + 3] = 255;
}

void Renderer::saveImageToTga(const char *fileName) {

    cout << "Saving started" << endl;

    //header ma 18 bajtów (9 x 2 bajty, bo short ma 2 bajty)
    unsigned short header[9] = {
            0x0000, 0x0002, 0x0000, 0x0000, 0x0000, 0x0000,
            0x0100, 0x0100, //width, height
            0x0820
    };

    FILE *file = fopen(fileName, "wb+");

    header[6] = static_cast<unsigned short>(imageWidth);
    header[7] = static_cast<unsigned short>(imageHeight);

    fwrite(header, 2, 9, file);
    fwrite(colorBuffer, 1, static_cast<size_t>(4 * imageWidth * imageHeight), file);

    fclose(file);
    delete file;

    cout << "Saving ended" << endl;
}

Renderer::~Renderer() {
    delete scene;
}

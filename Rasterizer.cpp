//
// Created by Adam on 2017-10-08.
//

#include <cstdio>
#include "Rasterizer.h"
#include "Scene.h"

void Rasterizer::render() {

    Scene scene;

    auto *triangle1 = new Triangle(new Point3(-1, -1, 0), new Point3(0, 1, 0), new Point3(1, 0, 0));
    triangle1->setColor(Color::Green);

    setImageSize(500, 500);
    initColorBuffer();
    scene.add(triangle1);
    scene.render(this);

    saveToTga();
}

void Rasterizer::saveToTga() {

    //header ma 18 bajtów (9 x 2 bajty, bo short ma 2 bajty)
    unsigned short header[9] = {
            0x0000, 0x0002, 0x0000, 0x0000, 0x0000, 0x0000,
            0x0100, 0x0100, //width, height
            0x0820
    };

    FILE *file = fopen("render.tga", "wb+");

    header[6] = static_cast<unsigned short>(imageWidth);
    header[7] = static_cast<unsigned short>(imageHeight);

    fwrite(header, 2, 9, file);
    fwrite(getColorBuffer(), 1, static_cast<size_t>(4 * imageWidth * imageHeight), file);

    fclose(file);
}

void Rasterizer::setImageSize(int imageWidth, int imageHeight) {
    this->imageWidth = imageWidth;
    this->imageHeight = imageHeight;
}

void Rasterizer::initColorBuffer() {
    colorBuffer.init(imageWidth, imageHeight);
}

unsigned char *Rasterizer::getColorBuffer() {
    return colorBuffer.getBuffer();
}

int Rasterizer::getImageWidth() const {
    return imageWidth;
}

int Rasterizer::getImageHeight() const {
    return imageHeight;
}

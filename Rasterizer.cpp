//
// Created by Adam on 2017-10-08.
//

#include "Rasterizer.h"

const char *FILE_NAME = "render.tga";

void Rasterizer::render() {
    ModelLoader modelLoader;
    Model3D *model3D = modelLoader.importFile("revolver.3DS");
    if (model3D != nullptr) {
        auto *scene = new Scene(getTriangles(*model3D));
        cout << "ilosc trojkatow: " << scene->triangles.size() << endl;

        auto renderer = Renderer(scene);
        renderer.setImageSize(600, 600);
        renderer.initColorBuffer();
        renderer.render();
        renderer.saveImageToTga(FILE_NAME);
        openFile(FILE_NAME);
    }
}

list<Triangle> Rasterizer::getTriangles(Model3D &model3D) const {
    list<Triangle> triangles;
    for (auto &object : model3D.objects) {
        for (int i = 0; i < object.facesQuantity; i++) {
            Vector3 a = object.vertices[object.faces[i].verticesIndexes[0]];
            Vector3 b = object.vertices[object.faces[i].verticesIndexes[1]];
            Vector3 c = object.vertices[object.faces[i].verticesIndexes[2]];
            triangles.emplace_back(Point3(a.x, a.y, a.z), Point3(b.x, b.y, b.z), Point3(c.x, c.y, c.z));
        }
    }
    return triangles;
}

void Rasterizer::openFile(const char *fileName) {
    ShellExecute(nullptr, nullptr, fileName, nullptr, nullptr, SW_SHOW);
}

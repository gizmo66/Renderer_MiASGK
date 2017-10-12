//
// Created by Adam on 2017-10-08.
//

#include "Rasterizer.h"

const char *MODEL_3D_FILE_NAME = "revolver.3DS";
const char *RENDER_FILE_NAME = "render.tga";
int IMAGE_WIDTH = 600;
int IMAGE_HEIGHT = 600;

void Rasterizer::render() {
    ModelLoader modelLoader;
    Model3D *model3D = modelLoader.importFile(MODEL_3D_FILE_NAME);
    if (model3D != nullptr) {
        auto *scene = new Scene(getTriangles(*model3D));
        cout << "ilosc trojkatow: " << scene->triangles.size() << endl;
        auto renderer = Renderer(scene);
        renderer.setImageSize(IMAGE_WIDTH, IMAGE_HEIGHT);
        renderer.initColorBuffer();
        renderer.render();
        renderer.saveImageToTga(RENDER_FILE_NAME);
        openFile(RENDER_FILE_NAME);
    }
}

vector<Triangle> Rasterizer::getTriangles(Model3D &model3D) const {
    vector<Triangle> triangles;
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

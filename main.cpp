#include <Windows.h>
#include <random>
#include <iostream>
#include <chrono>
#include "ModelLoader.h"
#include "Scene.h"
#include "Renderer.h"

HDC imageDC;
HBITMAP imageBmp;
HBITMAP imageBmpOld;

const int screenSize_X = 450;
const int screenSize_Y = 450;

static const char *const FPS = " fps";
const char *MODEL_3D_FILE_NAME = "revolver1.3DS";
int IMAGE_WIDTH = 320;
int IMAGE_HEIGHT = 320;
Model3D *model;

bool finish = false;

bool rotateModel = true;
float moveSpeed = 0.05f;
float rotation = 0.1f;
bool X = true;
short int currentObjectId = 3;

vector<Model3D> lightSpheres;

enum direction {
    upDirection = 0,
    downDirection = 1,
    leftDirection = 2,
    rightDirection = 3,
    frontDirection = 4,
    backDirection = 5
};

enum transformation {
    shiftObject,
    rotateObject
};

void cleanUpImage() {
    SelectObject(imageDC, imageBmpOld);
    DeleteObject(imageBmp);
    DeleteDC(imageDC);
}

void drawImage(HDC screen) {
    BitBlt(screen, 0, 0, screenSize_X, screenSize_Y, imageDC, 0, 0, SRCCOPY);
}

void changeObject() {
    for (int i = 0; i < model->objectsQuantity; i++) {
        if (model->objects.empty()) break;

        model->objects[i].hasTexture = model->id[i] != currentObjectId;
    }
}

void transformObject(direction direction, transformation transformation) {
    vector<Object> objectsToTransform;
    int objectsToTransformQuantity = 0;

    objectsToTransform.push_back(model->objects[currentObjectId]);
    objectsToTransformQuantity++;

    if (!model->objects[currentObjectId].children.empty()) {
        for (unsigned int i = 0; i < model->objects[currentObjectId].children.size(); i++) {
            for (int j = 0; j < model->objectsQuantity; j++) {
                if (model->id[j] == model->objects[currentObjectId].children[i]) {
                    objectsToTransform.push_back(model->objects[j]);
                }
            }
            objectsToTransformQuantity++;
        }
    }

    for (int j = 0; j < objectsToTransformQuantity; j++) {
        for (int i = 0; i < objectsToTransform[j].verticesQuantity; i++) {
            if (transformation == shiftObject) {
                if (direction == upDirection) {
                    objectsToTransform[j].vertices[i].y = objectsToTransform[j].vertices[i].y + moveSpeed;
                }

                if (direction == downDirection) {
                    objectsToTransform[j].vertices[i].y = objectsToTransform[j].vertices[i].y - moveSpeed;
                }

                if (direction == leftDirection) {
                    objectsToTransform[j].vertices[i].x = objectsToTransform[j].vertices[i].x + moveSpeed;
                }

                if (direction == rightDirection) {
                    objectsToTransform[j].vertices[i].x = objectsToTransform[j].vertices[i].x - moveSpeed;
                }

                if (direction == frontDirection) {
                    objectsToTransform[j].vertices[i].z = objectsToTransform[j].vertices[i].z - moveSpeed;
                }

                if (direction == backDirection) {
                    objectsToTransform[j].vertices[i].z = objectsToTransform[j].vertices[i].z + moveSpeed;
                }
            } else if (transformation == rotateObject) {
                Vector3 *pivot = objectsToTransform[j].pivot;

                float x = objectsToTransform[j].vertices[i].x;
                float y = objectsToTransform[j].vertices[i].y;
                float z = objectsToTransform[j].vertices[i].z;

                if (direction == frontDirection) {
                    objectsToTransform[j].vertices[i].z = pivot->z + z * cos(rotation) - y * sin(rotation);
                    objectsToTransform[j].vertices[i].y = pivot->y + z * sin(rotation) + y * cos(rotation);
                }

                if (direction == backDirection) {
                    objectsToTransform[j].vertices[i].z = pivot->z + z * cos(-rotation) - y * sin(-rotation);
                    objectsToTransform[j].vertices[i].y = pivot->y + z * sin(-rotation) + y * cos(-rotation);
                }

                if (direction == leftDirection) {
                    objectsToTransform[j].vertices[i].z = pivot->z + z * cos(rotation) - x * sin(rotation);
                    objectsToTransform[j].vertices[i].x = pivot->x + z * sin(rotation) + x * cos(rotation);
                }

                if (direction == rightDirection) {
                    objectsToTransform[j].vertices[i].z = pivot->z + z * cos(-rotation) - x * sin(-rotation);
                    objectsToTransform[j].vertices[i].x = pivot->x + z * sin(-rotation) + x * cos(-rotation);
                }
            }
        }
    }
}

LRESULT CALLBACK wndProc(HWND wnd, UINT msg, WPARAM w, LPARAM l) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps{};
            HDC screen = BeginPaint(wnd, &ps);
            drawImage(screen);
            EndPaint(wnd, &ps);
        }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_CHAR: {
            WCHAR name[1024];
            UINT scanCode = MapVirtualKeyW(w, MAPVK_VK_TO_VSC);
            LONG lParamValue = (scanCode << 16);
            int result = GetKeyNameTextW(lParamValue, name, 1024);
            if (result > 0) {
                //std::wcout << name << " pressed" << endl;
            }
            switch (w) {
                case VK_ESCAPE:
                    finish = true;
                    break;

                case ' ':
                    rotateModel = !rotateModel;
                    break;

                case 'c':
                    X = !X;
                    break;

                case ',':
                    currentObjectId--;
                    changeObject();
                    break;

                case '.':
                    currentObjectId++;
                    changeObject();
                    break;

                case 'w':

                    transformObject(upDirection, shiftObject);
                    break;

                case 's':

                    transformObject(downDirection, shiftObject);
                    break;

                case 'a':

                    transformObject(leftDirection, shiftObject);
                    break;

                case 'd':

                    transformObject(rightDirection, shiftObject);
                    break;

                case 'z':

                    transformObject(frontDirection, shiftObject);
                    break;

                case 'x':

                    transformObject(backDirection, shiftObject);
                    break;

                case '8':

                    transformObject(frontDirection, rotateObject);
                    break;

                case '5':

                    transformObject(backDirection, rotateObject);
                    break;

                case '4':

                    transformObject(leftDirection, rotateObject);
                    break;

                case '6':

                    transformObject(rightDirection, rotateObject);
                    break;
                default:
                    break;
            }
            break;
        }

        default:
            break;
    }

    return DefWindowProc(wnd, msg, w, l);
}

HWND createWindow(HINSTANCE inst) {
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hInstance = inst;
    wc.lpfnWndProc = wndProc;
    wc.lpszClassName = TEXT("DisplayImage");

    RegisterClassEx(&wc);

    int style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

    RECT rc = {0, 0, screenSize_X, screenSize_Y};
    AdjustWindowRect(&rc, static_cast<DWORD>(style), FALSE);

    return CreateWindow(TEXT("DisplayImage"), TEXT("Display an Image"), style | WS_VISIBLE, 100, 100,
                        rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, inst, nullptr);
}

using namespace std;

vector<Triangle> getTriangles(Model3D &model1, bool isLightMarker) {
    vector<Triangle> triangles;
    Vector3 a, b, c, normalVector;
    for (auto &object : model1.objects) {
        for (int i = 0; i < object.facesQuantity; i++) {
            a = object.vertices[object.faces[i].verticesIndexes[0]];
            b = object.vertices[object.faces[i].verticesIndexes[1]];
            c = object.vertices[object.faces[i].verticesIndexes[2]];

            //N = (V1 - V0) x (V2 - V0)
            normalVector = Vector3::cross(Vector3::subtract(b, a), Vector3::subtract(c, a));

            triangles.emplace_back(Vector3(a.x, a.y, a.z), Vector3(b.x, b.y, b.z), Vector3(c.x, c.y, c.z),
                                   normalVector, Color(object.color->g, object.color->r, object.color->b),
                                   isLightMarker);
        }
    }
    return triangles;
}

BITMAPINFO initBitmap() {
    BITMAPINFOHEADER bmih{};
    bmih.biSize = sizeof(BITMAPINFOHEADER);
    bmih.biWidth = IMAGE_WIDTH;
    bmih.biHeight = -IMAGE_HEIGHT;
    bmih.biPlanes = 1;
    bmih.biBitCount = 24;
    bmih.biCompression = BI_RGB;
    bmih.biSizeImage = 0;
    bmih.biXPelsPerMeter = 10;
    bmih.biYPelsPerMeter = 10;
    bmih.biClrUsed = 0;
    bmih.biClrImportant = 0;

    BITMAPINFO dbmi{};
    (&dbmi, sizeof(dbmi));
    dbmi.bmiHeader = bmih;
    dbmi.bmiColors->rgbBlue = 0;
    dbmi.bmiColors->rgbGreen = 0;
    dbmi.bmiColors->rgbRed = 0;
    dbmi.bmiColors->rgbReserved = 0;
    return dbmi;
}

void updateChildren(Model3D &model1) {
    for (int i = 0; i < model1.objectsQuantity; i++) {
        for (int j = 0; j < model1.objectsQuantity; j++) {
            if (model1.parentId[j] > model1.parentId[i]) {
                model1.objects[i].children.push_back(model1.id[j]);
            }
        }
    }
}

int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmd, int show) {

    unsigned char pixels[IMAGE_WIDTH * IMAGE_HEIGHT * 3];
    for (unsigned char &pixel : pixels) {
        pixel = 0;
    }
    auto *bits = (void *) &(pixels[0]);

    BITMAPINFO dbmi = initBitmap();
    HWND window = createWindow(inst);

    ModelLoader modelLoader;
    model = modelLoader.importFile(MODEL_3D_FILE_NAME, true);
    if (model != nullptr) {
        updateChildren(*model);

        auto *scene = new Scene(getTriangles(*model, false));

        scene->lights.emplace_back(Vector3(0.0f, 0.0f, 0.0f));
        scene->lights.emplace_back(Vector3(0.7f, 0.0f, 0.0f));
        scene->lights.emplace_back(Vector3(-0.7f, 0.7f, 0.0f));

        float deltaX, deltaY;
        for (const auto &light : scene->lights) {
            Model3D *sphereModel = modelLoader.importFile("sphere.3DS", false);
            updateChildren(*sphereModel);

            deltaX = sphereModel->objects[0].vertices[0].x - light.position.x;
            deltaY = sphereModel->objects[0].vertices[0].y - light.position.y;

            for (int i = 0; i < sphereModel->objects[0].verticesQuantity; i++) {
                sphereModel->objects[0].vertices[i].x += -deltaX;
                sphereModel->objects[0].vertices[i].y += -deltaY;
            }

            vector<Triangle> sphereTriangles = getTriangles(*sphereModel, true);
            for (auto triangle : sphereTriangles) {
                scene->triangles.emplace_back(triangle);
            }

            lightSpheres.emplace_back(*sphereModel);
        }

        cout << "Triangles quantity: " << scene->triangles.size() << endl;

        auto renderer = Renderer(scene);
        renderer.setImageSize(IMAGE_WIDTH, IMAGE_HEIGHT);

        double elapsedTime = 0.0;
        long long int frameTime = 0;
        string fps;
        MSG msg{};
        while (!finish) {
            auto start = chrono::system_clock::now();

            scene->triangles = getTriangles(*model, false);
            for (auto sphereModel : lightSpheres) {
                vector<Triangle> sphereTriangles = getTriangles(sphereModel, true);
                for (auto triangle : sphereTriangles) {
                    scene->triangles.emplace_back(triangle);
                }
            }
            if (PeekMessage(&msg, window, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            } else {
                for (unsigned char &pixel : pixels) {
                    pixel = 0;
                }

                renderer.render(pixels);

                cleanUpImage();
                imageDC = CreateCompatibleDC(nullptr);
                imageBmp = CreateDIBSection(imageDC, &dbmi, DIB_RGB_COLORS, &bits, nullptr, 0);
                memcpy(bits, pixels, sizeof(pixels));
                imageBmpOld = (HBITMAP) SelectObject(imageDC, imageBmp);
            }

            auto end = chrono::system_clock::now();
            frameTime = (end - start).count();

            elapsedTime += (frameTime / 1.0e9);
            if (elapsedTime > 0.3) {
                fps = std::to_string((int) (1.0e9 / frameTime)) + FPS;
                SetWindowText(window, fps.c_str());
                elapsedTime = 0.0;
            }
            InvalidateRect(window, nullptr, TRUE);
            UpdateWindow(window);
        }
        //renderer.saveImageToTga(RENDER_FILE_NAME, pixels);
        //openFile(RENDER_FILE_NAME);
    }

    return 0;
}


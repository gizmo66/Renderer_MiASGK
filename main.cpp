#include <Windows.h>
#include <random>
#include <iostream>
#include "ModelLoader.h"
#include "Scene.h"
#include "Renderer.h"

HDC imageDC;
HBITMAP imageBmp;
HBITMAP imageBmpOld;

const int screenSize_X = 640;
const int screenSize_Y = 480;

const char *MODEL_3D_FILE_NAME = "revolver.3DS";
const char *RENDER_FILE_NAME = "render.tga";
int IMAGE_WIDTH = 80;
int IMAGE_HEIGHT = 80;

void cleanUpImage() {
    SelectObject(imageDC, imageBmpOld);
    DeleteObject(imageBmp);
    DeleteDC(imageDC);
}

void drawImage(HDC screen) {
    BitBlt(screen, 0, 0, screenSize_X, screenSize_Y, imageDC, 0, 0, SRCCOPY);
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

void openFile(const char *fileName) {
    ShellExecute(nullptr, nullptr, fileName, nullptr, nullptr, SW_SHOW);
}

vector<Triangle> getTriangles(Model3D &model3D) {
    vector<Triangle> triangles;
    for (auto &object : model3D.objects) {
        for (int i = 0; i < object.facesQuantity; i++) {
            Vector3 a = object.vertices[object.faces[i].verticesIndexes[0]];
            Vector3 b = object.vertices[object.faces[i].verticesIndexes[1]];
            Vector3 c = object.vertices[object.faces[i].verticesIndexes[2]];

            //N = (V1 - V0) x (V2 - V0)
            Vector3 normalVector = Vector3::cross(Vector3::subtract(b, a), Vector3::subtract(c, a));

            triangles.emplace_back(Vector3(a.x, a.y, a.z), Vector3(b.x, b.y, b.z), Vector3(c.x, c.y, c.z),
                                   normalVector);
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

int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmd, int show) {

    unsigned char pixels[IMAGE_WIDTH * IMAGE_HEIGHT * 3];
    for (unsigned char &pixel : pixels) {
        //BGR (nie RGB)
        pixel = 0;
    }
    auto *bits = (void *) &(pixels[0]);

    BITMAPINFO dbmi = initBitmap();
    HWND window = createWindow(inst);

    ModelLoader modelLoader;
    Model3D *model3D = modelLoader.importFile(MODEL_3D_FILE_NAME);
    if (model3D != nullptr) {
        auto *scene = new Scene(getTriangles(*model3D));
        cout << "Triangles quantity: " << scene->triangles.size() << endl;
        auto renderer = Renderer(scene);
        renderer.setImageSize(IMAGE_WIDTH, IMAGE_HEIGHT);

        bool finish = false;
        double elapsedTime = 0;
        string fps;
        while (!finish) {
            double time = renderer.render(pixels);
            elapsedTime += time;

            if(elapsedTime > 0.2f) {
                fps = std::to_string((int)(1.0f / time)) + " fps";
                elapsedTime = 0;
                SetWindowText(window, fps.c_str());
            }

            cleanUpImage();
            imageDC = CreateCompatibleDC(nullptr);
            imageBmp = CreateDIBSection(imageDC, &dbmi, DIB_RGB_COLORS, &bits, nullptr, 0);
            memcpy(bits, pixels, sizeof(pixels));
            imageBmpOld = (HBITMAP) SelectObject(imageDC, imageBmp);

            InvalidateRect(window, nullptr, TRUE);
            UpdateWindow(window);
        }
        renderer.saveImageToTga(RENDER_FILE_NAME, pixels);
        openFile(RENDER_FILE_NAME);
    }

    return 0;
}


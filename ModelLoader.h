//
// Created by Adam on 2017-10-11.
//

#ifndef RENDERER_MIASGK_3DS_H
#define RENDERER_MIASGK_3DS_H

//>------ Primary Chunk, at the beginning of each file
#define PRIMARY       0x4D4D

//>------ Main Chunks
#define OBJECTINFO    0x3D3D                // This gives the version of the mesh and is found right before the material and object information
#define VERSION       0x0002                // This gives the version of the .3ds file
#define EDITKEYFRAME  0xB000                // This is the header for all of the key frame info

//>------ sub defines of OBJECTINFO
#define MATERIAL      0xAFFF                // This stored the texture info
#define OBJECT        0x4000                // This stores the faces, vertices, etc... (object name)

//>------ sub defines of MATERIAL
#define MATNAME       0xA000                // This holds the material name
#define MATDIFFUSE    0xA020                // This holds the color of the object/material
#define MATMAP        0xA200                // This is a header for a new material
#define MATMAPFILE    0xA300                // This holds the file name of the texture

#define OBJECT_MESH   0x4100                // This lets us know that we are reading a new object

//>------ sub defines of OBJECT_MESH
#define OBJECT_VERTICES     0x4110          // The objects vertices
#define OBJECT_FACES        0x4120          // The objects faces
#define OBJECT_MATERIAL     0x4130          // This is found if the object has a material, either texture map or color
#define OBJECT_UV           0x4140          // The UV texture coordinates

#define MESH_INFO             0xB002        // Mesh Information Chunk
#define OBJECT_NAME           0xB010        // Object Name
#define OBJECT_PIVOT_POINT    0xB013        // Object Pivot Point
#define OBJECT_HIERARCHY_POS  0xB030        // Hierarchy Position

#include <cstdio>
#include <windows.h>
#include <vector>
#include "Vector3.h"
#include "Vector2.h"
#include "Face.h"

struct coordinates {

    unsigned short a, b, c, visible;
};

struct Chunk {
    unsigned short int id;
    unsigned int length;
    unsigned int readBytes;
};

using namespace std;

struct Texture {
    char name[255];
    char textureName[255];
    BYTE color[3];
    int textureId;
};

struct Object {
    int verticesQuantity;
    int facesQuantity;
    int textureCoordinatesQuantity;
    int materialId;
    bool hasTexture;
    char name[255];
    Vector3 *vertices;
    Vector3 *normals;
    Vector3 *pivot;
    Vector2 *textureCoordinates;
    Face *faces;
    vector<short int> children;
};

struct Model3D {
    int objectsQuantity;
    int materialsQuantity;
    vector<Texture> materials;
    vector<Object> objects;
    vector<short int> id;
    vector<short int> parentId;
};

class ModelLoader {
public:

    ModelLoader();

    Model3D *importFile(const char *fileName);

private:

    int readString(char *);

    void LoadChunk(Chunk *);

    void ReadNextChunk(Model3D *model, Chunk *);

    void readNextObject(Model3D *model, Object *object, Chunk *);

    void readNextMaterialFromChunk(Model3D *model, Chunk *);

    void readNextKeyFrame(Model3D *model, Chunk *previousChunk);

    void readColor(Texture *material, Chunk *chunk);

    void readVertices(Object *object, Chunk *);

    void readPivot(Object *object, Chunk *previousChunk);

    void readFaces(Object *object, Chunk *);

    void readUV(Object *object, Chunk *);

    void readMaterials(Model3D *model, Object *object, Chunk *previousChunk);

    void CalculateNormals(Model3D *model);

    void Clean();

    FILE *file;

    Chunk *currentChunk;
    Chunk *tempChunk;

    void *buffer;

    string getCurrentDirectoryOnWindows();
};


#endif


/*
0x4D4D // Main Chunk
├─ 0x3D3D // 3D Editor Chunk
│  ├─ 0x4000 // Object Chunk
│  │  ├─ 0x4100 // Triangular Mesh
│  │  │  ├─ 0x4110 // Vertices List
│  │  │  ├─ 0x4120 // Faces Description
│  │  │  │  ├─ 0x4130 // Faces Material
│  │  │  │  └─ 0x4150 // Smoothing Group List
│  │  │  ├─ 0x4140 // Mapping Coordinates List
│  │  │  └─ 0x4160 // Local Coordinates System
│  │  ├─ 0x4600 // Light
│  │  │  └─ 0x4610 // Spotlight
│  │  └─ 0x4700 // Camera
│  └─ 0xAFFF // Material Chunk
│     ├─ 0xA000 // Material Name
│     ├─ 0xA010 // Ambient Color
│     ├─ 0xA020 // Diffuse Color
│     ├─ 0xA030 // Specular Color
│     ├─ 0xA200 // Texture Map 1
│     ├─ 0xA230 // Bump Map
│     └─ 0xA220 // Reflection Map
│        │  // Sub Chunks For Each Map //
│        ├─ 0xA300 // Mapping Filename
│        └─ 0xA351 // Mapping Parameters
└─ 0xB000 // Keyframer Chunk
├─ 0xB002 // Mesh Information Chunk
│   ├─ 0xB010 // Object Name
│   ├─ 0xB013 // Object Pivot Point
│   ├─ 0xB020 // Position Track
│   ├─ 0xB021 // Rotation Track
│   ├─ 0xB022 // Scale Track
│   └─ 0xB030 // Hierarchy Position
├─ 0xB007 // Spot Light Information Chunk
└─ 0xB008 // Frames (Start and End)
*/


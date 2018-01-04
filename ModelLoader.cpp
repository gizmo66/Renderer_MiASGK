//
// Created by Adam on 2017-10-11.
//

#include <iostream>
#include "ModelLoader.h"

ModelLoader::ModelLoader() {
    currentChunk = new Chunk;
    tempChunk = new Chunk;
}

string ModelLoader::getCurrentDirectoryOnWindows()
{
    const unsigned long maxDir = 260;
    char currentDir[maxDir];
    GetCurrentDirectory(maxDir, currentDir);
    return string(currentDir);
}

Model3D *ModelLoader::importFile(const char *fileName) {
    string path = getCurrentDirectoryOnWindows() + "\\" + fileName;
    file = fopen(path.c_str(), "rb"); // r - tryb odczytu; b - otworz jako plik binarny
    if (!file) {
        cout << "No such file: \"" << fileName << "\"!" << endl;
        cout << "Path: " << path << endl;
        return nullptr;
    }

    // id pierwszego bloku w pliku .3ds musi byc rowne 0x4D4D (PRIMARY Chunk)
    LoadChunk(currentChunk);

    if (currentChunk->id != PRIMARY) {
        cout << "File cannot be loaded: \"" << fileName << "\"!";
        return nullptr;
    }

    buffer = new unsigned char[65536 * 3 * sizeof(float)];

    auto *model = new Model3D();
    ReadNextChunk(model, currentChunk);
    CalculateNormals(model);
    Clean();
    return model;
}

void ModelLoader::Clean() {
    fclose(file);
    delete currentChunk;
    delete tempChunk;
}

void ModelLoader::ReadNextChunk(Model3D *model, Chunk *previousChunk) {
    Object newObject = {0};
    Texture newTexture = {0};
    unsigned int version = 0;

    currentChunk = new Chunk;
    while (previousChunk->readBytes < previousChunk->length) {
        LoadChunk(currentChunk);

        switch (currentChunk->id) {
            case VERSION:
                currentChunk->readBytes += fread(&version, 1, currentChunk->length - currentChunk->readBytes, file);
                break;

            case OBJECTINFO:
                LoadChunk(tempChunk);
                tempChunk->readBytes += fread(&version, 1, tempChunk->length - tempChunk->readBytes, file);
                currentChunk->readBytes += tempChunk->readBytes;
                ReadNextChunk(model, currentChunk);
                break;

            case MATERIAL:
                model->materialsQuantity++;
                model->materials.push_back(newTexture);
                readNextMaterialFromChunk(model, currentChunk);
                break;

            case OBJECT:
                model->objectsQuantity++;
                model->objects.push_back(newObject);
                memset(&(model->objects[model->objectsQuantity - 1]), 0, sizeof(Object));
                currentChunk->readBytes += readString(model->objects[model->objectsQuantity - 1].name);
                readNextObject(model, &(model->objects[model->objectsQuantity - 1]), currentChunk);
                printf("Object name: %s\n", model->objects[model->objectsQuantity - 1].name);
                break;

            case EDITKEYFRAME:
                readNextKeyFrame(model, currentChunk);
                break;

            default:
                currentChunk->readBytes += fread(buffer, 1, currentChunk->length - currentChunk->readBytes, file);
                break;
        }
        previousChunk->readBytes += currentChunk->readBytes;
    }

    delete currentChunk;
    currentChunk = previousChunk;
}

void ModelLoader::readNextKeyFrame(Model3D *model, Chunk *previousChunk) {
    currentChunk = new Chunk;
    short int id = -1;
    short int parentId;

    while (previousChunk->readBytes < previousChunk->length) {
        LoadChunk(currentChunk);

        switch (currentChunk->id) {
            case MESH_INFO:
                readNextKeyFrame(model, currentChunk);
                break;

            case OBJECT_HIERARCHY_POS:
                currentChunk->readBytes += fread(&id, 1, 2, file);
                model->id.push_back(id);
                break;

            case OBJECT_NAME:
                char name[255];
                currentChunk->readBytes += readString(name);
                currentChunk->readBytes += fread(buffer, 1, 4, file);
                currentChunk->readBytes += fread(&parentId, 1, 2, file);
                model->parentId.push_back(parentId);
                break;

            case OBJECT_PIVOT_POINT:
                if (id != -1) {
                    readPivot(&model->objects[id], currentChunk);
                }
                break;

            default:
                currentChunk->readBytes += fread(buffer, 1, currentChunk->length - currentChunk->readBytes, file);
                break;
        }
        previousChunk->readBytes += currentChunk->readBytes;
    }
    delete currentChunk;
    currentChunk = previousChunk;
}

void ModelLoader::readNextObject(Model3D *model, Object *object, Chunk *previousChunk) {
    currentChunk = new Chunk;

    while (previousChunk->readBytes < previousChunk->length) {
        LoadChunk(currentChunk);
        switch (currentChunk->id) {
            case OBJECT_MESH:
                readNextObject(model, object, currentChunk);
                break;
            case OBJECT_VERTICES:
                readVertices(object, currentChunk);
                break;
            case OBJECT_FACES:
                readFaces(object, currentChunk);
                break;
            case OBJECT_MATERIAL:
                readMaterials(model, object, currentChunk);
                break;
            case OBJECT_UV:
                readUV(object, currentChunk);
                break;
            default:
                currentChunk->readBytes += fread(buffer, 1, currentChunk->length - currentChunk->readBytes, file);
                break;
        }
        previousChunk->readBytes += currentChunk->readBytes;
    }
    delete currentChunk;
    currentChunk = previousChunk;
}

void ModelLoader::readNextMaterialFromChunk(Model3D *model, Chunk *previousChunk) {
    currentChunk = new Chunk;

    while (previousChunk->readBytes < previousChunk->length) {
        LoadChunk(currentChunk);

        switch (currentChunk->id) {
            case MATNAME:
                currentChunk->readBytes += fread(model->materials[model->materialsQuantity - 1].name, 1,
                                                 currentChunk->length - currentChunk->readBytes, file);
                break;
            case MATDIFFUSE:
                readColor(&(model->materials[model->materialsQuantity - 1]), currentChunk);
                break;
            case MATMAP:
                readNextMaterialFromChunk(model, currentChunk);
                break;
            case MATMAPFILE:
                currentChunk->readBytes += fread(model->materials[model->materialsQuantity - 1].textureName, 1,
                                                 currentChunk->length - currentChunk->readBytes, file);
                break;
            default:
                currentChunk->readBytes += fread(buffer, 1, currentChunk->length - currentChunk->readBytes, file);
                break;
        }
        previousChunk->readBytes += currentChunk->readBytes;
    }
    delete currentChunk;
    currentChunk = previousChunk;
}

void ModelLoader::LoadChunk(Chunk *chunk) {
    // id bloku (2 bajty)
    chunk->readBytes = fread(&chunk->id, 1, 2, file);

    // dlugosc bloku (4 bajty)
    chunk->readBytes += fread(&chunk->length, 1, 4, file);
}

int ModelLoader::readString(char *buffer) {
    int index = 0;

    // 1 bajt string'a (pierwsza litera)
    fread(buffer, 1, 1, file);
    while (*(buffer + index++) != 0) {
        fread(buffer + index, 1, 1, file);
    }
    return strlen(buffer) + 1;
}

void ModelLoader::readColor(Texture *material, Chunk *chunk) {
    LoadChunk(tempChunk);
    // kolor RGB (3 bajty od 0-255)
    tempChunk->readBytes += fread(material->color, 1, tempChunk->length - tempChunk->readBytes, file);
    chunk->readBytes += tempChunk->readBytes;
}

void ModelLoader::readFaces(Object *object, Chunk *previousChunk) {
    unsigned short index = 0;

    previousChunk->readBytes += fread(&object->facesQuantity, 1, 2, file);
    object->faces = new Face[object->facesQuantity];
    memset(object->faces, 0, sizeof(Face) * object->facesQuantity);

    for (int i = 0; i < object->facesQuantity; i++) {
        for (int j = 0; j < 4; j++) {
            previousChunk->readBytes += fread(&index, 1, sizeof(index), file);
            if (j < 3) {
                object->faces[i].verticesIndexes[j] = index;
            }
        }
    }
}

void ModelLoader::readUV(Object *object, Chunk *previousChunk) {
    previousChunk->readBytes += fread(&object->textureCoordinatesQuantity, 1, 2, file);
    object->textureCoordinates = new Vector2[object->textureCoordinatesQuantity];
    previousChunk->readBytes += fread(object->textureCoordinates, 1,
                                      previousChunk->length - previousChunk->readBytes, file);
}

void ModelLoader::readVertices(Object *object, Chunk *previousChunk) {
    previousChunk->readBytes += fread(&(object->verticesQuantity), 1, 2, file);

    object->vertices = new Vector3[object->verticesQuantity];
    memset(object->vertices, 0, sizeof(Vector3) * object->verticesQuantity);

    previousChunk->readBytes += fread(object->vertices, 1, previousChunk->length - previousChunk->readBytes, file);

    for (int i = 0; i < object->verticesQuantity; i++) {
        float temp = object->vertices[i].y;

        object->vertices[i].y = object->vertices[i].z;
        object->vertices[i].z = -temp;
    }
}

void ModelLoader::readPivot(Object *object, Chunk *previousChunk) {
    previousChunk->readBytes += fread(buffer, 1, 4, file);
    object->pivot = new Vector3[1];
    previousChunk->readBytes += fread(object->pivot, 1, previousChunk->length - previousChunk->readBytes,
                                      file);
}

void ModelLoader::readMaterials(Model3D *model, Object *object, Chunk *previousChunk) {
    char materialName[255] = {0};

    previousChunk->readBytes += readString(materialName);

    for (int i = 0; i < model->materialsQuantity; i++) {
        if (strcmp(materialName, model->materials[i].name) == 0) {
            object->materialId = i;
            if (strlen(model->materials[i].textureName) > 0) {
                object->hasTexture = true;
            }
            break;
        } else {
            object->materialId = -1;
        }
    }

    previousChunk->readBytes += fread(buffer, 1, previousChunk->length - previousChunk->readBytes, file);
}

void ModelLoader::CalculateNormals(Model3D *model) {
    Vector3 vector1{};
    Vector3 vector2{};
    Vector3 normalVector{};
    Vector3 faces[3];

    if (model->objectsQuantity <= 0) {
        return;
    }

    for (int index = 0; index < model->objectsQuantity; index++) {
        Object *object = &(model->objects[index]);

        auto *normals = new Vector3[object->facesQuantity];
        auto *tempNormals = new Vector3[object->facesQuantity];
        object->normals = new Vector3[object->verticesQuantity];

        for (int i = 0; i < object->facesQuantity; i++) {
            faces[0] = object->vertices[object->faces[i].verticesIndexes[0]];
            faces[1] = object->vertices[object->faces[i].verticesIndexes[1]];
            faces[2] = object->vertices[object->faces[i].verticesIndexes[2]];

            vector1 = Vector3::vectorFromTwoPoints(faces[0], faces[2]);
            vector2 = Vector3::vectorFromTwoPoints(faces[2], faces[1]);

            normalVector = vector1.multiply(vector2);
            tempNormals[i] = normalVector;
            normalVector.normalize();

            normals[i] = normalVector;
        }

        Vector3 sum = {0.0, 0.0, 0.0};
        Vector3 zero = sum;
        int commonVerticesQuantity = 0;

        for (int i = 0; i < object->verticesQuantity; i++) {
            for (int j = 0; j < object->facesQuantity; j++) { // Sprawdzenie, czy wierzcholek nalezy do innej sciany
                if (object->faces[j].verticesIndexes[0] == i ||
                    object->faces[j].verticesIndexes[1] == i ||
                    object->faces[j].verticesIndexes[2] == i) {
                    sum = sum.add(tempNormals[j]);
                    commonVerticesQuantity++;
                }
            }

            object->normals[i] = sum.divideByNumber(float(-commonVerticesQuantity));
            object->normals[i] = object->normals[i].normalize();
            sum = zero;
            commonVerticesQuantity = 0;
        }

        delete[] tempNormals;
        delete[] normals;
    }
}
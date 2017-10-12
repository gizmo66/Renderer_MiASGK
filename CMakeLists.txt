cmake_minimum_required(VERSION 3.8)
project(Renderer_MiASGK)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(SOURCE_FILES main.cpp
        Rasterizer.cpp Rasterizer.h
        Scene.cpp Scene.h
        Color.cpp Color.h
        Triangle.cpp Triangle.h
        Point3.cpp Point3.h
        ColorBuffer.cpp ColorBuffer.h
        ModelLoader.cpp ModelLoader.h
        Face.cpp Face.h
        Vector2.cpp Vector2.h
        Vector3.cpp Vector3.h Renderer.cpp Renderer.h Math.cpp Math.h)

add_executable(Renderer_MiASGK ${SOURCE_FILES})
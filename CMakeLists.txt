cmake_minimum_required(VERSION 3.8)
project(Rasteryzer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(SOURCE_FILES main.cpp
        Scene.cpp Scene.h
        Color.cpp Color.h
        Triangle.cpp Triangle.h
        ModelLoader.cpp ModelLoader.h
        Face.cpp Face.h
        Vector2.cpp Vector2.h
        Vector3.cpp Vector3.h
        Renderer.cpp Renderer.h
        Math.cpp Math.h Light.cpp Light.h)
add_executable(Rasteryzer ${SOURCE_FILES})
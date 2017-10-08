cmake_minimum_required(VERSION 3.8)
project(Renderer_MiASGK)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Rasterizer.cpp Rasterizer.h Scene.cpp Scene.h Entity.cpp Entity.h Color.cpp Color.h Triangle.cpp Triangle.h Point3.cpp Point3.h ColorBuffer.cpp ColorBuffer.h)
add_executable(Renderer_MiASGK ${SOURCE_FILES})
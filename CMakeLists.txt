cmake_minimum_required(VERSION 3.20)
project(EvilHangman)

set(CMAKE_CXX_STANDARD 14)

include("EmbedResource.cmake")

add_executable(EvilHangman
        main.cpp
        EvilSet.cpp)

embed_resource(dictionary.txt dict.h DICT)
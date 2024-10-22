cmake_minimum_required(VERSION 3.5)

project(gpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES
        bindings/java/gpp_java_bindings.cpp
        lib/gpp.cpp)

set(HEADER_FILES
        bindings/java/util.h
        lib/gpp.h)

include_directories(
        /usr/lib/jvm/java-1.21.0-openjdk-amd64/include
        /usr/lib/jvm/java-1.21.0-openjdk-amd64/include/linux)

link_directories(
        /usr/lib/jvm/java-1.21.0-openjdk-amd64/include
        /usr/lib/jvm/java-1.21.0-openjdk-amd64/include/linux)

add_library(gpplib SHARED ${HEADER_FILES} ${SOURCE_FILES})
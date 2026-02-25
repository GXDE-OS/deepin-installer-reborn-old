CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

project(simpleini)

set(SOURCES
    SimpleIni.h
    ConvertUTF.h
    ConvertUTF.c
)

add_library(simpleini STATIC ${SOURCES})

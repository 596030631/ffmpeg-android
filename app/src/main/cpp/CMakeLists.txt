# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project("ffmpeg")

include_directories(${CMAKE_SOURCE_DIR}/${ANDROID_ABI}/include)

add_library(avdevice
        SHARED
        IMPORTED)

set_target_properties(avdevice
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/${ANDROID_ABI}/libavdevice.so)

add_library(postproc
        SHARED
        IMPORTED)

set_target_properties(postproc
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/${ANDROID_ABI}/libpostproc.so)

add_library(avcodec
        SHARED
        IMPORTED)

set_target_properties(avcodec
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/${ANDROID_ABI}/libavcodec.so)

#add_library(avdevice-57
#        SHARED
#        IMPORTED)
#set_target_properties(avdevice-57
#        PROPERTIES IMPORTED_LOCATION
#        ${CMAKE_SOURCE_DIR}/${ANDROID_ABI}/libavdevice-57.so)

add_library(avformat
        SHARED
        IMPORTED)
set_target_properties(avformat
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/${ANDROID_ABI}/libavformat.so)

add_library(avutil
        SHARED
        IMPORTED)
set_target_properties(avutil
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/${ANDROID_ABI}/libavutil.so)

#add_library(postproc-54
#        SHARED
#        IMPORTED)
#set_target_properties(postproc-54
#        PROPERTIES IMPORTED_LOCATION
#        ${CMAKE_SOURCE_DIR}/${ANDROID_ABI}/libpostproc-54.so)

add_library(swresample
        SHARED
        IMPORTED)
set_target_properties(swresample
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/${ANDROID_ABI}/libswresample.so)

add_library(swscale
        SHARED
        IMPORTED)
set_target_properties(swscale
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/${ANDROID_ABI}/libswscale.so)

add_library(avfilter
        SHARED
        IMPORTED)

set_target_properties(avfilter
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/${ANDROID_ABI}/libavfilter.so)


# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

file(GLOB native-src
        native-lib.cpp
        RtspClient.cpp
        ALog.h
        )

add_library( # Sets the name of the library.
        ffmpeg-test

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        ${native-src})

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        ffmpeg-test
        avdevice
        avfilter
        avcodec
        avformat
        avutil
        swresample
        swscale
        postproc
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})
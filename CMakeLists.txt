cmake_minimum_required(VERSION 3.21)
project(midi_playground CXX)

set(BUILD_SHARED_LIBS OFF)

add_subdirectory(SDL EXCLUDE_FROM_ALL)

set(RTMIDI_TARGETNAME_UNINSTALL rtmidi_uninstall)
add_subdirectory(rtmidi EXCLUDE_FROM_ALL)

add_subdirectory(imgui EXCLUDE_FROM_ALL)

add_subdirectory(fmt EXCLUDE_FROM_ALL)

set(CMAKE_CXX_STANDARD 17)

add_executable(midi_playground main.cpp app.cpp app.h)

target_link_libraries(midi_playground PRIVATE SDL2main SDL2-static rtmidi imgui fmt)

cmake_minimum_required(VERSION 3.10)
project(PPE_Streaming_Server)

set(CMAKE_CXX_STANDARD 17)


set(GSTREAMER_ROOT "D:/gstreamer/msvc_x86_64") 

include_directories(
    "${GSTREAMER_ROOT}/include/gstreamer-1.0"
    "${GSTREAMER_ROOT}/include/glib-2.0"
    "${GSTREAMER_ROOT}/lib/glib-2.0/include"
)

link_directories("${GSTREAMER_ROOT}/lib")

add_executable(ppe_server src/main.cpp)

target_link_libraries(ppe_server 
    gstreamer-1.0 
    glib-2.0 
    gobject-2.0 
    gstrtspserver-1.0
)
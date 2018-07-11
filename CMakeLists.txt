cmake_minimum_required (VERSION 3.2)
project (aruco-server VERSION 0.1)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/module)

find_package(OpenCV REQUIRED core opencv_imgcodecs opencv_highgui opencv_imgproc opencv_videoio opencv_aruco)
if (NOT OpenCV_FOUND)
    message(FATAL_ERROR "Could not find OpenCV")
endif()

find_package(redisimagehelper)
find_package(hiredis)
find_package(RapidJSON)
find_package(ev)

set(ARUCO_DETECTION_SERVER
        src/server.cpp)
add_executable(aruco-server ${ARUCO_DETECTION_SERVER})
target_link_libraries(aruco-server ${OpenCV_LIBS})

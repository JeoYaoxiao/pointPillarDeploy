CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
message(${CMAKE_SOURCE_DIR})

project(pointPillar)

add_compile_options(-O3)

include_directories(${CMAKE_SOURCE_DIR}/platform/${PTF}/)
aux_source_directory(${CMAKE_SOURCE_DIR}/platform/${PTF}/ PLATFORM_SRC)

set(INFERENCE_DIR ${CMAKE_SOURCE_DIR}/inference)
set(LIDAR_CORE_DIR ${CMAKE_SOURCE_DIR}/src)
set(PROCESS_DIR ${CMAKE_SOURCE_DIR}/process)
set(UTILS_DIR ${CMAKE_SOURCE_DIR}/utils)

set(CMAKE_CXX_FLAGS "-O3 -std=c++17 -fopenmp -Wno-error=deprecated-declarations -Wno-error=register -Wno-deprecated-declarations ")

# inference
message(STATUS "**${INFERENCE_DIR}**")
include_directories(${INFERENCE_DIR})
aux_source_directory(${INFERENCE_DIR} INFERENCE_SRC)

set(BSNN_LIB bsnn)
set(TFLITE_LIB tensorflowlite)
include_directories(${CMAKE_SOURCE_DIR}/bst/lib/include)
link_directories(${CMAKE_SOURCE_DIR}/bst/lib/lib64/)

# lidar core src
include_directories(${LIDAR_CORE_DIR})
aux_source_directory(${LIDAR_CORE_DIR} LIDAR_CORE_SRC)

# process
include_directories(${PROCESS_DIR})
aux_source_directory(${PROCESS_DIR} PROCESS_SRC)

# post process
include_directories(${CMAKE_SOURCE_DIR}/post_process/include)
link_directories(${CMAKE_SOURCE_DIR}/post_process/lib)

# utils
include_directories(${UTILS_DIR})
aux_source_directory(${UTILS_DIR} UTILS_SRC)

set(MAIN_SRC ${CMAKE_SOURCE_DIR}/main.cpp)

add_library(pointPillar_shared SHARED ${MAIN_SRC} ${GTEST_SRC} ${INFERENCE_SRC} ${LIDAR_CORE_SRC} ${PROCESS_SRC} ${COMMUNICATION_SRC} ${UTILS_SRC})

add_executable(pointPillar ${MAIN_SRC} ${GTEST_SRC} ${INFERENCE_SRC} ${LIDAR_CORE_SRC} ${PROCESS_SRC} ${COMMUNICATION_SRC} ${UTILS_SRC})

set(DEPEND_LIBRARY pthread m post_lidar ${BSNN_LIB} ${TFLITE_LIB})
message("**DEPEND_LIBRARY:${DEPEND_LIBRARY}**")

target_link_libraries(pointPillar ${DEPEND_LIBRARY})
target_link_libraries(pointPillar_shared ${DEPEND_LIBRARY})


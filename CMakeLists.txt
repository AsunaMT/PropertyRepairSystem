cmake_minimum_required(VERSION 3.0) # cmake .. -G "Unix Makefiles"
project(BOOKMARK)

# set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_FLAGS "-O3")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/include")

aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/src/src" DIR_SRCS)
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/src/src/controller" DIR_SRCS)
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/src/src/data" DIR_SRCS)
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/src/src/data/comment_sys" DIR_SRCS)
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/src/src/data/complaint_sys" DIR_SRCS)
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}./src/src/data/repair_sys" DIR_SRCS)
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/src/src/data/role" DIR_SRCS)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(test)
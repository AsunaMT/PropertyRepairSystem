cmake_minimum_required(VERSION 3.0)
project(GTESTWIN_TEST)

# set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_FLAGS "-O3")
add_subdirectory(./googletest-release-1.8.1)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/googletest-release-1.8.1/googletest/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/googletest-release-1.8.1/googlemock/include)

include_directories("../src/include")
aux_source_directory("../src/src" SRCS)
aux_source_directory("../src/src/controller" SRCS)
aux_source_directory("../src/src/data" SRCS)
aux_source_directory("../src/src/data/comment_sys" SRCS)
aux_source_directory("../src/src/data/complaint_sys" SRCS)
aux_source_directory("../src/src/data/repair_sys" SRCS)
aux_source_directory("../src/src/data/role" SRCS)
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}" TEST_SRCS)

add_executable(repair_sys_test ${DIR_SRCS} ${TEST_SRCS})

target_link_libraries(repair_sys_test
        PRIVATE
        gtest
        gtest_main
        gmock
        gmock_main
        pthread)

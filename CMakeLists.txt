cmake_minimum_required (VERSION 3.6.1)
project(pandemic-hoya-2002)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_COMPILER "g++")
add_compile_options(-g)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

get_filename_component(cadmium "../cadmium/json" REALPATH)

if(EXISTS "${cadmium}")
    message("Not found")
    include_directories(../cadmium/include)
    include_directories(../cadmium/json/include)
else()
    message("Found cadmium")
    include_directories(../../cadmium/include)
    include_directories(../../cadmium/json/include)
endif()

set(Boost_USE_MULTITHREADED TRUE)
find_package(Boost COMPONENTS unit_test_framework system thread REQUIRED)

file(MAKE_DIRECTORY logs)

add_executable(hoya src/main.cpp model/cells/zhong_cell.hpp model/zhong_coupled.hpp model/cells/vicinity.hpp model/cells/sir.hpp model/cells/simulation_configuration.hpp)

target_link_libraries(hoya PUBLIC ${Boost_LIBRARIES} -lstdc++fs)

cmake_minimum_required(VERSION 3.12)
project(mesh_voxelization)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenMP REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(HDF5 REQUIRED COMPONENTS C CXX)
find_package(Boost REQUIRED COMPONENTS filesystem program_options)

include_directories(${EIGEN3_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${HDF5_INCLUDE_DIRS})
include_directories(include)

add_executable(voxelize
    src/voxelize.cpp
    src/off_loader.cpp
    src/voxelization.cpp
)

target_link_libraries(voxelize
    ${Boost_LIBRARIES}
    ${HDF5_LIBRARIES}
    OpenMP::OpenMP_CXX
)

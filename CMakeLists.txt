cmake_minimum_required(VERSION 2.8)
set(CMAKE_SUPPRESS_REGENERATION true)
project(experimento-3d)

set(VTK_DIR "C:/libs/VTK-7.0.0-64bits")
set(ITK_DIR "C:/libs/InsightToolkit-4.8.1-64bits")
# Find VTK
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
# Find ITK
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

file(GLOB my_source_files "*.h" "*.cpp" "*.cxx")
add_executable(experimento-3d ${my_source_files} )  
#bota vtk e ITK
target_link_libraries(experimento-3d  ${VTK_LIBRARIES} ${ITK_LIBRARIES})

include_directories(C:/libs/spdlog/include/spdlog)

#suprime o warning chato dos infernos que a ITK gera por usar funções inseguras da api do windows
add_definitions(-D_SCL_SECURE_NO_WARNINGS)



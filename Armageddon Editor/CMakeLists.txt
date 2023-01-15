cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_PROJECT_VERSION 2)
set(CMAKE_PROJECT_VERSION_MAJOR 0)
set(CMAKE_PROJECT_VERSION_MINOR 0)
set(CMAKE_PROJECT_VERSION_PATCH 0)

project(Armageddon-Editor)

find_package(Vulkan REQUIRED)
include_directories("$ENV{VULKAN_SDK}/Include")


# add src files
#file(GLOB_RECURSE SOURCE_FILES 
#	${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
#	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
	
# add header files
#file(GLOB_RECURSE HEADER_FILES 
#	${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
#	${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)

# For debug purpose (TODO: remove this)
set(SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/Editor.cpp")
set(HEADER_FILES)


# Include files 
include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/src"
)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../Armageddon\ 2.0/" "${CMAKE_CURRENT_SOURCE_DIR}/lib" )


set(LIBS Armageddon-2.0 spdlog) # create LIBS var for all librairies that we use (TODO: see with visual studio with shared libs)

include_directories(includes/imgui-docking)
include_directories(includes/ImGuiFileDialog)
include_directories(includes/ImGuizmo)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Armageddon\ 2.0/src/)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${LIBS})


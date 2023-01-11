cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_PROJECT_VERSION 2)
set(CMAKE_PROJECT_VERSION_MAJOR 0)
set(CMAKE_PROJECT_VERSION_MINOR 0)
set(CMAKE_PROJECT_VERSION_PATCH 0)
set(NAME Armageddon-2.0)

project(Armageddon-2.0)


# Checking for vulkan
IF (NOT Vulkan_FOUND)
		find_library(Vulkan_LIBRARY NAMES vulkan HINTS "$ENV{VULKAN_SDK}/lib" "${CMAKE_SOURCE_DIR}/libs/vulkan" REQUIRED)
		IF (Vulkan_LIBRARY)
			set(Vulkan_FOUND ON)
			MESSAGE("Using bundled Vulkan library version")
		ENDIF()
	ENDIF()

# Finding the package 
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

#Temporary for debugging purpose
set(SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/Application.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/Log.cpp")
set(HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/Application.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/Log.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/Main.h")

# Include files 
include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/src"
)


# disable things that we don't want

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)

# build gml & glfw & assimp
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/includes/glfw/")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/includes/glm/")
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/includes/assimp/")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/includes/spdlog/")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/includes/yaml-cpp/")


# he doesn't like my craziness  
# add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/includes/entt/")


set(LIBS glm glfw assimp entt imgui spdlog yaml-cpp) # create LIBS var for all librairies that we use (TODO: see with visual studio with shared libs)

 if(WIN32) 
	include_directories(includes/DirectXTex)
endif()
	

# Includes files that aren't libs for our project.. 
include_directories(includes/imgui-docking)
include_directories(includes/spdlog/include)
include_directories(includes/ImGuizmo)
include_directories(includes/PhysX/physx/include)
include_directories(includes/PhysX/pxshared/include)
include_directories(includes/assimp/include)


#add_library(Armageddon-2.0 SHARED ${LIBRARY_SOURCE})
add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
#target_link_libraries(${PROJECT_NAME} ${LIBS})

#Executable ? 
#add_executable(Armageddon_test_library)
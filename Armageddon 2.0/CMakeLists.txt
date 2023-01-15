cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_PROJECT_VERSION 2)
set(CMAKE_PROJECT_VERSION_MAJOR 0)
set(CMAKE_PROJECT_VERSION_MINOR 0)
set(CMAKE_PROJECT_VERSION_PATCH 0)
set(NAME Armageddon-2.0)

project(Armageddon-2.0)

MESSAGE("Making Armageddon-2.O library")

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


# add ImGui src & header files
set(IMGUI_SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/includes/imgui-docking")

set(IMGUI_BASE_SOURCE_FILES "${IMGUI_SOURCE_PATH}/imgui.cpp" "${IMGUI_SOURCE_PATH}/imgui_draw.cpp" "${IMGUI_SOURCE_PATH}/imgui_tables.cpp"
"${IMGUI_SOURCE_PATH}/imgui_widgets.cpp")	

set(IMGUI_BASE_HEADER_FILES "${IMGUI_SOURCE_PATH}/imgui.h"
"${IMGUI_SOURCE_PATH}/imconfig.h" 
"${IMGUI_SOURCE_PATH}/imgui_internal.h"
"${IMGUI_SOURCE_PATH}/imstb_rectpack.h"
"${IMGUI_SOURCE_PATH}/imstb_textedit.h"
"${IMGUI_SOURCE_PATH}/imstb_truetype.h")

# "dx11" only if win32
set(IMGUI_BACKENDS_SOURCES_FILES)
set(IMGUI_BACKENDS_HEADER_FILES)
set(IMGUI_BACKENDS "glfw" "vulkan")
foreach(ITEM IN LISTS IMGUI_BACKENDS)
  set(IMGUI_BACKENDS_SOURCES_FILES ${IMGUI_BACKENDS_SOURCES_FILES} "${IMGUI_SOURCE_PATH}/backends/imgui_impl_${ITEM}.cpp")
  set(IMGUI_BACKENDS_HEADER_FILES ${IMGUI_BACKENDS_HEADER_FILES} "${IMGUI_SOURCE_PATH}/backends/imgui_impl_${ITEM}.h")
endforeach()

set(IMGUI_SOURCE_FILES ${IMGUI_BASE_SOURCE_FILES} ${IMGUI_BACKENDS_SOURCES_FILES})
set(IMGUI_HEADER_FILES ${IMGUI_BASE_HEADER_FILES} ${IMGUI_BACKENDS_HEADER_FILES})


# TODO Separate folder for windows and linux to link specific file when it's differnt os

#Temporary for debugging purpose
set(SRC_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/Application.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/Log.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/Window/Window.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/Window/GlfwWindow.cpp" 
"${CMAKE_CURRENT_SOURCE_DIR}/src/Window/Events/KeyBoardEvents.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/Window/Events/MouseEvent.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/Window/KeyBoard.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/RendererAPI.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/Renderer.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/Vulkan/VulkanRenderer.cpp"
)
set(SRC_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/Application.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/Log.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/Main.h"
"${CMAKE_CURRENT_SOURCE_DIR}/src/Window/Window.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/Window/GlfwWindow.h" 
"${CMAKE_CURRENT_SOURCE_DIR}/src/Window/WindowInterface.h"
"${CMAKE_CURRENT_SOURCE_DIR}/src/Window/Events/Event.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/Window/Events/KeyBoardEvents.h"
"${CMAKE_CURRENT_SOURCE_DIR}/src/Window/Events/MouseEvent.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/Window/KeyBoard.h"
"${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/RendererAPI.h"
"${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/Renderer.h"
"${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/Vulkan/VulkanRenderer.h"
)

set(SOURCE_FILES ${SRC_SOURCE_FILES} ${IMGUI_SOURCE_FILES} )
set(HEADER_FILES ${SRC_HEADER_FILES} ${IMGUI_HEADER_FILES} )



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


set(LIBS glm glfw ${Vulkan_LIBRARIES}) # create LIBS var for all librairies that we use (TODO: see with visual studio with shared libs)

 if(WIN32) 
	include_directories(includes/DirectXTex)
endif()
	

# Includes files that aren't libs for our project.. 
include_directories(includes/imgui-docking)
include_directories(includes/ImGuizmo)

include_directories(includes/spdlog/include)
include_directories(includes/PhysX/physx/include)
include_directories(includes/PhysX/pxshared/include)
include_directories(includes/assimp/include)


#add_library(Armageddon-2.0 SHARED ${LIBRARY_SOURCE})
add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${LIBS})

#Executable ? 
#add_executable(Armageddon_test_library)
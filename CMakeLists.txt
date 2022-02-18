cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
cmake_policy(VERSION 2.8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(NAME Armageddon-2.0)
include_directories(external)

#shared library link ? 
message(Building \ Armageddon \ Library)
add_subdirectory(Armageddon\ 2.0)
message(Building \ The \ editor)
add_subdirectory(Armageddon\ Editor)


#Pour linux : 
IF (NOT Vulkan_FOUND)
		find_library(Vulkan_LIBRARY NAMES vulkan HINTS "$ENV{VULKAN_SDK}/lib" "${CMAKE_SOURCE_DIR}/libs/vulkan" REQUIRED)
		IF (Vulkan_LIBRARY)
			set(Vulkan_FOUND ON)
			MESSAGE("Using bundled Vulkan library version")
		ENDIF()
	ENDIF()
#Check si on est sur windows
#IF(WIN32)

#Sinon on est sur linux
#ELSEIF(LINUX)
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
cmake_policy(VERSION 2.8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(NAME Armageddon-2.0)

include_directories(external)

#shared library link ? 
message(Building \ Armageddon \ Library)
add_subdirectory("${CMAKE_SOURCE_DIR}/Armageddon\ 2.0/")

message(Building \ The \ editor)
add_subdirectory("${CMAKE_SOURCE_DIR}/Armageddon\ Editor/")

message(Building Runtime template)
add_subdirectory("${CMAKE_SOURCE_DIR}/Armageddon-Runtime/")


#Check si on est sur windows
#IF(WIN32)

#Sinon on est sur linux
#ELSEIF(LINUX)

# Install script for directory: /home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/llvm-objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimpd.so.5.0.1"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimpd.so.5"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon Editor/lib/includes/assimp/bin/libassimpd.so.5.0.1"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon Editor/lib/includes/assimp/bin/libassimpd.so.5"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimpd.so.5.0.1"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimpd.so.5"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/llvm-strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimpd.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimpd.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimpd.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon Editor/lib/includes/assimp/bin/libassimpd.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimpd.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimpd.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/llvm-strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimpd.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/anim.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/aabb.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/ai_assert.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/camera.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/color4.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/color4.inl"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon Editor/lib/includes/assimp/code/../include/assimp/config.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/ColladaMetaData.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/commonMetaData.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/defs.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/cfileio.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/light.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/material.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/material.inl"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/matrix3x3.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/matrix3x3.inl"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/matrix4x4.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/matrix4x4.inl"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/mesh.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/pbrmaterial.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/postprocess.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/quaternion.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/quaternion.inl"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/scene.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/metadata.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/texture.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/types.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/vector2.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/vector2.inl"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/vector3.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/vector3.inl"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/version.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/cimport.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/importerdesc.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/Importer.hpp"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/DefaultLogger.hpp"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/ProgressHandler.hpp"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/IOStream.hpp"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/IOSystem.hpp"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/Logger.hpp"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/LogStream.hpp"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/NullLogger.hpp"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/cexport.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/Exporter.hpp"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/DefaultIOStream.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/DefaultIOSystem.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/ZipArchiveIOSystem.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/SceneCombiner.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/fast_atof.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/qnan.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/BaseImporter.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/Hash.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/MemoryIOWrapper.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/ParsingUtils.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/StreamReader.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/StreamWriter.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/StringComparison.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/StringUtils.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/SGSpatialSort.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/GenericProperty.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/SpatialSort.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/SkeletonMeshBuilder.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/SmallVector.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/SmoothingGroups.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/SmoothingGroups.inl"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/StandardShapes.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/RemoveComments.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/Subdivision.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/Vertex.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/LineSplitter.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/TinyFormatter.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/Profiler.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/LogAux.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/Bitmap.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/XMLTools.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/IOStreamBuffer.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/CreateAnimMesh.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/XmlParser.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/BlobIOSystem.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/MathFunctions.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/Exceptional.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/ByteSwapper.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/Compiler/pushpack1.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/Compiler/poppack1.h"
    "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/assimp/code/../include/assimp/Compiler/pstdint.h"
    )
endif()


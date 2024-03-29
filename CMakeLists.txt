cmake_minimum_required(VERSION 3.9)

project(
  specni
  VERSION 0.1
  DESCRIPTION "An astrology app"
  LANGUAGES C CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(CMAKE_CXX_EXTENSIONS OFF)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  include(CTest)

  find_package(Doxygen)
  if(Doxygen_FOUND)
    add_subdirectory(docs)
  else()
    message(STATUS "Doxygen not found, not building docs")
  endif()
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-fno-omit-frame-pointer")
set(CMAKE_C_FLAGS_DEBUG "-fno-omit-frame-pointer")

set(OpenGL_GL_PREFERENCE GLVND)
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_subdirectory(deps)
add_subdirectory(src)

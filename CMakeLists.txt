cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(OpenGLTemplate CXX)

#===========================================================================================
# GLAD CONFIGURATION
#
# Configure GLAD loader generation variables (lines below pulled from the GLAD top-level
# CMakeLists.txt file, with slight modifications.
# Refer to https://github.com/Dav1dde/glad for more information)

set(GLAD_OUT_DIR        "${CMAKE_CURRENT_BINARY_DIR}" CACHE STRING "Output directory" FORCE)
set(GLAD_PROFILE        "core" CACHE STRING "OpenGL profile" FORCE)
set(GLAD_API            "gl=4.1" CACHE STRING "API type/version pairs, like \"gl=3.2,gles=\", no version means latest" FORCE)
set(GLAD_GENERATOR      "c" CACHE STRING "Language to generate the binding for" FORCE)
set(GLAD_EXTENSIONS     "" CACHE STRING "Path to extensions file or comma separated list of extensions, if missing all extensions are included" FORCE)
set(GLAD_SPEC           "gl" CACHE STRING "Name of the spec" FORCE)
set(GLAD_ALL_EXTENSIONS OFF CACHE BOOL "Include all extensions instead of those specified by GLAD_EXTENSIONS" FORCE)
set(GLAD_NO_LOADER      OFF CACHE BOOL "No loader" FORCE)
set(GLAD_REPRODUCIBLE   OFF CACHE BOOL "Reproducible build" FORCE)

#===========================================================================================

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set directory paths
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

file(GLOB_RECURSE SRCS ${SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HDRS ${SOURCE_DIR}/*.h)
set(sources ${SRCS})
set(includes ${HDRS})

add_executable(planet-gen ${sources} ${includes})

add_custom_target(
        copy_shader_files
        ${CMAKE_COMMAND}
        -D SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
        -D DESTINATION_DIR=${CMAKE_CURRENT_BINARY_DIR} 
        -P ${CMAKE_DIR}/CopyShaders.cmake
        COMMENT "Copying Files for target: planet-gen" VERBATIM 
        )

add_dependencies (planet-gen copy_shader_files)

# Perform dependency linkage
include(${CMAKE_DIR}/LinkGLFW.cmake)
LinkGLFW(planet-gen PRIVATE)

include(${CMAKE_DIR}/LinkGLAD.cmake)
LinkGLAD(planet-gen PRIVATE)

include(${CMAKE_DIR}/LinkIMGUI.cmake)
LinkIMGUI(planet-gen PRIVATE)

include(${CMAKE_DIR}/LinkGLM.cmake)
LinkGLM(planet-gen PRIVATE)

include(${CMAKE_DIR}/LinkDate.cmake)
LinkDate(planet-gen PRIVATE)

find_package(OpenGL REQUIRED)
if (OpenGL_FOUND)
    target_include_directories(planet-gen PRIVATE ${OPENGL_INCLDUE_DIRS})
    target_link_libraries(planet-gen PRIVATE ${OPENGL_LIBRARIES})
else()
    message(FATAL_ERROR "OpenGL was not found on the system")
endif()

# Enable C++20
set_target_properties(planet-gen PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)

# Set project folders
set_target_properties(planet-gen PROPERTIES FOLDER ${PROJECT_NAME})

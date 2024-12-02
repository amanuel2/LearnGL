# CMakeList.txt : CMake project for LearnGL, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("LearnGL")

# GLFW
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw-3.4)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/glm)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/glad/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw-3.4/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/glm)

# Add source to this project's executable.
add_executable (LearnGL ${CMAKE_CURRENT_SOURCE_DIR}/lib/glad/src/glad.c "LearnGL.cpp"  "Triangle.h"  "Shader.h" "Texture.h" "Transforms.h" "Vertices.h" "Camera.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET LearnGL PROPERTY CXX_STANDARD 20)
endif()

# Link GLFW library 
target_link_libraries(LearnGL glfw opengl32)

# TODO: Add tests and install targets if needed

add_subdirectory("Lighting")

cmake_minimum_required(VERSION 3.10)

project(OpenGLLearning VERSION 1.0 LANGUAGES C CXX)

include_directories(include libs)

file(GLOB SRC "${CMAKE_SOURCE_DIR}/src/*.cpp" "${CMAKE_SOURCE_DIR}/src/*/*.cpp" "${CMAKE_SOURCE_DIR}/src/*/*.c")

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

add_compile_options(-std=c++17)
add_executable(OpenGLLearning ${SRC})
add_custom_command(TARGET OpenGLLearning
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                           ${CMAKE_SOURCE_DIR}/shaders
                           ${CMAKE_CURRENT_BINARY_DIR}/shaders)
add_custom_command(TARGET OpenGLLearning
                  COMMAND ${CMAKE_COMMAND} -E copy_directory
                          ${CMAKE_SOURCE_DIR}/model
                          ${CMAKE_CURRENT_BINARY_DIR}/model)
target_include_directories(OpenGLLearning PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
                           $<INSTALL_INTERFACE:include>)
target_link_libraries(OpenGLLearning PUBLIC assimp glfw OpenGL::GL ${CMAKE_DL_LIBS})

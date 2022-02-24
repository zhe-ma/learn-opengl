cmake_minimum_required(VERSION 3.1.0)

project(LearnOpenGL)

# C++ standard requirements.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# CMake 3.1.0+ required.
# See: https://stackoverflow.com/a/29871891
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

################################################################################
# Third party libraries

# glfw
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(3rdparty/glfw)

# glad
add_subdirectory(3rdparty/glad)

# assimp
option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(3rdparty/assimp)

# bullet
option(BUILD_BULLET2_DEMOS OFF)
option(BUILD_CPU_DEMOS OFF)
option(BUILD_EXTRAS OFF)
option(BUILD_OPENGL3_DEMOS OFF)
option(BUILD_UNIT_TESTS OFF)
add_subdirectory(3rdparty/bullet)

if(NOT WIN32)
    set(GLAD_LIBRARIES dl)
endif()
include_directories(3rdparty/
                    3rdparty/assimp/include/
                    3rdparty/bullet/src/
                    3rdparty/glad/include/
                    3rdparty/glfw/include/
                    3rdparty/glm/
                    3rdparty/spdlog/include)

################################################################################

set(RESOURCE_DIR "${CMAKE_SOURCE_DIR}/res/")
configure_file(config/globals.h.in config/globals.h)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}  # for configure_file
    )

add_subdirectory(src/learnopengl)

# target_link_libraries(${PROJECT_NAME} assimp glfw
#                       ${GLFW_LIBRARIES} ${GLAD_LIBRARIES}
#                       BulletDynamics BulletCollision LinearMath)


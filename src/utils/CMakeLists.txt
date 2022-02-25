file(GLOB SRCS
        RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

set(TARGET_NAME utils)

ADD_LIBRARY(${TARGET_NAME} STATIC ${SRCS})

set(LIBS
        glfw
        glad
        ${GLFW_LIBRARIES}
        "${CMAKE_THREAD_LIBS_INIT}"
        )

target_link_libraries(${TARGET_NAME} ${LIBS})
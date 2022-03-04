set(LIBS
    glfw
    glad
    utils
    ${GLFW_LIBRARIES}
    "${CMAKE_THREAD_LIBS_INIT}"
    )

add_executable(hello-window 01-hello-window.cc)
target_link_libraries(hello-window ${LIBS})

add_executable(simple-triangle 02-simple-triangle.cc)
target_link_libraries(simple-triangle ${LIBS})

add_executable(test test.cpp)
target_link_libraries(test ${LIBS})

add_executable(1.1camera_circle 1.getting_started/1.1camera_circle.cpp)
target_link_libraries(1.1camera_circle ${LIBS})

add_executable(1.2texture 1.getting_started/1.2texture.cpp)
target_link_libraries(1.2texture ${LIBS})

add_executable(1.3fps_camera 1.getting_started/1.3fps_camera.cpp)
target_link_libraries(1.3fps_camera ${LIBS})

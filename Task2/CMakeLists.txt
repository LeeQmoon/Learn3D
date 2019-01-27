include_directories(${LEARN3D_INCLUDE_DIR})

add_executable(Task2
  ${LEARN3D_LIBRARY_DIR}/glad.c
  opengl.cpp
)
target_link_libraries(Task2
  ${LEARN3D_LIBRARY_DIR}/glfw3.lib
)

message("${EXECUTABLE_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}")


foreach(configure_type ${CMAKE_CONFIGURATION_TYPES})
  install(TARGETS Task2
    RUNTIME DESTINATION ${EXECUTABLE_OUTPUT_PATH}/${configure_type}
    CONFIGURATIONS ${configure_type}
  )
  install(
    FILES container.jpg fshaderOne.txt fshaderTwo.txt vshader.txt awesomeface.png
    DESTINATION ${EXECUTABLE_OUTPUT_PATH}/${configure_type}
    CONFIGURATIONS ${configure_type}
  )
endforeach(configure_type)

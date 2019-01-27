project(Learn3D)
cmake_minimum_required(VERSION 2.4.7)

macro(check_path dir)
  if(EXISTS ${dir})
    message(FATAL_ERROR "${dir} is not exist.")
  endif(EXISTS ${dir})
endmacro(check_path)

#search header for learn3D
if(NOT EXISTS ${LEARN3D_INCLUDE_DIR})
  if(EXISTS ${PROJECT_SOURCE_DIR}/3rd/include)
    set(LEARN3D_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/3rd/include CACHE PATH "learn3D include dir" FORCE)
    message("set learn3D include dir:${PROJECT_SOURCE_DIR}/3rd/include")
  endif(EXISTS ${PROJECT_SOURCE_DIR}/3rd/include)
endif(NOT EXISTS ${LEARN3D_INCLUDE_DIR})

#search library for learn3D
if(NOT EXISTS ${LEARN3D_LIBRARY_DIR})
  if(EXISTS ${PROJECT_SOURCE_DIR}/3rd/libs)
    set(LEARN3D_LIBRARY_DIR ${PROJECT_SOURCE_DIR}/3rd/libs CACHE PATH "learn3d library dir" FORCE)
    message("set learn3D library dir:${PROJECT_SOURCE_DIR}/3rd/libs")
  endif(EXISTS ${PROJECT_SOURCE_DIR}/3rd/libs)
endif(NOT EXISTS ${LEARN3D_LIBRARY_DIR})

check_path(LEARN3D_INCLUDE_DIR)
check_path(LEARN3D_LIBRARY_DIR)

add_subdirectory(Task2)

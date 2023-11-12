cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME "Space Invaders")
set(PROJECT_VERSION "1.0")
set(FORCE_C99 ON)

set(EXECUTABLE_NAME "spaceinvaders")

# ::: SOURCES :::
set(SOURCES "src/main.c")

list(APPEND SOURCES 
    "src/engine/engine.c"
    "src/engine/util/dynamic-array.c"
)

list(APPEND SOURCES "deps/glad/src/glad.c")


# ::: INCLUDES :::
set(INCLUDE_PATHS 
    "deps/glfw/include"
    "deps/glad/include"
    "deps/cglm/include"
)

# ::: SUBDIRECTORIES :::
set(SUBDIRECTORIES
    "deps/glfw"
    "deps/cglm"
)

# ::: LIBRARIES :::
set(LIBRARIES "glfw" "cglm")

# ::: CONFIG :::
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)


# -------------------------------
# |          COMMANDS           |
# -------------------------------

project(${PROJECT_NAME} LANGUAGES C VERSION ${PROJECT_VERSION})

if (FORCE_C99)
    set(CMAKE_C_STANDARD_REQUIRED ON)
    set(CMAKE_C_STANDARD 99)
endif()

add_executable(${EXECUTABLE_NAME} ${SOURCES})

foreach(INCLUDE_PATH ${INCLUDE_PATHS})
    include_directories(${EXECUTABLE_NAME} ${INCLUDE_PATH})
endforeach(INCLUDE_PATH ${INCLUDE_PATHS})


foreach(SUBDIR ${SUBDIRECTORIES})
    add_subdirectory(${SUBDIR})
endforeach(SUBDIR ${SUBDIRECTORIES})


foreach(LIB ${LIBRARIES})
    target_link_libraries(${EXECUTABLE_NAME} ${LIB})
endforeach(LIB ${LIBRARIES})

cmake_minimum_required(VERSION 3.28)
project(BLACKJACK)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

set(SDLTTF_VENDORED ON)

add_subdirectory(${PROJECT_SOURCE_DIR}/include/external/vendored/SDL EXCLUDE_FROM_ALL)

add_subdirectory(${PROJECT_SOURCE_DIR}/include/external/vendored/SDL_ttf EXCLUDE_FROM_ALL)


#[[ Source - https://stackoverflow.com/a/52887900
Posted by J Agustin Barrachina, modified by community. See post 'Timeline' for change history
Retrieved 2026-07-31, License - CC BY-SA 4.0]]

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/event
    ${PROJECT_SOURCE_DIR}/include/game
    ${PROJECT_SOURCE_DIR}/include/input
    ${PROJECT_SOURCE_DIR}/include/render
    ${PROJECT_SOURCE_DIR}/include/ui
    ${PROJECT_SOURCE_DIR}/include/update
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/event
    ${PROJECT_SOURCE_DIR}/src/game
    ${PROJECT_SOURCE_DIR}/src/input
    ${PROJECT_SOURCE_DIR}/src/render
    ${PROJECT_SOURCE_DIR}/src/ui
    ${PROJECT_SOURCE_DIR}/src/update
)

file(GLOB SOURCES
    "${PROJECT_SOURCE_DIR}/include/*.h"
    "${PROJECT_SOURCE_DIR}/include/event/*.h"
    "${PROJECT_SOURCE_DIR}/include/game/*.h"
    "${PROJECT_SOURCE_DIR}/include/input*.h"
    "${PROJECT_SOURCE_DIR}/include/render*.h"
    "${PROJECT_SOURCE_DIR}/include/ui/*.h"
    "${PROJECT_SOURCE_DIR}/include/update*.h"
    "${PROJECT_SOURCE_DIR}/src/*.c"
    "${PROJECT_SOURCE_DIR}/src/event/*.c"
    "${PROJECT_SOURCE_DIR}/src/game/*.c"
    "${PROJECT_SOURCE_DIR}/src/input/*.c"
    "${PROJECT_SOURCE_DIR}/src/render/*.c"
    "${PROJECT_SOURCE_DIR}/src/ui/*.c"
    "${PROJECT_SOURCE_DIR}/src/update/*.c"
)

add_executable(main ${SOURCES})
target_link_libraries(main PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf)

file(COPY "${PROJECT_SOURCE_DIR}/resources" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
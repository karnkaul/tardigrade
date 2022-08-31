cmake_minimum_required(VERSION 3.18)

project(tardigrade-example)

if(NOT TARGET vulkify)
  include(FetchContent)
  FetchContent_Declare(
    vulkify
    GIT_REPOSITORY https://github.com/vulkify/vulkify
    GIT_TAG c367190
    GIT_SHALLOW TRUE
  )
endif()

FetchContent_MakeAvailable(vulkify)

if(NOT TARGET tardigrade)
  find_package(tardigrade CONFIG REQUIRED)
endif()

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE example.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE vulkify::vulkify tardigrade::tardigrade)

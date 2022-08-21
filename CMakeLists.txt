cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

enable_language(CXX)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_DEBUG_POSTFIX "-d")
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()

include(CheckPIESupported)
check_pie_supported()

project(tardigrade)

set(is_root_project OFF) # indicate if this is the top-level project

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(is_root_project ON)
  message(STATUS "${PROJECT_NAME} is top level")
endif()

add_library(${PROJECT_NAME})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} ${system} PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_sources(${PROJECT_NAME} PRIVATE
  include/tardigrade/detail/delta_time.hpp
  include/tardigrade/detail/ptr.hpp
  include/tardigrade/detail/render_list.hpp
  include/tardigrade/detail/type_id_hasher.hpp
  include/tardigrade/detail/type_id.hpp

  include/tardigrade/attachment.hpp
  include/tardigrade/director.hpp
  include/tardigrade/entity.hpp
  include/tardigrade/render_attachment.hpp
  include/tardigrade/render_target.hpp
  include/tardigrade/scene.hpp
  include/tardigrade/services.hpp
  include/tardigrade/tick_attachment.hpp
  include/tardigrade/to_string.hpp

  src/attachment.cpp
  src/director.cpp
  src/entity.cpp
  src/scene.cpp
)

if(CMAKE_CXX_COMPILER_ID STREQUAL Clang OR CMAKE_CXX_COMPILER_ID STREQUAL GNU)
  target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall -Wextra -Wpedantic -Wconversion -Werror=return-type
  )
endif()

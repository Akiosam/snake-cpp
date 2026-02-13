cmake_minimum_required(VERSION 3.22)

project(snake_cpp
  VERSION 0.1.0
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# ---- function: apply strict warnings to a target ----
function(snake_enable_warnings target)
  if(MSVC)
    target_compile_options(${target} PRIVATE /W4 /WX /permissive-)
  else()
    target_compile_options(${target} PRIVATE
      -Wall -Wextra -Wpedantic -Werror
      -Wshadow -Wconversion -Wsign-conversion
      -Wnon-virtual-dtor -Wold-style-cast
      -Woverloaded-virtual -Wnull-dereference
    )
  endif()
endfunction()

# ---- targets (empty for now; we'll add files next) ----
add_executable(snake
  src/main.cpp
  src/game.cpp
  src/qt/game_widget.cpp
  src/qt/qt_input.cpp
)
target_include_directories(snake PRIVATE src src/qt)
target_link_libraries(snake PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
snake_enable_warnings(snake)

# Put the built app binary in the repo root.
set_target_properties(snake PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)
foreach(cfg IN ITEMS Debug Release RelWithDebInfo MinSizeRel)
  string(TOUPPER "${cfg}" cfg_uc)
  set_target_properties(snake PROPERTIES
    "RUNTIME_OUTPUT_DIRECTORY_${cfg_uc}" "${CMAKE_SOURCE_DIR}"
  )
endforeach()

enable_testing()
add_executable(snake_tests
  src/tests_main.cpp
)
snake_enable_warnings(snake_tests)
add_test(NAME snake_tests COMMAND snake_tests)

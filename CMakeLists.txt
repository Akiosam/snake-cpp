cmake_minimum_required(VERSION 3.22)

project(snake_cpp
  VERSION 0.1.0
  LANGUAGES CXX
)

include(GNUInstallDirs)

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
set(SNAKE_APP_SOURCES
  src/main.cpp
  src/game.cpp
  src/qt/game_widget.cpp
  src/qt/qt_input.cpp
)

if(WIN32)
  add_executable(snake WIN32 ${SNAKE_APP_SOURCES})
else()
  add_executable(snake ${SNAKE_APP_SOURCES})
endif()

target_include_directories(snake PRIVATE src src/qt)
target_link_libraries(snake PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
snake_enable_warnings(snake)

# Put built binaries in a stable bin directory.
set_target_properties(snake PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
foreach(cfg IN ITEMS Debug Release RelWithDebInfo MinSizeRel)
  string(TOUPPER "${cfg}" cfg_uc)
  set_target_properties(snake PROPERTIES
    "RUNTIME_OUTPUT_DIRECTORY_${cfg_uc}" "${CMAKE_BINARY_DIR}/bin"
  )
endforeach()

enable_testing()
add_executable(snake_tests
  src/tests_main.cpp
)
snake_enable_warnings(snake_tests)

set_target_properties(snake_tests PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
foreach(cfg IN ITEMS Debug Release RelWithDebInfo MinSizeRel)
  string(TOUPPER "${cfg}" cfg_uc)
  set_target_properties(snake_tests PROPERTIES
    "RUNTIME_OUTPUT_DIRECTORY_${cfg_uc}" "${CMAKE_BINARY_DIR}/bin"
  )
endforeach()

add_test(NAME snake_tests COMMAND snake_tests)

install(TARGETS snake snake_tests
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6 AND COMMAND qt_generate_deploy_app_script)
  qt_generate_deploy_app_script(
    TARGET snake
    OUTPUT_SCRIPT snake_deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
  )
  install(SCRIPT ${snake_deploy_script})
endif()

set(CPACK_PACKAGE_NAME "snake-cpp")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_FILE_NAME
  "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}"
)
if(WIN32)
  set(CPACK_GENERATOR "ZIP")
else()
  set(CPACK_GENERATOR "TGZ")
endif()
include(CPack)

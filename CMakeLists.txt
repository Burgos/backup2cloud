cmake_minimum_required(VERSION 2.8.12)

project(Backup2Cloud)

add_definitions("-std=c++17")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(KEEP_RPATHS)

if(APPLE)
    set(CMAKE_INSTALL_RPATH "@executable_path/../lib")
else()
    set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
endif()

if (MSVC)
else()
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra -O2")
endif()

add_executable(backup2cloud src/backup2cloud.cpp)
set_property(TARGET backup2cloud PROPERTY
  MSVC_RUNTIME_LIBRARY "MultiThreaded")
set_property(TARGET backup2cloud PROPERTY CXX_STANDARD 17)
set_property(TARGET backup2cloud PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(backup2cloud ${CONAN_LIBS})

cmake_minimum_required(VERSION 3.14)

project(NanoOdbcTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
                EXPECTED_HASH SHA256=396e16d0f5eabdc6a14afddbcfff62a54a7ee75c6da23f32f7a31bc85db23484
                TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

conan_cmake_autodetect(settings)

conan_cmake_configure(REQUIRES nanodbc/2.13.0
                      GENERATORS cmake_find_package)

conan_cmake_install(PATH_OR_REFERENCE .
                    BUILD missing
                    REMOTE conancenter
                    SETTINGS ${settings})

find_package(nanodbc REQUIRED)


add_executable(NanoOdbcTest main.cpp)


if( supported )
    message(STATUS "IPO / LTO enabled")
    set_property(TARGET NanoOdbcTest PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    message(STATUS "IPO / LTO not supported: <${error}>")
endif()

target_link_libraries(${PROJECT_NAME}
    nanodbc::nanodbc)

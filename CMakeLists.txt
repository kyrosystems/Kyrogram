cmake_minimum_required(VERSION 3.14)
project(Kyrogram C)

set(CMAKE_C_STANDARD 11)

# Fix Raylib CMake Policy on macOS
if(POLICY CMP0000)
    cmake_policy(SET CMP0000 NEW)
endif()

include(FetchContent)

# 1. Загрузка Raylib
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG        5.0
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

# 2. Загрузка mbedTLS
FetchContent_Declare(
  mbedtls
  GIT_REPOSITORY https://github.com/Mbed-TLS/mbedtls.git
  GIT_TAG        v3.5.0
)
set(ENABLE_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(mbedtls)

include_directories(include)
include_directories(${raylib_SOURCE_DIR}/src)
include_directories(${mbedtls_SOURCE_DIR}/include)

# Поиск TDLib
find_library(TD_JSON_LIB NAMES tdjson tdjson_static PATHS ${CMAKE_SOURCE_DIR}/libs)

set(SOURCES 
    src/main.c
    src/security/storage.c
    src/ui/theme.c
    src/network/tdlib_wrapper.c
    src/core/settings.c
    src/core/media.c
    src/network/proxy_manager.c
)

if(NOT TD_JSON_LIB)
    message(STATUS "TDLib not found. Building with stubs for CI.")
    list(APPEND SOURCES src/network/tdlib_stub.c)
else()
    add_definitions(-DHAS_TDLIB)
endif()

add_executable(kyrogram ${SOURCES})

target_link_libraries(kyrogram raylib mbedtls mbedcrypto mbedx509)

if(TD_JSON_LIB)
    target_link_libraries(kyrogram ${TD_JSON_LIB})
endif()

# System Libraries
if (WIN32)
    target_link_libraries(kyrogram winmm gdi32 opengl32 shell32 user32)
elseif(APPLE)
    target_link_libraries(kyrogram "-framework IOKit" "-framework Cocoa" "-framework OpenGL" "-framework CoreVideo")
else()
    target_link_libraries(kyrogram m pthread dl X11)
endif()

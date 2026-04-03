cmake_minimum_required(VERSION 3.10)
project(Kyrogram C)

set(CMAKE_C_STANDARD 11)

# Optimization flags
if(MSVC)
    add_compile_options(/O2 /MT)
else()
    add_compile_options(-O3 -march=native)
endif()

# Include directories
include_directories(include)

# Source files
set(SOURCES 
    src/main.c
    src/security/storage.c
    src/ui/theme.c
    src/network/tdlib_wrapper.c
    src/core/settings.c
    src/core/media.c
    src/network/proxy_manager.c
)

# Raylib
find_package(raylib 4.0 REQUIRED)

# mbedTLS
find_package(mbedTLS REQUIRED)

# TDLib (Searching for tdjson)
find_library(TD_JSON_LIB NAMES tdjson tdjson_static REQUIRED)

# Executable
add_executable(kyrogram ${SOURCES})

# Linking
target_link_libraries(kyrogram 
    raylib 
    mbedtls mbedcrypto mbedx509 
    ${TD_JSON_LIB}
)

if (WIN32)
    target_link_libraries(kyrogram winmm gdi32 opengl32 shell32 user32)
elseif(APPLE)
    target_link_libraries(kyrogram "-framework IOKit" "-framework Cocoa" "-framework OpenGL" "-framework CoreVideo")
else()
    target_link_libraries(kyrogram m pthread dl X11)
endif()

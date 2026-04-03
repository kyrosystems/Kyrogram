cmake_minimum_required(VERSION 3.10)
project(Kyrogram C)

set(CMAKE_C_STANDARD 11)

# Options
option(USE_EXTERNAL_LIBS "Use externally installed libraries" ON)

# Include directories
include_directories(include)

# Source files
set(SOURCES 
    src/main.c
    src/security/storage.c
    src/ui/theme.c
    src/network/tdlib_wrapper.c
)

# Find Raylib
find_package(raylib 4.0 QUIET)
if (NOT raylib_FOUND)
    message(STATUS "Raylib not found, please install it or add to CMAKE_PREFIX_PATH")
endif()

# Find mbedTLS
find_package(mbedTLS QUIET)

# Executable
add_executable(kyrogram ${SOURCES})

# Linking (Platform specific adjustments)
if (WIN32)
    target_link_libraries(kyrogram raylib mbedtls mbedcrypto mbedx509 winmm gdi32 opengl32)
else()
    target_link_libraries(kyrogram raylib mbedtls mbedcrypto mbedx509 m pthread dl)
endif()

# Installation rules
install(TARGETS kyrogram DESTINATION bin)

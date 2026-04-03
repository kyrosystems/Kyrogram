cmake_minimum_required(VERSION 3.14)
project(Kyrogram C)

set(CMAKE_C_STANDARD 11)

include(FetchContent)

# 1. Загрузка Raylib (Графика)
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG        5.0
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

# 2. Загрузка mbedTLS (Шифрование)
FetchContent_Declare(
  mbedtls
  GIT_REPOSITORY https://github.com/Mbed-TLS/mbedtls.git
  GIT_TAG        v3.5.0
)
set(ENABLE_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(mbedtls)

# Настройка путей
include_directories(include)
include_directories(${raylib_SOURCE_DIR}/src)
include_directories(${mbedtls_SOURCE_DIR}/include)

# Исходники
set(SOURCES 
    src/main.c
    src/security/storage.c
    src/ui/theme.c
    src/network/tdlib_wrapper.c
    src/core/settings.c
    src/core/media.c
    src/network/proxy_manager.c
)

add_executable(kyrogram ${SOURCES})

# Линковка
target_link_libraries(kyrogram raylib mbedtls mbedcrypto mbedx509)

# Системные зависимости
if (WIN32)
    target_link_libraries(kyrogram winmm gdi32 opengl32 shell32 user32)
elseif(APPLE)
    # macOS frameworks for raylib
    target_link_libraries(kyrogram "-framework IOKit" "-framework Cocoa" "-framework OpenGL" "-framework CoreVideo")
else()
    target_link_libraries(kyrogram m pthread dl X11)
endif()

# Опциональная TDLib
find_library(TD_JSON_LIB NAMES tdjson tdjson_static PATHS ${CMAKE_SOURCE_DIR}/libs)
if(TD_JSON_LIB)
    target_link_libraries(kyrogram ${TD_JSON_LIB})
endif()

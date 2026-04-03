cmake_minimum_required(VERSION 3.10)
project(Kyrogram C)

set(CMAKE_C_STANDARD 11)

# Добавляем пути для поиска библиотек в папке проекта
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/libs")
include_directories(include)

# Исходники (Включаем cJSON для парсинга)
set(SOURCES 
    src/main.c
    src/security/storage.c
    src/ui/theme.c
    src/network/tdlib_wrapper.c
    src/core/settings.c
    src/core/media.c
    src/network/proxy_manager.c
    src/network/cJSON.c
)

# Поиск библиотек
find_package(raylib REQUIRED)
find_package(mbedtls REQUIRED)

# Поиск TDJSON (Telegram Core)
find_library(TDJSON_LIB NAMES tdjson PATHS "${CMAKE_CURRENT_SOURCE_DIR}/bin" "${CMAKE_CURRENT_SOURCE_DIR}/libs")

add_executable(kyrogram ${SOURCES})

# Линковка
if (WIN32)
    target_link_libraries(kyrogram ${TDJSON_LIB} raylib mbedtls mbedcrypto mbedx509 winmm gdi32 opengl32 shell32)
else()
    target_link_libraries(kyrogram ${TDJSON_LIB} raylib mbedtls mbedcrypto mbedx509 m pthread dl)
endif()

# Копирование бинарников после сборки (Windows)
if (WIN32)
    add_custom_command(TARGET kyrogram POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/bin/tdjson.dll"
        $<TARGET_FILE_DIR:kyrogram>)
endif()

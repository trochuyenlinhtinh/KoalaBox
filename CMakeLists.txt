cmake_minimum_required(VERSION 3.24)

project(
    KoalaBox LANGUAGES CXX
    DESCRIPTION "A box of koality tools"
)

include(cmake/KoalaBox.cmake)

# Configure variables

include(GNUInstallDirs)

set(
    KOALABOX_HEADERS
    include/koalabox/config.hpp
    include/koalabox/core.hpp
    include/koalabox/globals.hpp
    include/koalabox/logger.hpp
    include/koalabox/hook.hpp
    include/koalabox/io.hpp
    include/koalabox/loader.hpp
    include/koalabox/paths.hpp
    include/koalabox/util.hpp
    include/koalabox/win_util.hpp
)

set(
    KOALABOX_SOURCES
    src/koalabox/core.cpp
    src/koalabox/globals.cpp
    src/koalabox/hook.cpp
    src/koalabox/io.cpp
    src/koalabox/loader.cpp
    src/koalabox/ntapi.hpp
    src/koalabox/paths.cpp
    src/koalabox/util.cpp
    src/koalabox/win_util.cpp
)

add_library(KoalaBox OBJECT ${KOALABOX_HEADERS} ${KOALABOX_SOURCES})

# Configure dependencies

## Json - https://github.com/nlohmann/json
fetch_library(nlohmann_json nlohmann/json v3.11.2)

## Polyhook2 - https://github.com/stevemk14ebr/PolyHook_2_0
set(POLYHOOK_FEATURE_INLINENTD OFF)
set(POLYHOOK_FEATURE_EXCEPTION OFF)
fetch_library(PolyHook_2 stevemk14ebr/PolyHook_2_0 71d273463a3c4e30ec0a4031c4b477b85ea773fb)

## Spdlog - https://github.com/gabime/spdlog
fetch_library(spdlog gabime/spdlog v1.11.0)

# Setup KoalaBox exports
target_include_directories(
    KoalaBox PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include;${CMAKE_CURRENT_SOURCE_DIR}/src>"
    "$<INSTALL_INTERFACE:include>"
)

set(PCH_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include/koalabox/pch.hpp)
target_precompile_headers(KoalaBox PUBLIC "$<$<COMPILE_LANGUAGE:CXX>:${PCH_PATH}>")

install(
    TARGETS KoalaBox DESTINATION ${LIB_INSTALL_DIR}
    INCLUDES ${KOALABOX_HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR}
)

# Exports Generator

add_executable(exports_generator src/exports_generator/exports_generator.cpp)

target_link_libraries(exports_generator PUBLIC KoalaBox)

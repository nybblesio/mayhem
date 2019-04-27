cmake_minimum_required(VERSION 3.14)
project(mayhem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# need position indepdent code on most *nix platforms
if (UNIX AND NOT APPLE)
    set(CMAKE_CXX_FLAGS "-fPIC")
endif()

# configure ccache if available
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
    message(STATUS "${PROJECT_NAME} using ccache.")
endif()

if ("${CMAKE_C_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_C_FLAGS "-Wall" CACHE STRING "compile flags" FORCE)
    message(STATUS "Using clang flags: ${CMAKE_C_FLAGS}")
elseif ("${CMAKE_C_COMPILER_ID}" MATCHES "GNU")
    set(CMAKE_C_FLAGS "-Wall" CACHE STRING "compile flags" FORCE)
    message(STATUS "Using gcc flags: ${CMAKE_C_FLAGS}")
else ()
    message(STATUS "Unknown compiler; no custom flags set: ${CMAKE_C_COMPILER_ID}")
endif()

# libfmt
add_subdirectory(ext/fmt-5.3.0 EXCLUDE_FROM_ALL)

# utf8proc 
add_subdirectory(ext/utf8proc-2.2.0 EXCLUDE_FROM_ALL)
include_directories(ext/utf8proc-2.2.0)

# freetype
add_subdirectory(ext/freetype-2.9 EXCLUDE_FROM_ALL)
include_directories(ext/freetype-2.9/include)

# sdl2
set(SDL_SHARED OFF)
add_subdirectory(ext/SDL2-2.0.9 EXCLUDE_FROM_ALL)
include_directories(ext/SDL2-2.0.9/include)

# sdl2_ttf
add_subdirectory(ext/SDL2_ttf-2.0.14 EXCLUDE_FROM_ALL)
include_directories(ext/SDL2_ttf-2.0.14)

# SDL_FontCache
include_directories(ext/SDL_FontCache)

# entt
include_directories(ext/entt-3.0.0/src)

# stb
include_directories(ext/stb-2.22)

# inih
include_directories(ext/inih-42)

# mstch
add_subdirectory(ext/mstch-1.0.2 EXCLUDE_FROM_ALL)
include_directories(${mstch_INCLUDE_DIR})

# yaml-cpp 
option(BUILD_SHARED_LIBS "Build Shared Libraries" ON)
add_subdirectory(ext/yaml-cpp-0.6.2 EXCLUDE_FROM_ALL)
include_directories(ext/yaml-cpp-0.6.2/include)

# game projects
add_subdirectory(mayhem)
add_subdirectory(client)
add_subdirectory(server)

# dummy target used for file copies
add_custom_target(dummy-target ALL DEPENDS custom-output)
add_custom_command(OUTPUT custom-output COMMAND ${CMAKE_COMMAND} -E echo DEPENDS always-rebuild)
add_custom_command(OUTPUT always-rebuild COMMAND ${CMAKE_COMMAND} -E echo)

add_custom_target(mayhem-configured DEPENDS dummy-target client server)
add_custom_command(
        TARGET mayhem-configured
        COMMAND ${CMAKE_COMMAND} -E echo  "make_directory: ${PROJECT_BINARY_DIR}/logs"
        COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/logs

        COMMAND ${CMAKE_COMMAND} -E echo "copy_directory: ${PROJECT_SOURCE_DIR}/assets to ${PROJECT_BINARY_DIR}/assets"
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/assets ${PROJECT_BINARY_DIR}/assets
)

cmake_minimum_required(VERSION 3.30.1)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/Dependencies/FIND")
find_package(Git REQUIRED)
find_package(Wayland REQUIRED)

project("Morningstar" LANGUAGES C)
set(PROJECT_NAME_FULL "Morningstar Framework")

if(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    message(FATAL_ERROR 
        "${PROJECT_NAME_FULL} is not currently meant to run anywhere but Linux.")
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/../.git")
    execute_process(COMMAND bash "-c" "git tag | wc -l" OUTPUT_VARIABLE 
        PROJECT_VERSION_MAJOR OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND bash "-c" 
        "git rev-list  `git rev-list --tags --no-walk --max-count=1`..HEAD --count" 
        OUTPUT_VARIABLE PROJECT_VERSION_MINOR OUTPUT_STRIP_TRAILING_WHITESPACE)

    file(WRITE ${CMAKE_SOURCE_DIR}/Assets/version.txt 
        "${PROJECT_VERSION_MAJOR} ${PROJECT_VERSION_MINOR}")
else()
    file(READ ${CMAKE_SOURCE_DIR}/Assets/version.txt PROJECT_VERSION_STRING)
    separate_arguments(PROJECT_VERSION_STRING)
    list(GET PROJECT_VERSION_STRING 0 PROJECT_VERSION_MAJOR)
    list(GET PROJECT_VERSION_STRING 1 PROJECT_VERSION_MINOR)
endif()

set(CMAKE_C_STANDARD 11)
add_compile_options(-Wall -Werror -Wpedantic)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME})

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/Dependencies/XDGS/xdg-shell.h" OR NOT EXISTS 
    "${CMAKE_SOURCE_DIR}/Dependencies/XDGS/xdg-shell.c")
    execute_process(COMMAND bash "-c" 
    "wayland-scanner client-header /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml Dependencies/XDGS/xdg-shell.h")
    execute_process(COMMAND bash "-c" 
    "wayland-scanner private-code /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml Dependencies/XDGS/xdg-shell.c")
endif()

file(GLOB PROJECT_FILES 
    ${CMAKE_SOURCE_DIR}/Source/*.c 
    ${CMAKE_SOURCE_DIR}/Source/Windowing/*.c 
    ${CMAKE_SOURCE_DIR}/Source/Input/*.c 
    ${CMAKE_SOURCE_DIR}/Source/Output/*.c 
    ${CMAKE_SOURCE_DIR}/Source/Memory/*.c 
    ${CMAKE_SOURCE_DIR}/Source/Diagnostic/*.c 
    ${CMAKE_SOURCE_DIR}/Source/Rendering/*.c 
    ${CMAKE_SOURCE_DIR}/Source/Utilities/Utilities.c 
    ${CMAKE_SOURCE_DIR}/Dependencies/XDGS/xdg-shell.c)
file(GLOB PROJECT_HEADERS ${CMAKE_SOURCE_DIR}/Source/*.h 
    ${CMAKE_SOURCE_DIR}/Source/*.h
    ${CMAKE_SOURCE_DIR}/Source/Windowing/*.h
    ${CMAKE_SOURCE_DIR}/Source/Input/*.h
    ${CMAKE_SOURCE_DIR}/Source/Output/*.h
    ${CMAKE_SOURCE_DIR}/Source/Memory/*.h
    ${CMAKE_SOURCE_DIR}/Source/Rendering/*.h
    ${CMAKE_SOURCE_DIR}/Source/Diagnostic/*.h
    ${CMAKE_SOURCE_DIR}/Source/Utilities/*.h)

foreach(file ${PROJECT_FILES} ${PROJECT_HEADERS} ${CMAKE_SOURCE_DIR}/Source/Main.c)
    cmake_path(GET file FILENAME CURRENT_FILENAME)
    set_source_files_properties(${file} PROPERTIES COMPILE_DEFINITIONS 
        FILENAME="${CURRENT_FILENAME}")
endforeach()

include_directories(${CMAKE_SOURCE_DIR}/Dependencies ${CMAKE_SOURCE_DIR}/Source)
link_directories(${CMAKE_SOURCE_DIR}/Dependencies/GLAD 
    ${CMAKE_SOURCE_DIR}/Dependencies/STBI)

add_library(morningstar_source OBJECT ${PROJECT_FILES})
set_property(TARGET morningstar_source PROPERTY POSITION_INDEPENDENT_CODE 1)

add_library(morningstar_shared SHARED $<TARGET_OBJECTS:morningstar_source>)
add_library(morningstar_static STATIC $<TARGET_OBJECTS:morningstar_source>)
target_link_libraries(morningstar_shared PRIVATE wayland-client PRIVATE EGL
    PRIVATE wayland-egl PRIVATE GLESv2 PRIVATE glad PRIVATE stbi)
target_link_libraries(morningstar_static PRIVATE wayland-client PRIVATE EGL
    PRIVATE wayland-egl PRIVATE GLESv2 PRIVATE glad PRIVATE stbi)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_EXPORT_COMPILE_COMMANDS YES)
    add_compile_options(-g -fsanitize=undefined)    
    add_link_options(-fsanitize=undefined)
    add_compile_definitions(DEBUG)

    # If we're compiling in debug mode, add a test executable if the 
    # file exists.
    if(EXISTS "${CMAKE_SOURCE_DIR}/Source/Main.c")
        list(APPEND CMAKE_BUILD_RPATH "$ORIGIN/")
        add_executable(MorningstarDemo ${CMAKE_SOURCE_DIR}/Source/Main.c)
        target_link_libraries(MorningstarDemo PRIVATE morningstar_shared)
    endif()
    file(COPY ${CMAKE_SOURCE_DIR}/Assets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
else()
    add_compile_options(-Ofast)
    file(COPY 
        ${CMAKE_SOURCE_DIR}/Source/Windowing/Windowing.h 
        ${CMAKE_SOURCE_DIR}/Source/Windowing/Windowing-Types.h
        DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

string(TOLOWER ${CMAKE_BUILD_TYPE} OUTPUT_VARIABLE CMAKE_BUILD_LOWERCASE)
set(PROJECT_TITLE "${PROJECT_NAME_FULL} | \
v${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}${CMAKE_BUILD_LOWERCASE}")

add_compile_definitions(
    ID="${PROJECT_NAME}"
    TITLE="${PROJECT_TITLE}"
    MAJOR=${PROJECT_VERSION_MAJOR} MINOR=${PROJECT_VERSION_MINOR})
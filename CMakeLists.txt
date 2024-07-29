cmake_minimum_required(VERSION 3.30.1)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/Dependencies/FND")
find_package(Git REQUIRED)
find_package(Wayland REQUIRED)

# Template names for the compilation of the engine, change these to match
# your project!
project("Morningstar" LANGUAGES C)
set(PROJECT_NAME_FULL "Morningstar Framework")

if(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    message(FATAL_ERROR 
        "${PROJECT_NAME_FULL} is not currently meant to run anywhere but Linux.")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_EXPORT_COMPILE_COMMANDS YES)
    add_compile_options(-g -fsanitize=undefined)    
    add_link_options(-fsanitize=undefined)
else()
    add_compile_options(-Ofast)
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
    execute_process(COMMAND bash "-c" "git tag | wc -l" OUTPUT_VARIABLE 
        PROJECT_VERSION_MAJOR OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND bash "-c" 
        "git rev-list  `git rev-list --tags --no-walk --max-count=1`..HEAD --count" 
        OUTPUT_VARIABLE PROJECT_VERSION_MINOR OUTPUT_STRIP_TRAILING_WHITESPACE)

    file(WRITE ${CMAKE_SOURCE_DIR}/Source/Assets/version.txt 
        "${PROJECT_VERSION_MAJOR} ${PROJECT_VERSION_MINOR}")
else()
    file(READ ${CMAKE_SOURCE_DIR}/Source/Assets/version.txt PROJECT_VERSION_STRING)
    separate_arguments(PROJECT_VERSION_STRING)
    list(GET PROJECT_VERSION_STRING 0 PROJECT_VERSION_MAJOR)
    list(GET PROJECT_VERSION_STRING 1 PROJECT_VERSION_MINOR)
endif()

set(CMAKE_C_STANDARD 11)
add_compile_options(-Wall -Werror -Wpedantic)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME})

execute_process(COMMAND bash "-c" 
"wayland-scanner client-header /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml Dependencies/XDG_SH/xdg-shell.h")
execute_process(COMMAND bash "-c" 
"wayland-scanner private-code /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml Dependencies/XDG_SH/xdg-shell.c")

file(GLOB PROJECT_FILES 
    ${CMAKE_SOURCE_DIR}/Source/Systems/*.c 
    ${CMAKE_SOURCE_DIR}/Source/Systems/Wayland/*.c 
    ${CMAKE_SOURCE_DIR}/Source/Systems/Input/*.c 
    ${CMAKE_SOURCE_DIR}/Source/Utilities/Utilities.c 
    ${CMAKE_SOURCE_DIR}/Dependencies/XDG_SH/xdg-shell.c)
file(GLOB PROJECT_HEADERS ${CMAKE_SOURCE_DIR}/Source/*.h 
    ${CMAKE_SOURCE_DIR}/Source/Systems/*.h
    ${CMAKE_SOURCE_DIR}/Source/Systems/Wayland/*.h
    ${CMAKE_SOURCE_DIR}/Source/Systems/Input/*.h
    ${CMAKE_SOURCE_DIR}/Source/Utilities/*.h)

foreach(file ${PROJECT_FILES} ${PROJECT_HEADERS} ${CMAKE_SOURCE_DIR}/Source/Main.c)
    cmake_path(GET file FILENAME CURRENT_FILENAME)
    set_source_files_properties(${file} PROPERTIES COMPILE_DEFINITIONS 
        FILENAME="${CURRENT_FILENAME}") 
endforeach()

include_directories(${CMAKE_SOURCE_DIR}/Dependencies ${CMAKE_SOURCE_DIR}/Source 
    ${CMAKE_SOURCE_DIR}/Source/Types ${CMAKE_SOURCE_DIR}/Source/Systems)

add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/Source/Main.c)
set_target_properties(${PROJECT_NAME} PROPERTIES
        BUILD_WITH_INSTALL_RPATH FALSE
        LINK_FLAGS "-Wl,-rpath,$ORIGIN/")
add_library(morningstar SHARED ${PROJECT_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE morningstar 
    PRIVATE wayland-client PUBLIC rt)

string(TOLOWER ${CMAKE_BUILD_TYPE} OUTPUT_VARIABLE CMAKE_BUILD_LOWERCASE)
set(PROJECT_TITLE "${PROJECT_NAME_FULL} | \
v${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}${CMAKE_BUILD_LOWERCASE}")

add_compile_definitions(
    TITLE="${PROJECT_TITLE}"
    MAJOR=${PROJECT_VERSION_MAJOR} MINOR=${PROJECT_VERSION_MINOR})
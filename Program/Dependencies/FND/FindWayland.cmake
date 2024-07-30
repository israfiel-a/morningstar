project(find-wayland)
include(FindPackageHandleStandardArgs)

find_path(WAYLAND_INCLUDE_DIR NAMES wayland-client.h)
find_library(WAYLAND_LIBRARY NAMES wayland-client libwayland-client)
if(WAYLAND_INCLUDE_DIR AND WAYLAND_LIBRARY)
    add_library(wayland UNKNOWN IMPORTED)
    set_target_properties(wayland PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "C"
        INTERFACE_INCLUDE_DIRECTORIES "${WAYLAND_INCLUDE_DIR}"
        IMPORTED_LOCATION "${WAYLAND_LIBRARY}"
    )
endif()

find_package_handle_standard_args(Wayland 
    REQUIRED_VARS WAYLAND_LIBRARY WAYLAND_INCLUDE_DIR)
mark_as_advanced(WAYLAND_INCLUDE_DIR WAYLAND_LIBRARY)
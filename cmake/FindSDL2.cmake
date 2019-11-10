find_path(SDL2_INCLUDE_DIR SDL2/SDL.h
    HINTS "${SDL2_DIR}"
    PATH_SUFFIXES include
    NO_DEFAULT_PATH
)

find_library(SDL2_LIBRARY
    NAMES SDL2
    HINTS "${SDL2_DIR}"
    PATH_SUFFIXES lib lib64
    NO_DEFAULT_PATH
)

if (WIN32)
    find_file(SDL2_DLL
        NAMES SDL2.dll
        HINTS "${SDL2_DIR}"
        PATH_SUFFIXES bin
        NO_DEFAULT_PATH
    )
endif()

find_library(SDL2MAIN_LIBRARY
    NAMES SDL2main
    HINTS "${SDL2_DIR}"
    PATH_SUFFIXES lib lib64
    NO_DEFAULT_PATH
)

add_library(SDL2::SDL SHARED IMPORTED)
add_library(SDL2::SDLmain STATIC IMPORTED)

set_target_properties(SDL2::SDL PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIR}" INTERFACE_LINK_LIBRARIES SDL2::SDLmain)

if (WIN32)
    set_target_properties(SDL2::SDL PROPERTIES IMPORTED_LOCATION "${SDL2_DLL}" IMPORTED_IMPLIB "${SDL2_LIBRARY}")
else()
    set_target_properties(SDL2::SDL PROPERTIES IMPORTED_LOCATION "${SDL2_LIBRARY}")
endif()

set_target_properties(SDL2::SDLmain PROPERTIES IMPORTED_LOCATION "${SDL2MAIN_LIBRARY}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL2 DEFAULT_MSG SDL2_LIBRARY SDL2_INCLUDE_DIR)

mark_as_advanced(SDL2_LIBRARY SDL2_INCLUDE_DIR)

find_package(OpenGL REQUIRED)

find_path(GLEW_INCLUDE_DIR GL/glew.h
    HINTS "${GLEW_DIR}"
    PATH_SUFFIXES include
    NO_DEFAULT_PATH
)

find_library(GLEW_LIBRARY
    NAMES glew glew32
    HINTS "${GLEW_DIR}"
    PATH_SUFFIXES lib lib64
    NO_DEFAULT_PATH
)

if (WIN32)
    find_file(GLEW_DLL
        NAMES glew.dll glew32.dll
        HINTS "${GLEW_DIR}"
        PATH_SUFFIXES bin
        NO_DEFAULT_PATH
    )
endif()

add_library(GLEW::GLEW SHARED IMPORTED)

set_target_properties(GLEW::GLEW PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${GLEW_INCLUDE_DIR}" INTERFACE_LINK_LIBRARIES OpenGL::GL)

if (WIN32)
    set_target_properties(GLEW::GLEW PROPERTIES IMPORTED_LOCATION "${GLEW_DLL}" IMPORTED_IMPLIB "${GLEW_LIBRARY}")
else()
    set_target_properties(GLEW::GLEW PROPERTIES IMPORTED_LOCATION "${GLEW_LIBRARY}")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLEW DEFAULT_MSG GLEW_LIBRARY GLEW_INCLUDE_DIR)

mark_as_advanced(GLEW_LIBRARY GLEW_INCLUDE_DIR)

cmake_minimum_required(VERSION 3.10)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

option(SUPERBUILD "Use a superbuild" ON)

set(SDL2_DIR "" CACHE PATH "The SDL2 root dir")
set(GLEW_DIR "" CACHE PATH "The GLEW root dir")

if (SUPERBUILD)
    project(SUPERBUILD)
    include(ExternalProject)
    add_subdirectory(vendor)
    ExternalProject_Add(
        NoFlowers_EP
        SOURCE_DIR      "${CMAKE_SOURCE_DIR}"
        INSTALL_DIR     "${CMAKE_BINARY_DIR}"
        CMAKE_ARGS
            "-DSUPERBUILD=OFF"
            "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
            "-DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}"
            "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/bin"
            "-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}"
            "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}"
            "-DSDL2_DIR=${CMAKE_BINARY_DIR}/_vendor"
            "-DGLEW_DIR=${CMAKE_BINARY_DIR}/_vendor"
        DEPENDS
        SDL2
        GLEW
    )
    ExternalProject_Add_Step(
		NoFlowers_EP
		reconfigure
		COMMAND ${CMAKE_COMMAND} -E echo "Forcing a superbuild reconfigure"
		DEPENDEES download
		DEPENDERS configure
		ALWAYS ON
    )
    add_executable(NoFlowers IMPORTED)
    set_target_properties(NoFlowers PROPERTIES IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/bin/NoFlowers.exe")
    install(DIRECTORY "${CMAKE_BINARY_DIR}/bin/" DESTINATION . USE_SOURCE_PERMISSIONS)
else()
    project(NoFlowers)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
    if (NOT MSVC)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -fomit-frame-pointer -fstrict-aliasing")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fomit-frame-pointer -fstrict-aliasing -fno-exceptions -fno-rtti")
    else()
        string(REGEX REPLACE "/GR" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        string(REGEX REPLACE "/W[0-4]" "/W3" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        string(REGEX REPLACE "/EH[a-z]+" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc- /GR-")
    endif()
    add_subdirectory(src)
endif()

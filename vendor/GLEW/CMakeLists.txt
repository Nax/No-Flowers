include(ExternalProject)
set(GLEW_VERSION 2.1.0)

# The NONE configuration is a workaround
# for https://github.com/nigels-com/glew/issues/180

ExternalProject_Add(
    GLEW
    URL             "${CMAKE_CURRENT_SOURCE_DIR}/glew-${GLEW_VERSION}.tar.gz"
	INSTALL_DIR     "${CMAKE_BINARY_DIR}/_vendor"
    SOURCE_SUBDIR   "build/cmake"
	CMAKE_ARGS
		"-DCMAKE_BUILD_TYPE=NONE"
		"-DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>"
        "-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}"
        "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}"
)

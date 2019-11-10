find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

file(GLOB_RECURSE SOURCES "*.cpp")
add_executable(NoFlowers WIN32 MACOSX_BUNDLE ${SOURCES})
target_include_directories(NoFlowers PRIVATE "${CMAKE_SOURCE_DIR}/src")
target_link_libraries(NoFlowers SDL2::SDL GLEW::GLEW)
install(TARGETS NoFlowers RUNTIME DESTINATION . BUNDLE DESTINATION .)
install(FILES "${SDL2_DLL}" "${GLEW_DLL}" DESTINATION .)

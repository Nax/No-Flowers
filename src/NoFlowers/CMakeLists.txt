find_package(SDL2 REQUIRED)

file(GLOB_RECURSE SOURCES "*.cpp")
add_executable(NoFlowers WIN32 MACOSX_BUNDLE ${SOURCES})
target_link_libraries(NoFlowers SDL2::SDL)
install(TARGETS NoFlowers RUNTIME DESTINATION . BUNDLE DESTINATION .)
install(FILES "${SDL2_DLL}" DESTINATION .)

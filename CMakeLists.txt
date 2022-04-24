cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(Snake)

add_subdirectory(dependencies/raylib/raylib-4-src dependencies/raylib/bin)

file(GLOB PROJECT_SOURCE_FILES CONFIGURE_DEPENDS *.c)
# include_directories(include/)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})
#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib)

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
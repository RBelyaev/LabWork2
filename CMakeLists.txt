cmake_minimum_required(VERSION 3.10)

project(game)

# Find ncurses library
find_package(Curses REQUIRED)

if(Curses_FOUND)
    # Add executable
    add_executable(game main.cpp)

    # Link ncurses library
    target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})
else()
    message("NETU")
endif()
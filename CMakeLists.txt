cmake_minimum_required(VERSION 3.28)
if (NOT DEFINED ENV{VCPKG_CMAKE})
    message(FATAL_ERROR "VCPKG_CMAKE environment variable is not set.")
endif ()

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_CMAKE}" CACHE STRING "Vcpkg toolchain file")
project(ETG LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

# Make GNU path format to resolve any binary copy paste requirement for SFML
include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

find_package(SFML CONFIG REQUIRED system window graphics network audio)
find_package(benchmark CONFIG REQUIRED)
find_package(Threads REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)
find_package(boost_describe CONFIG REQUIRED)
find_package(boost_json CONFIG REQUIRED)

# Get all cpp files recursively from the src directory
file(GLOB_RECURSE ALL_CPP_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Define the list of libraries to link against
set(REQUIRED_LIBRARIES
        sfml-audio
        sfml-graphics
        $<$<PLATFORM_ID:Windows>:sfml-main>  # Only link sfml-main on Windows
        sfml-network
        sfml-system
        sfml-window
        benchmark::benchmark
        ImGui-SFML::ImGui-SFML
        Boost::describe
        Boost::json
)

# Create an executable for each cpp file
foreach(CPP_FILE ${ALL_CPP_FILES})
    # Get the filename without extension
    get_filename_component(EXECUTABLE_NAME ${CPP_FILE} NAME_WE)

    # Add executable
    add_executable(${EXECUTABLE_NAME} ${CPP_FILE})

    # Link with required libraries
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${REQUIRED_LIBRARIES})

    # target_include_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
endforeach()
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

add_executable(ETG src/main.cpp
        src/UniversalPrint.h

)

target_link_libraries(ETG PRIVATE
        sfml-audio
        sfml-graphics
        $<$<PLATFORM_ID:Windows>:sfml-main>  # Only link sfml-main on Windows
        sfml-network
        sfml-system
        sfml-window
        benchmark::benchmark
        ImGui-SFML::ImGui-SFML
        Boost::describe
)
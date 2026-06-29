cmake_minimum_required(VERSION 3.20)

project(SortingAnalysis
    VERSION 1.0.0
    DESCRIPTION "Console application for testing and comparing sorting algorithms"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Main source file
set(SORTING_ANALYSIS_SOURCES
    src/SortingAnalysis/main.cpp
)

# Header-only modules. CONFIGURE_DEPENDS lets CMake notice newly added headers
# after re-running configure/generate.
file(GLOB_RECURSE SORTING_ANALYSIS_HEADERS CONFIGURE_DEPENDS
    src/SortingAnalysis/core/*.h
    src/SortingAnalysis/utils/*.h
    src/SortingAnalysis/sorts/*.h
)

add_executable(SortingAnalysis
    ${SORTING_ANALYSIS_SOURCES}
    ${SORTING_ANALYSIS_HEADERS}
)

target_include_directories(SortingAnalysis PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SortingAnalysis
)

target_compile_features(SortingAnalysis PRIVATE cxx_std_17)

# Prevent Windows.h from defining min/max macros that break std::min, std::max
# and std::numeric_limits<T>::min()/max() in MSVC builds.
target_compile_definitions(SortingAnalysis PRIVATE
    NOMINMAX
    WIN32_LEAN_AND_MEAN
)

# Better UTF-8 support in Visual Studio/MSVC builds.
if (MSVC)
    target_compile_options(SortingAnalysis PRIVATE /utf-8 /W4)
else()
    target_compile_options(SortingAnalysis PRIVATE -Wall -Wextra -pedantic)
endif()

# Make Visual Studio show headers in convenient groups.
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src/SortingAnalysis
    PREFIX "src"
    FILES ${SORTING_ANALYSIS_SOURCES} ${SORTING_ANALYSIS_HEADERS}
)

# Set this project as the startup project in Visual Studio.
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SortingAnalysis)

# Put executable into a predictable folder for single-config generators.
set_target_properties(SortingAnalysis PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Put executable into a predictable folder for multi-config generators like Visual Studio.
foreach(CONFIG_TYPE Debug Release RelWithDebInfo MinSizeRel)
    string(TOUPPER ${CONFIG_TYPE} CONFIG_TYPE_UPPER)
    set_target_properties(SortingAnalysis PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_${CONFIG_TYPE_UPPER} ${CMAKE_BINARY_DIR}/bin/${CONFIG_TYPE}
    )
endforeach()

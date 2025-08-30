cmake_minimum_required(VERSION 3.10)
project(huta_DSP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include project headers
include_directories(${CMAKE_SOURCE_DIR}/includes
                    ${CMAKE_SOURCE_DIR}/gui/includes)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Source files
set(SOURCES
    src/main.cpp
    src/core/CanonicalFilter.cpp
    src/interface/IFilter.cpp
    src/test/Signal.cpp
    src/dsp_math/QNumber.cpp  
    src/core/StateVariableFilter.cpp
    src/core/ShelvingFilter.cpp
    src/core/PeakFilter.cpp
    src/core/BiquadFilter.cpp
    src/core/ParametricEqualizer.cpp
    src/modules/ToneControl.cpp
    src/test/TestObject.cpp
    gui/src/MainWindow.cpp

    #Qt
    gui/includes/MainWindow.hpp
)

add_executable(huta_dsp ${SOURCES})
# Link Qt
target_link_libraries(huta_dsp PRIVATE Qt6::Core Qt6::Widgets Qt6::Multimedia)

# ------------------------------------------
# Cross-platform handling for libsndfile
# ------------------------------------------
if(WIN32)
    # Windows path to libsndfile
    # Must download libsndfile Windows binary
    set(SNDFILE_INCLUDE "C:/vcpkg/installed/x64-windows/include")
    set(SNDFILE_LIB "C:/vcpkg/installed/x64-windows/lib/sndfile.lib")
    target_include_directories(huta_dsp PRIVATE ${SNDFILE_INCLUDE})
    target_link_libraries(huta_dsp PRIVATE ${SNDFILE_LIB})

    message(STATUS "Building on Windows: libsndfile linked from ${SNDFILE_LIB}")
else()
    # Linux / macOS
    target_include_directories(huta_dsp PRIVATE /usr/include)   
    target_link_libraries(huta_dsp PRIVATE sndfile)
endif()

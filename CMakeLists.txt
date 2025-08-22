cmake_minimum_required(VERSION 3.10)
project(huta_DSP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include project headers
include_directories(${CMAKE_SOURCE_DIR}/includes)

# Source files
set(SOURCES
    src/main.cpp
    src/module/CanonicalFilter.cpp
    src/interface/IFilter.cpp
    src/test/Signal.cpp
    src/dsp_math/QNumber.cpp  
    src/module/StateVariableFilter.cpp
    src/module/ShelvingFilter.cpp
    src/module/PeakFilter.cpp
)

add_executable(huta_dsp ${SOURCES})

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

cmake_minimum_required(VERSION 3.10)

# Tên project
project(huta_DSP)

# Chuẩn C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Thêm include path
include_directories(${CMAKE_SOURCE_DIR}/includes)

# Liệt kê source files
set(SOURCES
    src/main.cpp
    src/module/CanonicalFilter.cpp
    src/interface/IFilter.cpp
    src/module/Signal.cpp
    src/dsp_math/QNumber.cpp  
)

# Tạo executable
add_executable(huta_dsp ${SOURCES})

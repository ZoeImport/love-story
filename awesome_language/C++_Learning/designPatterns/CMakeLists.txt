cmake_minimum_required(VERSION 3.0)
project(designPatterns)
file(GLOB CPP_FILES "*.cpp")
set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS OFF)
foreach(CPP_FILE ${CPP_FILES})
    get_filename_component(EXE_NAME ${CPP_FILE} NAME_WE)
    add_executable(${EXE_NAME} ${CPP_FILE})
    target_compile_options(${EXE_NAME} PRIVATE -g)    
endforeach()
    

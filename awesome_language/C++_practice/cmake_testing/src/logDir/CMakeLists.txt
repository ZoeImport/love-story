cmake_minimum_required(VERSION 3.20)
project(logLib)
# include_directories(${PROJECT_SOURCE_DIR})
aux_source_directory(${PROJECT_SOURCE_DIR} SRC)
add_library(LOG SHARED ${SRC})
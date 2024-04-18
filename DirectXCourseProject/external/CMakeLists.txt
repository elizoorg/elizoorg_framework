cmake_minimum_required(VERSION 3.8.2) # 3.8.0 is minimum version that allow Linux remote building and debugging


project(External VERSION 0.0.1)

include_directories(src)


set(HEADER_FILES
    
)

set(MODULE_FILES 
   


include(${CMAKE_SOURCE_DIR}/DirectXCourseProject/include/Exports.h)
include(${CMAKE_SOURCE_DIR}/DirectXCourseProject/include/pch.h)
add_library(${PROJECT_NAME} SHARED ${MODULE_FILES} ${HEADER_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
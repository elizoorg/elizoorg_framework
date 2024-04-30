cmake_minimum_required(VERSION 3.8.2) # 3.8.0 is minimum version that allow Linux remote building and debugging

project(DirectXCourseProject VERSION 0.0.1)

# Remove trailing '\' and '/' at the end of the path, so can reliably compare
STRING(REGEX REPLACE "[/\\]+$" "" CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR})
STRING(REGEX REPLACE "[/\\]+$" "" CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# When compiling DLib as stand alone library set to C++11,
# when library is part of other project, respect parent project settings.
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	# Enforce c++11 standard.
	set (CMAKE_CXX_STANDARD 20)
endif()

# Notification when too low version of C++ is set
if(NOT DEFINED CMAKE_CXX_STANDARD OR CMAKE_CXX_STANDARD LESS 20)
	message(FATAL_ERROR "ERROR: CMAKE_CXX_STANDARD is not set or it is too low. Minimum C++ v20 is required to compile \"DirectXProject\" library.")
endif()



include_directories(include)




set(HEADER_FILES 
	include/Application.h 
	include/Camera.h 
	include/Display.h 
	include/GameComponent.h
	include/DebugRenderSysImpl.h
	include/InputDevice.h 
	include/PlaneComponent.h 
	include/SphereComponent.h
	include/Transform.h 
	include/TriangleComponent.h 
	include/VertexPositionColor.h
	include/VertexPositionNormalBinormalTangentColorTex.h
	include/VertexPositionTex.h
	include/Keys.h 
	include/MathTypes.h 
	include/pch.h
	include/Exports.h
	include/Delegates.h
    include/SimpleMath.h
    include/SimpleMath.inl
)



set(MODULE_FILES
	src/Application.cpp 
	src/Camera.cpp 
	src/DebugRenderSysImpl.cpp
	src/Display.cpp 
	src/GameComponent.cpp
	src/InputDevice.cpp 
	src/PlaneComponent.cpp 
	src/SphereComponent.cpp
	src/Transform.cpp 
	src/TriangleComponent.cpp
	src/VertexPositionColor.cpp
	src/VertexPositionNormalBinormalTangentColorTex.cpp
	src/VertexPositionTex.cpp
	src/Delegates.cpp
    src/SimpleMath.cpp
)


add_library(${PROJECT_NAME} SHARED ${MODULE_FILES} ${HEADER_FILES})


 if (MSVC)
	target_compile_definitions(${PROJECT_NAME} PUBLIC UNICODE _UNICODE)
	target_compile_options(${PROJECT_NAME} PRIVATE "/MP")
 endif()


find_package(imgui CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
target_link_libraries(
	DirectXCourseProject 
imgui::imgui 
assimp::assimp
)

#include(GenerateExportHeader)
#generate_export_header(DLib)


# Make sure the compiler can find include files for our Hello library
# when other libraries or executables link to Hello
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


add_compile_definitions(ENGINE_DLL)

# List files that install and/or package should provide.
# Each library should know what it want to distribute,
# which files are internal or intermediate and which are public library export.
install(TARGETS ${PROJECT_NAME} DESTINATION ${PROJECT_NAME}-bin)
install(FILES ${HEADER_FILES} DESTINATION ${PROJECT_NAME}-include)
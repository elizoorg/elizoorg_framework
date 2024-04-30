
include_directories(${CMAKE_CURRENT_BINARY_DIR})




set(HEADER_FILES 
	GameApplication.h
)

set(MODULE_FILES
	GameApplication.cpp
	main.cpp
)

add_executable(GameApplication ${MODULE_FILES} ${HEADER_FILES})

if (MSVC)
     target_compile_definitions(GameApplication  PUBLIC UNICODE _UNICODE)
	target_compile_options(GameApplication PRIVATE "/MP")
endif()

find_package(imgui CONFIG REQUIRED)

target_link_libraries( 
	GameApplication PRIVATE
	DirectXCourseProject
	imgui::imgui
)







install(TARGETS GameApplication DESTINATION GameApplication)

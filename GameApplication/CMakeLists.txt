
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
find_package(assimp CONFIG REQUIRED)
find_package(ZLIB REQUIRED)
SET(ZLIB_NAMES z zlib zdll zlib1 zlibd zlibd1 zlibwapi)

target_link_libraries( 
	GameApplication PUBLIC
	DirectXCourseProject
	imgui::imgui
	assimp::assimp
	ZLIB::ZLIB
)



add_custom_command(TARGET GameApplication POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:GameApplication> $<TARGET_FILE_DIR:GameApplication>
  COMMAND_EXPAND_LISTS
  )



install(TARGETS GameApplication DESTINATION GameApplication)

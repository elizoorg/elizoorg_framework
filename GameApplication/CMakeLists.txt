
include_directories(${CMAKE_CURRENT_BINARY_DIR})


find_package(imgui REQUIRED)

add_executable(GameApplication "main.cpp" "GameApplication.cpp" "GameApplication.h")


target_link_libraries(
	GameApplication
	DirectXCourseProject
)



install(TARGETS GameApplication DESTINATION GameApplication)


include_directories(${CMAKE_CURRENT_BINARY_DIR})





set(HEADER_FILES 
	imconfig.h
	imgui.h
	imgui_impl_dx11.h
	imgui_impl_win32.h
	imgui_internal.h
	imstb_rectpack.h
	imstb_textedit.h
	imstb_truetype.h
	GameApplication.h
)

set(MODULE_FILES
	GameApplication.cpp
	main.cpp
	imgui.cpp
	imgui_draw.cpp
	imgui_impl_dx11.cpp
	imgui_impl_win32.cpp
	imgui_tables.cpp
	imgui_widgets.cpp
)

add_executable(GameApplication ${MODULE_FILES} ${HEADER_FILES})

if (MSVC)
     target_compile_definitions(GameApplication  PUBLIC UNICODE _UNICODE)
endif()

target_link_libraries(
	GameApplication
	DirectXCourseProject
)



install(TARGETS GameApplication DESTINATION GameApplication)

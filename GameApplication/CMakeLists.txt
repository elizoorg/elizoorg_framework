project(GameApplication)

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

target_link_libraries( 
	GameApplication PUBLIC
	DirectXCourseProject
	imgui::imgui
	assimp::assimp
	ZLIB::ZLIB 
)

# Указываем путь к папке с ассетами и шейдерами внутри проекта
set(ASSETS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/assets)

set(SHADERS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Shaders)

# Копируем папку с ассетами и шейдерами в папку сборки
add_custom_command(TARGET GameApplication POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ASSETS_DIRECTORY}
        $<TARGET_FILE_DIR:GameApplication>/assets)


add_custom_command(TARGET GameApplication POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
			${SHADERS_DIRECTORY}
			$<TARGET_FILE_DIR:GameApplication>/Shaders)


add_custom_command(TARGET GameApplication POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:GameApplication> $<TARGET_FILE_DIR:GameApplication>
  COMMAND_EXPAND_LISTS
  )


install(TARGETS GameApplication DESTINATION GameApplication)

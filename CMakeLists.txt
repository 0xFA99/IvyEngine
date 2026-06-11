cmake_minimum_required(VERSION 3.20)
project(IvyEngine C)

set(CMAKE_C_STANDARD 11)

add_library(${PROJECT_NAME} STATIC
		src/core.c
		src/math.c
		src/input.c
		src/gfx.c
		src/window.c
		src/glfw_loader.c
		src/glad_loader.c
		src/arena/linear.c
)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PUBLIC)

add_executable(IvyTest src/ivy.c)
target_link_libraries(IvyTest PRIVATE ${PROJECT_NAME})

target_compile_definitions(${PROJECT_NAME} PUBLIC
		$<$<CONFIG:Debug>:IVY_DEBUG>
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw3.dll"
		"$<TARGET_FILE_DIR:${PROJECT_NAME}>/lib/glfw3.dll"
)
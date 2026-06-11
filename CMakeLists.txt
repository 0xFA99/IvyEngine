cmake_minimum_required(VERSION 3.20)
project(IvyEngine C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(src/external/glfw)

add_library(${PROJECT_NAME} STATIC
		src/core.c
		src/math.c
		src/input.c
		src/gfx.c
		src/window.c
		src/glad_loader.c
		src/arena/linear.c
)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

add_executable(IvyTest src/ivy.c)
target_link_libraries(IvyTest PRIVATE ${PROJECT_NAME})

target_compile_definitions(${PROJECT_NAME} PUBLIC
		$<$<CONFIG:Debug>:IVY_DEBUG>
)
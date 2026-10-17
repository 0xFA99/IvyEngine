cmake_minimum_required(VERSION 3.20)
project(IvyEngine C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME}
        src/main.c
        src/engine.c
        src/impl.c
        src/core/time.c
        src/platform/window.c
        src/platform/input.c
        src/graphics/renderer.c
        src/graphics/virtual_resolution.c
)

target_include_directories(${PROJECT_NAME} PRIVATE
    include
)

option(IVY_ENABLE_LOGGING "Enable logging in Release builds" OFF)

target_compile_definitions(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:IVY_DEBUG>
        $<$<AND:$<CONFIG:Release>,$<NOT:$<BOOL:${IVY_ENABLE_LOGGING}>>>:IVY_LOG_DISABLE>
)

if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        dl
        m
        pthread
    )
endif()
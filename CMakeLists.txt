cmake_minimum_required(VERSION 3.20)
project(IvyEngine C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME}
        src/impl.c
        src/main.c
        src/engine.c
        src/window.c
        src/renderer.c
)

target_include_directories(${PROJECT_NAME} PRIVATE
    include
)

target_compile_definitions(IvyEngine PRIVATE
        $<$<CONFIG:Debug>:IVY_DEBUG>
)

if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        dl
        m
        pthread
    )
endif()
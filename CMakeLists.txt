cmake_minimum_required(VERSION 3.20)
project(IvyEngine C)

set(CMAKE_C_STANDARD 11)

if(UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(WAYLAND_CLIENT REQUIRED wayland-client)
    pkg_check_modules(WAYLAND_EGL REQUIRED wayland-egl)
    pkg_check_modules(XKBCOMMON REQUIRED xkbcommon)
    pkg_check_modules(EGL REQUIRED egl)
    pkg_check_modules(X11 REQUIRED x11)

    find_program(WAYLAND_SCANNER_EXECUTABLE NAMES wayland-scanner)
    execute_process(
        COMMAND ${WAYLAND_SCANNER_EXECUTABLE} client-header /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml ${CMAKE_CURRENT_BINARY_DIR}/xdg-shell-client-protocol.h
    )
    execute_process(
        COMMAND ${WAYLAND_SCANNER_EXECUTABLE} private-code /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml ${CMAKE_CURRENT_BINARY_DIR}/xdg-shell-client-protocol.c
    )
endif()

add_library(${PROJECT_NAME} STATIC
		src/core.c
		src/math.c
		src/input.c
		src/gfx.c
		src/window.c
		src/platform_common.c
		src/platform_x11.c
		src/platform_wayland.c
		src/platform_win32.c
		src/arena/linear.c
		src/virtual_resolution.c
		$<$<PLATFORM_ID:Linux>:${CMAKE_CURRENT_BINARY_DIR}/xdg-shell-client-protocol.c>
)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(UNIX AND NOT APPLE)
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${WAYLAND_CLIENT_INCLUDE_DIRS}
        ${WAYLAND_EGL_INCLUDE_DIRS}
        ${XKBCOMMON_INCLUDE_DIRS}
        ${EGL_INCLUDE_DIRS}
        ${X11_INCLUDE_DIRS}
        ${CMAKE_CURRENT_BINARY_DIR}
    )
    target_link_libraries(${PROJECT_NAME} PUBLIC
        ${WAYLAND_CLIENT_LIBRARIES}
        ${WAYLAND_EGL_LIBRARIES}
        ${XKBCOMMON_LIBRARIES}
        ${EGL_LIBRARIES}
        ${X11_LIBRARIES}
        dl
    )
endif()

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC
        opengl32
        gdi32
        user32
        shell32
    )
endif()

add_executable(IvyTest src/ivy.c)
target_link_libraries(IvyTest PRIVATE ${PROJECT_NAME})

target_compile_definitions(${PROJECT_NAME} PUBLIC
		$<$<CONFIG:Debug>:IVY_DEBUG>
)
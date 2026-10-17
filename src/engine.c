#include "ivy/engine.h"
#include "ivy/core/fwd.h"
#include "ivy/core/log.h"
#include "ivy/core/types.h"
#include "ivy/core/time.h"
#include "ivy/graphics/gl_loader.h"
#include "ivy/graphics/renderer.h"
#include "ivy/platform/window.h"
#include "ivy/platform/input.h"
#include "ivy/platform/glfw_loader.h"

IvyStatus Ivy_Engine_Init(IvyEngine *engine)
{
    IVY_ASSERT(engine != NULL, "(IvyEngine) is NULL!");

    IVY_LOG_INFO("(IvyEngine) Initializing...");

    if (Ivy_GLFW_Load() != IVY_SUCCESS) {
        IVY_LOG_ERROR("Failed to dynamically load GLFW library.");
        return IVY_ERROR_LOAD_FAILED;
    }

    if (!glfwInit()) {
        IVY_LOG_ERROR("Failed to initialize GLFW.");
        Ivy_GLFW_Unload();
        return IVY_ERROR_NULL_PTR;
    }

    const IvyStatus window_status = Ivy_Window_Init(&engine->window, "IvyEngine", 800, 600);
    if (window_status != IVY_SUCCESS) {
        IVY_LOG_ERROR("Failed to initialize window.");
        glfwTerminate();
        Ivy_GLFW_Unload();
        return window_status;
    }

    glfwMakeContextCurrent(engine->window.handle);

    if (Ivy_GL_Load() != IVY_SUCCESS) {
        IVY_LOG_ERROR("Failed to load OpenGL functions.");
        Ivy_Window_Terminate(&engine->window);
        glfwTerminate();
        Ivy_GLFW_Unload();
        return IVY_ERROR_LOAD_FAILED;
    }

    Ivy_Renderer_Init(&engine->renderer, &engine->window);
    Ivy_Input_Init(&engine->input, &engine->window);
    Ivy_Time_Init(&engine->time);

    Ivy_Signal_Init(&engine->event_signal);

    glfwSwapInterval(1);

    engine->is_running = true;

    IVY_LOG_INFO("(IvyEngine) Initialized successfully.");
    return IVY_SUCCESS;
}

void Ivy_Engine_Run(IvyEngine *engine)
{
    IVY_ASSERT(engine, "(IvyEngine) is NULL!");

    GLFWwindow *window = engine->window.handle;

    IVY_LOG_INFO("(IvyEngine) Starting main loop...");

    while (engine->is_running && !glfwWindowShouldClose(window))
    {
        Ivy_Time_Update(&engine->time);
        Ivy_Input_Update(&engine->input);

        glfwPollEvents();

        // update
        // test
        if (Ivy_Input_IsKeyPressed(&engine->input, GLFW_KEY_SPACE)) {
            IVY_LOG_INFO("(IvyInput) Key Space Pressed!");
        }

        if (Ivy_Input_IsKeyDown(&engine->input, GLFW_KEY_SPACE)) {
            IVY_LOG_INFO("(IvyInput) Key Space Down!");
        }

        if (Ivy_Input_IsKeyReleased(&engine->input, GLFW_KEY_SPACE)) {
            IVY_LOG_INFO("(IvyInput) Key Space Released!");
        }

        // render
        glfwSwapBuffers(window);
    }

    IVY_LOG_INFO("(IvyEngine) Main loop exited.");
}

void Ivy_Engine_Stop(IvyEngine *engine)
{
    IVY_ASSERT(engine, "(IvyEngine) is NULL!");

    engine->is_running = false;

    IVY_LOG_INFO("(IvyEngine) Stopping main loop requested.");
}

void Ivy_Engine_Terminate(IvyEngine *engine)
{
    IVY_ASSERT(engine, "(IvyEngine) is NULL!");

    Ivy_Renderer_Terminated(&engine->renderer);
    Ivy_Window_Terminate(&engine->window);
    glfwTerminate();
    Ivy_GLFW_Unload();

    IVY_LOG_INFO("(IvyEngine) Terminated.");
}
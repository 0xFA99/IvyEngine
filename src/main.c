#include "engine.h"
#include "log.h"

int main(void)
{
    IvyEngine engine;

    const IvyStatus status = Ivy_Engine_Init(&engine);
    if (status != IVY_SUCCESS) {
        IVY_LOG_FATAL("IvyEngine failed to init, error code: %d", status);
        return EXIT_FAILURE;
    }

    Ivy_Engine_Run(&engine);
    Ivy_Engine_Stop(&engine);
    Ivy_Engine_Terminate(&engine);

    return 0;
}
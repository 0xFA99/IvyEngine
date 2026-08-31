#include "engine.h"
#include "log.h"

int main(void)
{
    const IvyLogConfig logConfig = {
#ifdef IVY_DEBUG
        .mode               = IVY_LOG_MODE_SYNC,
        .min_level          = IVY_LOG_TRACE,
#else
        .mode		        = IVY_LOG_MODE_ASYNC,
        .min_level          = IVY_LOG_INFO,
#endif
        .policy		        = IVY_LOG_POLICY_DROP,
        .queue_capacity     = 4096,
        .batch_size         = 128,
        .flush_interval_ms  = 50,
        .use_color          = true,
        .show_timestamp     = true
    };

    Ivy_Log_Init(&logConfig);

    IvyEngine engine;

    const IvyStatus status = Ivy_Engine_Init(&engine);
    if (status != IVY_SUCCESS) {
        IVY_LOG_FATAL("IvyEngine failed to init, error code: %d", status);
        return EXIT_FAILURE;
    }

    Ivy_Engine_Run(&engine);
    Ivy_Engine_Stop(&engine);
    Ivy_Engine_Terminate(&engine);

    Ivy_Log_Terminate();

    return 0;
}
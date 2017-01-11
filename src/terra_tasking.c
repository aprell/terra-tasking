#include "tasking.h"

void terra_tasking_init(int *argc, char **argv[])
{
    TASKING_INIT(argc, argv);
}

void terra_tasking_exit(void)
{
    TASKING_EXIT();
}

void terra_tasking_barrier(void)
{
    TASKING_BARRIER();
}

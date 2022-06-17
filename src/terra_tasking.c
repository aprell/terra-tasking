#include "tasking.h"

void terra_tasking_init(int *argc, char **argv[])
{
    tasking_init(argc, argv);
}

void terra_tasking_exit(void)
{
    tasking_exit_signal();
    tasking_exit();
}

void terra_tasking_barrier(void)
{
    RT_barrier();
}

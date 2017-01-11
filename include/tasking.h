#ifndef TASKING_H
#define TASKING_H

#define TASKING_INIT(argc, argv) \
do { \
    tasking_init(argc, argv); \
} while (0)

#define TASKING_EXIT() \
do { \
    tasking_exit_signal(); \
    tasking_exit(); \
} while (0)

#define TASKING_BARRIER() \
do { \
    RT_barrier(); \
} while (0)

int tasking_init(int *argc, char ***argv);
int tasking_exit_signal(void);
int tasking_exit(void);
int RT_barrier(void);

#endif // TASKING_H

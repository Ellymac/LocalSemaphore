#include <unistd.h>

static inline long sem_initialize(int nb){
    return syscall(337,nb);
}

static inline long sem_destroy(int nb)
{
    return syscall(338, nb);
}

static inline long sem_acquire(int nb)
{
    return syscall(339, nb);
}

static inline long sem_release(int nb)
{
    return syscall(340, nb);
}
static inline long sem_dbg(int nb)
{
    return syscall(341, nb);
}

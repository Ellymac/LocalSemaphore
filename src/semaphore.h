struct t_sem
{
    int id;
    int nb_max;
    int nb_available;
    int *waitlist;
};

typedef struct t_sem *sem;

sem sem_initialize(int nb);
int sem_destroy(int sid);
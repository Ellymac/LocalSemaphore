struct t_sem
{
    int id;
    int nb_max;
    int nb_available;
    task_struct *waitlist;
    int nb_elt_proc;
    int count_ref;
};

typedef struct t_sem *sem;

sem sem_initialize(int nb);
int sem_destroy(int sid);
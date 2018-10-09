
#include "RBcomputation.h"

//typedef int pthread_barrierattr_t;

void my_barrier_init(my_barrier_t *barrier, unsigned int count)
{
    
    barrier->tripCount = count;
    barrier->count = 0;
    pthread_mutex_init(&(barrier->mutex), NULL);
    pthread_cond_init(&(barrier->cond), NULL);
}

int my_barrier_destroy(my_barrier_t *barrier)
{
    pthread_cond_destroy(&barrier->cond);
    pthread_mutex_destroy(&barrier->mutex);
    return 0;
}

int my_barrier_wait(my_barrier_t *barrier)
{
    pthread_mutex_lock(&barrier->mutex);
    ++(barrier->count);
    if(barrier->count >= barrier->tripCount)
    {
        barrier->count = 0;
        pthread_cond_broadcast(&barrier->cond);
        pthread_mutex_unlock(&barrier->mutex);
        return 1;
    }
    else
    {
        pthread_cond_wait(&barrier->cond, &(barrier->mutex));
        pthread_mutex_unlock(&barrier->mutex);
        return 0;
    }
}

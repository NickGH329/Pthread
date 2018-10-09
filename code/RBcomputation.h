#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <pthread.h>
#include<sys/time.h>

int **build_array(int row,int colum);
void free_malloc(int **board);

void get_input(int *n_t,int *n,int *t,int *c,int *max_iters);
void print_board(int **board, int row,int column);
int* print_tile(int **board,int id, int n,int t);

void swap(int *a,int *b);
void board_init(int **board,int n);
int *alloc_procs(int numprocs, int t);

void RedMove(int **pro_board, int row, int column);
void BlueMove(int **pro_board, int n,int start, int end);
void *check_c(int **pro_board,int k,int n,int t,int c);

//typedef struct pthread_barrier_t pthread_barrier_t;
typedef struct
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int count;
    int tripCount;
} my_barrier_t;
void my_barrier_init(my_barrier_t *barrier, unsigned int count);
int my_barrier_destroy(my_barrier_t *barrier);
int my_barrier_wait(my_barrier_t *barrier);

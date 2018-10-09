#include "RBcomputation.h"

int finish = 0;
pthread_mutex_t finish_mutex;
my_barrier_t barrier;
int *result1,*result2;
struct timeval begin,stop;
double r1,r2;
int n,k,c,max_iters,t;
int **board,**board_copy;

struct thr_data{
    int id;
    int start;
    int end;
    int tile_num;
};

void *do_work(void *thr_arg){
    struct thr_data *t_data;
    int id,start,end,tile_num,n_iter;
    int i,j;

//get the values
    n_iter = 0;
    t_data = (struct thr_data *) thr_arg;
    id = t_data->id;
    start = t_data->start;
    end = t_data->end;
    tile_num = t_data->tile_num;
    k = end - start;

    while(n_iter < max_iters && finish ==0){
        RedMove(&board[start],k,n);
        my_barrier_wait(&barrier);
        BlueMove(board,n,start,end);
   
        int *p =(int*)malloc(tile_num*t*sizeof(int));
		p=check_c(&board[start],k,n,t,c);
		for(i = 0; i < tile_num*t; i++){
			if(p[i] != 0){
                pthread_mutex_lock (&finish_mutex);
				finish = 1;
                pthread_mutex_unlock (&finish_mutex);
				printf("\nParallel computation finished, the tile has colored cells than %d%% is:\n",c);
				result1 = print_tile(&board[start],i,n,t);
			}
		}
        my_barrier_wait(&barrier);
        if(finish==1){
            break;
        }
        n_iter++;
    }
    if(n_iter == max_iters && id == 0)
		printf("\nParallel computation finished, Run out of max_iters and no tile has the colored squares more than %d%% one color (blue or red).\n",c);
    
    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    int n_t,tile;
    int *tile_num;
    int n_iter;
    int i,j;
    struct thr_data *t_arg;
    pthread_t *thread_id;
    pthread_attr_t attr;

    get_input(&n_t,&n,&t,&c,&max_iters);
    board = build_array(n,n);
    board_copy = build_array(n,n);
    board_init(board,n);
    for(i =0; i < n; i++)
        for(j = 0; j < n; j++)
            board_copy[i][j]=board[i][j];
    tile = n/t;
    tile_num = (int*)malloc(n_t*sizeof(int));
    tile_num = alloc_procs(n_t,t);

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    my_barrier_init(&barrier, n_t);

    thread_id = (pthread_t *)malloc(sizeof(pthread_t)*n_t);
	t_arg = (struct thr_data *)malloc(sizeof(struct thr_data)*n_t);
    for(i = 0; i < n_t; i++){
        t_arg[i].id = i;
        t_arg[i].start = tile_num[i]*tile*i;
        t_arg[i].end = t_arg[i].start + tile_num[i]*tile;
        t_arg[i].tile_num = tile_num[i];

        pthread_create(&thread_id[i],&attr,do_work,(void *) &t_arg[i]);
    }

    gettimeofday(&begin, NULL);
    for (i=0; i < n_t; i++) {
            pthread_join(thread_id[i], NULL);
        }
    gettimeofday(&stop, NULL);
    r1 = (double)stop.tv_sec - begin.tv_sec + (1.0 * (stop.tv_usec - begin.tv_usec))/1000000;
    my_barrier_destroy(&barrier);

//Excute the sequentive computation  
    gettimeofday(&begin, NULL);
    n_iter = 0;
    finish = 0;      
    while(n_iter < max_iters && finish ==0){
        RedMove(board_copy,n,n);
        BlueMove(board_copy,n,0,n);
		int *p =(int*)malloc(t*t*sizeof(int));
		p=check_c(board_copy,n,n,t,c);
		for(i = 0; i < t*t; i++){
			if(p[i] != 0){
				finish = 1;
				printf("\nSequential computation finished, the tile has colored cells than %d%% is:\n",c);
				result2 = print_tile(board_copy,i,n,t);
			}
		}
		n_iter++;
    }
    if(n_iter == max_iters)
		printf("\nSequential computation finished, Run out of max_iters and no tile has the colored squares more than %d%% one color (blue or red).\n",c);
    gettimeofday(&stop, NULL);
    r2 = (double)stop.tv_sec - begin.tv_sec + (1.0 * (stop.tv_usec - begin.tv_usec))/1000000;
    printf("\nThe running time of Parallel computation is %f\n", r1);
    printf(" The running time of Sequential computation is %f\n", r2);
//check the results of parallel and sequentive computation
    int num_count = 0;
    for(i = 0; i < tile*tile; i++){
        //printf("%d",result1[i]);
        if(result1[i] == result2[i])
            num_count++;
    }
    
    if(num_count == tile*tile)
        printf("The Parallel computation and Sequential computation have same result\n");
    else
        printf("The Parallel computation and Sequential computation have different result\n");
    
    free(tile_num);
    free(result1);
    free(result2);
    free_malloc(board);
    free_malloc(board_copy);

    /* Clean up and exit */
	pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&finish_mutex);
	pthread_exit (NULL);
    return 0;
}

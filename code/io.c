#include<stdio.h>
#include<stdlib.h>

void get_input(int *n_t,int *n,int *t,int *c,int *max_iters){
	printf("Please input the number of the threads:\n");
	scanf("%d",n_t);
	printf("Please input n:\n");
    scanf("%d",n);
    printf("Please input t:\n");
    scanf("%d",t);
    printf("Please input c:\n");
    scanf("%d",c);
    printf("Please input the max iteration number:\n");
    scanf("%d",max_iters);
}

//print board
void print_board(int **board, int row,int column){
	int i,j;
	for(i = 0; i < row; i++)
		for(j = 0; j < column; j++){
			printf("%d ",board[i][j]);
			if(j == column-1)
				printf("\n");
		}
}
int* print_tile(int **board,int id, int n,int t){
	int tile = n/t;
	int i,j;
	int *result = (int*)malloc(tile*tile*sizeof(int));
	int count=0;
	for(i = id/t*tile; i < (id/t+1)*tile; i++)
		 for(j = id%t*tile; j < (id%t+1)*tile; j++){
			 result[count++] = board[i][j];
			 printf("%d ", board[i][j]);
			 if(j == (id%t+1)*tile - 1)
				  printf("\n");
		 }
	return result;
}


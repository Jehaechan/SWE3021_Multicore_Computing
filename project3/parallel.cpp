#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
int count_nbr (char** grid, int i, int j, int x_limit, int y_limit)
{
    int n_count = 0;
    if (i-1 >= 0 && j-1 >= 0){
        if (grid[i-1][j-1]){
            n_count++;
        }
    }

    if (i-1 >= 0){
        if(grid[i-1][j]){
            n_count++;
        }
    }

    if (i-1 >= 0 && j+1 < y_limit){
        if(grid[i-1][j+1]){
            n_count++;
        }
    }

    if (j-1 >= 0){
        if(grid[i][j-1]){
            n_count++;
        }
    }

    if (j+1 < y_limit){
        if(grid[i][j+1]){
            n_count++;
        }
    }

    if (i+1 < x_limit && j-1 >=0){
        if(grid[i+1][j-1]){
            n_count++;
        }
    }

    if (i+1 < x_limit){
        if(grid[i+1][j]){
            n_count++;
        }
    }

    if (i+1 < x_limit && j+1 < y_limit){
        if(grid[i+1][j+1]){
            n_count++;
        }
    }

    return n_count;
}

int main(int argc, char* argv[]){
    char* file_name = argv[1];
    int num_gen = atoi(argv[2]);
    int x_limit = atoi(argv[3]);
    int y_limit = atoi(argv[4]);

    MPI_Init(&argc, &argv);
    int np, Iam;
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &Iam);

    int x_size = x_limit + (np - (x_limit % np)) % np;
    char** board = (char**)malloc(sizeof(char*) * x_size);
    board[0] = (char*)calloc(x_size*y_limit, sizeof(char));
    for(int i = 0; i < x_size; i++){
        board[i] = board[0] + i*y_limit;
    }

    char** board_cpy = (char**)malloc(sizeof(char*) * x_size);
    board_cpy[0] = (char*)calloc(x_size*y_limit, sizeof(char));
    for(int i = 0; i < x_size; i++){
        board_cpy[i] = board_cpy[0] + i*y_limit;
    }

    FILE *fp;
    fp = fopen(file_name, "r");
    if(fp == NULL){
        printf("File Read Error!\n");
        return 0;
    }

    char buf[100];
    while(!feof(fp)){
        fgets(buf, 100, fp);
        int i = 0;
        int startpoint;
        while(buf[i] != '\0'){
            if(buf[i] == ' '){
                buf[i] = '\0';
                startpoint = i + 1;
            }
            if(buf[i] == '\n')
                buf[i] = '\0';
            i++;
        }
        board[atoi(buf)][atoi(buf + startpoint)] = 1;
    }
      
    int partition = x_size / np;
    int startpoint = partition * Iam;

    for(int steps = 0; steps < num_gen; steps++){
        if(steps)
            MPI_Bcast(board[0], x_limit*y_limit, MPI_CHAR, 0, MPI_COMM_WORLD);
        memcpy(board_cpy[0], board[0], sizeof(char)*x_limit*y_limit);

        for(int i = startpoint; i < startpoint + partition && i < x_limit; i++){
            for(int j = 0; j < y_limit; j++){
                int nbr_cnt = count_nbr(board_cpy, i, j, x_limit, y_limit);
                if(board[i][j]){
                    if(nbr_cnt < 2 || nbr_cnt > 3){
                        board[i][j] = 0;
                    }
                }
                else{
                    if(nbr_cnt == 3){
                        board[i][j] = 1;
                    }
                }
            }
        }
        MPI_Gather(board[startpoint], partition * y_limit, MPI_CHAR, board[0], partition * y_limit, MPI_CHAR, 0, MPI_COMM_WORLD);

    }
    if(Iam == 0){
        for(int i = 0; i < x_limit; i++){
            for(int j = 0; j < y_limit; j++){
                if(board[i][j]){
                    printf("%d %d\n",i,j);
                }
            }
        }
    }
    MPI_Finalize();
    return 0;
  
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

// board[x][y]
void printboard(char** board, int x_limit, int y_limit){
    for(int i = y_limit - 1; i >= 0; i--){
        for(int j = 0; j < x_limit; j++){
            printf("%d ",board[j][i]);
        }
        printf("\n");
    }
}

int count_nbr (char** grid, int i, int j, int x_limit, int y_limit)
{
    int n_count = 0;
    if (i-1 >= 0 && j-1 >= 0){
        //printf("1 ");
        if (grid[i-1][j-1]){
            //printf("o ");
            n_count++;
        }
    }

    if (i-1 >= 0){
        //printf("2 ");
        if(grid[i-1][j]){
            //printf("o ");
            n_count++;
        }
    }

    if (i-1 >= 0 && j+1 < y_limit){
        //printf("3 ");
        if(grid[i-1][j+1]){
            //printf("o ");
            n_count++;
        }
    }

    if (j-1 >= 0){
        //printf("4 ");
        if(grid[i][j-1]){
            //printf("o ");
            n_count++;
        }
    }

    if (j+1 < y_limit){
        //printf("5 ");
        if(grid[i][j+1]){
            //printf("o ");
            n_count++;
        }
    }

    if (i+1 < x_limit && j-1 >=0){
        //printf("6 ");
        if(grid[i+1][j-1]){
            //printf("o ");
            n_count++;
        }
    }

    if (i+1 < x_limit){
        //printf("7 ");
        if(grid[i+1][j]){
            //printf("o ");
            n_count++;
        }
    }

    if (i+1 < x_limit && j+1 < y_limit){
        //printf("8 ");
        if(grid[i+1][j+1]){
            //printf("o ");
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
    //printf("%s %d %d %d\n", file_name, num_gen, x_limit, y_limit);

    MPI_Init(&argc, &argv);
    int np;
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    char** board = (char**)malloc(sizeof(char*) * x_limit);
    board[0] = (char*)calloc(x_limit*y_limit, sizeof(char));
    for(int i = 0; i < x_limit; i++){
        board[i] = board[0] + i*y_limit;
    }

    char** board_cpy = (char**)malloc(sizeof(char*) * x_limit);
    board_cpy[0] = (char*)calloc(x_limit*y_limit, sizeof(char));
    for(int i = 0; i < x_limit; i++){
        board_cpy[i] = board_cpy[0] + i*y_limit;
    }
    /*
    char** board_cpy = (char**)malloc(sizeof(char*) * x_limit);
    for(int i = 0; i < x_limit; i++){
        board_cpy[i] = (char*)calloc(y_limit, sizeof(char));
    }
    */

    //printboard(board, x_limit, y_limit);<F7>

    FILE *fp;
    fp = fopen(file_name, "r");
    if(fp == NULL){
        printf("File Read Error!\n");
        return 0;
    }

    char buf[100];
    while(!feof(fp)){
        fgets(buf, 100, fp);
        //printf("%s",buf);
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
        //printf("%s %s\n", buf, buf + startpoint);
        board[atoi(buf)][atoi(buf + startpoint)] = 1;
    }
      
    int* cnt_arr = (int*)malloc(sizeof(int)*(np + 1));
    int quo = x_limit / np;
    int rem = x_limit % np;
    for(int i = 0; i < np; i++)
        cnt_arr[i] = quo;
    for(int i = 0; i < np; i++){
        if(rem == 0)
            break;
        cnt_arr[i]++;
        rem--;
    }
    cnt_arr[np] = x_limit;
    for(int i = 1; i <= np; i++){
         cnt_arr[i] = cnt_arr[i] + cnt_arr[i-1];
    }
    for(int i = 0; i < np; i++){
        printf("%d ",cnt_arr[i]);
    }
    for(int steps = 0; steps < num_gen; steps++){
        if(steps)
            MPI_Bcast(board, x_limit*y_limit, MPI_CHAR, 0, MPI_COMM_WORLD);
        memcpy(board_cpy[0], board[0], sizeof(char)*x_limit*y_limit);

        //printboard(board_cpy, x_limit, y_limit);
        for(int i = 0; i < x_limit; i++){
            for(int j = 0; j < y_limit; j++){
                int nbr_cnt = count_nbr(board_cpy, i, j, x_limit, y_limit);
                //printf("i: %d, j: %d, nbr_cnt: %d\n", i, j, nbr_cnt);
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


    }
  

    //printboard(board, x_limit, y_limit);
}

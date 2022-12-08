#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <math.h>
#include "omp.h"

#define row(idx, N) (idx / N)
#define col(idx, N) (idx % N)


void shortest_path(int* edges_in, int *edges_out, int N){
    int tid,src,dst,via;
    int *tmp;
    int change;
    int loop = 0;

    while(1){
	// multiply edges_in by itself
        loop++;
	    change=0;
        for(tid = 0; tid < N*N; tid++) {
		    edges_out[tid] = edges_in[tid];
		    src = row(tid,N);
		    dst = col(tid,N);
            for(via=0;via<N;via++) {
                if(edges_in[src*N + via] != INT_MAX && edges_in[via*N + dst] != INT_MAX) {
			        if(edges_out[tid] > (edges_in[src*N + via]+edges_in[via*N + dst])){ 
                        edges_out[tid] = edges_in[src*N + via]+edges_in[via*N + dst]; 
			            change=1;
			        }
                }
            }
        }
	    if(change){
            tmp = edges_out; 
            edges_out = edges_in;
            edges_in = tmp;
	    }
	    else break;
    }
}

int main(int argc, char *argv[])
{      
    int i,j, step, src, dst, via, weight;
    int N;
    int *edges_in, *edges_out, *tmp;

    if(argc < 2){
        printf("Usage: %s input_file \n", argv[0]);
        exit(1);
    }

    FILE *fp = fopen(argv[1], "r");
    if(fp==NULL) {
        perror("File open error: ");
        exit(1);
    }

    fscanf(fp, "%d", &N);
    edges_in = (int*) malloc(sizeof(int)*N*N);
    edges_out = (int*) malloc(sizeof(int)*N*N);
    for(i=0;i<N*N;i++){
        if(row(i,N) == col(i,N)) {
            edges_in[i] = 0;
	}
	else {
            edges_in[i] = INT_MAX;
	}
    }

    while (fscanf(fp, "%d %d %d\n", &src, &dst, &weight) > 0) {
        edges_in[src*N + dst] = weight;
    }

    shortest_path(edges_in, edges_out, N);

    for(i = 0; i < N*N; i++) {
            if(edges_out[i] !=0 && edges_out[i] != INT_MAX){
                printf("%d %d %d\n", row(i,N), col(i,N), edges_out[i]);
            }
    }

    free(edges_in);
    free(edges_out);

    return 0;
}


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <math.h>

#define row(idx, N) (idx / N)
#define col(idx, N) (idx % N)

__global__ void shortest_path(int* edges_in, int *edges_out, int N, int* change){
	// multiply edges_in by itself
    int tid, via, src, dst;
    tid = threadIdx.x + blockIdx.x * 1024;
    
    edges_out[tid] = edges_in[tid];
    src = row(tid,N);
    dst = col(tid,N);
    for(via = 0; via < N; via++) {
        if(edges_in[src*N + via] != INT_MAX && edges_in[via*N + dst] != INT_MAX){
            if(edges_out[tid] > (edges_in[src*N + via]+edges_in[via*N + dst])){
                edges_out[tid] = edges_in[src*N + via]+edges_in[via*N + dst];
                *change = 1;
            }
        }
    }
}

int main(int argc, char *argv[])
{      
    int i, src, dst, weight;
    int N;
    int *edges_in, *edges_out;

    // Time
    /*
    time_t start, end;
    double result;
    start = clock();
    */

    if(argc < 3){
        printf("Usage: %s input_file <print>\n", argv[0]);
        exit(1);
    }

    FILE *fp = fopen(argv[1], "r");
    if(fp==NULL) {
        perror("File open error: ");
        exit(1);
    }

    int print_flag = atoi(argv[2]);

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

    // cudaMalloc
    int *edges_in_d, *edges_out_d, *change_d, *tmp;
    int change;
    cudaMalloc((void**)&edges_in_d, sizeof(int)*N*N);
    cudaMalloc((void**)&edges_out_d, sizeof(int)*N*N);
    cudaMalloc((void**)&change_d, sizeof(int));
    
    // cudaMemcpy from host to device
    cudaMemcpy(edges_in_d, edges_in, sizeof(int)*N*N, cudaMemcpyHostToDevice);

    while(1){
        change = 0;
        cudaMemcpy(change_d, &change, sizeof(int), cudaMemcpyHostToDevice);
        shortest_path<<<1024,1024>>>(edges_in_d, edges_out_d, N, change_d); 
        cudaMemcpy(&change, change_d,sizeof(int), cudaMemcpyDeviceToHost);
        if(change){
            tmp = edges_out_d;
            edges_out_d = edges_in_d;
            edges_in_d = tmp;
        }
        else break;
    }

    // cudaMemcpy from device to host 
    cudaMemcpy(edges_out, edges_out_d, sizeof(int)*N*N, cudaMemcpyDeviceToHost);

    if(print_flag){
        for(i = 0; i < N*N; i++) {
            if(edges_out[i] !=0 && edges_out[i] != INT_MAX){
                printf("%d %d %d\n", row(i,N), col(i,N), edges_out[i]);
            }
        }
    }

    free(edges_in);
    free(edges_out);

    // Time
    /*
    end = clock();
    result = (double)(end - start)/CLOCKS_PER_SEC;
    if(!print_flag)
        printf("%fms\n",result);
    */

    return 0;
}

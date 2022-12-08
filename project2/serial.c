#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// for qsort
#include <string.h>

#define BILLION  1000000000L

// for qsort
int compare(const void *a, const void *b){
    return strcmp((const char*)a, (const char*)b);
}

int main(int argc, char* argv[]){
    // declarations
    struct timespec start, stop;

    // read an input file
    char* file_name = argv[1];
    long long total_num = atoll(argv[2]);
    long long start_idx = atoll(argv[3]);
    long long print_num = atoll(argv[4]);
    long long num_threads = atoll(argv[5]);

    /*
    printf("file_name: %s\n", file_name);
    printf("total_num: %lld\n", total_num);
    printf("start_idx: %lld\n", start_idx);
    printf("print_num: %lld\n", print_num);
    printf("num_threads: %lld\n", num_threads);
    */

    char** arr = (char**)malloc(sizeof(char*)*total_num);
    arr[0] = (char*)malloc(sizeof(char)*total_num*257);
    FILE *fp;
    fp = fopen(file_name, "r");
    if(fp == NULL){
        printf("File Read Error!\n");
        exit(-1);
    }

    for(int i = 0; i < total_num; i++){
        arr[i] = arr[0] + i * 257;
        fgets(arr[i], sizeof(char)*257, fp);
        int len = strlen(arr[i]);
        if(len > 0 && arr[i][len-1] == '\n')
            arr[i][len-1] = 0;
        //printf("%s\n",arr[i]);
        //printf("%p %p ", (char*)arr, (char*)arr + i * 257);
        //printf("%p\n", arr[i]);
    }

    clock_gettime( CLOCK_REALTIME, &start);

    // sorting... - qsort
    qsort((void *)&arr[0][0], total_num, sizeof(char)*257, compare);

    clock_gettime( CLOCK_REALTIME, &stop);
    
    
    // sorting... - parallel radix sort
    

    for(int i = start_idx; i < start_idx + print_num; i++){
        printf("%s\n", arr[i]);
    }
    

    double elapsed_time = (stop.tv_sec - start.tv_sec) + ((double) (stop.tv_nsec - start.tv_nsec))/BILLION;

    printf("Elapsed time: %f sec\n", elapsed_time);
    return 0;
}

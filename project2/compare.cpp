#include <iostream>
#include <fstream>
#include <cstring>
#include <omp.h>

// for qsort
#include <algorithm>

using namespace std;

#define BILLION  1000000000L

int main(int argc, char* argv[]){
    // declarations
    struct timespec start, stop;

    char* file_name = argv[1];
    int total_num = atoi(argv[2]);
    int start_idx = atoi(argv[3]);
    int print_num = atoi(argv[4]);
    int num_threads = atoi(argv[5]);

    // set OMP_NUM_THREADS
    omp_set_num_threads(num_threads);
    
    // Dynamic Allocation
    string *arr = new string[total_num];

    int** hist = (int**)malloc(sizeof(int*) * num_threads);
    
    for(int i = 0; i < num_threads; i++){
        hist[i] = (int*)malloc(sizeof(int) * 256);
    }
    /*
    stack<string> **hist = new stack<string>*[num_threads];
    for(int i = 0; i < num_threads; i++)
        hist[i] = new stack<string>[256];
    */

    // read an input file
    ifstream inputfile;
    inputfile.open(file_name);

    for(int i = 0; i < total_num; i++) 
        getline(inputfile, arr[i]);

    // sorting... - qsort
    /*
    clock_gettime( CLOCK_REALTIME, &start);

         sort(arr, arr + total_num);

    clock_gettime( CLOCK_REALTIME, &stop);
    */

    // sorting... - parallel radix sort

    // FOR LOOP
    clock_gettime(CLOCK_REALTIME, &start);

    int i;
    for(i = 0; i < num_threads; i++)
    {
        for(int j = 0; j < 256; j++)
            hist[i][j] = 0;
    }
        
    clock_gettime(CLOCK_REALTIME, &stop);
    double elapsed_time = (stop.tv_sec - start.tv_sec) + ((double) (stop.tv_nsec - start.tv_nsec))/BILLION;
    cout << "<FOR LOOP>\nElapsed time: " << elapsed_time << " sec\n" << endl;



    // MEMSET
    for(i = 0; i < num_threads; i++)
    {
        memset(hist[i], 0, sizeof(int)*255);
    }
        
    clock_gettime(CLOCK_REALTIME, &stop);
    elapsed_time = (stop.tv_sec - start.tv_sec) + ((double) (stop.tv_nsec - start.tv_nsec))/BILLION;
    cout << "<memset>\nElapsed time: " << elapsed_time << " sec\n" << endl;



    // just making thread
    clock_gettime(CLOCK_REALTIME, &start);

#pragma omp parallel shared(hist) num_threads(num_threads)
{
}
    clock_gettime(CLOCK_REALTIME, &stop);
    elapsed_time = (stop.tv_sec - start.tv_sec) + ((double) (stop.tv_nsec - start.tv_nsec))/BILLION;
    cout << "<just making thread>\nElapsed time: " << elapsed_time << " sec\n" << endl;



    // FOR LOOP + parallel
    clock_gettime(CLOCK_REALTIME, &start);

#pragma omp parallel shared(hist) num_threads(num_threads)
{
    #pragma omp for private(i)
    for(i = 0; i < num_threads; i++)
    {
        for(int j = 0; j < 256; j++)
            hist[i][j] = 0;
    }
        
}
    clock_gettime(CLOCK_REALTIME, &stop);
    elapsed_time = (stop.tv_sec - start.tv_sec) + ((double) (stop.tv_nsec - start.tv_nsec))/BILLION;
    cout << "<FOR LOOP + parallel>\nElapsed time: " << elapsed_time << " sec\n" << endl;



    // FOR LOOP + memset
    clock_gettime(CLOCK_REALTIME, &start);

#pragma omp parallel shared(hist) num_threads(num_threads)
{
    #pragma omp for private(i)
    for(i = 0; i < num_threads; i++)
    {
        memset(hist[i], 0, sizeof(int)*255);
    }
        
}
    clock_gettime(CLOCK_REALTIME, &stop);
    elapsed_time = (stop.tv_sec - start.tv_sec) + ((double) (stop.tv_nsec - start.tv_nsec))/BILLION;
    cout << "<MEMSET + parallel>\nElapsed time: " << elapsed_time << " sec\n" << endl;

    /*
    for(int i = start_idx; i < start_idx + print_num; i++){
        cout << arr[i] << endl;
    }
    */

    // double elapsed_time = (stop.tv_sec - start.tv_sec) + ((double) (stop.tv_nsec - start.tv_nsec))/BILLION;
    // cout << "Elapsed time: " << elapsed_time << " sec" << endl;
    return 0;
}

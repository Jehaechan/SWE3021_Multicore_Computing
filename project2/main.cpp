#include <iostream>
#include <fstream>
#include <string>
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

    string *arr = new string[total_num];
    // read an input file
    ifstream inputfile;
    inputfile.open(file_name);
    
    for(int i = 0; i < total_num; i++) 
        getline(inputfile, arr[i]);
    
    clock_gettime( CLOCK_REALTIME, &start);

    // sorting... - qsort
    sort(arr, arr + total_num);

    clock_gettime( CLOCK_REALTIME, &stop);
    
    
    // sorting... - parallel radix sort
    

    for(int i = start_idx; i < start_idx + print_num; i++){
        cout << arr[i] << endl;
    }
    

    double elapsed_time = (stop.tv_sec - start.tv_sec) + ((double) (stop.tv_nsec - start.tv_nsec))/BILLION;

    printf("Elapsed time: %f sec\n", elapsed_time);
    return 0;
}

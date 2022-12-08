#include <iostream>
#include <fstream>
#include <cstring>
#include <omp.h>

// for qsort
#include <algorithm>

using namespace std;

#define BILLION  1000000000L
#define EXT_ASCII_SIZE 256

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
    
    // dynamic allocation - string array
    string *arr = new string[total_num];
    string *buffer = new string[total_num];

    // read an input file
    ifstream inputfile;
    inputfile.open(file_name);

    int max_digit = 0;
    for(int i = 0; i < total_num; i++){ 
        getline(inputfile, arr[i]);
        int length = arr[i].length();
        if(max_digit < length)
            max_digit = length;
    }
    inputfile.close();
    
    size_t i;
    clock_gettime(CLOCK_REALTIME, &start);
    
    for(int digit = 0; digit < max_digit; digit++){
        size_t bucket[EXT_ASCII_SIZE] = { 0 };
        int tgt = max_digit - 1;

        size_t local_bucket[EXT_ASCII_SIZE] = { 0 };
        #pragma omp parallel firstprivate(local_bucket)
        {
            //printf("%d\n",digit);
            #pragma omp for schedule(static) nowait
            for(i = 0; i < total_num; i++) {
                unsigned char tmp;
                string str = arr[i];
                if(str.length() < max_digit - digit) {
                    tmp = 0;
                }
                else{
                    tmp = str[tgt - digit];
                }
                //printf("tmp: %c\n",tmp);
                local_bucket[tmp]++;
            }

            #pragma omp critical
            for (i = 0; i < EXT_ASCII_SIZE; i++) {
                bucket[i] += local_bucket[i];
            }
            #pragma omp barrier

            #pragma omp single
            for(i = 1; i < EXT_ASCII_SIZE; i++) {
                bucket[i] += bucket[i - 1];
            }
            int tid = omp_get_thread_num();
            for(int cur_t = num_threads - 1; cur_t >= 0; cur_t--) {
                if(cur_t == tid) {
                    for(i = 0; i < EXT_ASCII_SIZE; i++) {
                        bucket[i] -= local_bucket[i];
                        local_bucket[i] = bucket[i];
                    }
                }
                else {
                    #pragma omp barrier
                }
            }

            #pragma omp for schedule(static)
            for(i = 0; i < total_num; i++) {
                unsigned char tmp;
                string str = arr[i];
                if(str.length() < max_digit - digit) {
                    tmp = 0;
                }
                else{
                    tmp = str[tgt - digit];
                }
                buffer[local_bucket[tmp]++] = arr[i];
            }
        }
        string *temp = arr;
        arr = buffer;
        buffer = temp;
    }
    
    clock_gettime(CLOCK_REALTIME, &stop);

    for(i = start_idx; i < start_idx + print_num; i++) {
        cout << arr[i] << endl;
    }

    double elapsed_time = (stop.tv_sec - start.tv_sec) + ((double) (stop.tv_nsec - start.tv_nsec))/BILLION;
    cout << "Elapsed time: " << elapsed_time << " sec" << endl;
    
    return 0;
}

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
    string *arr2 = new string[total_num];

    // dynamic allocation - histogram               hist[tid][digit]
    int** hist = (int**)malloc(sizeof(int*) * num_threads);
    for(int i = 0; i < num_threads; i++){
        hist[i] = (int*)malloc(sizeof(int) * EXT_ASCII_SIZE);
    }

    // dynamic allocation - prefix sum              prefix_sum[digit][tid+1]
    int** prefix_sum = (int**)malloc(sizeof(int*) * EXT_ASCII_SIZE);
    for(int i = 0; i < EXT_ASCII_SIZE; i++){
        prefix_sum[i] = (int*)calloc(num_threads + 1, sizeof(int));
    }

    // dynamic allocation - offset
    int* offset = new int(total_num);

    int* threadnum = new int(total_num);

    int prefix_digit[EXT_ASCII_SIZE + 1];
    prefix_digit[0] = 0;

    // read an input file
    ifstream inputfile;
    inputfile.open(file_name);

    int max_digit = 0;
    for(int i = 0; i < total_num; i++){ 
        getline(inputfile, arr[i]);
        arr2[i] = arr[i];
        int length = arr[i].length();
        if(max_digit < length)
            max_digit = length;
    }
    inputfile.close();

    // sorting... - parallel radix sort

    int i, digit;

    clock_gettime(CLOCK_REALTIME, &start);

    for(digit = 0; digit < max_digit; digit++){
    #pragma omp parallel shared(hist, arr, arr2, prefix_sum, offset, threadnum) num_threads(num_threads)
    {
        //printf("digit: %d\n",digit);
        int tid = omp_get_thread_num();
        string *cur_arr;
        string *next_arr;
        if(digit % 2 == 0){
            cur_arr = arr;
            next_arr = arr2;
        }
        else{
            cur_arr = arr2;
            next_arr = arr;
        }
        #pragma omp for private(i)
        for(i = 0; i < num_threads; i++)
        {
            memset(hist[i], 0, sizeof(int) * EXT_ASCII_SIZE);
        }
        #pragma omp barrier
        
        // histogram
        #pragma omp for private(i)
        for(i = 0; i < total_num; i++)
        {
            threadnum[i] = omp_get_thread_num();
            printf("%d\n",threadnum[i]);
            unsigned char target_char;
            string str = cur_arr[i];
            if(str.length() < digit) {
                target_char = 0;
            }
            else {
                target_char = str[digit];
            }
            offset[i] = hist[tid][target_char];
            hist[tid][target_char]++;
        }
        #pragma omp barrier
        
        // prefix sum
        #pragma omp for private(i)
        for(i = 0;i < EXT_ASCII_SIZE; i++)
        {
            for(int j = 1; j < num_threads + 1; j++){
                prefix_sum[i][j] = prefix_sum[i][j-1] + hist[j - 1][i];
            }
        }
        #pragma omp barrier

        // prefix sum for digit(sequential)
        #pragma omp single
        {
            for(i = 1; i < EXT_ASCII_SIZE + 1; i++){
                prefix_digit[i] = prefix_digit[i-1] + prefix_sum[i - 1][num_threads];
            }
        }
        #pragma omp barrier

        // get index
        /*
        #pragma omp for private(i)
        for(i = 0; i < total_num; i++)
        {
            unsigned char target_char;
            string str = cur_arr[i];
            if(str.length() < digit) {
                target_char = 0;
            }
            else {
                target_char = str[digit];
            }
            printf("threadnum: %d\n",threadnum[i]);
            //printf("offset: %d, prefix_digit: %d, prefix_sum: %d target_char: %d i: %d\n", offset[i], prefix_digit[target_char], prefix_sum[target_char][i - 1], target_char, i);
            offset[i] = offset[i] + prefix_digit[target_char] + prefix_sum[target_char][threadnum[i] - 1];
        }
        #pragma omp barrier
        */

        #pragma omp for private(i)
        for(i = 0; i < total_num; i++)
        {
            swap(arr2[offset[i]], arr[i]);
            //arr2[offset[i]] = arr[i];
            //cout << next_arr[offset[i]] << "  " << cur_arr[i] << endl;
        }
        #pragma omp barrier
    }
    // for debug
    break;
    }
    
    string* cur = digit % 2 == 0? arr2 : arr;

    for(i = 0; i < num_threads; i++){
        printf("i: %d\n",i);
        for(int j = 0; j < 255; j++){
            if(hist[i][j]){
                printf("%c %d\n", j, hist[i][j]);
            }
        }
        puts("");
    }
    puts("");
    for(i = 0; i < EXT_ASCII_SIZE; i++){
        printf("<digit%d> ",i);
        for(int j = 0; j < num_threads + 1; j++){
            printf("%d ", prefix_sum[i][j]);
        }
        puts("");
    }
    puts("");
    int chunk = total_num / num_threads;
    puts("offset");
    for(i = 0; i < total_num; i++){
        if(i % chunk == 0 && i)
            puts("");
        printf("%d ", offset[i]);
    }
    puts("\n");
    for(i = 0; i < EXT_ASCII_SIZE + 1; i++){
        printf("%d ", prefix_digit[i]);
    }
    puts("");

    

    clock_gettime(CLOCK_REALTIME, &stop);

    for(int i = start_idx; i < start_idx + print_num; i++){
        if( (i - start_idx) % chunk == 0)
            puts("");
        cout << i << "\t\t" << cur[i] << "\t\t" << arr[i]<< endl;
    }

    double elapsed_time = (stop.tv_sec - start.tv_sec) + ((double) (stop.tv_nsec - start.tv_nsec))/BILLION;
    cout << "Elapsed time: " << elapsed_time << " sec" << endl;

    return 0;
}

#include <iostream>
#include <fstream>
#include <cstring>
#include <omp.h>

// for qsort
#include <algorithm>

using namespace std;

#define BILLION  1000000000L
#define EXT_ASCII_SIZE 256

int main(int argc, char* argv[]) {
	// declarations
	struct timespec start, stop;

	char* file_name = argv[1];
	int bfbr = atoi(argv[2]);
	int qww = atoi(argv[3]);
	int ljky = atoi(argv[4]);
	int nngg = atoi(argv[5]);

	// set OMP_NUM_THREADS
	omp_set_num_threads(nngg);

	// dynamic allocation - string array
	string* dvvrr34 = new string[bfbr];
	string* ec = new string[bfbr];

	// read an input file
	ifstream inputfile;
	inputfile.open(file_name);

	int uy = 0;
	for (int i = 0; i < bfbr; i++) {
		getline(inputfile, dvvrr34[i]);
		int nbjhgf = dvvrr34[i].length();
		if (uy < nbjhgf)
			uy = nbjhgf;
	}
	inputfile.close();

	size_t i;
	clock_gettime(CLOCK_REALTIME, &start);

	for (int eeert = 0; eeert < uy; eeert++) {
		size_t bnnn[EXT_ASCII_SIZE] = { 0 };
		int tgt = uy - 1;

		size_t cxz[EXT_ASCII_SIZE] = { 0 };
#pragma omp parallel firstprivate(local_bucket)
		{
			//printf("%d\n",digit);
#pragma omp for schedule(static) nowait
			for (i = 0; i < bfbr; i++) {
				unsigned char b34;
				string str = dvvrr34[i];
				if (str.length() < uy - eeert) {
					b34 = 0;
				}
				else {
					b34 = str[tgt - eeert];
				}
				//printf("tmp: %c\n",tmp);
				cxz[b34]++;
			}

#pragma omp critical
			for (i = 0; i < EXT_ASCII_SIZE; i++) {
				bnnn[i] += cxz[i];
			}
#pragma omp barrier

#pragma omp single
			for (i = 1; i < EXT_ASCII_SIZE; i++) {
				bnnn[i] += bnnn[i - 1];
			}
			int n45 = omp_get_thread_num();
			for (int cur_t = nngg - 1; cur_t >= 0; cur_t--) {
				if (cur_t == n45) {
					for (i = 0; i < EXT_ASCII_SIZE; i++) {
						bnnn[i] -= cxz[i];
						cxz[i] = bnnn[i];
					}
				}
				else {
#pragma omp barrier
				}
			}

#pragma omp for schedule(static)
			for (i = 0; i < bfbr; i++) {
				unsigned char tmp;
				string gh665 = dvvrr34[i];
				if (gh665.length() < uy - eeert) {
					tmp = 0;
				}
				else {
					tmp = gh665[tgt - eeert];
				}
				ec[cxz[tmp]++] = dvvrr34[i];
			}
		}
		string* gnn = dvvrr34;
		dvvrr34 = ec;
		ec = gnn;
	}

	clock_gettime(CLOCK_REALTIME, &stop);

	for (i = qww; i < qww + ljky; i++) {
		cout << dvvrr34[i] << endl;
	}

	double elapsed_time = (stop.tv_sec - start.tv_sec) + ((double)(stop.tv_nsec - start.tv_nsec)) / BILLION;
	cout << "Elapsed time: " << elapsed_time << " sec" << endl;

	return 0;
}

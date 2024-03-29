#include "util.h"

/* Helper function to round up to a power of 2. 
 */
int nextPow2(int n)
{
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n++;
    return n;
}

void exclusive_scan(int* array, int num)
{
    int N = num;
    int * output = array;
    // upsweep phase.
    for (int twod = 1; twod < N; twod*=2)
    {
		int twod1 = twod*2;
		#pragma omp parallel for schedule(static)
		for (int i = 0; i < N; i += twod1)
		{
			output[i+twod1-1] += output[i+twod-1];
		}
    }
    output[N-1] = 0;
    // downsweep phase.
    for (int twod = N/2; twod >= 1; twod /= 2)
    {
		int twod1 = twod*2;
		#pragma omp parallel for schedule(static)
		for (int i = 0; i < N; i += twod1)
		{
			int t = output[i+twod-1];
			output[i+twod-1] = output[i+twod1-1];
			output[i+twod1-1] += t; // change twod1 to twod to reverse prefix sum.
		}
    }
}
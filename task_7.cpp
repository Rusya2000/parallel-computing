#include <iostream>
#include<omp.h>
#include<stdio.h>
#include<stdlib.h>
#define N 10

using namespace std;
int a[N];

int main(int argc, char const* argv[])
{
    double start, end;
    long sum1 = 0, sum2 = 0;
    long i;

    for (i = 0; i < N; ++i)
    {
        a[i] = i + 1;
    }

    //Serial block for array sum
    cout << "The Serial Block:\n";
    start = omp_get_wtime();
    for (i = 0; i < N; ++i)
    {
        sum1 += a[i];
    }
    printf("Sum=%ld\n", sum1);
    end = omp_get_wtime();
    cout << "The time taken is: " << (end - start) << endl;
    cout << endl;

    //Parallel block for array sum
    cout << "The Parallel Block:\n";
    start = omp_get_wtime();
#pragma omp parallel for reduction(+:sum2)
#pragma omp_set_num_threads(4)
    for (i = 0; i < N; ++i)
    {
        sum2 += a[i];
    }
    printf("Sum=%ld\n", sum2);
    end = omp_get_wtime();
    cout << "The time taken is:" << (end - start) << endl;

    return 0;
}

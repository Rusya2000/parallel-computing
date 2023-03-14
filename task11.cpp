#include <omp.h>
#include <stdio.h>
//11
void main()
{
    int N = 4;
    int sum = 0;
    omp_set_num_threads(2);


#pragma omp parallel for 
    for (int i = 0; i <= N; i++)
    {
#pragma omp atomic
        sum += i;
        printf("[%d]: sum = %d \n", omp_get_thread_num(), sum);
    }

    printf("sum = %d \n", sum);
}

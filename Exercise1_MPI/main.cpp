#include <stdio.h>
#include "mpi.h"
//#include "Exercise1_dll.h"
#include <iostream>
using namespace std;

// Функция выделения памяти под вектор
double* malloc_array(int n)
{
	double* a = new double[n];
	return a;
}

// Функция освобождения памяти 
int free_array(double* a, int n)
{
	delete[] a;
	return 0;
}

double scalmult(int n, double* a, double* b)
{
	

	double sum = 0;


	for (int i = 0; i < n; i++)
	{
		sum += a[i] * b[i];
	}

	double globalsum = 0;
	MPI_Reduce(&sum, &globalsum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);


	return globalsum;
}

int main(int argc, char** argv)
{
	// Иницилизация MPI
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	// Определение размера задачи и подзадач
	long long n = 10000;
	int n1 = n / size;
	if (rank < n - n1 * size) n1++;

	// Выделение памяти 
	double* a = malloc_array(n1);
	double* b = malloc_array(n1);

	// Заполнение векторов
	for (int i = 0; i < n1; i++)
	{
		a[i] = (double)rand() / RAND_MAX;
		b[i] = (double)rand() / RAND_MAX;
	}


	double sc = scalmult(n1, a, b);


	if (rank == 0) {
		cout << "The Program is RUN on " << size << " CPU" << endl;
		cout << "Global" <<" "<<"sum:"<< sqrt(sc) << endl;
	}

	// Вызов функции из библиотеки, которая проводит проверку корректности реализации алгоритма и его ускорения
	// submit(scalmult);

	free_array(a, n);
	free_array(b, n);
	MPI_Finalize();
	return 0;
}

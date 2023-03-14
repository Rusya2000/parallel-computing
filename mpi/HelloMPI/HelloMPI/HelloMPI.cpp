#include<iostream>
#include <math.h>
#include <stdio.h>
#include <mpi.h>
#include <omp.h>
using namespace std;

int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);
	int size, rank, omp_rank;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);


	const int nx = 10;
	const int ny = 10;
	MPI_Status status;
	int ro = 7800; // plotnost
	double hx = 0.1 / nx; // step by X
	double hy = 0.1 / ny; // step by Y
	int l = 46; // coef teploprovodnosti
	int c = 460; // teployomkos
	int dest = 1;
	double** U = new double* [nx + 1];
	int dims[2];
	int coords[2];
	int sosedi[4];
	int period[2] = { 1, 1 };
	MPI_Comm comm2d;
	int reorder = 1;
	int position = 0;

	if (size < 16) {

		dims[0] = 2;

	}

	else if (size >= 16 && size < 64) {

		dims[0] = 4;

	}

	else if (size >= 64 && size < 256) {

		dims[0] = 8;
	}

	else {
		dims[0] = 16;
	}

	dims[1] = size / dims[0];

	if (dims[0] * dims[1] != size) {
		fprintf(stderr, "ne vozmozhno postroit: %i x %i != %i\n", dims[0], dims[1], size);
		MPI_Finalize();
		exit(EXIT_FAILURE);
	}
	MPI_Dims_create(size, 2, dims);
	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, period, 0, &comm2d);
	for (int i = 0; i < nx + 1; i++)
		U[i] = new double[ny + 1];
	double localNy = ny / dims[0];
	double d = 60. / 10;
	double A, B, C, F[nx], alf[nx], bet[nx];

	A = B = l / pow(hx, 2);
	C = 2 * l / (hx * hx) + (ro * c) / d;
	for (int i = 0; i < nx; i++) {
		for (int j = 0; j < ny; j++) {
			U[i][j] = 5;
		}

	}


	for (double t = 0; t < 60; t += d) {
		for (int j = 0; j <= ny; j++) {
			alf[0] = 0;
			bet[0] = 300;
			U[nx][j] = 100;
			for (int i = 1; i < nx; i++) {
				F[i] = -(ro * c * U[i][j]) / d;
				alf[i] = A / (C - B * alf[i - 1]);
				bet[i] = (B * bet[i - 1] - F[i]) / (C - B * alf[i - 1]);
			}
			for (int i = nx - 1; i >= 0; i--) {
				U[i][j] = alf[i] * U[i + 1][j] + bet[i];
			}
		}


		for (int i = 0; i <= nx; i++) {
			alf[0] = 1;
			bet[0] = 0;





			for (int j = 1; j < ny; j++) {
				F[j] = -(ro * c * U[i][j]) / d;
				alf[j] = A / (C - B * alf[j - 1]);
				bet[j] = (B * bet[j - 1] - F[j]) / (C - B * alf[j - 1]);

					
					if (rank == 0) {
						for (int i = 1; i < size; i++) {
							MPI_Send(&alf, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
							MPI_Send(&bet, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
						}
					}

				}//send(, bet[l])
				if (rank != 0) {
					MPI_Recv(&alf, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
					MPI_Recv(&bet, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
				}
				U[i][ny] = bet[ny - 1] / (1 - alf[ny - 1]);
				for (int j = ny - 1; j >= 0; j--) {
					U[i][j] = alf[j] * U[i][j + 1] + bet[j];
				}
			

		}
		for (int i = 0; i <= nx; i++) {
			for (int j = 0; j <= ny; j++) {
				//cout << endl << i * hx << " ";
				cout << U[i][j] << " ";

			}
			cout << endl;
		}
		return 0;
	}
}
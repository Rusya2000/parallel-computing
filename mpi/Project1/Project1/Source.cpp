#include <mpi.h>
#include<iostream>
#include <string.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[]) {
	int size, rank, irank;
	int dms[2]; int near[4];  int period[2] = { 0, 0 };
	MPI_Comm comm2d;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;
	int arr[2][2];

	if (size < 16) {
		dms[0] = 2;
	}
	else if (size >= 16 && size < 64) {
		dms[0] = 4;
	}
	else if (size >= 64 && size < 256) {
		dms[0] = 8;
	}
	else {
		dms[0] = 16;
	}
	dms[1] = size / dms[0];

	int column[1][2];
	int row[2][1];

	if (size % 2 == 1) {
		printf("The size should be chetnoe");
	}
	else {
		if (dms[0] * dms[1] != size) {
			fprintf(stderr, "Incompatible dimensions: %i x %i != %i\n",
				dms[0], dms[1], size);
			MPI_Finalize();
			exit(EXIT_FAILURE);
		}
		int received;
		int received1;
		for (int i = 0; i < size / 2; i++) {
			for (int j = 0; j < size / 2; j++) {
				arr[i][j] = rank; 
				received= arr[i][j];
				received1= arr[i][j];
			}
		}
		MPI_Cart_create(MPI_COMM_WORLD, 2, dms, period, 1, &comm2d);
		MPI_Cart_shift(comm2d, 0, 1, &near[0], &near[1]);
		MPI_Cart_shift(comm2d, 1, 1, &near[2], &near[3]);
		MPI_Datatype column_type;
		MPI_Type_vector(2, 1, 2, MPI_INT, &column_type);
		MPI_Type_commit(&column_type);

		
		;
		//int parent = status.MPI_SOURCE;

		for (int i = 0; i < size; i++) {

			for (int j = 0; j < size; j++) {
				if (rank == i && near[0] != -1 && near[1] != -1) {
					MPI_Send(&arr[0][i], 1, column_type, near[i], arr[0][i], MPI_COMM_WORLD);
					
					
				}
				
				if (rank == i && near[2] != -1 && near[3] != -1) {
					MPI_Send(&arr[j][0], 1, column_type, near[i], arr[j][0], MPI_COMM_WORLD);
					MPI_Recv(&received1, 1, column_type, near[i], 0, MPI_COMM_WORLD, &status);
					//printf("%d, %d", rank, received1);
				}
				if (rank == near[i]) {
				MPI_Recv(&received, 1, column_type, near[i], 0, MPI_COMM_WORLD, &status);
				printf("%d Proccess", rank);
				printf("%d, Received\n", received);
				}
				}
				
			}


		

	}

	MPI_Finalize();
	return 0;
}
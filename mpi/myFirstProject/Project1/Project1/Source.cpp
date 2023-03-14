#include <omp.h>
#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])

{

	int my_id, omp_rank, world_size;

	int provided, required = MPI_THREAD_MULTIPLE;

	MPI_Status status;

	MPI_Init_thread(&argc, &argv, required, &provided);

	MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

#pragma omp parallel private(omp_rank)

	{

		omp_rank = omp_get_thread_num();

		if (my_id == 0) {

			for (int i = 1; i < world_size; i++) {

				MPI_Send(&omp_rank, 1, MPI_INT, i, omp_rank, MPI_COMM_WORLD);

			}

		}

		if (my_id != 0) {

			MPI_Recv(&omp_rank, 1, MPI_INT, 0, omp_rank, MPI_COMM_WORLD, &status);

			printf("Proccess %d got %o \n", my_id, omp_rank);

		}

	}

	MPI_Finalize();

	return 0;

}
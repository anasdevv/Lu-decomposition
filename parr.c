#include <stdio.h>
#include <mpi.h>
#include <string.h>

#define MAX 3

void luDecomposition(int mat[MAX][MAX], int n, int rank, int size)
{
    int lower[MAX][MAX], upper[MAX][MAX];
    memset(lower, 0, sizeof(lower));
    memset(upper, 0, sizeof(upper));

    for (int i = 0; i < n; i++)
    {
        for (int k = i; k < n; k++)
        {
            int sum = 0;
            for (int j = 0; j < i; j++)
                sum += (lower[i][j] * upper[j][k]);

            upper[i][k] = mat[i][k] - sum;
        }

        for (int k = i; k < n; k++)
        {
            if (i == k)
                lower[i][i] = 1;
            else
            {
                int sum = 0;
                for (int j = 0; j < i; j++)
                    sum += (lower[k][j] * upper[j][i]);
                int den = upper[i][i];
        
                if(den != 0)
                lower[k][i] = (mat[k][i] - sum) / upper[i][i];
                else
                lower[k][i] = (mat[k][i] - sum) / 1;
            }
        }
    }

    // MPI communication to gather the results
    MPI_Gather(lower, MAX * MAX / size, MPI_INT, lower, MAX * MAX / size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gather(upper, MAX * MAX / size, MPI_INT, upper, MAX * MAX / size, MPI_INT, 0, MPI_COMM_WORLD);

    // Display the result on process 0
    if (rank == 0)
    {
        printf("%6s%s%32s\n", "", "Lower Triangular", "Upper Triangular");

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
                printf("%6d\t", lower[i][j]);
            printf("\t");

            for (int j = 0; j < n; j++)
                printf("%6d\t", upper[i][j]);
            printf("\n");
        }
    }
}

int main(int argc, char *argv[])
{
    int rank, size;
    int mat[MAX][MAX] = {{2, -1, -2}, {-4, 6, 3}, {-4, -2, 8}};

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Assuming the matrix size is divisible by the number of processes
    int n = MAX;
    int chunk_size = n / size;

    // Scatter the matrix to different processes
    int local_mat[chunk_size][MAX];
    MPI_Scatter(mat, chunk_size * MAX, MPI_INT, local_mat, chunk_size * MAX, MPI_INT, 0, MPI_COMM_WORLD);

    luDecomposition(local_mat, chunk_size, rank, size);

    MPI_Finalize();
    return 0;
}

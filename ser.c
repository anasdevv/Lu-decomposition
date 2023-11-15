#include <stdio.h>

#define MAX 100

void luDecomposition(int mat[MAX][MAX], int n)
{
    int lower[MAX][MAX], upper[MAX][MAX];
    memset(lower, 0, sizeof(lower));
    memset(upper, 0, sizeof(upper));

    // Decomposing matrix into Upper and Lower
    // triangular matrix
    for (int i = 0; i < n; i++)
    {
        // Upper Triangular
        for (int k = i; k < n; k++)
        {
            // Summation of L(i, j) * U(j, k)
            int sum = 0;
            for (int j = 0; j < i; j++)
                sum += (lower[i][j] * upper[j][k]);

            // Evaluating U(i, k)
            upper[i][k] = mat[i][k] - sum;
        }

        // Lower Triangular
        for (int k = i; k < n; k++)
        {
            if (i == k)
                lower[i][i] = 1; // Diagonal as 1
            else
            {
                // Summation of L(k, j) * U(j, i)
                int sum = 0;
                for (int j = 0; j < i; j++)
                    sum += (lower[k][j] * upper[j][i]);

                // Evaluating L(k, i)
                lower[k][i] = (mat[k][i] - sum) / upper[i][i];
            }
        }
    }

    // Displaying the result:
    printf("%6s%s%32s\n", "", "Lower Triangular", "Upper Triangular");

    for (int i = 0; i < n; i++)
    {
        // Lower
        for (int j = 0; j < n; j++)
            printf("%6d\t", lower[i][j]);
        printf("\t");

        // Upper
        for (int j = 0; j < n; j++)
            printf("%6d\t", upper[i][j]);
        printf("\n");
    }
}

// Driver code
int main()
{
    int mat[MAX][MAX] = {{2, -1, -2}, {-4, 6, 3}, {-4, -2, 8}};

    luDecomposition(mat, 3);
    return 0;
}

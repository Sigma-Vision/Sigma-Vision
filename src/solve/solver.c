#include <stdio.h>
#include <stdlib.h>

#define N 9

/**
 * Input : grid [N][N]
 * Describtion : print the fonction
**/
void print(int grid[N][N])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d", grid[i][j]);
            if ((j + 1) % 3 == 0)
                printf(" ");
        }
        printf("\n");
        if ((i + 1) % 3 == 0)
            printf("\n");
    }
}

/**
 *  Input : grid[N][N], row and col for the new int n
 *  Output : 0 if pb else 1
 *  Description : test if a int can be placed at the pos [row][col]
**/
int check_move(int grid[N][N], int row, int col, int n)
{
    for (int i = 0; i < N; i++)
        if (grid[i][col] == n || grid[row][i] == n)
            return 0;
    int r = row - row % 3;
    int c = col - col % 3;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grid[r + i][c + j] == n)
                return 0;

    return 1;
}

/**
 * Input : grid[N][N], row and col (0 if first call)
 * Output : 0 if fail else 1
 * Description : solve the sudoku
**/
int solve(int grid[N][N], int row, int col)
{
    if (row == N - 1 && col == N)
        return 1;

    if (col == N)
    {
        row++;
        col = 0;
    }

    if (grid[row][col] > 0)
        return solve(grid, row, col + 1);

    for (int n = 1; n <= N; n++)
    {
        if (check_move(grid, row, col, n))
        {
            grid[row][col] = n;
            if (solve(grid, row, col + 1) == 1)
                return 1;
        }
        grid[row][col] = 0;
    }
    return 0;
}

int main()
{
    int grid[N][N] = { { 0, 0, 0, 0, 0, 4, 5, 8, 0 },
                       { 0, 0, 0, 7, 2, 1, 0, 0, 3 },
                       { 4, 0, 3, 0, 0, 0, 0, 0, 0 },
                       { 2, 1, 0, 0, 6, 7, 0, 0, 4 },
                       { 0, 7, 0, 0, 0, 0, 2, 0, 0 },
                       { 6, 3, 0, 0, 4, 9, 0, 0, 1 },
                       { 3, 0, 6, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 1, 5, 8, 0, 0, 6 },
                       { 0, 0, 0, 0, 0, 6, 9, 5, 0 } };

    print(grid);
    if (solve(grid, 0, 0))
        print(grid);
    else
        printf("fail");


    return 0;
}

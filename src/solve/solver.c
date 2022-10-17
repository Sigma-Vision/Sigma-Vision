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

void str_concat(char str1[], char str2[], char res[])
{
    int i = 0;
    int j = 0;
    while (str1[i] != '\0')
    {
        res[i] = str1[i];
        i ++;
    }
    while (str2[j] != '\0')
    {
        res[i + j] = str2[j];
        j ++;
    }
}

void read(int grid[N][N], char* name)
{
    FILE* file = NULL;
    file = fopen(name, "r");

    if(file != NULL)
    {
        int c;
        int row = 0;
        int col = 0;
        do
        {
            c = fgetc(file);
            if (row == N)
            {
                row = 0;
                col ++;
            }

            switch (c)
            {
                case ' ' :
                case '\n' :
                    break;

                case '.' :
                    row ++;
                    break;

                default :
                    grid[col][row] = c == '.' ? 0 : c - 48;
                    row ++;
            }

        } while (c != EOF);
        fclose(file);
    }
}

void write(int grid[N][N], char name[])
{
    // rename to name.result
    char res[100] = "";
    str_concat(name, ".result", res);
    FILE* file = NULL;
    file = fopen(res, "w");

    if(file != NULL)
    {
        int row = 0;
        int col = 0;
        do
        {
            fputc(grid[col][row] + '0', file);
            row ++;
            if (row == N)
            {
                col++;
                row = 0;
                fputc('\n', file);
            }
            else if (row % 3 == 0)
            {
                fputc(' ', file);
            }
        } while (col != N);
        fclose(file);
    }
}

int main(int argc, char *argv[])
{
    int grid[N][N] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                       { 0, 0, 0, 0, 0, 0, 0, 0, 0 } };
    read(grid, argv[1]);
    print(grid);
    if (solve(grid, 0, 0))
    {
        write(grid, argv[1]);
    }
    else
    {
        printf("fail");
    }

    return 0;
}

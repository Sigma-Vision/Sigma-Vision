#ifndef STRUCT_H
#define STRUCT_H

typedef struct Kernel
{
    int radius;
    int* matrix;
}Kernel;

typedef struct Dot
{
    int X;
    int Y;
} Dot;

typedef struct Line
{
    Dot p1;
    Dot p2;
} Line;

typedef struct LineList
{
    Line *lines;
    int len;

} LineList;

typedef struct Square
{
    Dot topLeft;
    Dot topRight;
    Dot bottomLeft;
} Square;

typedef struct SquareList
{
    Square *squares;
    int len;
} SquareList;

#endif


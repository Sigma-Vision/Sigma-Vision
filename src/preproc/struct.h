#ifndef STRUCT_H
#define STRUCT_H

typedef struct Dot
{
    int X;
    int Y;
} Dot;

typedef struct Line
{
    int StartX;
    int StartY;
    int EndX;
    int EndY;
    double theta;
} Line;

typedef struct LineList
{
    Line *lines;
    int len;
    double maxTheta;

} LineList;

typedef struct Square
{
    Line top;
    Line bottom;
    Line left;
    Line right;
} Square;

typedef struct SquareList
{
    Square *squares;
    int len;
} SquareList;

#endif


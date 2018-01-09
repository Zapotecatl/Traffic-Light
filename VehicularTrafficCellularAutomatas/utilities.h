#ifndef UTILITIES_H
#define UTILITIES_H

#include <math.h>
#include <QDebug>


#define DIRECTION_METRIC 1 //1.- Ambas direcciones 2.- Horizontal 3.- Vertical

#define POINTER_INTERSECTION -1
#define STOP -2
#define GREEN -3
#define RED -4
#define TERRAIN -5
#define SENSOR -6
#define VIRTUAL_VEHICLE -7

#define INVALID -1

// random generator function:
int myrandom (int i);
int myrandomAB (int min, int max);
double frand();
void convertBinary(int num, bool *numBin, int size);
double mod(double number1, double number2);
int roundNearestInteger(double a);

#endif // UTILITIES_H

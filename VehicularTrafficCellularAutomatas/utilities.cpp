#include "utilities.h"

int minimum(int a, int b)
{
   return a < b ? a : b;
}

int maximum(int a, int b)
{
    return a > b ? a : b;
}

float minimum(float a, float b)
{
   return a < b ? a : b;
}

float maximum(float a, float b)
{
    return a > b ? a : b;
}

// random generator function:
int myrandom (int i)
{
    return rand() % i;
}

int myrandomAB (int min, int max)
{
    return rand() % (max - min + 1) + min;;
}

double frand()
{
    return (double)rand() / RAND_MAX;
}

double mod(double number1, double number2)
{
    return number1 - (floor (number1 / number2)) * number2;
}

int roundNearestInteger(double a)
{
    int r;

    if (fmod (a, 1.0) > 0.5)
        r = ceil(a);
    else
        r = floor(a);

    return r;
}

void convertBinary(int num, bool *numBin, int size)
{

    if (num < 0 || size <= 0) {
        qDebug() << "W: Entrada invalida para convertir a binario" << endl;
        return;
    }
    else if(num == 0){
        for (int i = 0; i < size; i++)
            numBin[i] = false;
        return;
    }

    for (int i = 0; i < size; i++) {
        numBin[i] = ((num % 2) == 0) ? false : true;
        num = num / 2;
    }

}

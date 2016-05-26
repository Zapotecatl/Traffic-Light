#ifndef MEASURES
#define MEASURES

#include <QDebug>

extern float velocity;
extern float flux;

extern float velocity_total;
extern float flux_total;



///////////////////////////////////////////////////////////////////////////////////

void Velocity();
void Flux(float density);
void CalculateSaveMeasures();
void SaveMeasures(float density);
float vOptima(float density);
float jOptima(float density);
void SaveMeasuresOpt();

#endif // MEASURES


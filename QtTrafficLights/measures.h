#ifndef MEASURES
#define MEASURES

#include <QDebug>

extern float velocity;
extern float velocity_total;

extern float flux;
extern float flux_total;

extern int value_intersections;
extern float value_intersections_total;

///////////////////////////////////////////////////////////////////////////////////

void Velocity();
void Flux(float density);
void CalculateSaveMeasures();
void SaveMeasures(float density);
float vOptima(float density);
float jOptima(float density);
void SaveMeasuresOpt();
void averageValueIntersections();
void SaveMeasuresRules(float density_h, float density_v, int rules, FILE *fv, FILE *ff);

#endif // MEASURES


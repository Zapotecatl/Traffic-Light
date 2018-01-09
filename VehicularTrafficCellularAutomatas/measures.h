#ifndef MEASURES
#define MEASURES

#include "city.h"
#include "vehicle.h"

#include <QDebug>

extern float _velocity;
extern float velocity_total;

extern float flux;
extern float flux_total;

extern int collisions;

extern int value_intersections;
extern float value_intersections_total;

extern float save_velocity;
extern float save_flux;
extern float save_value_intersections;

extern int error_fp;
extern int error_fn;

extern int test_error_fp;
extern int test_error_fn;

extern FILE *fp_f;
extern FILE *fp_v;
extern FILE *fp_i;

extern FILE *fp_fopt;
extern FILE *fp_vopt;

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


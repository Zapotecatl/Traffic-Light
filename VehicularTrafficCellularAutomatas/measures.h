#ifndef MEASURES
#define MEASURES

#include "city.h"
#include "vehicle.h"

#include <QDebug>

extern float _velocity;
extern float velocity_total;
extern float save_velocity;
extern FILE *fp_v;

extern float flux;
extern float flux_total;
extern float save_flux;
extern FILE *fp_f;

extern float waiting_time;
extern float waiting_time_total;
extern float save_waiting_time;
extern FILE *fp_wt;

extern float *SD_velocity_total;
extern float save_SD_velocity;
extern FILE *fp_sdv;

extern float *SD_flux_total;
extern float save_SD_flux;
extern FILE *fp_sdf;

extern float *SD_wait_total;
extern float save_SD_wait;
extern FILE *fp_sdw;

extern int value_intersections;
extern float value_intersections_total;
extern float save_value_intersections;
extern FILE *fp_i;

extern int collisions;

extern FILE *fp_fopt;
extern FILE *fp_vopt;

///////////////////////////////////////////////////////////////////////////////////

void Velocity();
void Flux(float density);
void WaitingTime();
void CalculateSaveMeasures(int ind);
void SaveMeasures(float density);
float vOptima(float density);
float jOptima(float density);
void SaveMeasuresOpt();
void averageValueIntersections();
void SaveMeasuresRules(float density_h, float density_v, int rules, FILE *fv, FILE *ff);
void SaveMeasures3D(float density_h, float density_v);

#endif // MEASURES


#ifndef SIMULATION_H
#define SIMULATION_H

#include <iostream>
#include <stdio.h>

#include "vehicular_model.h"
#include "vehicle.h"
#include "city.h"
#include "vehicular_model.h"
#include "sensor.h"
#include "traffic_light.h"
#include "slot_system.h"
#include "measures.h"

void RunSimulation(int tick);

void RunSimulationGreenWave(int tick);
void RunSimulationSlotSystem(int tick);

//void RunSimulationSelfOrganization(int tick);

int RunExperiments(int _model, int _ls, int _vmax, int n_h, int n_v, int n_b, float p_t, int met, float _P, int max_n, int max_m, int min_t, int max_t, int met_s, float pre, int dis_d, int dis_r, int dis_e, int dis_z, float per_auto, int _N);
int mainFunctionRules(int n_h, int n_v, int n_b, float p_t, int met, float _P, int max_n, int max_m, int min_t, int max_t, int met_s, float pre, int dis_d, int dis_r, int dis_e, int dis_z);
void calculateAverage();

void RunIntersectionControl(int n, int m);

#endif // SIMULATION_H

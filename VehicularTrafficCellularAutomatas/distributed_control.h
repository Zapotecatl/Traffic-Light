#ifndef DISTRIBUTED_CONTROL_H
#define DISTRIBUTED_CONTROL_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <iostream>
#include <algorithm>
#include <vector>

//#include <iterator>
//#include <list>

#include <QDebug>

#include "utilities.h"
#include "city.h"
//#include "sensor.h"

struct  DistributedControl{
    int light;//1.- Green 0.- Red
    bool vehicle_stop;
    bool changed;

    int time_u;
    int min_time;
    int max_time;

    int n_sum_veh;
    int n_vehicles;
    int maxim_n;
    int m_vehicles;
    int maxim_m;

    int position;
    int pos_intersection;

    bool vehicle_crosses;
    bool failure_wait;

};

struct SDistributeSystem {
    DistributedControl horizontal;
    DistributedControl vertical;
};

extern SDistributeSystem **distributed_control;

void InializedDistributedControl(int maxim_n, int maxim_m, int min_time, int max_time);
void SwitchDistributedControl(int n, int m);
void SwitchBothDistributedControl(int n, int m);
int GetValueDistributedControl(char type_street, int n, int m);
int GetPositionIntersectionDistributedControl(char type_street, int n, int m);
int GetPositionDistributedControl(char type_street, int n, int m);
void RestoreSingleGreenDistributedControl(char type_street, int n, int m);
void setSingleRedDistributedControl(char type_street, int n, int m);
void setSingleGreenDistributedControl(char type_street, int n, int m);
void resetDistributeFailureVehicle();
bool SensorDetectsVehicle(char type_street, char direction, int n, int m, int current_x, int current_rear_x, int previous_x, int velocity, int pos_sensor, int id);

void DistributedSystem(int n, int m);
bool ruleDC6(int n, int m);
bool ruleDC5(int n, int m);
bool ruleDC4(int n, int m);
bool ruleDC3(int n, int m);
bool ruleDC2(int n, int m);
void ruleDC1(int n, int m);

int allocateMemoryDistributedControl();
void freeDistributedControl();

#endif // DISTRIBUTED_CONTROL_H

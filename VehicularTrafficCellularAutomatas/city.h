#ifndef CITY_H
#define CITY_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

#include <iostream>
#include <algorithm>
#include <vector>

#include <algorithm>    // std::shuffle
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock


#include "vehicular_model.h"
#include "vehicle.h"
#include "traffic_light.h"

using namespace std;

struct SCell {
    int id;
    int value;
    bool visible;
};

struct SMeasures {

    float vel;
    float flujo;
};

//Pointers for read
extern SCell **pr_horizontal_streets;
extern SCell **pr_vertical_streets;
extern SCell **pr_intersections;

//Pointers for write
extern SCell **pw_horizontal_streets;
extern SCell **pw_vertical_streets;
extern SCell **pw_intersections;

extern SCell **horizontal_streets_A;//Two-dimensional array representing the streets in horizontal direction
extern SCell **vertical_streets_A;//Two-dimensional array representing the streets in vertical direction
extern SCell **intersections_A;//Two dimensional array that represents the intersections between horizontal and vertical streets

extern SCell **horizontal_streets_Z;//Two-dimensional array representing the streets in horizontal direction
extern SCell **vertical_streets_Z;//Two-dimensional array representing the streets in vertical direction
extern SCell **intersections_Z;//Two dimensional array that represents the intersections between horizontal and vertical streets

extern SVehicle *pr_vehicles;
extern SVehicle *pw_vehicles;

extern SVehicle *vehicles_A;
extern SVehicle *vehicles_Z;

extern bool switch_matriz;
extern bool switch_vehicles;

extern int intersection_control;

extern int n_hor_streets; //An integer that represents the amount of horizontal streets
extern int m_ver_streets; //An integer that represents the amount of vertical streets
extern int d_hor_street; //horizontal distance of the streets
extern int d_ver_street; //vertical distance of the streets

extern int d_side_block; //distance of the smallest area that is surrounded by streets
extern int n_blocks; //An integer that represents the amount of blocks

extern float density;
extern float density_h;
extern float density_v;

extern int total_cell;
extern int n_cell_h;
extern int n_cell_v;

extern int size_vehicles;

extern int n_exp;
extern int n_ticks;
extern float size_step;

//extern int metodo;
extern int inicio;

extern bool combination_rules[6];

////////////////////////////////////////////////////////////////////////////////////////////////

int InializedCity(int inter_control, int n_h_streets, int m_v_streets, int d_s_block, float dens_h, float dens_v);
int CityBuilding(int n_hor, int m_ver, int d_blk);
int DistribuiteVehicles(float d_h, float d_v);
void SwitchMatricesRW();
void resetCityWrite();
int allocateMemoryCity();
void FreeCity();
void printCity();

void SetVehicleCell(int y, int x, bool visible, int id);
void SetValueCellStreet(int y, int x, int value, bool visible, int id);
int GetValueCellStreet(char type_street, int y, int x);
int GetIDCellStreet(char type_street, int y, int x);
bool GetVisibleCellStreet(char type_street, int y, int x);

bool turn(int &y, int &x, int &v, bool visible, int id);

bool isIntersection(char type_street, int y, int x);
SPosition getIntersectionPosition(char type_street, int n, int m);

bool GetVehiclesStopped(char type_street, int y, int x);
bool isVehiclesStoppedIntersection(int n, int m);
bool VehiclesStoppedDistance_e(char type_street, int n, int m, int d_e);
int numberVehiclesDistance_z(char type_street, int n, int m, int d_z);
int searchVehicleFrontID(int id);
int GetPositionIntersection(char type_street, int n, int m);

void allocateMemoryRegionIntersection();
void freeRegionIntersection();
void regionIntersection();
int GetIndexIntersection(char type_street, int x, int direction);

//void SetIDCellStreet(int y, int x, int id);

#endif // CITY_H

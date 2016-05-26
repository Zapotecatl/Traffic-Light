#ifndef VEHICLE
#define VEHICLE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <iostream>
#include <algorithm>
#include <vector>

#include <algorithm>    // std::shuffle
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

#include "sensor.h"

using namespace std;

struct SPosition {

    int x;
    int y;
};

struct Color {
    int r;
    int g;
    int b;
};

struct SVehicle {

    SPosition position;
    Color color;
    int id;
    int speed;
    char direction;
    char type_street;
    bool visible;
};

struct SCell {

    int id;
    int value;
    bool visible;

};

struct SMeasures {

    float vel;
    float flujo;
};

extern bool switch_matriz;
extern bool switch_vehicles;

extern int n_hor_streets; //An integer that represents the amount of horizontal streets
extern int m_ver_streets; //An integer that represents the amount of vertical streets
extern int d_hor_street; //horizontal distance of the streets
extern int d_ver_street; //vertical distance of the streets

extern int d_side_block; //distance of the smallest area that is surrounded by streets
extern int n_blocks; //An integer that represents the amount of blocks

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

extern int size_vehicles;

extern SVehicle *pr_vehicles;
extern SVehicle *pw_vehicles;

extern SVehicle *vehicles_A;
extern SVehicle *vehicles_Z;

extern vector<SVehicle> random_pos;
extern vector<SVehicle> random_pos_h;
extern vector<SVehicle> random_pos_v;

//extern float density;

extern int total_cell;
extern int n_cell_h;
extern int n_cell_v;
extern float p_turn;

//TODO porque velocity y flux tambien estan en measures.h?
extern float velocity;
extern float flux;

extern float velocity_total;
extern float flux_total;
extern int collisions;

extern float save_velocity;
extern float save_flux;

extern int n_exp;
extern int n_ticks;
extern float size_step;

extern int error_fp;
extern int error_fn;

extern int test_error_fp;
extern int test_error_fn;

extern FILE *fp_f;
extern FILE *fp_v;

extern FILE *fp_fopt;
extern FILE *fp_vopt;

//extern int metodo;
extern int inicio;

////////////////////////////////////////////////////////////////////////////////////////////////

// random generator function:
int myrandom (int i);
double frand();

void SetPositionVehicle(char type_street, int y, int x, int id);
SPosition GetPositionVehicle(int id);
void SetVelocityVehicle(int spd, int id);
int GetVelocityVehicle(int id);
void SetDirectionVehicle(char dir, int id);
char GetDirectionVehicle(int id);
void SetColorVehicle(struct Color color, int id);
struct Color GetColorVehicle(int id);
char GetTypeStreetVehicle(int id);
void SetVisibleVehicle(int id, bool visible);
bool GetVisibleVehicle(int id);
void SetTypeStreetVehicle(char type, int id);

bool GetVehiclesStopped(char type_street, int y, int x);
bool isVehiclesStoppedIntersection(int n, int m);
bool VehiclesStoppedDistance_e(char type_street, int n, int m, int d_e);

void SetValueCellStreet(char type_street, int y, int x, int value, bool visible, int id);
int GetValueCellStreet(char type_street, int y, int x);
void SetIDCellStreet(char type_street, int y, int x, int id);
int GetIDCellStreet(char type_street, int y, int x);
bool GetVisibleCellStreet(char type_street, int y, int x);
int Rule_184(char type_street, int y, int x, char direction);
int Rule_252(char type_street, int y, int x, char direction);
bool isIntersection(char type_street, int y, int x);

void InializedCity(int n_h_streets, int m_v_streets, int d_s_block, float dens_h, float dens_v, float p_t);
int RunSimulation(int tick);
int DistribuiteVehicles(float d_h, float d_v);
void SwitchVehiclesRW();
void resetCityWrite();
void printCity();
void FreeCity();
void SwitchMatricesRW();
int CityBuilding(int n_hor, int m_ver, int d_blk);

int allocateMemoryCity();

void RunSimulationGreenWave(int tick);
void RunSimulationSelfOrganization(int tick);

bool turn(char &type_street, int &y, int &x, int &v, char &direction, bool visible, int id);
int mainFunction(int n_h, int n_v, int n_b, float p_t, int met, float _P, int max_n, int max_m, int min_t, int max_t, int met_s, float pre, int dis_d, int dis_r, int dis_e);

#endif // VEHICLE


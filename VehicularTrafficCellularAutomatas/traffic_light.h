#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H


/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Description: This program implements the algorithms of the Green Wave and Self Organization to control traffic ligths in a city proposed by
Carlos Gershenson and David A. Rosenblueth. There have been several highway traffic models proposed based on cellular au-
tomata. The simplest one is elementary cellular automaton rule 184. This model to city traffic with cellular automata coupled at
intersections using only rules 184, 252, and 136. The simplicity of the model offers a clear understanding of the main properties
of city traffc and its phase transitions.

The green-wave method that tries to optimize phases according to expected flows and a self-organizing method that adapts to the
current traffic conditions. The self-organizing method delivers considerable improvements over the green-wave method.

Autor: Jorge Luis Zapotecatl López

Date: 01/31/2014

*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
#include "sensor.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TRAFFIC LIGHTS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct  STraffic{
    int light;//0 Red 1 Green
    int time;
    int offset_t;

    int position;
    int pos_intersection;

    char direction;
    double T_local; //Only for test
};

struct STrafficLight {
    STraffic horizontal;
    STraffic vertical;
};

struct  STrafficSO{
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
    int z_vehicles;
    int maxim_m;

    int position;
    int pos_intersection;

   // bool r7;
    //Zapo

    int red_time;
    int green_time;

   // float p_density;
    int theta;
    int tau;
    int d_vehicles;
    int total_vehicles;

};

struct STrafficSelfOrganizing {
 STrafficSO horizontal;
 STrafficSO vertical;
};

extern STrafficLight **traffic_light;
extern STrafficSelfOrganizing **traffic_light_so;

extern int *h_regionIntersection[2];
extern int *v_regionIntersection[2];

extern float P; //Duration of a green light, i.e. half a period (T/2). To avoid stopping of vehicels, set this equal to half the length of the street or equal to a factor of half the length of the street.
extern float T;


/////////////////////////////////////////////////

void InializedTrafficLights(float _P, int maxim_n, int maxim_m, int min_time, int max_time);
void InializedTrafficLightGreen(float _P);
void SwitchTrafficLight(int n, int m);
int GetValueTrafficLight(char type_street, int n, int m);
int GetPositionTrafficLight(char type_street, int n, int m);
int GetPositionIntersectionTrafficLight(char type_street, int n, int m);

void InializedTrafficSelfOrganizing(int maxim_n, int maxim_m, int min_time, int max_time);
void SwitchTrafficLightSO(int n, int m);
void SwitchBothRedTrafficLightSO(int n, int m);
int GetValueTrafficLightSO(char type_street, int n, int m);
int GetPositionIntersectionTrafficLightSO(char type_street, int n, int m);
int GetPositionTrafficLightSO(char type_street, int n, int m);
void RestoreSingleGreen(char type_street, int n, int m);
void setSingleRed(char type_street, int n, int m);
void setSingleGreen(char type_street, int n, int m);

//Metodo de Ola verde
void TrafficLightGreenWave(int n, int m);

//Metodo auto-organizante
void TrafficLightSelfOrganizing(int n, int m);
bool rule6(int n, int m);
bool rule5(int n, int m);
bool rule4(int n, int m);
bool rule3(int n, int m);
bool rule2(int n, int m);
void rule1(int n, int m);

void testRules(int n, int m);
//void OptimizedTrafficLightSelfOrganizing(int n, int m);

//Metodo auto-organizante basado en presion
void TrafficLightImpulseSelfOrganizing(int n, int m);
bool narrowWaitTimes(int n, int m);
bool avoidObstructingIntersection(int n, int m);
void greenStreetGreaterVehicularImpulse(int n, int m);

void ImpulseSetSingleRed(char type_street, int n, int m);
void ImpulseSetSingleGreen(char type_street, int n, int m);

int allocateMemoryTrafficLight();
int allocateMemoryTrafficLightSO();

void freeTrafficLight();
void freeTrafficLightSO();
void FreeTrafficLights();

#endif // TRAFFIC_LIGHT_H

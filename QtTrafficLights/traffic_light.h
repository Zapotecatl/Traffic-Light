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

#include "sensor.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TRAFFIC LIGHTS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct STrafficLight {

    int light;//0 Red 1 Green
    int time;
    int offset_t;

    int position;
    int pos_intersection;
};

struct STrafficSelfOrganizing {

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


   // bool r7;


};

extern STrafficLight **h_traffic_light;
extern STrafficLight **v_traffic_light;

extern STrafficSelfOrganizing **h_traffic_light_so;
extern STrafficSelfOrganizing **v_traffic_light_so;

extern int *h_regionTraffic_light[2];
extern int *v_regionTraffic_light[2];

extern int *h_regionTraffic_lightSO[2];
extern int *v_regionTraffic_lightSO[2];

extern float P; //Duration of a green light, i.e. half a period (T/2). To avoid stopping of vehicels, set this equal to half the length of the street or equal to a factor of half the length of the street.
extern float T;
extern int metodo_light;


/////////////////////////////////////////////////

double mod(double number1, double number2);

void freeTrafficLightSO();
void regionTrafficLight();
void regionTrafficLightSO();

void InializedTrafficLights(int metodo, float _P, int maxim_n, int maxim_m, int min_time, int max_time);
void InializedTrafficLightGreen(float _P);

void SwitchTrafficLight(int n, int m);
int GetValueTrafficLight(char type_street, int n, int m);
int GetPositionTrafficLight(char type_street, int n, int m);
void InializedTrafficSelfOrganizing(int maxim_n, int maxim_m, int min_time, int max_time);

void SwitchTrafficLightSO(int n, int m);
int GetRegionTrafficLight(char type_street, int x, int direction);
int GetRegionTrafficLightSO(char type_street, int x, int direction);
void SwitchBothRedTrafficLightSO(int n, int m);
int GetValueTrafficLightSO(char type_street, int n, int m);
int GetPositionIntersectionTrafficLightSO(char type_street, int n, int m);
int GetPositionTrafficLightSO(char type_street, int n, int m);
void RestoreSingleGreen(char type_street, int n, int m);
void setSingleRed(char type_street, int n, int m);

void TrafficLightGreenWave(int n, int m);
void TrafficLightSelfOrganizing(int n, int m);
bool rule6(int n, int m);
bool rule5(int n, int m);
bool rule4(int n, int m);
bool rule3(int n, int m);
bool rule2(int n, int m);
void rule1(int n, int m);

void testRules(int n, int m);
//void OptimizedTrafficLightSelfOrganizing(int n, int m);

void RunTrafficLight(int n, int m);

int allocateMemoryTrafficLight();
int allocateMemoryTrafficLightSO();
int freeTrafficLight();

#endif // TRAFFIC_LIGHT_H

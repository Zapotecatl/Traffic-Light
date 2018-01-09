#ifndef SLOT_SYSTEM_H
#define SLOT_SYSTEM_H

#include <iostream>
#include <list>
#include <math.h> /* ceil */

#include <QDebug>

#include "city.h"
#include "vehicle.h"

struct SSlot{
    int id;
    char type_street;
    int atv; //Tiempo de arrivo y ocupacion de la interseccion
    int tv; //Tiempo de arrivo asignado por el metodo slot system
};

struct SSlotBasedSystem {
    //Parametros del algoritmo
    int T1;
    int T2;
    int Tsep;
    int N;
    int tl;
    int t_res;
    char dl;

    //Parametros de la interseccion
    int pos_h;
    int pos_v;
    int pos_intersection_h;
    int pos_intersection_v;
    int occupancy_id;
    char direction_h;
    char direction_v;

    std::list<SSlot> ordered_list;
    std::list<SSlot> list_H;
    std::list<SSlot> list_V;
    std::list<SSlot> batch;

    SSlot slot_last_h;
    SSlot slot_last_v;

    bool vehicle_stop_H;
    bool vehicle_stop_V;
    int state;//1.- normal
};

extern SSlotBasedSystem **slot_system;

void InializedSlotSystem(int N);
void slotBasedSystem(int n, int m);
void insertSlotsList(int n, int m);
void updateAtv(int n, int m);
void calculateOrderedList(int n, int m);
int GetOccupancyIdSlotSystem(int n, int m);
int GetPositionDirectionSlotSystem(char type_street, int n, int m);
int GetPositionIntersectionSlotSystem(char type_street, int n, int m);
void allocateMemorySlotSystem();
void freeSlotSystem();
void printList(int n, int m);
int GetValueSlotBasedSystem(char type_street, int n, int m, int id);

int calculateT1();
int calculateT2(int vel);
int calculateATV(int vel, int position_1, int position_2, char dir, int distance_street);
bool compare_atv(const SSlot &a, const SSlot &b);
bool compare_tv(const SSlot &a, const SSlot &b);

#endif // SLOT_SYSTEM_H

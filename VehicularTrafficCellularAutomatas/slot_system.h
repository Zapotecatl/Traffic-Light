#ifndef SLOT_SYSTEM_H
#define SLOT_SYSTEM_H

#include <iostream>
#include <list>
#include <math.h> /* ceil */
#include <algorithm>

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
    std::list<SSlot> batch;

    int id_slot_last_h;
    int id_slot_last_v;

    int state;//0.-Sin bloqueos 3.- Ambos bloqueados 2.- Bloqueado en H 1.- Bloqueado en V
};

extern SSlotBasedSystem **slot_system;

void InializedSlotSystem(int N);
void slotBasedSystem(int n, int m);
int slot_rule_e(int n, int m);

void insertSlotsList(int n, int m);
void leftInsert(char type_street, int y, int _start, int _end, int _pos_inter_x, int _d_street, std::list<SSlot>& list);
void rightInsert(char type_street, int y, int _start, int _end, int _pos_inter_x, int _d_street, std::list<SSlot>& list);
bool isIDStillValid(int n, int m, int id);

void updateAtv(int n, int m);
void calculateOrderedList(int n, int m);
int GetOccupancyIdSlotSystem(int n, int m);
int GetPositionDirectionSlotSystem(char type_street, int n, int m);
int GetPositionIntersectionSlotSystem(char type_street, int n, int m);
void allocateMemorySlotSystem();
void freeSlotSystem();
void printList(int n, int m);
void printBatch(int n, int m);
int GetValueSlotBasedSystem(char type_street, int n, int m, int id);

int calculateT1();
int calculateT2();
int calculateATV(int id, int position_intersection, char dir, int distance_street, int atv1);
bool compare_atv(const SSlot &a, const SSlot &b);
bool compare_tv(const SSlot &a, const SSlot &b);

#endif // SLOT_SYSTEM_H

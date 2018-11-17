#ifndef SENSOR_H
#define SENSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <iostream>
#include <algorithm>
#include <vector>

#include "vehicular_model.h"
#include "vehicle.h"
#include "city.h"

using namespace std;

struct SVirtualCell {
    int velocity;
    int value;
};

struct VirtualVehicle {
    int position;
    int velocity;
    int id;
    bool visible;
    char direction;

};

struct STSensor {
    int position;
    char direction;
    int distance_d;
    int distance_r;
    int distance_e;
    int distance_z;
    int start;
    int end;
};

struct STraditionalSensor {
    STSensor horizontal;
    STSensor vertical;
};

struct SDCSensor {
    int position;
    char direction;
    int distance_d;
    int distance_r;
    int distance_e;
    int start;
    int end;
};

struct SDistributedSensor {
    SDCSensor horizontal;
    SDCSensor vertical;
};


struct SSensor {

    int position;
    char direction;
    int distance_d;
    int distance_r;
    int distance_e;
    int distance_z;

    int state_light;//1 Green 2 Red
    int p_traffic_light;//Posicion del semaforo
    int p_sensor_f;//Posicion del sensor de enfrente

    bool stop_front; //Representa al mensaje para detenerse porque la calle posterior esta detenida
    bool stop_received; //Representa la variable que indica que hay autos detenidos en su mapa de recibidos
    bool stop_sended; //Representa la variable que indica que hay autos detenidos en su mapa de enviados
    bool my_stop; // Representa la conlusion de que hay autos detenidos en al principio de mi calle
    int received;
    int sended;
    int epsilon;

    VirtualVehicle sensed_vehicle;

    //Virtual environment ////////////////////

    bool switch_virtual_cell;

    SVirtualCell *pr_street; //Pointers for read
    SVirtualCell *pw_street; //Pointers for write

    SVirtualCell *virtualCell_A;
    SVirtualCell *virtualCell_Z;

    vector<VirtualVehicle> vehicles_write;
    vector<VirtualVehicle> vehicles_read;

};

struct SDeliberativeSensors {
    SSensor horizontal;
    SSensor vertical;
};


struct SResults {

    bool arrived_sended;
    bool received_stop;
    bool sent_stop;
};

struct SResults2 {

    int n;
    int m;
    int n_z;
};

struct Package1 {

    int light_state;
    bool stop_front;
    int reived_front;
};

struct Package2 {
    int n;
    int m;
    int n_z;
    bool stop;
};

struct Package3 {

    bool mystop;
    int received;
};

extern STraditionalSensor **t_sensores;
extern SDeliberativeSensors **d_sensores;
extern SDistributedSensor **distributed_sensores;

extern float precison_sensor;
extern int metodo_sensado;

extern int stopped_distance;

///////////////////////////////////////////////////////////////////////////////////////////////

void InializedTraditionalSensors(int distance_d, int distance_r, int distance_e, int distance_z);
void regionTraditionalSensores();
int allocateTraditionalMemorySensors();
int GetIndexTraditionalSensors(char type_street, char direction, int x);
void freeTraditionalSensors();
void traditionalSensing(int n, int m);

void InializedDeliberativeSensors(int distance_d, int distance_r, int distance_e, int distance_z);
void regionDeliberativeSensores();
int GetIndexDeliberativeSensors(char type_street, int direction, int x);
void freeDeliberativeSensors();
void deliberativeSensing(int n, int m);
void FillVehicles(int n, int m, int x, int y);
int GetPositionSensor(char type_street, int n, int m);
char GetDirectionSensor(char type_street, int n, int m);
int GetOffsetSensor(char type_street, int n, int m);
int GetSizeReadSensor(char type_street, int n, int m);
int GetPositionVirtualVehicleSensor(char type_street, int n, int m, int i);
int IndexFrontSensor(char type_street, int n, int m);
bool determineVisible(char type_street, char direction, int x, int n, int m, bool prev_visible);
int PositionFrontSensor(char type_street, int n, int m);
int OneCellBeforePositionFrontSensor(char type_street, char direction, int n, int m);
int GetPositionSensedSensor(char type_street, int n, int m);
void SetVisibleSensedVehicle(char type_street, int n, int m, bool visible);
bool GetVisibleSensedVehicle(char type_street, int n, int m);

Package1 ReceivesMessageFrontCT(char type_street, int n, int m, int tmp);
void SendMessageFrontCT(char type_street, Package2 &package2, int n, int m);
void SendMessageBackCT(char type_street, Package3 &package3, int n, int m);
bool ReceivesMessageBackCT(char type_street, int n, int m);
SResults SimulateVirtualEnvironment(char type_street, int n, int m);
void SimulationHorizontal(int n, int m, SResults &results);
void SimulationVertical(int n, int m, SResults &results);
SResults2 countVirtualEnvironment(char type_street, int n, int m);

int localVirtualVehiclePosition(char type_street, char direction, int n, int m, int x);
void SwitchVirtualCellRW(char type_street, int n, int m);
void SetVirtualVehicleCell(char type_street, char direction, int n, int m, int x, int velocity);
void SetVirtualCellValue(char type_street, int n, int m, int x, int value);
void SetVirtualCellVelocity(char type_street, int n, int m, int i, int vel);
int GetVirtualCellValue(char type_street, int n, int m, int i);
int GetVirtualCellVelocity(char type_street, int n, int m, int i);
bool GetReceivedStopVirtualVehicles(char type_street, char direction, int n, int m);
bool GetSendedStopVirtualVehicles(char type_street, char direction, int n, int m);
int GetReceivedSizeVirtualVehicles(char type_street, char direction, int n, int m);
int GetSendedSizeVirtualVehicles(char type_street, char direction, int n, int m);
void resetVirtualWrite(char type_street,int n, int m);
bool isThereVehicle(char type_street, int y, int x, int n, int m);
bool isThereVirtualVehicle(char type_street, int n, int m, int x);
int GetValueTrafficLightDeliberativeSensor(char type_street, int n, int m);
int GetPositionTrafficLightDeliberativeSensor(char type_street, int n, int m);

int distanceVirtualFrontVehicle(char type_street, char direction, int n, int m, int x, int *velocity);
int CalculateVirtualVelocity(char type_street, char direction, int n, int m, int x, int v);
int CalculateVirtualVelocityFrontSensor(char type_street, char direction, int n, int m, int x, int v);

int allocateMemorySensors();

void InializedSensors(int metodo_s, float precision, int distance_d, int distance_r, int distance_e, int distance_z);
void FreeSensors();

void resetAllDetectVehicle();
void resetOneDetectVehicle(char type_street, int n, int m);
void DetectVehicle(char type_street, char direction, int n, int m, int current_x, int current_rear_x, int previous_x, int velocity, int distance_street, int id);


// Distributed Sensors
int allocateMemoryDistributedSensors();
void freeDistributedSensors();
void regionDistributedSensores();
int GetIndexDistributedSensors(char type_street, char direction, int x);
void DistributedSensing(int n, int m);
void InializedDistributedSensors(int distance_d, int distance_r, int distance_e, int distance_z);

#endif // SENSOR_H


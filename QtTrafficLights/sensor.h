#ifndef SENSOR
#define SENSOR

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

struct SVirtualCell {

    int stop_cell;
    int value;

};

struct VirtualVehicle {

    int position;
    int direction;
    int velocity;

};

struct SSensorTraditional {

     int position;
     char direction;
     int distance_d;
     int distance_r;
     int distance_e;
     int start;
     int end;

};

struct SSensor {

    int position;
    char direction;
    int distance_d;
    int distance_r;
    int distance_e;

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

    //Virtual environment ////////////////////

    bool switch_virtual_cell;

    SVirtualCell *pr_street; //Pointers for read
    SVirtualCell *pw_street; //Pointers for write

    SVirtualCell *virtualCell_A;
    SVirtualCell *virtualCell_Z;

    vector<VirtualVehicle> vehicles_write;
    vector<VirtualVehicle> vehicles_read;

};

struct SResults {

    bool arrived_sended;
    bool received_stop;
    bool sent_stop;
};

struct SResults2 {

    int n;
    int m;
};

struct Package1 {

    int light_state;
    bool stop_front;
    int reived_front;
};

struct Package2 {
    int n;
    int m;
    bool stop;
};

struct Package3 {

    bool mystop;
    int received;
};

extern SSensorTraditional **v_sensores_t;
extern SSensorTraditional **h_sensores_t;

extern SSensor **v_sensores;
extern SSensor **h_sensores;

extern int *h_regionSensoresT[2];
extern int *v_regionSensoresT[2];

extern int *h_regionSensoresNew[2];
extern int *v_regionSensoresNew[2];

extern float precison_sensor;
extern int metodo_sensado;


///////////////////////////////////////////////////////////////////////////////////////////////

void InializedSensoresTraditional(int distance_d, int distance_r, int distance_e);
int allocateMemorySensorsTraditional();
void freeSensorsTraditional();
void SensingSelfOrganizing(int n, int m);



void InializedSensoresDeliberative(int distance_d, int distance_r, int distance_e);
void FillVehicles(int n, int m, int x, int y);
int GetPositionSensor(char type_street, int n, int m);
char GetDirectionSensor(char type_street, int n, int m);
int GetOffsetSensor(char type_street, int n, int m);
int GetSizeReadSensor(char type_street, int n, int m);
int GetPositionVirtualVehicleSensor(char type_street, int n, int m, int i);
int PositionFrontSensor(char type_street, int n, int m);
void regionSensoresTraditional();
int GetSensoresTraditional(char type_street, char direction, int x);
void regionSensoresNew();
int GetSensoresRegion(char type_street, int direction, int x);
bool determineVisible(char type_street, char direction, int x, bool prev_visible);
void freeSensors();
void SensingSelfOrganizingSensor(int n, int m);

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
void SetVirtualCellValue(char type_street, int n, int m, int i, int value);
void SetVirtualCellStop(char type_street, int n, int m, int i, int vel);
int GetVirtualCellValue(char type_street, int n, int m, int i);
int GetVirtualCellStop(char type_street, int n, int m, int i);
bool GetReceivedStopVirtualVehicles(char type_street, char direction, int n, int m);
bool GetSendedStopVirtualVehicles(char type_street, char direction, int n, int m);
int GetReceivedSizeVirtualVehicles(char type_street, char direction, int n, int m);
int GetSendedSizeVirtualVehicles(char type_street, char direction, int n, int m);
void resetVirtualWrite(char type_street,int n, int m);
int Virtual_Rule_184(char type_street, char direction, int n, int m, int x);
int Virtual_Rule_252(char type_street, char direction, int n, int m, int x);

int allocateMemorySensors();




void InializedSensores(int metodo_s, float precision, int distance_d, int distance_r, int distance_e);

#endif // SENSOR


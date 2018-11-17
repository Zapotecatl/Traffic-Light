#ifndef VEHICLE
#define VEHICLE

#include "vehicular_model.h"

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

    char direction;
    char type_street;
    SPosition position;
    Color color;
    int id;
    int speed;
    int ls;
    int rear_position;
    int front_position;
    int previous_position;
    bool visible;
    bool autonomous;
    bool failure;

};

SVehicle GetVehicle(int id);
void SetVehicle(SVehicle *vehicle, int id);
SPosition GetPositionVehicle(int id);
void SetPositionVehicle(int y, int x, int id);
int GetRearPositionVehicle(int id);
int GetFrontPositionVehicle(int id);
int GetVelocityVehicle(int id);
void SetVelocityVehicle(int speed, int id);
int GetLengthVehicle(int id);
void SetLengthVehicle(int length, int id);
char GetTypeStreetVehicle(int id);
void SetTypeStreetVehicle(char type, int id);
char GetDirectionVehicle(int id);
void SetDirectionVehicle(char dir, int id);
bool GetVisibleVehicle(int id);
void SetVisibleVehicle(bool visible, int id);
struct Color GetColorVehicle(int id);
void SetColorVehicle(struct Color color, int id);
bool GetAutonomousVehicle(int id);


void SwitchVehiclesRW();
int allocateMemoryVehicles(int size_vehicles);
void FreeVehicles();

int calculateDistance(int position_1, int position_2, char dir, int distance_street);
int CalculateVelocity(unsigned int dn, int vp, int n, int m, int d_street, int id);

#endif // VEHICLE


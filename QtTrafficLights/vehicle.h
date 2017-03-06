#ifndef VEHICLE
#define VEHICLE

#include "city.h"
#include "sensor.h"

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

bool turn(char &type_street, int &y, int &x, int &v, char &direction, bool visible, int id);


#endif // VEHICLE


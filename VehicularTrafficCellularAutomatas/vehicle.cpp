#include "traffic_light.h"
#include "slot_system.h"
#include "vehicle.h"
#include "sensor.h"
#include "measures.h"

#include <qdebug.h>

SVehicle GetVehicle(int id)
{
    if (id < 0 || id >= size_vehicles) {
        qDebug() << "ERROR: size_vehicles" << __PRETTY_FUNCTION__ << id;
        id = 0;
    }

    /*
    SVehicle vehicle;

    vehicle.position.x = pr_vehicles[id].position.x;
    vehicle.position.y = pr_vehicles[id].position.y;
    vehicle.color.r = pr_vehicles[id].color.r;
    vehicle.color.g = pr_vehicles[id].color.g;
    vehicle.color.b = pr_vehicles[id].color.b;
    vehicle.id = pr_vehicles[id].id;
    vehicle.speed = pr_vehicles[id].speed;
    vehicle.ls = pr_vehicles[id].ls;
    vehicle.rear_position = pr_vehicles[id].rear_position;
    vehicle.front_position = pr_vehicles[id].front_position;
    vehicle.previous_position = pr_vehicles[id].previous_position;
    vehicle.direction = pr_vehicles[id].direction;
    vehicle.type_street = pr_vehicles[id].type_street;
    vehicle.visible = pr_vehicles[id].visible;
    vehicle.autonomous = pr_vehicles[id].autonomous;

    return vehicle;*/

    return pr_vehicles[id];
}

void SetVehicle(SVehicle *vehicle, int id)
{
    if (id < 0 || id >= size_vehicles) {
        qDebug() << "ERROR: size_vehicles" << __PRETTY_FUNCTION__ << id;
        id = 0;
    }

    /*
    pw_vehicles[id].position.x = vehicle->position.x;
    pw_vehicles[id].position.y = vehicle->position.y;
    pw_vehicles[id].color.r = vehicle->color.r;
    pw_vehicles[id].color.g = vehicle->color.g;
    pw_vehicles[id].color.b = vehicle->color.b;
    pw_vehicles[id].id = vehicle->id;
    pw_vehicles[id].speed = vehicle->speed;
    pw_vehicles[id].ls = vehicle->ls;
    pw_vehicles[id].rear_position = vehicle->rear_position;
    pw_vehicles[id].front_position = vehicle->front_position;
    pw_vehicles[id].previous_position = vehicle->previous_position;
    pw_vehicles[id].direction = vehicle->direction;
    pw_vehicles[id].type_street = vehicle->type_street;
    pw_vehicles[id].visible = vehicle->visible;
    pw_vehicles[id].autonomous = vehicle->autonomous;*/

    pw_vehicles[id] = *vehicle;
}

SPosition GetPositionVehicle(int id)
{
    if (id < 0 || id >= size_vehicles) {
        qDebug() << "ERROR: size_vehicles" << __PRETTY_FUNCTION__;
        id = 0;
    }
    return pr_vehicles[id].position;
}

void SetPositionVehicle(int y, int x, int id)
{
    int d_rear;
    int d_street;

    if (id < 0 || id >= size_vehicles) {
        qDebug() << "ERROR: " << __PRETTY_FUNCTION__;
        id = 0;
    }

    if (pr_vehicles[id].type_street == 'H'){
        d_street = d_hor_street;
        if (y < 0 || y >= n_hor_streets) {
            qDebug() << "ERROR: n_hor_streets" << __PRETTY_FUNCTION__;
            y = 0;
        }
        if (x < 0 || x >= d_hor_street) {
            qDebug() << "ERROR: d_hor_street" << __PRETTY_FUNCTION__;
            x = 0;
        }
    }
    else {
        d_street = d_ver_street;
        if (y < 0 || y >= m_ver_streets){
            qDebug() << "ERROR: m_ver_streets" << __PRETTY_FUNCTION__;
            y = 0;
        }
        if (x < 0 || x >= d_ver_street){
            qDebug() << "ERROR: d_ver_street" << __PRETTY_FUNCTION__;
            x = 0;
        }
    }

    pw_vehicles[id].previous_position = pr_vehicles[id].position.x;

    pw_vehicles[id].position.x = x;
    pw_vehicles[id].position.y = y;

    pw_vehicles[id].front_position = x;

    if (pr_vehicles[id].direction == 'R') {
        d_rear = x - (ls - 1);
        if (d_rear < 0)
            pw_vehicles[id].rear_position =  d_street + d_rear;
        else
            pw_vehicles[id].rear_position =  d_rear;
    }
    else {
        d_rear = x + (ls - 1);
        if (d_rear > (d_street - 1))
            pw_vehicles[id].rear_position =  d_rear - d_street;
        else
            pw_vehicles[id].rear_position =  d_rear;
    }

}

int GetRearPositionVehicle(int id)
{
    if (id < 0 || id >= size_vehicles) {
        qDebug() << "ERROR: size_vehicles" << __PRETTY_FUNCTION__;
        id = 0;
    }
    return pr_vehicles[id].rear_position;
}

int GetFrontPositionVehicle(int id)
{
    if (id < 0 || id >= size_vehicles) {
        qDebug() << "ERROR: size_vehicles" << __PRETTY_FUNCTION__;
        id = 0;
    }
    return pr_vehicles[id].front_position;
}

int GetVelocityVehicle(int id)
{
    if (id < 0 || id >= size_vehicles) {
        qDebug() << "ERROR: size_vehicles" << __PRETTY_FUNCTION__;
        id = 0;
    }
    return pr_vehicles[id].speed;
}

void SetVelocityVehicle(int speed, int id)
{
    if (id < 0 || id >= size_vehicles) {
        qDebug() << "ERROR: size_vehicles" << __PRETTY_FUNCTION__;
        id = 0;
    }
    pw_vehicles[id].speed = speed;
}

int GetLengthVehicle(int id)
{
    if (id < 0 || id >= size_vehicles) {
        qDebug() << "ERROR: size_vehicles" << __PRETTY_FUNCTION__;
        id = 0;
    }
    return pr_vehicles[id].ls;
}

void SetLengthVehicle(int length, int id)
{
    if (id < 0 || id >= size_vehicles) {
        qDebug() << "ERROR: size_vehicles" << __PRETTY_FUNCTION__;
        id = 0;
    }

    if (length <= 0)
        length = 1;

    pw_vehicles[id].ls = length;
}

char GetTypeStreetVehicle(int id)
{
    if (id < 0 || id >= size_vehicles) {
        qDebug() << "ERROR: size_vehicles" << __PRETTY_FUNCTION__;
        id = 0;
    }
    return pr_vehicles[id].type_street;
}

void SetTypeStreetVehicle(char type, int id)
{
    if (id < 0 || id >= size_vehicles) {
        qDebug() << "ERROR: size_vehicles" << __PRETTY_FUNCTION__;
        id = 0;
    }
    pw_vehicles[id].type_street = type;
}

char GetDirectionVehicle(int id)
{
    if (id < 0 || id >= size_vehicles) {
        qDebug() << "ERROR: size_vehicles" << __PRETTY_FUNCTION__;
        id = 0;
    }
    return pr_vehicles[id].direction;
}

void SetDirectionVehicle(char dir, int id)
{
    if (id < 0 || id >= size_vehicles) {
        qDebug() << "ERROR: size_vehicles" << __PRETTY_FUNCTION__;
        id = 0;
    }
    pw_vehicles[id].direction = dir;
}

bool GetVisibleVehicle(int id)
{
    if (id < 0 || id >= size_vehicles) {
        qDebug() << "ERROR: size_vehicles" << __PRETTY_FUNCTION__;
        id = 0;
    }
    return pr_vehicles[id].visible;
}

void SetVisibleVehicle(bool visible, int id)
{
    if (id < 0 || id >= size_vehicles) {
        qDebug() << "ERROR: size_vehicles" << __PRETTY_FUNCTION__;
        id = 0;
    }
    pw_vehicles[id].visible = visible;
}

struct Color GetColorVehicle(int id)
{
    if (id < 0 || id >= size_vehicles) {
        qDebug() << "ERROR: size_vehicles" << __PRETTY_FUNCTION__;
        id = 0;
    }
    return pr_vehicles[id].color;
}

void SetColorVehicle(struct Color color, int id)
{
    if (id < 0 || id >= size_vehicles) {
        qDebug() << "ERROR: size_vehicles" << __PRETTY_FUNCTION__;
        id = 0;
    }
    pw_vehicles[id].color = color;
}

bool GetAutonomousVehicle(int id)
{
    if (id < 0 || id >= size_vehicles){
        qDebug() << "ERROR: size_vehicles" << __PRETTY_FUNCTION__;
        id = 0;
    }
    return pr_vehicles[id].autonomous;
}

int allocateMemoryVehicles(int size_vehicles)
{
    pr_vehicles = NULL;
    pw_vehicles = NULL;

    vehicles_A = NULL;
    vehicles_Z = NULL;

    if (size_vehicles <= 0) {
        qDebug() << "ERROR: invalid number of vehicles: " << size_vehicles;
        return -1;
    }
    vehicles_A = new SVehicle[size_vehicles];
    vehicles_Z = new SVehicle[size_vehicles];
}

void SwitchVehiclesRW()
{
    if (switch_vehicles == true){

        //Inicializa matriz de lectura
        pr_vehicles = vehicles_A;
        pw_vehicles = vehicles_Z;
    }
    else {

        //Inicializa matriz de lectura
        pr_vehicles = vehicles_Z;
        pw_vehicles = vehicles_A;
    }
    switch_vehicles = !switch_vehicles;
}

void FreeVehicles()
{
    pr_vehicles = NULL;
    pw_vehicles = NULL;

    if (vehicles_Z != NULL) {
        delete [] vehicles_Z;
        vehicles_Z = NULL;
    }

    if (vehicles_A != NULL) {
        delete [] vehicles_A;
        vehicles_A = NULL;
    }
}

///////////////////////////////////////////////////////////////////////////

bool GetVehiclesStopped(char type_street, int y, int x)
{
    if (GetValueCellStreet(type_street, y, x) >= 1){
        int id = GetIDCellStreet(type_street, y, x);

        if (id != INVALID){
            if (GetTypeStreetVehicle(id) == type_street)
                if (GetVelocityVehicle(id) == 0)
                    return true;
        }
    }
    return false;
}

bool isVehiclesStoppedIntersection(int n, int m)
{
    int x, y;

    y = n;
    x =  m * d_side_block + m;

    if (GetValueCellStreet('H', y, x) >= 1)
        if (GetVehiclesStopped('H', y, x) == true)
            return true;

    y = m;
    x = n * d_side_block + n;

    if (GetValueCellStreet('V', y, x) >= 1)
        if (GetVehiclesStopped('V', y, x) == true)
            return true;

    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief VehiclesStoppedDistance_e

bool VehiclesStoppedDistance_e(char type_street, int n, int m, int d_e)
{
    int x, y;
    int direction;
    int e;

    if (type_street == 'H') {
        y = n;
        direction = (n % 2) == 0 ? 'R' : 'L';

        if (direction == 'R') {

            for (e = d_e; e >= 0; e--) {
                x = (m * d_side_block + m) + e;
                if (GetValueCellStreet(type_street, y, x) >= 1) {//Front vehicle
                    if (GetVisibleCellStreet(type_street, n, x) == true){
                        if (GetVehiclesStopped(type_street, y, x) == true)
                            return true;
                    }
                }
            }
        }
        else {//Direccion L
            if (m > 0) {
                for (e = d_e; e >= 0; e--) {
                    x = (m * d_side_block + m) - e;
                    if (GetValueCellStreet(type_street, y, x) >= 1) {//Front vehicle
                        if (GetVisibleCellStreet(type_street, n, x) == true){
                            if (GetVehiclesStopped(type_street, y, x) == true)
                                return true;
                        }
                    }
                }
            }
            else {
                for (e = d_e; e >= 0; e--) {
                    x = (e != 0) ? d_hor_street - e : 0;
                    if (GetValueCellStreet(type_street, y, x) >= 1) {//Front vehicle
                        if (GetVisibleCellStreet(type_street, n, x) == true){
                            if (GetVehiclesStopped(type_street, y, x) == true)
                                return true;
                        }
                    }
                }
            }
        }
    } else {

        y = m;
        direction = (m % 2) == 0 ? 'R' : 'L';
        if (direction == 'R') {

            for (e = d_e; e >= 0; e--) {
                x = (n * d_side_block + n) + e;
                if (GetValueCellStreet(type_street, y, x) >= 1) {//Front vehicle
                    if (GetVisibleCellStreet(type_street, m, x) == true){
                        if (GetVehiclesStopped(type_street, y, x) == true)
                            return true;
                    }
                }
            }
        }
        else {
            if (n > 0) {
                for (e = d_e; e >= 0; e--) {
                    x = (n * d_side_block + n) - e;
                    if (GetValueCellStreet(type_street, y, x) >= 1) {//Front vehicle
                        if (GetVisibleCellStreet(type_street, m, x) == true){
                            if (GetVehiclesStopped(type_street, y, x) == true)
                                return true;
                        }
                    }
                }
            }
            else {
                for (e = d_e; e >= 0; e--) {
                    x = (e != 0) ? d_ver_street - e : 0;
                    if (GetValueCellStreet(type_street, y, x) >= 1) {//Front vehicle
                        if (GetVisibleCellStreet(type_street, m, x) == true){
                            if (GetVehiclesStopped(type_street, y, x) == true)
                                return true;
                        }
                    }
                }
            }
        }
    }

    return false;

}

int numberVehiclesDistance_z(char type_street, int n, int m, int d_z)
{
    int x, y;
    int direction;
    int z;
    int num_z;

    //d_z = d_z + (ls - 1);//Si deseas contar al vehiculo que permanece en la distancia z ,pero que el frente ya salio, ademas agregar condicion
    if (type_street == 'H') {
        y = n;
        direction = (n % 2) == 0 ? 'R' : 'L';
        num_z = 0; //Numero de vehiculos en distancia e (incluyendo interseccion)

        if (direction == 'R') {

            for (z = d_z; z >= 0; z--) {
                x = (m * d_side_block + m) + z;
                if (GetValueCellStreet(type_street, y, x) == 1) {//Front vehicle
                    if (GetVisibleCellStreet(type_street, n, x) == true)
                        num_z++;
                }
            }
        }
        else {//Direccion L
            if (m > 0) {

                for (z = d_z; z >= 0; z--) {
                    x = (m * d_side_block + m) - z;
                    if (GetValueCellStreet(type_street, y, x) == 1) {//Front vehicle
                        if (GetVisibleCellStreet(type_street, n, x) == true)
                            num_z++;
                    }
                }
            }
            else {

                for (z = d_z; z >= 0; z--) {
                    x = (z != 0) ? d_hor_street - z : 0;
                    if (GetValueCellStreet(type_street, y, x) == 1) {//Front vehicle
                        if (GetVisibleCellStreet(type_street, n, x) == true)
                            num_z++;
                    }
                }
            }
        }
    } else {

        y = m;
        direction = (m % 2) == 0 ? 'R' : 'L';
        num_z = 0; //Numero de vehiculos en distancia e (incluyendo interseccion)

        if (direction == 'R') {

            for (z = d_z; z >= 0; z--) {
                x = (n * d_side_block + n) + z;
                if (GetValueCellStreet(type_street, y, x) == 1) {//Front vehicle
                    if (GetVisibleCellStreet(type_street, m, x) == true)
                        num_z++;
                }
            }
        }
        else {

            if (n > 0) {
                for (z = d_z; z >= 0; z--) {
                    x = (n * d_side_block + n) - z;
                    if (GetValueCellStreet(type_street, y, x) == 1) {//Front vehicle
                        if (GetVisibleCellStreet(type_street, m, x) == true)
                            num_z++;
                    }
                }
            }
            else {
                for (z = d_z; z >= 0; z--) {
                    x = (z != 0) ? d_ver_street - z : 0;
                    if (GetValueCellStreet(type_street, y, x) == 1) {//Front vehicle
                        if (GetVisibleCellStreet(type_street, m, x) == true)
                            num_z++;
                    }
                }
            }
        }
    }

    return num_z;

}

int calculateDistance(int position_1, int position_2, char dir, int distance_street)
{
    int d;

    if (position_1 < 0 || position_1 >= distance_street)
        position_1 = 0;

    if (position_2 < 0 || position_2 >= distance_street)
        position_2 = 0;

    if (position_1 == position_2){
        //qDebug() << "WARNING: Esta en el mismo lugar (si es el semaforo no hay problema) " << position_1 << position_2;
        d = -1;
        return d;
    }

    if (dir == 'R') {
        //int new_position = position_2 - (length - 1);
        //position_2 = new_position >= 0 ? new_position : distance_street + new_position;

        if (position_1 < position_2){
            d = (position_2 - position_1) - 1;
            //qDebug() << "C1N: " << n << "pn1 - pn - 1: " << GetPositionVehicle(n1) << " - " << GetPositionVehicle(n) << " = " << d;
        }
        else {
            d = (distance_street + position_2) - position_1 - 1;
            //qDebug() << "C2P: " << n << "d = (l - 1) - pn - p0" << n << " : " << length_h - 1 << " - " << GetPositionVehicle(n) << " + " << GetPositionVehicle(n1) << " = " << d;
        }

    } else {

        //int new_position = position_2 + (length - 1);
        //position_2 = new_position <= (distance_street - 1) ? new_position : new_position - distance_street;

        if (position_2 < position_1){
            d = (position_1 - position_2) - 1;
            //qDebug() << "C1N: " << n << "pn1 - pn - 1: " << GetPositionVehicle(n1) << " - " << GetPositionVehicle(n) << " = " << d;
        }
        else {

            d = (distance_street + position_1) - position_2 - 1;
            //qDebug() << "C2P: " << n << "d = (l - 1) - pn - p0" << n << " : " << length_h - 1 << " - " << GetPositionVehicle(n) << " + " << GetPositionVehicle(n1) << " = " << d;
        }
    }

    return d;
}


int CalculateVelocity(unsigned int dn, int vp, int n, int m, int d_street, int id)
{
    int x = pr_vehicles[id].position.x;
    int v = pr_vehicles[id].speed;
    char type_street = pr_vehicles[id].type_street;
    char direction = pr_vehicles[id].direction;
    int previous_x = pr_vehicles[id].previous_position;
    int rear_x = pr_vehicles[id].rear_position;
    int velocity;

    if (model == 1) {
        int state_light;
        int pos_light;

        if (intersection_control == 1) {
            state_light = GetValueTrafficLight(type_street, n, m);
            pos_light = GetPositionTrafficLight(type_street, n, m);
        }
        else if (intersection_control == 2 || intersection_control == 3) {

            if (metodo_sensado == 2)
                DetectVehicle(type_street, direction, n, m, x, rear_x, previous_x, v, d_street, id);

            state_light = GetValueTrafficLightSO(type_street, n, m);
            pos_light = GetPositionTrafficLightSO(type_street, n, m);
        }
        else if (intersection_control == 4) {
            state_light = GetValueSlotBasedSystem(type_street, n, m, id);
            pos_light = GetPositionDirectionSlotSystem(type_street, n, m);
        }
        else if (intersection_control == 5) {
            state_light = GetValueDistributedControl(type_street, n, m);
            pos_light = GetPositionDistributedControl(type_street, n, m);
        }


        //state_light = 1;
        //184
        if (state_light == 0 && pos_light == x)
            velocity = Rule_252();
        else
            velocity = Rule_184(dn);
    }
    else {

        int vn_vehicle;
        int vn_light;
        int state_light;
        int pos_inter;

        if (intersection_control == 1) {
            state_light = GetValueTrafficLight(type_street, n, m);
            pos_inter = GetPositionIntersectionTrafficLight(type_street, n, m);
        }
        else if (intersection_control == 2 || intersection_control == 3) {

            if (metodo_sensado == 2)
                DetectVehicle(type_street, direction, n, m, x, rear_x, previous_x, v, d_street, id);

            state_light = GetValueTrafficLightSO(type_street, n, m);
            pos_inter = GetPositionIntersectionTrafficLightSO(type_street, n, m);
        }
        else if (intersection_control == 4) {
            state_light = GetValueSlotBasedSystem(type_street, n, m, id);
            pos_inter = GetPositionIntersectionSlotSystem(type_street, n, m);
        }
        else if (intersection_control == 5) {

            state_light = GetValueDistributedControl(type_street, n, m);

            int pos_sensor;
            int pos_before_intersection;

            if (type_street == 'H') {
                pos_sensor = distributed_control[n][m].horizontal.pos_intersection;
                pos_before_intersection = distributed_control[n][m].horizontal.position;
            }
            else {
                pos_sensor = distributed_control[n][m].vertical.pos_intersection;
                pos_before_intersection = distributed_control[n][m].vertical.position;
            }

            //Monitorear si cruzo la interseecion
            SensorDetectsVehicle(type_street, direction, n, m, x, rear_x, previous_x, v, pos_sensor, id);

            if (pr_vehicles[id].failure == true) {
                state_light = 0;

                if (v == 0 && pos_before_intersection == x) {

                    if (type_street == 'H') {
                        if (distributed_control[n][m].horizontal.light == 1) {
                            state_light = 1;
                            distributed_control[n][m].horizontal.failure_wait = false;
                        }
                        else
                            distributed_control[n][m].horizontal.failure_wait = true;
                    }
                    else {
                        if (distributed_control[n][m].vertical.light == 1){
                            state_light = 1;
                            distributed_control[n][m].vertical.failure_wait = false;
                        }
                        else
                            distributed_control[n][m].vertical.failure_wait = true;
                    }
                }
            }

            pos_inter = GetPositionIntersectionDistributedControl(type_street, n, m);
        }


        //LAI
        if (GetAutonomousVehicle(id) != true)
            vn_vehicle = CalculateVelocityLAI(dn, v, vp);
        else {
            vn_vehicle = CalculateVelocityAutonomousLAI(dn, v, vp);
            if (VehiclesStoppedDistance_e(type_street, n, m, stopped_distance) == true) {
                state_light = 0; //Cambiar la luz porque hay al menos un vehiculo detenido despues de la interseccion
            }
        }

        velocity = vn_vehicle;

        if (state_light == 0) {
            dn = calculateDistance(x, pos_inter, direction, d_street);

            if (GetAutonomousVehicle(id) != true)
                vn_light = CalculateVelocityLightLAI(dn, v);//vp es siempre 0 porque el semaforo tiene velocidad 0
            else
                vn_light = CalculateVelocityAutonomousLightLAI(dn, v);

            velocity = minimum(vn_vehicle, vn_light);
        }
    }

    return velocity;
}

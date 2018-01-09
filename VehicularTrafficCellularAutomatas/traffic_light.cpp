#include "traffic_light.h"
#include "slot_system.h"
#include "vehicle.h"
#include "sensor.h"
#include "measures.h"

STrafficLight **traffic_light;
STrafficSelfOrganizing **traffic_light_so;

float P;//Duration of a green light, i.e. half a period (T/2). To avoid stopping of vehicels, set this equal to half the length of the street or equal to a factor of half the length of the street.
float T;

bool vehicle_stop_H;
bool vehicle_stop_V;

//extern bool combination_rules[6];

void InializedTrafficLights(float _P, int maxim_n, int maxim_m, int min_time, int max_time)
{
    //Inicializa semaforos ////////////////////////////////////////////////////
    InializedTrafficLightGreen(_P); //Onda verde
    //Auto Organizado
    InializedTrafficSelfOrganizing(maxim_n, maxim_m, min_time, max_time);
}

void FreeTrafficLights()
{

    freeTrafficLightSO();
    freeTrafficLight();

}

int allocateMemoryTrafficLight()
{
    traffic_light = NULL;

    traffic_light = new STrafficLight*[n_hor_streets];
    for (int i = 0; i < n_hor_streets; i++)
        traffic_light[i] = new STrafficLight[m_ver_streets];
}

void freeTrafficLight()
{
    if (traffic_light != NULL) {
        for (int i = 0; i < n_hor_streets; i++)
            delete [] traffic_light[i];
        delete [] traffic_light;
        traffic_light = NULL;
    }
}

int allocateMemoryTrafficLightSO()
{
    traffic_light_so = NULL;

    traffic_light_so = new STrafficSelfOrganizing*[n_hor_streets];
    for (int i = 0; i < n_hor_streets; i++)
        traffic_light_so[i] = new STrafficSelfOrganizing[m_ver_streets];

    return 0;
}


void freeTrafficLightSO()
{
    if (traffic_light_so != NULL) {
        for (int i = 0; i < n_hor_streets; i++)
            delete [] traffic_light_so[i];
        delete [] traffic_light_so;
        traffic_light_so = NULL;
    }
}


//Green wave///////////////////////////////////////////////////////////////

void InializedTrafficLightGreen(float _P)
{
    int n, m;
    int x;
    int y;
    int offset_t;

    allocateMemoryTrafficLight();

    P = _P;// + 1;//Porque en C empieza en cero las posiciones compenso con uno
    T = 2  * P;

    int Limit_P = roundNearestInteger(P); //Para valores decimales de P, ie, 2.5 = 2 o 2.7 = 3

    for (n = 0; n < n_hor_streets; n++){

        for (m = 0; m < m_ver_streets; m++){

            traffic_light[n][m].horizontal.time = 0;
            traffic_light[n][m].vertical.time = 0;

            x = m * d_side_block + m;
            y = n * d_side_block + n;

            /*
            T = 2  * P;
            T = myrandomAB (2, T);
            Limit_P = roundNearestInteger(T / 2.0); //Para valores decimales de P, ie, 2.5 = 2 o 2.7 = 3
            h_traffic_light[n][m].T_local = T;
            v_traffic_light[m][n].T_local = T;*/

            //printf("Offset %d: %d\n", m, h_traffic_light[m].offset_t);

            if (n % 2 == 0) {

                traffic_light[n][m].horizontal.direction = 'R';

                if (x == 0) {
                    traffic_light[n][m].horizontal.position = d_hor_street - 1;
                    traffic_light[n][m].horizontal.pos_intersection = 0;
                }
                else {
                    traffic_light[n][m].horizontal.position = x - 1;
                    traffic_light[n][m].horizontal.pos_intersection = x;
                }
            }
            else {

                traffic_light[n][m].horizontal.direction = 'L';

                traffic_light[n][m].horizontal.position = x + 1;
                traffic_light[n][m].horizontal.pos_intersection = x;
            }

            if (m % 2 == 0) {

                traffic_light[n][m].vertical.direction = 'R';

                if (y == 0){
                    traffic_light[n][m].vertical.position = d_ver_street - 1;
                    traffic_light[n][m].vertical.pos_intersection = 0;
                }
                else {
                    traffic_light[n][m].vertical.position = y - 1;
                    traffic_light[n][m].vertical.pos_intersection = y;
                }
            }
            else {

                traffic_light[n][m].vertical.direction = 'L';

                traffic_light[n][m].vertical.position = y + 1;
                traffic_light[n][m].vertical.pos_intersection = y;
            }

            if (floor(mod(x - y, T) + 0.5) >= (T / 2.0)) {

                traffic_light[n][m].vertical.light = 1;// Green

                if (traffic_light[n][m].vertical.direction == 'R') {

                    offset_t = floor(mod(x - y, T / 2.0) + 0.5);
                    traffic_light[n][m].vertical.offset_t = Limit_P - offset_t;
                    traffic_light[n][m].vertical.time = -traffic_light[n][m].vertical.offset_t;
                }
                else {

                    offset_t = floor(mod(x - y, T / 2.0) + 0.5);
                    traffic_light[n][m].vertical.offset_t = offset_t;
                    traffic_light[n][m].vertical.time = -traffic_light[n][m].vertical.offset_t;
                }

                traffic_light[n][m].horizontal.light = 0;//Red
                traffic_light[n][m].horizontal.time = T / 2.0;


                //printf("Red Offset %d: %d\n", m, v_traffic_light[m][n].offset_t);
            }
            else {
                traffic_light[n][m].horizontal.light = 1;//Green

                if (traffic_light[n][m].horizontal.direction == 'R') {

                    offset_t = floor(mod(x - y, T / 2.0) + 0.5);
                    traffic_light[n][m].horizontal.offset_t = offset_t;
                    traffic_light[n][m].horizontal.time = -traffic_light[n][m].horizontal.offset_t;
                }
                else {

                    offset_t = floor(mod(x - y, T / 2.0) + 0.5);
                    traffic_light[n][m].horizontal.offset_t = Limit_P - offset_t;
                    traffic_light[n][m].horizontal.time = -traffic_light[n][m].horizontal.offset_t;
                }


                traffic_light[n][m].vertical.light = 0;//Red
                traffic_light[n][m].vertical.time = T / 2.0;
                // printf("Green Offset %d: %d\n", m, h_traffic_light[n][m].offset_t);
            }
        }
    }

}

void TrafficLightGreenWave(int n, int m)
{

    if (GetValueTrafficLight('H', n, m) == 1){

        //if (h_traffic_light[n][m].time < (h_traffic_light[n][m].T_local / 2.0))//Only for rendom test
        if (traffic_light[n][m].horizontal.time < P)
            traffic_light[n][m].horizontal.time++;
        else{
            SwitchTrafficLight(n, m);
        }
    }
    else {
        //if (v_traffic_light[m][n].time < (v_traffic_light[m][n].T_local / 2.0))//Only for rendom test
        if (traffic_light[n][m].vertical.time < P)
            traffic_light[n][m].vertical.time++;
        else{
            SwitchTrafficLight(n, m);
        }
    }
}

void SwitchTrafficLight(int n, int m)
{

    if (n < 0)
        n = 0;
    else if (n >= n_hor_streets)
        n = n_hor_streets - 1;

    if (m < 0)
        m = 0;
    else if (m >= m_ver_streets)
        m = m_ver_streets - 1;


    if (traffic_light[n][m].horizontal.light == 0){
        traffic_light[n][m].horizontal.light = 1;//Green
        traffic_light[n][m].vertical.light = 0;//Red

        traffic_light[n][m].horizontal.time = 0;
        traffic_light[n][m].vertical.time = P;
    }
    else {
        traffic_light[n][m].horizontal.light = 0;//Red
        traffic_light[n][m].vertical.light = 1;//Green

        traffic_light[n][m].vertical.time = 0;
        traffic_light[n][m].horizontal.time = P;
    }
}

int GetValueTrafficLight(char type_street, int n, int m)
{

    if (type_street == 'H') {
        return traffic_light[n][m].horizontal.light;
    }
    else if(type_street == 'V') {
        return traffic_light[n][m].vertical.light;
    }

    return INVALID;
}


int GetPositionTrafficLight(char type_street, int n, int m)
{
    int value;

    if (type_street == 'H')
        value = traffic_light[n][m].horizontal.position;
    else
        value = traffic_light[n][m].vertical.position;

    return value;
}

int GetPositionIntersectionTrafficLight(char type_street, int n, int m)
{
    int pos;

    if (type_street == 'H')
        pos = traffic_light[n][m].horizontal.pos_intersection;
    else
        pos = traffic_light[n][m].vertical.pos_intersection;

    return pos;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InializedTrafficSelfOrganizing(int maxim_n, int maxim_m, int min_time, int max_time)
{
    int n, m;
    int x, y;

    if (maxim_n < 0)
        maxim_n = 0;

    if (maxim_m < 0)
        maxim_m = 0;

    if (max_time < 0)
        max_time = 0;

    if (min_time < 0)
        min_time = 0;

    allocateMemoryTrafficLightSO();

    for (n = 0; n < n_hor_streets; n++) {
        for (m = 0; m < m_ver_streets; m++) {


            traffic_light_so[n][m].horizontal.light = 0;
            traffic_light_so[n][m].vertical.light = 1;

            traffic_light_so[n][m].horizontal.time_u = 0;
            traffic_light_so[n][m].vertical.time_u = 0;

            traffic_light_so[n][m].horizontal.n_sum_veh = 0;
            traffic_light_so[n][m].vertical.n_sum_veh = 0;

            traffic_light_so[n][m].horizontal.n_vehicles = 0;
            traffic_light_so[n][m].vertical.n_vehicles = 0;

            traffic_light_so[n][m].horizontal.m_vehicles = 0;
            traffic_light_so[n][m].vertical.m_vehicles = 0;

            traffic_light_so[n][m].horizontal.vehicle_stop = false;
            traffic_light_so[n][m].vertical.vehicle_stop = false;

            traffic_light_so[n][m].horizontal.changed = false;
            traffic_light_so[n][m].vertical.changed = false;

            traffic_light_so[n][m].horizontal.min_time = min_time;
            traffic_light_so[n][m].vertical.min_time = min_time;

            traffic_light_so[n][m].horizontal.max_time = max_time;
            traffic_light_so[n][m].vertical.max_time = max_time;

            traffic_light_so[n][m].horizontal.maxim_n = maxim_n;
            traffic_light_so[n][m].vertical.maxim_n = maxim_n;

            //qDebug() << "Tosssss" << maxim_m << maxim_n;

            traffic_light_so[n][m].horizontal.maxim_m = maxim_m;
            traffic_light_so[n][m].vertical.maxim_m = maxim_m;

            //Zapo//////////////////////////////////////////////////////////////////
            traffic_light_so[n][m].horizontal.red_time = 0;
            traffic_light_so[n][m].horizontal.green_time = 0;
            //h_traffic_light_so[n][m].p_density = 0;
            traffic_light_so[n][m].horizontal.theta_time = 10;
            traffic_light_so[n][m].horizontal.theta_vehicles = 0;
            traffic_light_so[n][m].horizontal.theta = 0;

            traffic_light_so[n][m].vertical.red_time = 0;
            traffic_light_so[n][m].vertical.green_time = 0;
            traffic_light_so[n][m].vertical.theta_time = 10;
            traffic_light_so[n][m].vertical.theta_vehicles = 0;
            //v_traffic_light_so[m][n].p_density = 0;
            traffic_light_so[n][m].vertical.theta = 0;

            ////////////////////////////////////////////////////

            x = m * d_side_block + m;
            y = n * d_side_block + n;

            if (n % 2 == 0) {
                if (x == 0) {
                    traffic_light_so[n][m].horizontal.position = d_hor_street - 1;
                    traffic_light_so[n][m].horizontal.pos_intersection = 0;
                }
                else {
                    traffic_light_so[n][m].horizontal.position = x - 1;
                    traffic_light_so[n][m].horizontal.pos_intersection = x;
                }
            }
            else {
                traffic_light_so[n][m].horizontal.position = x + 1;
                traffic_light_so[n][m].horizontal.pos_intersection = x;
            }

            if (m % 2 == 0) {
                if (y == 0){
                    traffic_light_so[n][m].vertical.position = d_ver_street - 1;
                    traffic_light_so[n][m].vertical.pos_intersection = 0;
                }
                else {
                    traffic_light_so[n][m].vertical.position = y - 1;
                    traffic_light_so[n][m].vertical.pos_intersection = y;
                }
            }
            else {
                traffic_light_so[n][m].vertical.position = y + 1;
                traffic_light_so[n][m].vertical.pos_intersection = y;
            }
        }
    }


}






bool rule6(int n, int m)
{

    if (traffic_light_so[n][m].horizontal.light == 0 && traffic_light_so[n][m].vertical.light == 0) {

        if (traffic_light_so[n][m].horizontal.vehicle_stop == false) {
            RestoreSingleGreen('H', n, m);
            return true;
        }


        if (traffic_light_so[n][m].vertical.vehicle_stop == false){
            RestoreSingleGreen('V', n, m);
            return true;
        }

        return false;
    }

    return true;
}


bool rule5(int n, int m)
{

    if (traffic_light_so[n][m].horizontal.vehicle_stop == true){
        setSingleRed('H',n, m);
        vehicle_stop_H = true;
    }
    else
        vehicle_stop_H = false;

    if (traffic_light_so[n][m].vertical.vehicle_stop == true){
        setSingleRed('V',n, m);
        vehicle_stop_V = true;
    }
    else
        vehicle_stop_V = false;

    return true;
}

bool rule4(int n, int m)
{
    if (vehicle_stop_V == false){
        if (traffic_light_so[n][m].horizontal.light == 1)
            if (traffic_light_so[n][m].vertical.n_vehicles >= 1  && traffic_light_so[n][m].horizontal.n_vehicles == 0) {
                SwitchTrafficLightSO(n, m);
                return false;
            }
    }

    if (vehicle_stop_H == false){
        if (traffic_light_so[n][m].vertical.light == 1)
            if (traffic_light_so[n][m].horizontal.n_vehicles >= 1  && traffic_light_so[n][m].vertical.n_vehicles == 0) {
                SwitchTrafficLightSO(n, m);
                return false;
            }
    }

    return true;
}

bool rule3(int n, int m)
{
    if (traffic_light_so[n][m].horizontal.light == 1)
        if (traffic_light_so[n][m].horizontal.m_vehicles > 0 && traffic_light_so[n][m].horizontal.m_vehicles <= traffic_light_so[n][m].horizontal.maxim_m)
            return false;

    if (traffic_light_so[n][m].vertical.light == 1)
        if (traffic_light_so[n][m].vertical.m_vehicles > 0 && traffic_light_so[n][m].vertical.m_vehicles <= traffic_light_so[n][m].vertical.maxim_m)
            return false;

    return true;
}

bool rule2(int n, int m)
{
    if (traffic_light_so[n][m].horizontal.light == 1)
        if (traffic_light_so[n][m].horizontal.time_u >= traffic_light_so[n][m].horizontal.min_time){
            if (traffic_light_so[n][m].vertical.time_u >= traffic_light_so[n][m].vertical.max_time) {
                SwitchTrafficLightSO(n, m);
                return false;
            }
            return true;
        }

    if (traffic_light_so[n][m].vertical.light == 1)
        if (traffic_light_so[n][m].vertical.time_u >= traffic_light_so[n][m].vertical.min_time){
            if (traffic_light_so[n][m].horizontal.time_u >= traffic_light_so[n][m].horizontal.max_time) {
                SwitchTrafficLightSO(n, m);
                return false;
            }
            return true;
        }

    return false;
}

void rule1(int n, int m)
{
    if (vehicle_stop_V == false)
        if (traffic_light_so[n][m].horizontal.light == 1)
            if (traffic_light_so[n][m].vertical.n_sum_veh >= traffic_light_so[n][m].vertical.maxim_n){
                SwitchTrafficLightSO(n, m);
                return;//Evita entrar en vertical
            }

    if (vehicle_stop_H == false) {
        if (traffic_light_so[n][m].vertical.light == 1){
            if (traffic_light_so[n][m].horizontal.n_sum_veh >= traffic_light_so[n][m].horizontal.maxim_n){
                SwitchTrafficLightSO(n, m);
                return;
            }
        }
    }
}

void TrafficLightSelfOrganizing(int n, int m)
{

#if 1
    traffic_light_so[n][m].horizontal.changed = false;
    traffic_light_so[n][m].vertical.changed = false;

    vehicle_stop_H = false;
    vehicle_stop_V = false;

    if (rule6(n, m) == true) {

        if (rule5(n, m) == true) {
            if (rule4(n, m) == true) {
                if (rule3(n, m) == true) {
                    if (rule2(n, m) == true) {
                        rule1(n, m);
                    }
                }
            }
        }

    }

    traffic_light_so[n][m].horizontal.time_u++;
    traffic_light_so[n][m].vertical.time_u++;

#else
 //   testRules(n, m);
#endif

}

void testRules(int n, int m)
{

    traffic_light_so[n][m].horizontal.changed = false;
    traffic_light_so[n][m].vertical.changed = false;

    vehicle_stop_H = false;
    vehicle_stop_V = false;

    bool chain_value = true;

    if (combination_rules[5] == true){
        chain_value = rule6(n, m);
    }

    if (combination_rules[4] == true) {
        if (chain_value == true)
            chain_value = rule5(n, m);
    }

    if (combination_rules[3] == true) {
        if (chain_value == true)
            chain_value = rule4(n, m);
    }

    if (combination_rules[2] == true){
        if (chain_value == true)
            chain_value = rule3(n, m);
    }

    if (combination_rules[1] == true){
        if (chain_value == true)
            chain_value = rule2(n, m);
    }

    if (combination_rules[0] == true){
        if (chain_value == true) {
            //         qDebug() << "Rule 1";
            rule1(n, m);
        }
    }

    traffic_light_so[n][m].horizontal.time_u++;
    traffic_light_so[n][m].vertical.time_u++;

}


void SwitchTrafficLightSO(int n, int m)
{
    if (n < 0)
        n = 0;
    else if (n >= n_hor_streets)
        n = n_hor_streets - 1;

    if (m < 0)
        m = 0;
    else if (m >= m_ver_streets)
        m = m_ver_streets - 1;


    traffic_light_so[n][m].horizontal.time_u = 0;
    traffic_light_so[n][m].vertical.time_u = 0;

    if (traffic_light_so[n][m].horizontal.light == 0){

        traffic_light_so[n][m].horizontal.light = 1;//Green
        traffic_light_so[n][m].vertical.light = 0;//Red

        traffic_light_so[n][m].horizontal.n_sum_veh = 0;
        traffic_light_so[n][m].vertical.n_sum_veh = 0;

        //h_traffic_light_so[n][m].theta =  h_traffic_light_so[n][m].p_density *  h_traffic_light_so[n][m].maxim_n;
        //qDebug() << "H_theta: " << h_traffic_light_so[n][m].theta;
    }
    else {

        traffic_light_so[n][m].horizontal.light = 0;//Red
        traffic_light_so[n][m].vertical.light = 1;//Green

        traffic_light_so[n][m].horizontal.n_sum_veh = 0;
        traffic_light_so[n][m].vertical.n_sum_veh = 0;

        //v_traffic_light_so[m][n].theta =  v_traffic_light_so[m][n].p_density *  v_traffic_light_so[m][n].maxim_n;
        //qDebug() << "V_theta: " << v_traffic_light_so[m][n].theta;
    }

    traffic_light_so[n][m].horizontal.changed = true;
    traffic_light_so[n][m].vertical.changed = true;
}

void SwitchBothRedTrafficLightSO(int n, int m)
{
    traffic_light_so[n][m].horizontal.time_u = 0;
    traffic_light_so[n][m].vertical.time_u = 0;

    traffic_light_so[n][m].horizontal.light = 0;//Red
    traffic_light_so[n][m].vertical.light = 0;//Red

    traffic_light_so[n][m].horizontal.n_sum_veh = 0;
    traffic_light_so[n][m].vertical.n_sum_veh = 0;

}

void setSingleRed(char type_street, int n, int m)
{
    if (type_street == 'H'){

        traffic_light_so[n][m].horizontal.time_u = 0;
        traffic_light_so[n][m].horizontal.light = 0;//Red
    }
    else{
        traffic_light_so[n][m].vertical.time_u = 0;
        traffic_light_so[n][m].vertical.light = 0;//Red
    }
}

void setSingleGreen(char type_street, int n, int m)
{
    if (type_street == 'H'){
        traffic_light_so[n][m].horizontal.time_u = 0;
        traffic_light_so[n][m].horizontal.light = 1;//Green
    }
    else{
        traffic_light_so[n][m].vertical.time_u = 0;
        traffic_light_so[n][m].vertical.light = 1;//Green
    }
}

void RestoreSingleGreen(char type_street, int n, int m)
{
    traffic_light_so[n][m].horizontal.time_u = 0;
    traffic_light_so[n][m].vertical.time_u = 0;

    if (type_street == 'H'){
        traffic_light_so[n][m].horizontal.light = 1;//Green
        traffic_light_so[n][m].vertical.light = 0;//Red
    }
    else{
        traffic_light_so[n][m].horizontal.light = 0;//Red
        traffic_light_so[n][m].vertical.light = 1;//Green
    }
}

int GetValueTrafficLightSO(char type_street, int n, int m)
{

    if (n < 0 || m < 0)
        return -1;//no definido

    if (n >= n_hor_streets)
        n = 0;

    if (m >= m_ver_streets)
        m = 0;

    if (type_street == 'H')
        return traffic_light_so[n][m].horizontal.light;
    else
        return traffic_light_so[n][m].vertical.light;

    return -1;
}

int GetPositionIntersectionTrafficLightSO(char type_street, int n, int m)
{
    int pos;

    if (type_street == 'H')
        pos = traffic_light_so[n][m].horizontal.pos_intersection;
    else
        pos = traffic_light_so[n][m].vertical.pos_intersection;

    return pos;
}

int GetPositionTrafficLightSO(char type_street, int n, int m)
{
    int pos;

    if (type_street == 'H')
        pos = traffic_light_so[n][m].horizontal.position;
    else
        pos = traffic_light_so[n][m].vertical.position;

    return pos;
}

//Zapotecatl TrafficLight////////////////////////////////////////////////////////////

bool PressureRule6(int n, int m)
{

    bool stop_h = traffic_light_so[n][m].horizontal.vehicle_stop;
    bool stop_v = traffic_light_so[n][m].vertical.vehicle_stop;

    /* if (stop_h == false &&  n == 1 && m == 0) {
        qDebug() << n << m << stop_h << "avanza";
        qDebug() << n << m << stop_v << "?";

    }*/


    if (stop_h == true && stop_v == true) {
        PressureSetSingleRed('H',n, m);
        PressureSetSingleRed('V',n, m);
        return false;
    }

    if (stop_h == true) {
        PressureSetSingleRed('H',n, m);
        PressureSetSingleGreen('V', n, m);
        return false;
    }

    if (stop_v == true) {
        PressureSetSingleRed('V',n, m);
        PressureSetSingleGreen('H', n, m);
        return false;
    }

    if (traffic_light_so[n][m].horizontal.light == 0 && traffic_light_so[n][m].vertical.light == 0)
        if (traffic_light_so[n][m].horizontal.red_time >= traffic_light_so[n][m].vertical.red_time)
            PressureSetSingleGreen('H', n, m);
        else
            PressureSetSingleGreen('V', n, m);

    return true;

}

bool PressureRule2(int n, int m)
{

    if (traffic_light_so[n][m].horizontal.light == 1)
        if (traffic_light_so[n][m].horizontal.time_u >= traffic_light_so[n][m].horizontal.min_time){
            if (traffic_light_so[n][m].vertical.time_u >= traffic_light_so[n][m].vertical.max_time) {
                SwitchTrafficLightSO(n, m);
                return false;
            }
            return true;
        }

    if (traffic_light_so[n][m].vertical.light == 1)
        if (traffic_light_so[n][m].vertical.time_u >= traffic_light_so[n][m].vertical.min_time){
            if (traffic_light_so[n][m].horizontal.time_u >= traffic_light_so[n][m].horizontal.max_time) {
                SwitchTrafficLightSO(n, m);
                return false;
            }
            return true;
        }

    return false;
}

void PressureRule1(int n, int m)
{

    //qDebug() << "good" << myrandom(1000);

    traffic_light_so[n][m].horizontal.theta_time = 9;//h_sensores[n][m].distance_d + 2;
    traffic_light_so[n][m].vertical.theta_time = 9;//v_sensores[m][n].distance_d + 2;

    if (traffic_light_so[n][m].horizontal.light == 1){
        traffic_light_so[n][m].horizontal.theta =  traffic_light_so[n][m].horizontal.theta_time *  (traffic_light_so[n][m].horizontal.theta_vehicles +  traffic_light_so[n][m].horizontal.z_vehicles);// * (h_traffic_light_so[n][m].theta_vehicles +  h_traffic_light_so[n][m].e_vehicles);

        // if (n == 1 && m == 0)
        //  qDebug() <<  "HT: " << h_traffic_light_so[n][m].theta_time << h_traffic_light_so[n][m].theta_vehicles << h_traffic_light_so[n][m].e_vehicles << h_traffic_light_so[n][m].theta << v_traffic_light_so[m][n].n_sum_veh;

        if (traffic_light_so[n][m].vertical.n_sum_veh > traffic_light_so[n][m].horizontal.theta){
            SwitchTrafficLightSO(n, m);
            return;//Evita entrar en vertical
        }
    }

    if (traffic_light_so[n][m].vertical.light == 1){
        traffic_light_so[n][m].vertical.theta = traffic_light_so[n][m].vertical.theta_time * (traffic_light_so[n][m].vertical.theta_vehicles + traffic_light_so[n][m].vertical.z_vehicles);// * (v_traffic_light_so[m][n].theta_vehicles + v_traffic_light_so[m][n].e_vehicles);

        // if (n == 1 && m == 0)
        //  qDebug() <<  "VT: " << v_traffic_light_so[m][n].theta_time << v_traffic_light_so[m][n].theta_vehicles << v_traffic_light_so[m][n].e_vehicles << v_traffic_light_so[m][n].theta << h_traffic_light_so[n][m].n_sum_veh;

        if (traffic_light_so[n][m].horizontal.n_sum_veh > traffic_light_so[n][m].vertical.theta){
            SwitchTrafficLightSO(n, m);
            return;
        }
    }
}

void TrafficLightPressureSelfOrganizing(int n, int m)
{
    traffic_light_so[n][m].horizontal.changed = false;
    traffic_light_so[n][m].vertical.changed = false;

    // qDebug() << "HP: " << h_traffic_light_so[n][m].p_density << "VP: " << v_traffic_light_so[m][n].p_density;
    // qDebug() << "HT: " << h_traffic_light_so[n][m].theta << "VT: " << v_traffic_light_so[m][n].theta;

    if (PressureRule6(n, m) == true) {
        if (PressureRule2(n, m) == true)
            PressureRule1(n, m);
    }

    traffic_light_so[n][m].horizontal.red_time++;
    traffic_light_so[n][m].vertical.red_time++;
    traffic_light_so[n][m].horizontal.time_u++;
    traffic_light_so[n][m].vertical.time_u++;

}

void PressureSetSingleRed(char type_street, int n, int m)
{
    if (type_street == 'H'){

        if (traffic_light_so[n][m].horizontal.light == 1)
            traffic_light_so[n][m].horizontal.red_time = 0;
        traffic_light_so[n][m].horizontal.light = 0;//Red
    }
    else{

        if (traffic_light_so[n][m].vertical.light == 1)
            traffic_light_so[n][m].vertical.red_time = 0;
        traffic_light_so[n][m].vertical.light = 0;//Red
    }
}

void PressureSetSingleGreen(char type_street, int n, int m)
{
    if (type_street == 'H'){
        traffic_light_so[n][m].horizontal.light = 1;//Green
    }
    else{
        traffic_light_so[n][m].vertical.light = 1;//Green
    }
}


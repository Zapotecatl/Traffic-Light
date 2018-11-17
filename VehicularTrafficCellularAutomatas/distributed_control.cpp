#include "distributed_control.h"

//LeaderData/////////////////////////////////////////////////////////////

bool vehicle_stop_DCH;
bool vehicle_stop_DCV;

SDistributeSystem **distributed_control;

int allocateMemoryDistributedControl()
{
    distributed_control = NULL;

    distributed_control = new SDistributeSystem*[n_hor_streets];
    for (int i = 0; i < n_hor_streets; i++)
        distributed_control[i] = new SDistributeSystem[m_ver_streets];

    return 0;
}


void freeDistributedControl()
{
    if (distributed_control != NULL) {
        for (int i = 0; i < n_hor_streets; i++)
            delete [] distributed_control[i];
        delete [] distributed_control;
        distributed_control = NULL;
    }
}

void InializedDistributedControl(int maxim_n, int maxim_m, int min_time, int max_time)
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

    allocateMemoryDistributedControl();

    for (n = 0; n < n_hor_streets; n++) {
        for (m = 0; m < m_ver_streets; m++) {


            distributed_control[n][m].horizontal.light = 0;
            distributed_control[n][m].vertical.light = 1;

            distributed_control[n][m].horizontal.time_u = 0;
            distributed_control[n][m].vertical.time_u = 0;

            distributed_control[n][m].horizontal.n_sum_veh = 0;
            distributed_control[n][m].vertical.n_sum_veh = 0;

            distributed_control[n][m].horizontal.n_vehicles = 0;
            distributed_control[n][m].vertical.n_vehicles = 0;

            distributed_control[n][m].horizontal.m_vehicles = 0;
            distributed_control[n][m].vertical.m_vehicles = 0;

            distributed_control[n][m].horizontal.vehicle_stop = false;
            distributed_control[n][m].vertical.vehicle_stop = false;

            distributed_control[n][m].horizontal.changed = false;
            distributed_control[n][m].vertical.changed = false;

            distributed_control[n][m].horizontal.vehicle_crosses = false;
            distributed_control[n][m].vertical.vehicle_crosses = false;

            distributed_control[n][m].horizontal.failure_wait = false;
            distributed_control[n][m].vertical.failure_wait = false;


            distributed_control[n][m].horizontal.min_time = min_time;
            distributed_control[n][m].vertical.min_time = min_time;

            distributed_control[n][m].horizontal.max_time = max_time;
            distributed_control[n][m].vertical.max_time = max_time;

            distributed_control[n][m].horizontal.maxim_n = maxim_n;
            distributed_control[n][m].vertical.maxim_n = maxim_n;

            distributed_control[n][m].horizontal.maxim_m = maxim_m;
            distributed_control[n][m].vertical.maxim_m = maxim_m;

            x = m * d_side_block + m;
            y = n * d_side_block + n;

            if (n % 2 == 0) {
                if (x == 0) {
                    distributed_control[n][m].horizontal.position = d_hor_street - 1;
                    distributed_control[n][m].horizontal.pos_intersection = 0;
                }
                else {
                    distributed_control[n][m].horizontal.position = x - 1;
                    distributed_control[n][m].horizontal.pos_intersection = x;
                }
            }
            else {
                distributed_control[n][m].horizontal.position = x + 1;
                distributed_control[n][m].horizontal.pos_intersection = x;
            }

            if (m % 2 == 0) {
                if (y == 0){
                    distributed_control[n][m].vertical.position = d_ver_street - 1;
                    distributed_control[n][m].vertical.pos_intersection = 0;
                }
                else {
                    distributed_control[n][m].vertical.position = y - 1;
                    distributed_control[n][m].vertical.pos_intersection = y;
                }
            }
            else {
                distributed_control[n][m].vertical.position = y + 1;
                distributed_control[n][m].vertical.pos_intersection = y;
            }
        }
    }


}


bool SensorDetectsVehicle(char type_street, char direction, int n, int m, int current_x, int current_rear_x, int previous_x, int velocity, int pos_sensor, int id)
{

    int x, y;


    if (type_street == 'H'){
        x = pos_sensor;


        if (direction == 'R') {
            if ((x >= current_rear_x && x <= current_x) || (x > previous_x && x < current_rear_x)){

                distributed_control[n][m].horizontal.vehicle_crosses = true;
            }
        }
        else {
            if ((x <= current_rear_x && x >= current_x) || (x < previous_x && x > current_rear_x)){
                distributed_control[n][m].horizontal.vehicle_crosses = true;
            }
        }
    }
    else {
        x = pos_sensor;

        if (direction == 'R') {
            if ((x >= current_rear_x && x <= current_x) || (x > previous_x && x < current_rear_x)){
                distributed_control[n][m].vertical.vehicle_crosses = true;
            }
        }
        else {
            if ((x <= current_rear_x && x >= current_x) || (x < previous_x && x > current_rear_x)){
                distributed_control[n][m].vertical.vehicle_crosses = true;
            }
        }
    }


    return false;

}




bool ruleDC6(int n, int m)
{

    if (distributed_control[n][m].horizontal.light == 0 && distributed_control[n][m].vertical.light == 0) {

        if (distributed_control[n][m].horizontal.vehicle_stop == false) {
            RestoreSingleGreenDistributedControl('H', n, m);
            return true;
        }


        if (distributed_control[n][m].vertical.vehicle_stop == false){
            RestoreSingleGreenDistributedControl('V', n, m);
            return true;
        }

        return false;
    }

    return true;
}


bool ruleDC5(int n, int m)
{

    if (distributed_control[n][m].horizontal.vehicle_stop == true){
        setSingleRedDistributedControl('H',n, m);
        vehicle_stop_DCH = true;
    }
    else
        vehicle_stop_DCH = false;

    if (distributed_control[n][m].vertical.vehicle_stop == true){
        setSingleRedDistributedControl('V',n, m);
        vehicle_stop_DCV = true;
    }
    else
        vehicle_stop_DCV = false;

    return true;
}

bool ruleDC4(int n, int m)
{
    if (vehicle_stop_DCV == false){
        if (distributed_control[n][m].horizontal.light == 1)
            if (distributed_control[n][m].vertical.n_vehicles >= 1  && distributed_control[n][m].horizontal.n_vehicles == 0) {
                SwitchDistributedControl(n, m);
                return false;
            }
    }

    if (vehicle_stop_DCH == false){
        if (distributed_control[n][m].vertical.light == 1)
            if (distributed_control[n][m].horizontal.n_vehicles >= 1  && distributed_control[n][m].vertical.n_vehicles == 0) {
                SwitchDistributedControl(n, m);
                return false;
            }
    }

    return true;
}

bool ruleDC3(int n, int m)
{
    if (distributed_control[n][m].horizontal.light == 1)
        if (distributed_control[n][m].horizontal.m_vehicles > 0 && distributed_control[n][m].horizontal.m_vehicles <= distributed_control[n][m].horizontal.maxim_m)
            return false;

    if (distributed_control[n][m].vertical.light == 1)
        if (distributed_control[n][m].vertical.m_vehicles > 0 && distributed_control[n][m].vertical.m_vehicles <= distributed_control[n][m].vertical.maxim_m)
            return false;

    return true;
}

bool ruleDC2(int n, int m)
{
    if (distributed_control[n][m].horizontal.light == 1)
        if (distributed_control[n][m].horizontal.time_u >= distributed_control[n][m].horizontal.min_time){
            if (distributed_control[n][m].vertical.time_u >= distributed_control[n][m].vertical.max_time) {
                SwitchDistributedControl(n, m);
                return false;
            }
            return true;
        }

    if (distributed_control[n][m].vertical.light == 1)
        if (distributed_control[n][m].vertical.time_u >= distributed_control[n][m].vertical.min_time){
            if (distributed_control[n][m].horizontal.time_u >= distributed_control[n][m].horizontal.max_time) {
                SwitchDistributedControl(n, m);
                return false;
            }
            return true;
        }

    return false;
}

void ruleDC1(int n, int m)
{
    if (vehicle_stop_DCV == false)
        if (distributed_control[n][m].horizontal.light == 1)
            if (distributed_control[n][m].vertical.n_sum_veh >= distributed_control[n][m].vertical.maxim_n){
                SwitchDistributedControl(n, m);
                return;//Evita entrar en vertical
            }

    if (vehicle_stop_DCH == false) {
        if (distributed_control[n][m].vertical.light == 1){
            if (distributed_control[n][m].horizontal.n_sum_veh >= distributed_control[n][m].horizontal.maxim_n){
                SwitchDistributedControl(n, m);
                return;
            }
        }
    }
}

void ruleDC1oneByOne(int n, int m)
{

    if (distributed_control[n][m].horizontal.light == 1 && distributed_control[n][m].vertical.failure_wait == true){
        SwitchDistributedControl(n, m);
        return;
    }


    if (distributed_control[n][m].vertical.light == 1 && distributed_control[n][m].horizontal.failure_wait == true){
        SwitchDistributedControl(n, m);
        return;
    }

}


void resetDistributeFailureVehicle()
{

    int n, m;

    for (n = 0; n < n_hor_streets; n++){
        for (m = 0; m < m_ver_streets; m++){
            distributed_control[n][m].horizontal.failure_wait = false;
            distributed_control[n][m].vertical.failure_wait = false;
        }
    }

}

void DistributedSystem(int n, int m)
{

#if 1

    distributed_control[n][m].horizontal.changed = false;
    distributed_control[n][m].vertical.changed = false;

    vehicle_stop_DCH = false;
    vehicle_stop_DCV = false;

    if (ruleDC6(n, m) == true) {
        if (ruleDC5(n, m) == true) {
           // if (ruleDC4(n, m) == true) {
                //  if (ruleDC3(n, m) == true) {
                //if (ruleDC2(n, m) == true) {

                    if (distributed_control[n][m].horizontal.failure_wait == false && distributed_control[n][m].vertical.failure_wait == false)
                        ruleDC1(n, m);
                    else
                        ruleDC1oneByOne(n, m);
                //}
                //  }
           // }
        }
    }

    distributed_control[n][m].horizontal.time_u++;
    distributed_control[n][m].vertical.time_u++;

#else
    //   testRules(n, m);
#endif

}

void SwitchDistributedControl(int n, int m)
{
    if (n < 0)
        n = 0;
    else if (n >= n_hor_streets)
        n = n_hor_streets - 1;

    if (m < 0)
        m = 0;
    else if (m >= m_ver_streets)
        m = m_ver_streets - 1;


    distributed_control[n][m].horizontal.time_u = 0;
    distributed_control[n][m].vertical.time_u = 0;

    if (distributed_control[n][m].horizontal.light == 0){

        distributed_control[n][m].horizontal.light = 1;//Green
        distributed_control[n][m].vertical.light = 0;//Red

        distributed_control[n][m].horizontal.n_sum_veh = 0;
        distributed_control[n][m].vertical.n_sum_veh = 0;

        //distributed_control[n][m].theta =  distributed_control[n][m].p_density *  distributed_control[n][m].maxim_n;
        //qDebug() << "H_theta: " << distributed_control[n][m].theta;
    }
    else {

        distributed_control[n][m].horizontal.light = 0;//Red
        distributed_control[n][m].vertical.light = 1;//Green

        distributed_control[n][m].horizontal.n_sum_veh = 0;
        distributed_control[n][m].vertical.n_sum_veh = 0;

        //distributed_control[m][n].theta =  distributed_control[m][n].p_density *  distributed_control[m][n].maxim_n;
        //qDebug() << "V_theta: " << distributed_control[m][n].theta;
    }

    distributed_control[n][m].horizontal.vehicle_crosses = false;
    distributed_control[n][m].vertical.vehicle_crosses = false;

    distributed_control[n][m].horizontal.changed = true;
    distributed_control[n][m].vertical.changed = true;
}

void SwitchBothDistributedControl(int n, int m)
{
    distributed_control[n][m].horizontal.time_u = 0;
    distributed_control[n][m].vertical.time_u = 0;

    distributed_control[n][m].horizontal.light = 0;//Red
    distributed_control[n][m].vertical.light = 0;//Red

    distributed_control[n][m].horizontal.n_sum_veh = 0;
    distributed_control[n][m].vertical.n_sum_veh = 0;

}

void setSingleRedDistributedControl(char type_street, int n, int m)
{
    if (type_street == 'H'){

        distributed_control[n][m].horizontal.time_u = 0;
        distributed_control[n][m].horizontal.light = 0;//Red
    }
    else{
        distributed_control[n][m].vertical.time_u = 0;
        distributed_control[n][m].vertical.light = 0;//Red
    }
}

void setSingleGreenDistributedControl(char type_street, int n, int m)
{
    if (type_street == 'H'){
        distributed_control[n][m].horizontal.time_u = 0;
        distributed_control[n][m].horizontal.light = 1;//Green
    }
    else{
        distributed_control[n][m].vertical.time_u = 0;
        distributed_control[n][m].vertical.light = 1;//Green
    }
}

void RestoreSingleGreenDistributedControl(char type_street, int n, int m)
{
    distributed_control[n][m].horizontal.time_u = 0;
    distributed_control[n][m].vertical.time_u = 0;

    if (type_street == 'H'){
        distributed_control[n][m].horizontal.light = 1;//Green
        distributed_control[n][m].vertical.light = 0;//Red
    }
    else{
        distributed_control[n][m].horizontal.light = 0;//Red
        distributed_control[n][m].vertical.light = 1;//Green
    }
}

int GetValueDistributedControl(char type_street, int n, int m)
{

    if (n < 0 || m < 0)
        return -1;//no definido

    if (n >= n_hor_streets)
        n = 0;

    if (m >= m_ver_streets)
        m = 0;

    if (type_street == 'H')
        return distributed_control[n][m].horizontal.light;
    else
        return distributed_control[n][m].vertical.light;

    return -1;
}

int GetPositionIntersectionDistributedControl(char type_street, int n, int m)
{
    int pos;

    if (type_street == 'H')
        pos = distributed_control[n][m].horizontal.pos_intersection;
    else
        pos = distributed_control[n][m].vertical.pos_intersection;

    return pos;
}

int GetPositionDistributedControl(char type_street, int n, int m)
{
    int pos;

    if (type_street == 'H')
        pos = distributed_control[n][m].horizontal.position;
    else
        pos = distributed_control[n][m].vertical.position;

    return pos;
}

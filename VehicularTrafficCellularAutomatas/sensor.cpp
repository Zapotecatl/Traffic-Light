#include "sensor.h"

STraditionalSensor **t_sensores;
SDeliberativeSensors **d_sensores;
SDistributedSensor **distributed_sensores;

int *h_regionTraditionalSensors[2];
int *v_regionTraditionalSensors[2];

int *h_regionDeliberativeSensors[2];
int *v_regionDeliberativeSensors[2];

int *h_regionDistributedSensors[2];
int *v_regionDistributedSensors[2];

int stopped_distance;
float precision_sensor;
int metodo_sensado;

#define SPLIT_BLOCK 2

//////////////////////////////////////////////////////////////////////////////

void InializedSensors(int metodo_s, float precision, int distance_d, int distance_r, int distance_e, int distance_z)
{
    //sensores
    metodo_sensado = metodo_s; //1 cerca de interseccion 2 inicio de cuadras
    precision_sensor = precision; //1.0

    if (!(distance_e >= 0 && distance_e <= d_side_block - (ls - 1))){
        qDebug() << "ERROR: distance_e" << __PRETTY_FUNCTION__;
        stopped_distance = 0;
    }
    else
        stopped_distance = distance_e;

    //qDebug() <<  stopped_distance;

    InializedTraditionalSensors(distance_d, distance_r, stopped_distance, distance_z);
    InializedDeliberativeSensors(distance_d, distance_r, stopped_distance, distance_z);
    InializedDistributedSensors(distance_d, distance_r, distance_e, distance_z);

}

void FreeSensors()
{
    freeTraditionalSensors();
    freeDeliberativeSensors();
    freeDistributedSensors();
}


//Traditional sensors ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int allocateMemoryTraditionalSensors()
{
    t_sensores = NULL;

    h_regionTraditionalSensors[0] = NULL;
    h_regionTraditionalSensors[1] = NULL;

    v_regionTraditionalSensors[0] = NULL;
    v_regionTraditionalSensors[1] = NULL;

    t_sensores = new STraditionalSensor*[n_hor_streets];
    for (int i = 0; i < n_hor_streets; i++)
        t_sensores[i] = new STraditionalSensor[m_ver_streets];

    h_regionTraditionalSensors[0] = new int[d_hor_street];
    h_regionTraditionalSensors[1] = new int[d_hor_street];

    v_regionTraditionalSensors[0] = new int[d_ver_street];
    v_regionTraditionalSensors[1] = new int[d_ver_street];

    return 0;
}

void freeTraditionalSensors()
{
    if (v_regionTraditionalSensors[0] != NULL){
        delete [] v_regionTraditionalSensors[0];
        v_regionTraditionalSensors[0] = NULL;
    }

    if (v_regionTraditionalSensors[1] != NULL){
        delete [] v_regionTraditionalSensors[1];
        v_regionTraditionalSensors[1] = NULL;
    }

    if (h_regionTraditionalSensors[0] != NULL){
        delete [] h_regionTraditionalSensors[0];
        h_regionTraditionalSensors[0] = NULL;
    }

    if (h_regionTraditionalSensors[1] != NULL){
        delete [] h_regionTraditionalSensors[1];
        h_regionTraditionalSensors[1] = NULL;
    }

    if (t_sensores != NULL){
        for (int i = 0; i < n_hor_streets; i++)
            delete [] t_sensores[i];
        delete [] t_sensores;
        t_sensores = NULL;
    }

}

void regionTraditionalSensores()
{

    //Create region for each traffic light
    int k, i, m, n;

    for (k = 0; k < 2; k++){

        for (i = 0; i < d_hor_street; i++)
            h_regionTraditionalSensors[k][i] = -1;

        for (i = 0; i < d_ver_street; i++)
            v_regionTraditionalSensors[k][i] = -1;
    }

    int limit_n = n_hor_streets > 1 ? 2 : 1;

    for (n = 0; n < limit_n; n++){
        for (m = 0; m < m_ver_streets; m++){

            if (t_sensores[n][m].horizontal.direction == 'R') {

                if (m == 0) {
                    for (i = t_sensores[n][m].horizontal.start; i < d_hor_street; i++)
                        h_regionTraditionalSensors[n][i] = m;

                    for (i = 0; i <= t_sensores[n][m].horizontal.end; i++)
                        h_regionTraditionalSensors[n][i] = m;
                }
                else {
                    for (i = t_sensores[n][m].horizontal.start; i <= t_sensores[n][m].horizontal.end; i++)
                        h_regionTraditionalSensors[n][i] = m;
                }
            }
            else {

                if (m == 0) {

                    for (i = t_sensores[n][m].horizontal.start; i >= 0; i--)
                        h_regionTraditionalSensors[n][i] = m;

                    for (i =  d_hor_street - 1; i >= t_sensores[n][m].horizontal.end; i--)
                        h_regionTraditionalSensors[n][i] = m;

                }
                else {
                    for (i = t_sensores[n][m].horizontal.start; i >= t_sensores[n][m].horizontal.end; i--)
                        h_regionTraditionalSensors[n][i] = m;
                }
            }
        }
    }

    int limit_m = m_ver_streets > 1 ? 2 : 1;

    for (m = 0; m < limit_m; m++) {
        for (n = 0; n < n_hor_streets; n++) {

            if (t_sensores[n][m].vertical.direction == 'R') {

                if (n == 0) {

                    for (i = t_sensores[n][m].vertical.start; i < d_ver_street; i++)
                        v_regionTraditionalSensors[m][i] = n;

                    for (i = 0; i <= t_sensores[n][m].vertical.end; i++)
                        v_regionTraditionalSensors[m][i] = n;
                }
                else {
                    for (i = t_sensores[n][m].vertical.start; i <= t_sensores[n][m].vertical.end; i++)
                        v_regionTraditionalSensors[m][i] = n;
                }
            }
            else {

                if (n == 0) {

                    for (i = t_sensores[n][m].vertical.start; i >= 0; i--)
                        v_regionTraditionalSensors[m][i] = n;


                    for (i = d_ver_street - 1; i >= t_sensores[n][m].vertical.end; i--)
                        v_regionTraditionalSensors[m][i] = n;

                }
                else {
                    for (i = t_sensores[n][m].vertical.start; i >= t_sensores[n][m].vertical.end; i--)
                        v_regionTraditionalSensors[m][i] = n;
                }
            }
        }
    }



    /*
    for (k = 0; k < 2; k++){
       for (i = 0; i < d_hor_street; i++)
            qDebug() << h_regionSensoresT[k][i];
         qDebug() << endl;

        for (i = 0; i < d_ver_street; i++)
            qDebug() << v_regionSensoresT[k][i];

        qDebug() << endl;

    }
    */

}



int GetIndexTraditionalSensors(char type_street, char direction, int x)
{
    if (type_street == 'H') {
        if (direction == 'R')
            return h_regionTraditionalSensors[0][x];
        else
            return h_regionTraditionalSensors[1][x];
    }
    else {
        if (direction == 'R')
            return v_regionTraditionalSensors[0][x];
        else
            return v_regionTraditionalSensors[1][x];
    }

    return INVALID;//INVALID significa que no esta en un area de sensado
}

void traditionalSensing(int n, int m)
{
    int x;
    char direction;
    int value;
    int pos_t;
    int distance_e;
    int distance_d;
    int distance_r;
    int distance_z;

    //Horizontales ///////////////////////////////////////////////////////////////////////////
    traffic_light_so[n][m].horizontal.n_vehicles = 0;
    traffic_light_so[n][m].horizontal.m_vehicles = 0;

    direction = (n % 2) == 0 ? 'R' : 'L';

    pos_t = t_sensores[n][m].horizontal.position;

    distance_d = t_sensores[n][m].horizontal.distance_d;
    distance_r = t_sensores[n][m].horizontal.distance_r;
    distance_e = t_sensores[n][m].horizontal.distance_e;
    distance_z = t_sensores[n][m].horizontal.distance_z;

    traffic_light_so[n][m].horizontal.vehicle_stop = VehiclesStoppedDistance_e('H', n, m, distance_e);
    traffic_light_so[n][m].horizontal.z_vehicles = numberVehiclesDistance_z('H', n, m, distance_z);

    //if (n == 0 && m == 0)
    //qDebug() << "EH: " << traffic_light_so[n][m].horizontal.e_vehicles;

    if (direction == 'R'){

        for (x = (pos_t - distance_d) + 1; x <= pos_t; x++){
            //Regla 1 y 3
            value = 0;
            if (GetValueCellStreet('H', n, x) == 1) //Front vehicle
                if (GetVisibleCellStreet('H', n, x) == true)
                    value = 1;

            traffic_light_so[n][m].horizontal.n_vehicles+=value;

            if (x > pos_t - distance_r)
                traffic_light_so[n][m].horizontal.m_vehicles+= value;
        }

        traffic_light_so[n][m].horizontal.n_sum_veh+= traffic_light_so[n][m].horizontal.n_vehicles;
        traffic_light_so[n][m].horizontal.d_vehicles = traffic_light_so[n][m].horizontal.n_vehicles;
    }
    else{

        for (x = (pos_t + distance_d) - 1; x >= pos_t; x--){
            //Regla 1 y 3
            value = 0;
            if (GetValueCellStreet('H', n, x) == 1) //Front vehicle
                if (GetVisibleCellStreet('H', n, x) == true)
                    value = 1;

            traffic_light_so[n][m].horizontal.n_vehicles+=value;

            if (x <= pos_t + distance_r)
                traffic_light_so[n][m].horizontal.m_vehicles+= value;
        }

        traffic_light_so[n][m].horizontal.n_sum_veh+= traffic_light_so[n][m].horizontal.n_vehicles;
        traffic_light_so[n][m].horizontal.d_vehicles = traffic_light_so[n][m].horizontal.n_vehicles;
    }

    //Verticales ///////////////////////////////////////////////////////////////////////////

    traffic_light_so[n][m].vertical.n_vehicles = 0;
    traffic_light_so[n][m].vertical.m_vehicles = 0;

    direction = (m % 2) == 0 ? 'R' : 'L';

    pos_t = t_sensores[n][m].vertical.position;

    distance_d = t_sensores[n][m].vertical.distance_d;
    distance_r = t_sensores[n][m].vertical.distance_r;
    distance_e = t_sensores[n][m].vertical.distance_e;
    distance_z = t_sensores[n][m].vertical.distance_z;

    traffic_light_so[n][m].vertical.vehicle_stop = VehiclesStoppedDistance_e('V', n, m, distance_e);
    traffic_light_so[n][m].vertical.z_vehicles = numberVehiclesDistance_z('V', n, m, distance_z);

    //qDebug() << "EV: " << v_traffic_light_so[m][n].e_vehicles;

    if (direction == 'R'){

        for (x = (pos_t - distance_d) + 1; x <= pos_t; x++){
            //Regla 1 y 3

            value = 0;
            if (GetValueCellStreet('V', m, x) == 1)//Front vehicle
                if (GetVisibleCellStreet('V', m, x) == true)
                    value = 1;

            traffic_light_so[n][m].vertical.n_vehicles+=value;

            if (x > pos_t - distance_r)
                traffic_light_so[n][m].vertical.m_vehicles+= value;
        }

        //qDebug() << traffic_light_so[n][m].vertical.n_vehicles;

        traffic_light_so[n][m].vertical.n_sum_veh+= traffic_light_so[n][m].vertical.n_vehicles;
        traffic_light_so[n][m].vertical.d_vehicles = traffic_light_so[n][m].vertical.n_vehicles;

    }
    else{

        for (x = (pos_t + distance_d) - 1; x >= pos_t; x--){
            //Regla 1 y 3
            value = 0;
            if (GetValueCellStreet('V', m, x) == 1)//Front vehicle
                if (GetVisibleCellStreet('V', m, x) == true)
                    value = 1;

            traffic_light_so[n][m].vertical.n_vehicles+=value;

            if (x <= pos_t + distance_r)
                traffic_light_so[n][m].vertical.m_vehicles+= value;
        }
        traffic_light_so[n][m].vertical.n_sum_veh+= traffic_light_so[n][m].vertical.n_vehicles;
        traffic_light_so[n][m].vertical.d_vehicles = traffic_light_so[n][m].vertical.n_vehicles;
    }
}

void InializedTraditionalSensors(int distance_d, int distance_r, int distance_e, int distance_z)
{
    int n, m;
    int x;
    int y;

    if (!(distance_d >= 0 && distance_d <= d_side_block)) {
        qDebug() << "ERROR: d_side_block" << __PRETTY_FUNCTION__;
        distance_d = d_side_block;
    }

    if (!(distance_r >= 0 && distance_r <= distance_d)) {
        qDebug() << "ERROR: distance_r" << __PRETTY_FUNCTION__;
        distance_r = distance_d;
    }

    if (!(distance_e >= 0 && distance_e <= d_side_block - (ls - 1))){
        qDebug() << "ERROR: distance_e" << __PRETTY_FUNCTION__;
        //distance_e = d_side_block - (ls - 1);
        distance_e = 0;
    }

    if (!(distance_z >= 0 && distance_z <= distance_e)){
        qDebug() << "ERROR: distance_z" << __PRETTY_FUNCTION__;
        distance_z = distance_e;
    }

    allocateMemoryTraditionalSensors();


    for (n = 0; n < n_hor_streets; n++){
        for (m = 0; m < m_ver_streets; m++){

            x = m * d_side_block + m;
            y = n * d_side_block + n;

            t_sensores[n][m].horizontal.position = GetPositionTrafficLightSO('H', n, m);
            t_sensores[n][m].vertical.position = GetPositionTrafficLightSO('V', n, m);

            t_sensores[n][m].horizontal.distance_d = distance_d;
            t_sensores[n][m].vertical.distance_d = distance_d;

            t_sensores[n][m].horizontal.distance_r = distance_r;
            t_sensores[n][m].vertical.distance_r = distance_r;

            t_sensores[n][m].horizontal.distance_e = distance_e;
            t_sensores[n][m].vertical.distance_e = distance_e;

            t_sensores[n][m].horizontal.distance_z = distance_z;
            t_sensores[n][m].vertical.distance_z = distance_z;


            if (n % 2 == 0) {

                t_sensores[n][m].horizontal.direction = 'R';

                if (x == 0) {
                    t_sensores[n][m].horizontal.start = d_hor_street - distance_d;
                    t_sensores[n][m].horizontal.end = distance_e;
                }
                else {
                    t_sensores[n][m].horizontal.start = x - distance_d;
                    t_sensores[n][m].horizontal.end = x + distance_e;
                }
            }
            else {

                t_sensores[n][m].horizontal.direction = 'L';

                if (x == 0) {
                    t_sensores[n][m].horizontal.start = distance_d;
                    t_sensores[n][m].horizontal.end = d_hor_street - distance_e;
                }
                else {
                    t_sensores[n][m].horizontal.start = x + distance_d;
                    t_sensores[n][m].horizontal.end = x - distance_e;
                }
            }


            if (m % 2 == 0) {

                t_sensores[n][m].vertical.direction = 'R';

                if (y == 0) {
                    t_sensores[n][m].vertical.start = d_ver_street - distance_d;
                    t_sensores[n][m].vertical.end = distance_e;
                }
                else {
                    t_sensores[n][m].vertical.start = y - distance_d;
                    t_sensores[n][m].vertical.end = y + distance_e;
                }
            }
            else {

                t_sensores[n][m].vertical.direction = 'L';

                if (y == 0) {
                    t_sensores[n][m].vertical.start = distance_d;
                    t_sensores[n][m].vertical.end = d_ver_street - distance_e;
                }
                else {
                    t_sensores[n][m].vertical.start = y + distance_d;
                    t_sensores[n][m].vertical.end = y - distance_e;
                }
            }
        }
    }

    regionTraditionalSensores();

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool determineVisible(char type_street, char direction, int x, int n, int m, bool prev_visible)
{

    bool visible = true;
    int k_intersection;

    if (metodo_sensado == 1) {
        k_intersection = GetIndexTraditionalSensors(type_street, direction, x);

        if (k_intersection != INVALID) {//esta en zona de sensado

            if (prev_visible == false)
                visible = false;
            else {
                if (frand() <= precision_sensor)
                    visible = true;
                else
                    visible = false;
            }
        }
    }
    else {

        int position = GetPositionSensedSensor(type_street, n, m);
        if(position != INVALID){
            if (prev_visible == false)
                visible = false;
            else {
                if (frand() <= precision_sensor)
                    visible = true;
                else
                    visible = false;
            }
        }

        SetVisibleSensedVehicle(type_street, n, m, visible);
    }

    return visible;
}

//Deliberative //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int allocateMemorySensors()
{
    int i, m, n;

    d_sensores = NULL;
    //Sensores //////////////////////////////////////////////////////////////
    d_sensores = new SDeliberativeSensors*[n_hor_streets];
    for (i = 0; i < n_hor_streets; i++)
        d_sensores[i] = new SDeliberativeSensors[m_ver_streets];

    for (n = 0; n < n_hor_streets; n++) {
        for (m = 0; m < m_ver_streets; m++) {
            d_sensores[n][m].horizontal.pr_street = NULL;
            d_sensores[n][m].horizontal.pw_street = NULL;
            d_sensores[n][m].vertical.pr_street = NULL;
            d_sensores[n][m].vertical.pw_street = NULL;

            d_sensores[n][m].horizontal.virtualCell_A = NULL;
            d_sensores[n][m].horizontal.virtualCell_Z = NULL;
            d_sensores[n][m].vertical.virtualCell_A = NULL;
            d_sensores[n][m].vertical.virtualCell_Z = NULL;

            d_sensores[n][m].horizontal.virtualCell_A = new SVirtualCell[d_side_block + 1];
            d_sensores[n][m].horizontal.virtualCell_Z = new SVirtualCell[d_side_block + 1];
            d_sensores[n][m].vertical.virtualCell_A = new SVirtualCell[d_side_block + 1];
            d_sensores[n][m].vertical.virtualCell_Z = new SVirtualCell[d_side_block + 1];
        }
    }

    h_regionDeliberativeSensors[0] = NULL;
    h_regionDeliberativeSensors[1] = NULL;

    v_regionDeliberativeSensors[0] = NULL;
    v_regionDeliberativeSensors[1] = NULL;

    h_regionDeliberativeSensors[0] = new int[d_hor_street];
    h_regionDeliberativeSensors[1] = new int[d_hor_street];

    v_regionDeliberativeSensors[0] = new int[d_ver_street];
    v_regionDeliberativeSensors[1] = new int[d_ver_street];

    return 0;

}

void freeDeliberativeSensors()
{
    int i, n, m;

    if (v_regionDeliberativeSensors[0] != NULL) {
        delete [] v_regionDeliberativeSensors[0];
        v_regionDeliberativeSensors[0] = NULL;
    }

    if (v_regionDeliberativeSensors[1] != NULL) {
        delete [] v_regionDeliberativeSensors[1];
        v_regionDeliberativeSensors[1] = NULL;
    }

    if (h_regionDeliberativeSensors[0] != NULL) {
        delete [] h_regionDeliberativeSensors[0];
        h_regionDeliberativeSensors[0] = NULL;
    }

    if (h_regionDeliberativeSensors[1] != NULL) {
        delete [] h_regionDeliberativeSensors[1];
        h_regionDeliberativeSensors[1] = NULL;
    }

    if (d_sensores != NULL) {
        for (n = 0; n < n_hor_streets; n++) {
            for (m = 0; m < m_ver_streets; m++) {

                if (d_sensores[n][m].vertical.virtualCell_Z != NULL) {
                    delete [] d_sensores[n][m].vertical.virtualCell_Z;
                    d_sensores[n][m].vertical.virtualCell_Z = NULL;
                }

                if (d_sensores[n][m].vertical.virtualCell_A != NULL) {
                    delete [] d_sensores[n][m].vertical.virtualCell_A;
                    d_sensores[n][m].vertical.virtualCell_A = NULL;
                }

                if (d_sensores[n][m].horizontal.virtualCell_Z != NULL) {
                    delete [] d_sensores[n][m].horizontal.virtualCell_Z;
                    d_sensores[n][m].horizontal.virtualCell_Z = NULL;
                }

                if (d_sensores[n][m].horizontal.virtualCell_A != NULL) {
                    delete [] d_sensores[n][m].horizontal.virtualCell_A;
                    d_sensores[n][m].horizontal.virtualCell_A = NULL;
                }

                d_sensores[n][m].horizontal.pr_street = NULL;
                d_sensores[n][m].horizontal.pw_street = NULL;
                d_sensores[n][m].vertical.pr_street = NULL;
                d_sensores[n][m].vertical.pw_street = NULL;

            }
        }

        for (i = 0; i < n_hor_streets; i++)
            delete [] d_sensores[i];
        delete [] d_sensores;
        d_sensores = NULL;
    }

}

void regionDeliberativeSensores()
{

    //Create region for each traffic light
    int k, i, m, n;

    for (k = 0; k < 2; k++){

        for (i = 0; i < d_hor_street; i++)
            h_regionDeliberativeSensors[k][i] = -1;

        for (i = 0; i < d_ver_street; i++)
            v_regionDeliberativeSensors[k][i] = -1;
    }

    int limit_n = n_hor_streets > 1 ? 2 : 1;

    for (n = 0; n < limit_n; n++)
        for (m = 0; m < m_ver_streets; m++){
            h_regionDeliberativeSensors[n][d_sensores[n][m].horizontal.position] = m;
        }

    int limit_m = m_ver_streets > 1 ? 2 : 1;

    for (m = 0; m < limit_m; m++)
        for (n = 0; n < n_hor_streets; n++){
            v_regionDeliberativeSensors[m][d_sensores[n][m].vertical.position] = n;
        }

    /*

    for (k = 0; k < 2; k++){

      //for (i = 0; i < d_hor_street; i++)
        //   qDebug() << h_regionSensoresNew[k][i];

        //qDebug() << endl;

        for (i = 0; i < d_ver_street; i++)
            qDebug() << v_regionSensoresNew[k][i];

        qDebug() << endl;

    }*/
}

int GetIndexDeliberativeSensors(char type_street, int direction, int x)
{

    if (type_street == 'H') {

        if (direction == 'R')
            return h_regionDeliberativeSensors[0][x];
        else
            return h_regionDeliberativeSensors[1][x];
    }
    else {

        if (direction == 'R')
            return v_regionDeliberativeSensors[0][x];
        else
            return v_regionDeliberativeSensors[1][x];
    }

    return INVALID;//INVALID significa que no esta en un area de sensado

}

bool isThereVehicle(char type_street, int y, int x, int n, int m)
{

    /*  if (GetValueCellStreet(type_street, y, x) >= 1 && GetVisibleCellStreet(type_street, y, x) == true){//Cuerpo del vehiculo detectado en la zona de deteccion
        if (GetVisibleSensedVehicle(type_street, n, m) == true)//Simula errores de sensado
            return true;

    }else */

    if (GetPositionSensedSensor(type_street, n, m) != INVALID){//Vehiculo detectado aunque no este en la zona de deteccion
        if (GetVisibleSensedVehicle(type_street, n, m) == true)//Simula errores de sensado
            return true;
    }

    return false;
}

bool isThereVirtualVehicle(char type_street, int n, int m, int x)
{
    if (GetVirtualCellValue(type_street, n, m, x) >= 1)
        return true;

    return false;
}



void deliberativeSensing(int n, int m)
{

    int x;
    int y;
    bool light_change;
    int xl;

    Package1 pack1;
    Package2 pack2;
    Package3 pack3;

    SResults results;
    SResults2 results2;

    //Horizontal /////////////////////////////////////////////////////////////////////////////////////////////////

    pack1.light_state = 0;
    pack1.reived_front = 0;
    pack1.stop_front = false;

    pack2.m = 0;
    pack2.n = 0;
    pack2.n_z = 0;
    pack2.stop = false;

    pack3.mystop = false;
    pack3.received = 0;

    results.arrived_sended = false;
    results.received_stop = false;
    results.sent_stop = false;

    results2.m = 0;
    results2.n = 0;
    results2.n_z = 0;

    int m_tmp;

    m_tmp = IndexFrontSensor('H', n, m);
    x = GetPositionSensor('H', n, m);
    y = n;

    d_sensores[n][m].horizontal.my_stop = false;
    d_sensores[n][m].horizontal.stop_front = false;

    bool vehicle_state_h = false;
    bool virtual_state_h = false;

    vehicle_state_h = isThereVehicle('H', y, x, n, m);
    if (vehicle_state_h == true) {
        xl = localVirtualVehiclePosition('H', d_sensores[n][m].horizontal.direction, n, m, d_sensores[n][m].horizontal.sensed_vehicle.position);
        virtual_state_h = isThereVirtualVehicle('H', n, m, xl);

        if (virtual_state_h == false) {

            if (d_sensores[n][m].horizontal.sensed_vehicle.position != INVALID) {
                VirtualVehicle veh;
                veh.position = d_sensores[n][m].horizontal.sensed_vehicle.position;
                veh.velocity = d_sensores[n][m].horizontal.sensed_vehicle.velocity;
                veh.direction = d_sensores[n][m].horizontal.sensed_vehicle.direction;
                veh.id = d_sensores[n][m].horizontal.sensed_vehicle.id;

                d_sensores[n][m].horizontal.vehicles_read.push_back(veh);
            }
        }

        bool vehicle_stop = GetVehiclesStopped('H', y, x);

        if (vehicle_stop == true) {
            d_sensores[n][m].horizontal.my_stop = true;
        }
        else
            d_sensores[n][m].horizontal.received++;
    }

    pack1 = ReceivesMessageFrontCT('H', n, m, m_tmp);

    d_sensores[n][m].horizontal.state_light = pack1.light_state;
    d_sensores[n][m].horizontal.stop_front = pack1.stop_front;

    results = SimulateVirtualEnvironment('H', n, m);

    if (results.arrived_sended == true)
        d_sensores[n][m].horizontal.sended++;

    if (results.received_stop == true)
        d_sensores[n][m].horizontal.my_stop = true;

    if (results.sent_stop == true)
        d_sensores[n][m].horizontal.stop_front = true;

    //Correction in the count of the senden and received
    d_sensores[n][m].horizontal.epsilon = 0;
    if (d_sensores[n][m].horizontal.state_light == 1 && traffic_light_so[n][m].horizontal.changed == true) {
        d_sensores[n][m].horizontal.epsilon =  abs(d_sensores[n][m_tmp].horizontal.received - d_sensores[n][m].horizontal.sended);
        d_sensores[n][m].horizontal.sended = 0;
    }

    results2 = countVirtualEnvironment('H', n, m);

    pack2.n = results2.n + d_sensores[n][m].horizontal.epsilon;
    pack2.n_z = results2.n_z;
    pack2.m = results2.m;
    pack2.stop = d_sensores[n][m].horizontal.stop_front;

    SendMessageFrontCT('H', pack2, n, m);

    pack3.mystop = d_sensores[n][m].horizontal.my_stop;
    pack3.received =  d_sensores[n][m].horizontal.received;

    SendMessageBackCT('H', pack3, n, m);

    light_change = ReceivesMessageBackCT('H', n, m);

    //Para hacer la implementacion mas sencilla con los datos que se tiene se realiza a la inversa
    if (light_change == true)
        d_sensores[n][m_tmp].horizontal.received = 0;


    //Vertical /////////////////////////////////////////////////////////////////////////////////////////////////
    pack1.light_state = 0;
    pack1.reived_front = 0;
    pack1.stop_front = false;

    pack2.m = 0;
    pack2.n = 0;
    pack2.n_z = 0;
    pack2.stop = false;

    pack3.mystop = false;
    pack3.received = 0;

    results.arrived_sended = false;
    results.received_stop = false;
    results.sent_stop = false;

    results2.m = 0;
    results2.n = 0;
    results2.n_z = 0;

    int n_tmp;

    n_tmp = IndexFrontSensor('V', n, m);
    x = GetPositionSensor('V', n, m);
    y = m;

    d_sensores[n][m].vertical.my_stop = false;
    d_sensores[n][m].vertical.stop_front = false;

    bool vehicle_state_v = false;
    bool virtual_state_v = false;

    vehicle_state_v = isThereVehicle('V', y, x, n, m);
    if (vehicle_state_v == true) {
        xl = localVirtualVehiclePosition('V', d_sensores[n][m].vertical.direction, n, m, d_sensores[n][m].vertical.sensed_vehicle.position);
        virtual_state_v = isThereVirtualVehicle('V', n, m, xl);
        if (virtual_state_v == false) {

            if (d_sensores[n][m].vertical.sensed_vehicle.position != INVALID) {
                VirtualVehicle veh;
                veh.position = d_sensores[n][m].vertical.sensed_vehicle.position;
                veh.velocity = d_sensores[n][m].vertical.sensed_vehicle.velocity;
                veh.direction = d_sensores[n][m].vertical.sensed_vehicle.direction;
                veh.id = d_sensores[n][m].vertical.sensed_vehicle.id;

                d_sensores[n][m].vertical.vehicles_read.push_back(veh);
            }
        }

        bool vehicle_stop = GetVehiclesStopped('V', y, x);

        if (vehicle_stop == true)
            d_sensores[n][m].vertical.my_stop = true;
        else
            d_sensores[n][m].vertical.received++;
    }

    pack1 = ReceivesMessageFrontCT('V', n, m, n_tmp);

    d_sensores[n][m].vertical.state_light = pack1.light_state;
    d_sensores[n][m].vertical.stop_front = pack1.stop_front;

    results = SimulateVirtualEnvironment('V', n, m);

    if (results.arrived_sended == true)
        d_sensores[n][m].vertical.sended++;

    if (results.received_stop == true)
        d_sensores[n][m].vertical.my_stop = true;

    if (results.sent_stop == true)
        d_sensores[n][m].vertical.stop_front = true;

    //Correction in the count of the senden and received
    d_sensores[n][m].vertical.epsilon = 0;
    if (d_sensores[n][m].vertical.state_light == 1 &&  traffic_light_so[n][m].vertical.changed == true) {
        d_sensores[n][m].vertical.epsilon =  abs(d_sensores[n_tmp][m].vertical.received - d_sensores[n][m].vertical.sended);
        d_sensores[n][m].vertical.sended = 0;
    }

    results2 = countVirtualEnvironment('V', n, m);

    pack2.n = results2.n + d_sensores[n][m].vertical.epsilon;
    pack2.n_z = results2.n_z;
    pack2.m = results2.m;
    pack2.stop = d_sensores[n][m].vertical.stop_front;

    SendMessageFrontCT('V', pack2, n, m);

    pack3.mystop = d_sensores[n][m].vertical.my_stop;
    pack3.received =  d_sensores[n][m].vertical.received;

    SendMessageBackCT('V', pack3, n, m);

    light_change = ReceivesMessageBackCT('V', n, m);

    //Para hacer la implementacion mas sencilla con los datos que se tiene se realiza a la inversa
    if (light_change == true)
        d_sensores[n_tmp][m].vertical.received = 0;

}

void InializedDeliberativeSensors(int distance_d, int distance_r, int distance_e, int distance_z)
{
    int n, m;
    int x;
    int y;

    if (!(distance_d >= 0 && distance_d <= d_side_block)) {
        qDebug() << "ERROR: d_side_block" << __PRETTY_FUNCTION__;
        distance_d = d_side_block;
    }

    if (!(distance_r >= 0 && distance_r <= distance_d)) {
        qDebug() << "ERROR: distance_r" << __PRETTY_FUNCTION__;
        distance_r = distance_d;
    }

    if (!(distance_e >= 0 && distance_e <= d_side_block - (ls - 1))){
        qDebug() << "ERROR: distance_e" << __PRETTY_FUNCTION__;
        //distance_e = d_side_block - (ls - 1);
        distance_e = 0;
    }

    if (!(distance_z >= 0 && distance_z <= distance_e)){
        qDebug() << "ERROR: distance_z" << __PRETTY_FUNCTION__;
        distance_z = distance_e;
    }

    //qDebug() << distance_z;

    allocateMemorySensors();

    for (n = 0; n < n_hor_streets; n++) {
        for (m = 0; m < m_ver_streets; m++) {

            SwitchVirtualCellRW('H', n, m);
            resetVirtualWrite('H', n, m);
            SwitchVirtualCellRW('H', n, m);
            resetVirtualWrite('H', n, m);

            SwitchVirtualCellRW('V', n, m);
            resetVirtualWrite('V', n, m);
            SwitchVirtualCellRW('V', n, m);
            resetVirtualWrite('V', n, m);
        }
    }


    for (n = 0; n < n_hor_streets; n++){
        for (m = 0; m < m_ver_streets; m++){

            x = m * d_side_block + m;
            y = n * d_side_block + n;

            d_sensores[n][m].horizontal.state_light = traffic_light_so[n][m].horizontal.light;
            d_sensores[n][m].vertical.state_light = traffic_light_so[n][m].vertical.light;

            d_sensores[n][m].horizontal.p_traffic_light = traffic_light_so[n][m].horizontal.position;
            d_sensores[n][m].vertical.p_traffic_light = traffic_light_so[n][m].vertical.position;

            d_sensores[n][m].horizontal.received = 0;
            d_sensores[n][m].vertical.received = 0;

            d_sensores[n][m].horizontal.sended = 0;
            d_sensores[n][m].vertical.sended = 0;

            d_sensores[n][m].horizontal.switch_virtual_cell = false;
            d_sensores[n][m].vertical.switch_virtual_cell = false;

            d_sensores[n][m].horizontal.stop_front = false;
            d_sensores[n][m].vertical.stop_front = false;

            d_sensores[n][m].horizontal.my_stop = false;
            d_sensores[n][m].vertical.my_stop = false;

            d_sensores[n][m].horizontal.stop_sended = false;
            d_sensores[n][m].vertical.stop_sended = false;

            d_sensores[n][m].horizontal.epsilon = 0;
            d_sensores[n][m].vertical.epsilon = 0;

            d_sensores[n][m].horizontal.distance_d = distance_d;
            d_sensores[n][m].vertical.distance_d = distance_d;

            d_sensores[n][m].horizontal.distance_r = distance_r;
            d_sensores[n][m].vertical.distance_r = distance_r;

            d_sensores[n][m].horizontal.distance_e = distance_e;
            d_sensores[n][m].vertical.distance_e = distance_e;

            d_sensores[n][m].horizontal.distance_z = distance_z;
            d_sensores[n][m].vertical.distance_z = distance_z;

            d_sensores[n][m].horizontal.sensed_vehicle.velocity = INVALID;
            d_sensores[n][m].horizontal.sensed_vehicle.position = INVALID;
            d_sensores[n][m].horizontal.sensed_vehicle.id = INVALID;

            d_sensores[n][m].vertical.sensed_vehicle.velocity = INVALID;
            d_sensores[n][m].vertical.sensed_vehicle.position = INVALID;
            d_sensores[n][m].vertical.sensed_vehicle.id = INVALID;


            if (n % 2 == 0) {

                d_sensores[n][m].horizontal.direction = 'R';

                if (x == 0)
                    d_sensores[n][m].horizontal.position = d_hor_street - d_side_block + distance_e;
                else
                    d_sensores[n][m].horizontal.position = x - d_side_block + distance_e;
            }
            else {

                d_sensores[n][m].horizontal.direction = 'L';
                d_sensores[n][m].horizontal.position = x + d_side_block - distance_e;
            }


            if (m % 2 == 0) {

                d_sensores[n][m].vertical.direction = 'R';

                if (y == 0)
                    d_sensores[n][m].vertical.position = d_ver_street - d_side_block + distance_e;
                else
                    d_sensores[n][m].vertical.position = y - d_side_block + distance_e;
            }
            else {

                d_sensores[n][m].vertical.direction = 'L';
                d_sensores[n][m].vertical.position = y + d_side_block - distance_e;
            }

            FillVehicles(n, m, x, y);
        }
    }

    for (n = 0; n < n_hor_streets; n++){
        for (m = 0; m < m_ver_streets; m++){

            int m_tmp = IndexFrontSensor('H', n, m);
            d_sensores[n][m].horizontal.p_sensor_f = d_sensores[n][m_tmp].horizontal.position;//Interseccion y distanica e comienza des de 0

            int n_tmp = IndexFrontSensor('V', n, m);
            d_sensores[n][m].vertical.p_sensor_f = d_sensores[n_tmp][m].vertical.position;//Interseccion y distanica e comienza des de 0
        }
    }

    regionDeliberativeSensores();
}



Package1 ReceivesMessageFrontCT(char type_street, int n, int m, int tmp)
{
    Package1 pack1;

    pack1.light_state = 0;
    pack1.reived_front = 0;
    pack1.stop_front = false;

    if (type_street == 'H') {

        pack1.light_state = traffic_light_so[n][m].horizontal.light;
        pack1.stop_front = d_sensores[n][tmp].horizontal.my_stop;
        pack1.reived_front = d_sensores[n][tmp].horizontal.received;

    }
    else
    {
        pack1.light_state = traffic_light_so[n][m].vertical.light;
        pack1.stop_front = d_sensores[tmp][m].vertical.my_stop;
        pack1.reived_front = d_sensores[tmp][m].vertical.received;

    }

    return pack1;
}

void SendMessageFrontCT(char type_street, Package2 &package2, int n, int m)
{

    if (type_street == 'H') {

        traffic_light_so[n][m].horizontal.n_sum_veh+= package2.n;
        traffic_light_so[n][m].horizontal.n_vehicles = package2.n;
        traffic_light_so[n][m].horizontal.m_vehicles = package2.m;
        traffic_light_so[n][m].horizontal.vehicle_stop = package2.stop;

        //Zapo semaforo
        traffic_light_so[n][m].horizontal.d_vehicles = traffic_light_so[n][m].horizontal.n_vehicles;
        traffic_light_so[n][m].horizontal.z_vehicles = package2.n_z;
    }
    else {

        traffic_light_so[n][m].vertical.n_sum_veh+= package2.n;
        traffic_light_so[n][m].vertical.n_vehicles = package2.n;
        traffic_light_so[n][m].vertical.m_vehicles = package2.m;
        traffic_light_so[n][m].vertical.vehicle_stop = package2.stop;

        //Zapo semaforo
        traffic_light_so[n][m].vertical.d_vehicles = traffic_light_so[n][m].vertical.n_vehicles;
        traffic_light_so[n][m].vertical.z_vehicles = package2.n_z;

    }

}

void SendMessageBackCT(char type_street, Package3 &package3, int n, int m)
{
    //No es necesariohacer algo porque la estructura ya tiene los datos
}

bool ReceivesMessageBackCT(char type_street, int n, int m)
{

    //Para hacer la implementacion mas sencilla con los datos que se tiene se realiza a la inversa
    if (type_street == 'H') {
        if (d_sensores[n][m].horizontal.state_light == 1 &&  traffic_light_so[n][m].horizontal.changed == true)
            return true;
    }
    else {
        if (d_sensores[n][m].vertical.state_light == 1 &&  traffic_light_so[n][m].vertical.changed == true)
            return true;
    }

    return false;

}

int GetPositionSensor(char type_street, int n, int m)
{
    int pos;

    if (type_street == 'H')
        pos = d_sensores[n][m].horizontal.position;
    else
        pos = d_sensores[n][m].vertical.position;

    return pos;
}

int GetPositionSensedSensor(char type_street, int n, int m)
{
    int pos;

    if (type_street == 'H')
        pos = d_sensores[n][m].horizontal.sensed_vehicle.position;
    else
        pos = d_sensores[n][m].vertical.sensed_vehicle.position;

    return pos;
}


void SetVisibleSensedVehicle(char type_street, int n, int m, bool visible)
{

    if (type_street == 'H')
        d_sensores[n][m].horizontal.sensed_vehicle.visible = visible;
    else
        d_sensores[n][m].vertical.sensed_vehicle.visible = visible;

}

bool GetVisibleSensedVehicle(char type_street, int n, int m)
{

    if (type_street == 'H')
        return d_sensores[n][m].horizontal.sensed_vehicle.visible;
    else
        return d_sensores[n][m].vertical.sensed_vehicle.visible;

}


char GetDirectionSensor(char type_street, int n, int m)
{
    char dir;

    if (type_street == 'H')
        dir = d_sensores[n][m].horizontal.direction;
    else
        dir = d_sensores[n][m].vertical.direction;

    return dir;
}

int GetOffsetSensor(char type_street, int n, int m)
{
    int d_e;

    if (type_street == 'H')
        d_e = d_sensores[n][m].horizontal.distance_e;
    else
        d_e = d_sensores[n][m].vertical.distance_e;

    return d_e;
}

int GetSizeReadSensor(char type_street, int n, int m)
{
    int size;

    if (type_street == 'H')
        size = d_sensores[n][m].horizontal.vehicles_read.size();
    else
        size = d_sensores[n][m].vertical.vehicles_read.size();

    return size;
}

int GetPositionVirtualVehicleSensor(char type_street, int n, int m, int i)
{
    int pos;

    if (type_street == 'H') {

        if (!(i >= 0 && i < d_sensores[n][m].horizontal.vehicles_read.size()))
            i = 0;

        pos = d_sensores[n][m].horizontal.vehicles_read[i].position;
    }
    else {

        if (!(i >= 0 && i < d_sensores[n][m].vertical.vehicles_read.size()))
            i = 0;

        pos = d_sensores[n][m].vertical.vehicles_read[i].position;
    }

    return pos;
}

int IndexFrontSensor(char type_street, int n, int m)
{

    int s_f = 0;

    if (type_street == 'H') {

        if (d_sensores[n][m].horizontal.direction == 'R') {

            if (m + 1 >= m_ver_streets)
                s_f = 0;
            else
                s_f = m + 1;
        }
        else {

            if (m - 1 < 0)
                s_f = m_ver_streets - 1;
            else
                s_f = m - 1;
        }
    }
    else {

        if (d_sensores[n][m].vertical.direction == 'R') {

            if (n + 1 >= n_hor_streets)
                s_f = 0;
            else
                s_f = n + 1;
        }
        else {

            if (n - 1 < 0)
                s_f = n_hor_streets - 1;
            else
                s_f = n - 1;
        }
    }

    return s_f;
}




SResults SimulateVirtualEnvironment(char type_street, int n, int m)
{

    SResults results;

    results.arrived_sended = false;
    results.received_stop = false;
    results.sent_stop = false;

    resetVirtualWrite(type_street, n, m);

    if (type_street == 'H')
        SimulationHorizontal(n, m, results);

    else
        SimulationVertical(n, m, results);


    return results;

}

void SimulationHorizontal(int n, int m, SResults &results)
{
    int i;
    int k;
    //char type_street;
    char direction;
    int x;
    int v;
    int id;
    int vel;

    v = 0;

    d_sensores[n][m].horizontal.stop_sended = false; //Muy importante para no trabar en falso
    d_sensores[n][m].horizontal.vehicles_write.clear();

    if (d_sensores[n][m].horizontal.my_stop == true)
        d_sensores[n][m].horizontal.stop_received = true;
    else
        d_sensores[n][m].horizontal.stop_received = false;

    for (i = 0; i < d_sensores[n][m].horizontal.vehicles_read.size(); i++) {
        x = d_sensores[n][m].horizontal.vehicles_read[i].position;
        direction = d_sensores[n][m].horizontal.vehicles_read[i].direction;
        id = d_sensores[n][m].horizontal.vehicles_read[i].id;
        vel = d_sensores[n][m].horizontal.vehicles_read[i].velocity;

        d_sensores[n][m].horizontal.state_light = GetValueTrafficLightSO('H', n, m);

        if (d_sensores[n][m].horizontal.direction == 'R') {

            //Received
            if (x >= d_sensores[n][m].horizontal.position && x <= d_sensores[n][m].horizontal.p_traffic_light) {

                v = CalculateVirtualVelocity('H', 'R', n, m, x, vel);

                if (GetReceivedStopVirtualVehicles('H', 'R', n, m) == true)
                    d_sensores[n][m].horizontal.stop_received = true;
            }
            else {
                //Sended
                if (m == 0) {
                    if ( x >= 0 && x < d_sensores[n][m].horizontal.p_sensor_f)
                        v = CalculateVirtualVelocityFrontSensor('H', 'R', n, m, x, vel);
                }
                else {
                    if ( x >  d_sensores[n][m].horizontal.p_traffic_light && x < d_sensores[n][m].horizontal.p_sensor_f)
                        v = CalculateVirtualVelocityFrontSensor('H', 'R', n, m, x, vel);
                }

                if (GetSendedStopVirtualVehicles('H', 'R', n, m) == true)
                    d_sensores[n][m].horizontal.stop_sended = true;
            }

            if ((x + v) >= d_hor_street) {
                x = (x + v) % d_hor_street;
            }
            else
                x = x + v;

            VirtualVehicle veh;

            veh.position = x;
            veh.velocity = v;
            veh.direction = 'R';
            veh.id = id;

            if ((veh.position < d_sensores[n][m].horizontal.p_sensor_f) ||
                    (m == 0 && ((veh.position >= 0 && veh.position < d_sensores[n][m].horizontal.p_sensor_f) ||
                                (x >= d_sensores[n][m].horizontal.position && x <= d_sensores[n][m].horizontal.p_traffic_light)))) {

                k = localVirtualVehiclePosition('H', 'R', n, m, veh.position);

                SetVirtualVehicleCell('H', veh.direction, n, m, k, veh.velocity);
                d_sensores[n][m].horizontal.vehicles_write.push_back(veh);
            }
            else {

                results.arrived_sended = true;
                //h_sensores[n][m].sended++;//TODO ...
            }
        }
        else { //Direction L ////////////////////////////////////////////////////////////////////////////
            //Received
            if (x <= d_sensores[n][m].horizontal.position && x >= d_sensores[n][m].horizontal.p_traffic_light) {
                v = CalculateVirtualVelocity('H', 'L', n, m, x, vel);

                if (GetReceivedStopVirtualVehicles('H', 'L', n, m) == true)
                    d_sensores[n][m].horizontal.stop_received = true;
            }
            else {

                //Sended
                if (m == 0) {
                    if ( (x < d_hor_street && x > d_sensores[n][m].horizontal.p_sensor_f) || x == 0)
                        v = CalculateVirtualVelocityFrontSensor('H', 'L', n, m, x, vel);
                }
                else {
                    if ( x < d_sensores[n][m].horizontal.p_traffic_light && x > d_sensores[n][m].horizontal.p_sensor_f)
                        v = CalculateVirtualVelocityFrontSensor('H', 'L', n, m, x, vel);
                }

                if (GetSendedStopVirtualVehicles('H', 'L', n, m) == true)
                    d_sensores[n][m].horizontal.stop_sended = true;
            }

            if ((x - v) < 0)
                x = d_hor_street + ((x - v) % d_hor_street);
            else
                x = x - v;

            VirtualVehicle veh;

            veh.position = x;
            veh.velocity = v;
            veh.direction = 'L';
            veh.id = id;

            if ((veh.position > d_sensores[n][m].horizontal.p_sensor_f) ||
                    (m == 0 && ((veh.position < d_hor_street && veh.position > d_sensores[n][m].horizontal.p_sensor_f) || veh.position == 0) ||
                     (x <= d_sensores[n][m].horizontal.position && x >= d_sensores[n][m].horizontal.p_traffic_light))) {

                k = localVirtualVehiclePosition('H', 'L', n, m, veh.position);
                SetVirtualVehicleCell('H', veh.direction, n, m, k, veh.velocity);
                d_sensores[n][m].horizontal.vehicles_write.push_back(veh);
            }
            else {

                results.arrived_sended = true;
                //h_sensores[n][m].sended++;//TODO ...
            }
        }
    }

    d_sensores[n][m].horizontal.vehicles_read = d_sensores[n][m].horizontal.vehicles_write;//Esta operacion creo que es costosa...
    SwitchVirtualCellRW('H', n, m);

    results.sent_stop = d_sensores[n][m].horizontal.stop_sended;
    results.received_stop = d_sensores[n][m].horizontal.stop_received;

}

void SimulationVertical(int n, int m, SResults &results)
{

    int i;
    int k;
    //char type_street;
    char direction;
    int x;
    int v;
    int vel;
    int id;

    v = 0;

    d_sensores[n][m].vertical.stop_sended = false; //Muy importante para no trabar en falso
    d_sensores[n][m].vertical.vehicles_write.clear();

    if (d_sensores[n][m].vertical.my_stop == true)
        d_sensores[n][m].vertical.stop_received = true;
    else
        d_sensores[n][m].vertical.stop_received = false;

    for (i = 0; i < d_sensores[n][m].vertical.vehicles_read.size(); i++) {
        x = d_sensores[n][m].vertical.vehicles_read[i].position;
        direction = d_sensores[n][m].vertical.vehicles_read[i].direction;
        vel = d_sensores[n][m].vertical.vehicles_read[i].velocity;
        id = d_sensores[n][m].vertical.vehicles_read[i].id;

        d_sensores[n][m].vertical.state_light = GetValueTrafficLightSO('V', n, m);

        if (d_sensores[n][m].vertical.direction == 'R') {
            //Received
            if (x >= d_sensores[n][m].vertical.position && x <= d_sensores[n][m].vertical.p_traffic_light) {
                v = CalculateVirtualVelocity('V', 'R', n, m, x, vel);

                if (GetReceivedStopVirtualVehicles('V', 'R', n, m) == true)
                    d_sensores[n][m].vertical.stop_received = true;
            }
            else {
                //Sended

                if (n == 0) {
                    if ( x >= 0 && x < d_sensores[n][m].vertical.p_sensor_f)
                        v = CalculateVirtualVelocityFrontSensor('V', 'R', n, m, x, vel);
                }
                else {
                    if ( x >  d_sensores[n][m].vertical.p_traffic_light && x < d_sensores[n][m].vertical.p_sensor_f)
                        v = CalculateVirtualVelocityFrontSensor('V', 'R', n, m, x, vel);
                }

                if (GetSendedStopVirtualVehicles('V', 'R', n, m) == true)
                    d_sensores[n][m].vertical.stop_sended = true;
            }

            if ((x + v) >= d_ver_street)
                x = (x + v) % d_ver_street;
            else
                x = x + v;

            VirtualVehicle veh;

            veh.position = x;
            veh.velocity = v;
            veh.direction = 'R';
            veh.id = id;

            if ((veh.position < d_sensores[n][m].vertical.p_sensor_f) ||
                    (n == 0 && ((veh.position >= 0 && veh.position < d_sensores[n][m].vertical.p_sensor_f) ||
                                (x >= d_sensores[n][m].vertical.position && x <= d_sensores[n][m].vertical.p_traffic_light)))) {

                k = localVirtualVehiclePosition('V', 'R', n, m, veh.position);
                SetVirtualVehicleCell('V', veh.direction, n, m, k, veh.velocity);
                d_sensores[n][m].vertical.vehicles_write.push_back(veh);
            }
            else {
                results.arrived_sended = true;
                //v_sensores[m][n].sended++;//TODO ponte chido...
            }
        }
        else { //Direction L ////////////////////////////////////////////////////////////////////////////

            //Received
            if (x <= d_sensores[n][m].vertical.position && x >= d_sensores[n][m].vertical.p_traffic_light) {
                v = CalculateVirtualVelocity('V', 'L', n, m, x, vel);

                if (GetReceivedStopVirtualVehicles('V', 'L', n, m) == true)
                    d_sensores[n][m].vertical.stop_received = true;
            }
            else {
                //Sended
                if (n == 0) {
                    if ((x < d_ver_street && x > d_sensores[n][m].vertical.p_sensor_f) || x == 0)
                        v = CalculateVirtualVelocityFrontSensor('V', 'L', n, m, x, vel);
                }
                else {
                    if ( x < d_sensores[n][m].vertical.p_traffic_light && x > d_sensores[n][m].vertical.p_sensor_f)
                        v = CalculateVirtualVelocityFrontSensor('V', 'L', n, m, x, vel);
                }

                if (GetSendedStopVirtualVehicles('V', 'L', n, m) == true)
                    d_sensores[n][m].vertical.stop_sended = true;
            }

            if ((x - v) < 0)
                x = d_ver_street + ((x - v) % d_ver_street);
            else
                x = x - v;

            VirtualVehicle veh;

            veh.position = x;
            veh.velocity = v;
            veh.direction = 'L';
            veh.id = id;

            if ((veh.position > d_sensores[n][m].vertical.p_sensor_f) ||
                    (n == 0 && ((veh.position < d_ver_street && veh.position > d_sensores[n][m].vertical.p_sensor_f) || veh.position == 0) ||
                     (x <= d_sensores[n][m].vertical.position && x >= d_sensores[n][m].vertical.p_traffic_light))) {

                k = localVirtualVehiclePosition('V', 'L', n, m, veh.position);
                SetVirtualVehicleCell('V', veh.direction, n, m, k, veh.velocity);
                d_sensores[n][m].vertical.vehicles_write.push_back(veh);
            }
            else {
                results.arrived_sended = true;
                //v_sensores[m][n].sended++;//TODO...
            }
        }
    }

    d_sensores[n][m].vertical.vehicles_read = d_sensores[n][m].vertical.vehicles_write;//Esta operacion creo que es costosa...
    SwitchVirtualCellRW('V', n, m);

    results.sent_stop = d_sensores[n][m].vertical.stop_sended;
    results.received_stop = d_sensores[n][m].vertical.stop_received;

    //qDebug() << "Run simulation";
}

int GetValueTrafficLightDeliberativeSensor(char type_street, int n, int m)
{

    if (type_street == 'H') {
        return d_sensores[n][m].horizontal.state_light;
    }
    else if(type_street == 'V') {
        return d_sensores[n][m].vertical.state_light;
    }

    return -1;
}

int GetPositionTrafficLightDeliberativeSensor(char type_street, int n, int m)
{

    if (type_street == 'H')
        return d_sensores[n][m].horizontal.p_traffic_light;
    else
        return d_sensores[n][m].vertical.p_traffic_light;

    return -1;
}

int PositionFrontSensor(char type_street, int n, int m)
{

    int p_frontal_sensor;

    if (type_street == 'H')
        p_frontal_sensor = d_sensores[n][m].horizontal.p_sensor_f;
    else
        p_frontal_sensor = d_sensores[n][m].vertical.p_sensor_f;

    return p_frontal_sensor;
}


int OneCellBeforePositionFrontSensor(char type_street, char direction, int n, int m)
{

    int p_frontal_sensor;

    if (type_street == 'H') {
        if (direction == 'R') {
            p_frontal_sensor = d_sensores[n][m].horizontal.p_sensor_f - 1;
        }
        else {
            if (m == 0) {
                p_frontal_sensor = d_sensores[n][m].horizontal.p_sensor_f - 1;
            }
            else {
                p_frontal_sensor = d_sensores[n][m].horizontal.p_sensor_f + 1;
            }
        }
    }
    else {

        if (direction == 'R') {
            p_frontal_sensor = d_sensores[n][m].vertical.p_sensor_f - 1;
        }
        else {
            if (n == 0) {
                p_frontal_sensor = d_sensores[n][m].vertical.p_sensor_f - 1;
            }
            else {
                p_frontal_sensor = d_sensores[n][m].vertical.p_sensor_f + 1;
            }
        }
    }

    return p_frontal_sensor;

}

int CalculateVirtualVelocityFrontSensor(char type_street, char direction, int n, int m, int x, int v)
{

    int velocity = 0;

    if (model == 1) {

        int m_tmp = IndexFrontSensor(type_street, n, m);
        bool stop_received = d_sensores[n][m_tmp].horizontal.stop_received;
        int  before_frontal_sensor;

        before_frontal_sensor = OneCellBeforePositionFrontSensor(type_street, direction, n, m);

        //184
        if (stop_received == true && x == before_frontal_sensor)
            velocity = Rule_252();
        else {
            int xl = localVirtualVehiclePosition(type_street, direction, n, m, x);
            int d = distanceVirtualFrontVehicle(type_street, direction, n, m, xl, &velocity);
            velocity = Rule_184(d);
        }

    }
    else {

        int vn_vehicle;
        int vn_light;
        int dn;
        int vp;

        int m_tmp = IndexFrontSensor(type_street, n, m);
        bool stop_received = d_sensores[n][m_tmp].horizontal.stop_received;

        int xl_vehicle = localVirtualVehiclePosition(type_street, direction, n, m, x);
        dn = distanceVirtualFrontVehicle(type_street, direction, n, m, xl_vehicle, &vp);
        vn_vehicle = CalculateVelocityLAI(dn, v, vp);
        velocity = vn_vehicle;

#if 1
        if (stop_received == true) {

            /*int pos_sensor = PositionFrontSensor(type_street, n, m);
            int xl_sensor = localVirtualVehiclePosition(type_street, direction, n, m, pos_sensor);
            dn = calculateDistance(xl_vehicle, xl_sensor, direction, dis_block);*/

            if (direction == 'R')
                dn =  (d_side_block - xl_vehicle);//) - 1;
            else
                dn =  xl_vehicle;

            /*
            if (n == 1 && m == 1 && type_street == 'H') {
                qDebug() << "D: " << dn << xl_vehicle << rand() ;
            }*/

            /*
            if (n == 2 && m == 1 && type_street == 'H') {
                // qDebug() << "LAIS: " << pos_inter << d_sensores[n][m].horizontal.position;
                // qDebug() << "D: " << xl << dn << v << velocity << vp << rand() ;
                //qDebug() << "Aqui" << pos_sensor << xl_sensor << x << xl_vehicle << d_sensores[n][m].horizontal.position << dn;

                qDebug() << "Aqui" << xl_vehicle << xl_sensor << dn;

            }*/

            vn_light = CalculateVelocityLightLAI(dn, v);//vp es siempre 0 porque el semaforo tiene velocidad 0
            velocity = minimum(vn_vehicle, vn_light);
        }

#endif

    }

    return velocity;

}

int CalculateVirtualVelocity(char type_street, char direction, int n, int m, int x, int v)
{

    int velocity = 0;

    if (model == 1) {
        int state_light;
        int pos_light;

        state_light = GetValueTrafficLightDeliberativeSensor(type_street, n, m);
        pos_light = GetPositionTrafficLightDeliberativeSensor(type_street, n, m);

        //184
        if (state_light == 0 && pos_light == x)
            velocity = Rule_252();
        else {
            int xl = localVirtualVehiclePosition(type_street, direction, n, m, x);
            int d = distanceVirtualFrontVehicle(type_street, direction, n, m, xl, &velocity);
            velocity = Rule_184(d);
        }

    }
    else {
        int vn_vehicle;
        int vn_light;
        int state_light;
        int pos_inter;
        int dn;
        int vp;

        state_light = GetValueTrafficLightDeliberativeSensor(type_street, n, m);
        pos_inter = GetPositionIntersectionTrafficLightSO(type_street, n, m);//GetPositionTrafficLightDeliberativeSensor(type_street, n, m);

        int xl_vehicle = localVirtualVehiclePosition(type_street, direction, n, m, x);
        dn = distanceVirtualFrontVehicle(type_street, direction, n, m, xl_vehicle, &vp);

        vn_vehicle = CalculateVelocityLAI(dn, v, vp);
        velocity = vn_vehicle;

        /*
        if (n == 1 && m == 1 && type_street == 'H') {
            // qDebug() << "LAIS: " << pos_inter << d_sensores[n][m].horizontal.position;
            // qDebug() << "D: " << xl << dn << v << velocity << vp << rand() ;
            qDebug() << "x:" << x << "xl_vehicle:" << xl_vehicle << "xl_vehicle + dn: " << xl_vehicle - dn - 1 << "dn: " << dn << "v:" << v << "vp:" << vp << "velocity:" << velocity;
      }*/

        if (state_light == 0) {
            int xl_light = localVirtualVehiclePosition(type_street, direction, n, m, pos_inter);
            dn = calculateDistance(xl_vehicle, xl_light, direction, d_side_block);
            vn_light = CalculateVelocityLightLAI(dn, v);//vp es siempre 0 porque el semaforo tiene velocidad 0
            velocity = minimum(vn_vehicle, vn_light);
        }
    }

    return velocity;
}

int localVirtualVehiclePosition(char type_street, char direction, int n, int m, int x)
{

    int i = 0;

    if (type_street == 'H') {

        if (direction == 'R') {

            if (m == 0) {
                if (x >= d_sensores[n][m].horizontal.position && x <= d_sensores[n][m].horizontal.p_traffic_light)
                    i = x - d_sensores[n][m].horizontal.position;
                else
                    i = (d_hor_street - d_sensores[n][m].horizontal.position) + x;
            }
            else {
                i = x - d_sensores[n][m].horizontal.position;
            }
        }
        else if (direction == 'L') {

            if (m == 0) {

                if (x <= d_sensores[n][m].horizontal.position && x >= (d_sensores[n][m].horizontal.p_traffic_light - 1)) //cuenta tambien la interseccion
                    i = d_side_block - (d_sensores[n][m].horizontal.position - x);
                else
                    i = d_sensores[n][m].horizontal.distance_e - (d_hor_street - x);
            }
            else {
                i = d_side_block - (d_sensores[n][m].horizontal.position - x);
            }
        }
    }
    else {

        if (direction == 'R') {

            if (n == 0) {
                if (x >= d_sensores[n][m].vertical.position && x <= d_sensores[n][m].vertical.p_traffic_light)
                    i = x - d_sensores[n][m].vertical.position;
                else
                    i = (d_ver_street - d_sensores[n][m].vertical.position) + x;
            }
            else {
                i = x - d_sensores[n][m].vertical.position;
            }
        }
        else if (direction == 'L') {

            if (n == 0) {

                if (x <= d_sensores[n][m].vertical.position && x >= (d_sensores[n][m].vertical.p_traffic_light - 1)) //cuenta tambien la interseccion
                    i = d_side_block - (d_sensores[n][m].vertical.position - x);
                else
                    i = d_sensores[n][m].vertical.distance_e - (d_ver_street - x);
            }
            else {
                i = d_side_block - (d_sensores[n][m].vertical.position - x);
            }
        }
    }

    return i;
}


void SwitchVirtualCellRW(char type_street, int n, int m)
{
    if (type_street == 'H') {

        if (d_sensores[n][m].horizontal.switch_virtual_cell == true){

            //Inicializa matriz de lectura
            d_sensores[n][m].horizontal.pr_street = d_sensores[n][m].horizontal.virtualCell_A;
            d_sensores[n][m].horizontal.pw_street = d_sensores[n][m].horizontal.virtualCell_Z;
        }
        else {

            //Inicializa matriz de lectura
            d_sensores[n][m].horizontal.pr_street = d_sensores[n][m].horizontal.virtualCell_Z;
            d_sensores[n][m].horizontal.pw_street = d_sensores[n][m].horizontal.virtualCell_A;

        }

        d_sensores[n][m].horizontal.switch_virtual_cell = !(d_sensores[n][m].horizontal.switch_virtual_cell);
    }
    else {

        if (d_sensores[n][m].vertical.switch_virtual_cell == true){

            //Inicializa matriz de lectura
            d_sensores[n][m].vertical.pr_street = d_sensores[n][m].vertical.virtualCell_A;
            d_sensores[n][m].vertical.pw_street = d_sensores[n][m].vertical.virtualCell_Z;
        }
        else {

            //Inicializa matriz de lectura
            d_sensores[n][m].vertical.pr_street = d_sensores[n][m].vertical.virtualCell_Z;
            d_sensores[n][m].vertical.pw_street = d_sensores[n][m].vertical.virtualCell_A;

        }

        d_sensores[n][m].vertical.switch_virtual_cell = !(d_sensores[n][m].vertical.switch_virtual_cell);

    }

}

void SetVirtualCellValue(char type_street, int n, int m, int x, int value)
{

    if (n < 0 || n >= n_hor_streets){
        qDebug() << "ERROR: n_hor_streets" << __PRETTY_FUNCTION__;
        n = 0;
    }

    if (m < 0 || m >= m_ver_streets){
        qDebug() << "ERROR: m_ver_streets" << __PRETTY_FUNCTION__;
        m = 0;
    }

    if (x < 0 || x > d_side_block){
        qDebug() << "ERROR: d_side_block" << __PRETTY_FUNCTION__;
        x = 0;
    }

    if (type_street == 'H'){
        d_sensores[n][m].horizontal.pw_street[x].value = value;
    }
    else {
        d_sensores[n][m].vertical.pw_street[x].value = value;
    }

}


void SetVirtualVehicleCell(char type_street, char direction, int n, int m, int x, int velocity)
{
    int d_street = d_side_block + 1;

    //Body val = 1 (front), val = 2 (body), ..., val = ls (rear)
    int body;
    if (direction == 'R') {//direccion derecha
        int x_new;
        if (x >= 0 && x < d_street){
            for (body = 1; body <= ls; body++){
                x_new = x - (body - 1);
                if (x_new >= 0) {
                    SetVirtualCellValue(type_street, n, m, x_new, body);
                    SetVirtualCellVelocity(type_street, n, m, x_new, velocity);
                }
            }
        }
        else
            ;//qDebug() << "WARNING: x" << __PRETTY_FUNCTION__;
    }
    else {//direccion izquierda
        int x_new;
        if (x >= 0 && x < d_street){
            for (body = 1; body <= ls; body++){
                x_new = x + (body - 1);
                if (x_new < d_street){
                    SetVirtualCellValue(type_street, n, m, x_new, body);
                    SetVirtualCellVelocity(type_street, n, m, x_new, velocity);
                }
            }
        }
        else
            ;//qDebug() << "WARNING: x" << __PRETTY_FUNCTION__;
    }

}

void SetVirtualCellVelocity(char type_street, int n, int m, int i, int vel)
{

    if (n < 0 || n >= n_hor_streets){
        qDebug() << "ERROR: n_hor_streets" << __PRETTY_FUNCTION__;
        n = 0;
    }

    if (m < 0 || m >= m_ver_streets){
        qDebug() << "ERROR: m_ver_streets" << __PRETTY_FUNCTION__;
        m = 0;
    }

    if (i < 0 || i > d_side_block) {
        qDebug() << "ERROR: i" << __PRETTY_FUNCTION__;
        i = 0;
    }

    if (type_street == 'H')
        d_sensores[n][m].horizontal.pw_street[i].velocity = vel;
    else
        d_sensores[n][m].vertical.pw_street[i].velocity = vel;



}


int GetVirtualCellValue(char type_street, int n, int m, int i)
{

    int value = 0;

    if (n < 0 || n >= n_hor_streets)
        n = 0;

    if (m < 0 || m >= m_ver_streets)
        m = 0;

    if (i < 0 || i > d_side_block)
        i = 0;


    if (type_street == 'H'){
        value = d_sensores[n][m].horizontal.pr_street[i].value;
    }
    else {
        value = d_sensores[n][m].vertical.pr_street[i].value;
    }

    return value;
}



int GetVirtualCellVelocity(char type_street, int n, int m, int i)
{

    if (n < 0 || n >= n_hor_streets)
        n = 0;

    if (m < 0 || m >= m_ver_streets)
        m = 0;

    if (i < 0 || i > d_side_block)
        i = 0;

    if (type_street == 'H')
        return d_sensores[n][m].horizontal.pr_street[i].velocity;
    else
        return d_sensores[n][m].vertical.pr_street[i].velocity;

    return 0;
}


bool GetReceivedStopVirtualVehicles(char type_street, char direction, int n, int m)
{

    if (n < 0 || n >= n_hor_streets)
        n = 0;

    if (m < 0 || m >= m_ver_streets)
        m = 0;

    if (type_street == 'H'){
        if (direction == 'R') {

            if (d_sensores[n][m].horizontal.pr_street[0].value >= 1)
                if (d_sensores[n][m].horizontal.pr_street[0].velocity == 0)
                    return true;
        }
        else {
            if (d_sensores[n][m].horizontal.pr_street[d_side_block].value >= 1)
                if (d_sensores[n][m].horizontal.pr_street[d_side_block].velocity == 0)
                    return true;
        }
    }
    else {
        if (direction == 'R') {

            if (d_sensores[n][m].vertical.pr_street[0].value >= 1)
                if (d_sensores[n][m].vertical.pr_street[0].velocity == 0)
                    return true;
        }
        else {
            if (d_sensores[n][m].vertical.pr_street[d_side_block].value >= 1)
                if (d_sensores[n][m].vertical.pr_street[d_side_block].velocity == 0)
                    return true;
        }
    }

    return false;
}


bool GetSendedStopVirtualVehicles(char type_street, char direction, int n, int m)
{

    if (n < 0 || n >= n_hor_streets)
        n = 0;

    if (m < 0 || m >= m_ver_streets)
        m = 0;

    if (type_street == 'H'){
        if (direction == 'R') {
            for (int i = (d_side_block - d_sensores[n][m].horizontal.distance_e); i <= d_side_block; i++)
                if (d_sensores[n][m].horizontal.pr_street[i].value >= 1)
                    if (d_sensores[n][m].horizontal.pr_street[i].velocity == 0)
                        return true;
        }
        else {
            for (int i = d_sensores[n][m].horizontal.distance_e; i >= 0; i--)
                if (d_sensores[n][m].horizontal.pr_street[i].value >= 1)
                    if (d_sensores[n][m].horizontal.pr_street[i].velocity == 0)
                        return true;
        }
    }
    else {
        if (direction == 'R') {
            for (int i = (d_side_block - d_sensores[n][m].vertical.distance_e); i <= d_side_block; i++)
                if (d_sensores[n][m].vertical.pr_street[i].value >= 1)
                    if (d_sensores[n][m].vertical.pr_street[i].velocity == 0)
                        return true;
        }
        else {
            for (int i = d_sensores[n][m].vertical.distance_e; i >= 0; i--)
                if (d_sensores[n][m].vertical.pr_street[i].value >= 1)
                    if (d_sensores[n][m].vertical.pr_street[i].velocity == 0)
                        return true;
        }
    }

    return false;
}


int GetReceivedSizeVirtualVehicles(char type_street, char direction, int n, int m)
{

    int size = 0;

    if (n < 0 || n >= n_hor_streets)
        n = 0;

    if (m < 0 || m >= m_ver_streets)
        m = 0;


    if (type_street == 'H'){
        if (direction == 'R') {
            for (int i = 0; i <= (d_side_block - d_sensores[n][m].horizontal.distance_e - 1); i++)
                if (d_sensores[n][m].horizontal.pr_street[i].value == 1)
                    size++;
        }
        else {
            for (int i = d_side_block; i >= (d_sensores[n][m].horizontal.distance_e + 1); i--)
                if (d_sensores[n][m].horizontal.pr_street[i].value == 1)
                    size++;
        }
    }
    else {

        if (direction == 'R') {
            for (int i = 0; i <= (d_side_block - d_sensores[n][m].vertical.distance_e - 1); i++)
                if (d_sensores[n][m].vertical.pr_street[i].value == 1)
                    size++;
        }
        else {

            for (int i = d_side_block; i >= (d_sensores[n][m].vertical.distance_e + 1); i--)
                if (d_sensores[n][m].vertical.pr_street[i].value == 1)
                    size++;
        }
    }

    return size;
}

int GetSendedSizeVirtualVehicles(char type_street, char direction, int n, int m)
{

    int size = 0;

    if (n < 0 || n >= n_hor_streets)
        n = 0;

    if (m < 0 || m >= m_ver_streets)
        m = 0;


    if (type_street == 'H'){
        if (direction == 'R') {
            for (int i = (d_side_block - d_sensores[n][m].horizontal.distance_e); i <= d_side_block; i++)
                if (d_sensores[n][m].horizontal.pr_street[i].value == 1)
                    size++;
        }
        else {
            for (int i = d_sensores[n][m].horizontal.distance_e; i >= 0; i--)
                if (d_sensores[n][m].horizontal.pr_street[i].value == 1)
                    size++;
        }
    }
    else {
        if (direction == 'R') {
            for (int i = (d_side_block - d_sensores[n][m].vertical.distance_e); i <= d_side_block; i++)
                if (d_sensores[n][m].vertical.pr_street[i].value == 1)
                    size++;
        }
        else {
            for (int i = d_sensores[n][m].vertical.distance_e; i >= 0; i--)
                if (d_sensores[n][m].vertical.pr_street[i].value == 1)
                    size++;
        }
    }

    return size;
}



void resetVirtualWrite(char type_street,int n, int m) {


    if (n < 0 || n >= n_hor_streets)
        n = 0;

    if (m < 0 || m >= m_ver_streets)
        m = 0;

    if (type_street == 'H'){
        for (int i = 0; i <= d_side_block; i++) {
            d_sensores[n][m].horizontal.pw_street[i].value = 0;
            d_sensores[n][m].horizontal.pw_street[i].velocity = 0;//
        }
    }
    else {
        for (int i = 0; i <= d_side_block; i++) {
            d_sensores[n][m].vertical.pw_street[i].value = 0;
            d_sensores[n][m].vertical.pw_street[i].velocity = 0;//
        }
    }

}


int distanceVirtualFrontVehicle(char type_street, char direction, int n, int m, int x, int *velocity)
{

    int front = 0;
    int distance = 0;


    if (x < 0 || x > d_side_block) {
        qDebug() << "ERROR: x" << __PRETTY_FUNCTION__;
        x = 0;
    }

    *velocity = vmax;

    if (model == 1) {
        if (direction == 'R')
            front = x + 1;
        else
            front = x - 1;

        if (front > d_side_block || front < 0)
            return 1;  //distance = 1

        if (GetVirtualCellValue(type_street, n, m, front) >= 1) {
            *velocity = GetVirtualCellVelocity(type_street, n,  m, front);
            return 0; //distance = 0

        }

        distance = 1;
    }
    else {

        if (direction == 'R') {
            for (front = x + 1; front <= d_side_block; front++) {
                if (GetVirtualCellValue(type_street, n, m, front) >= 1){
                    *velocity = GetVirtualCellVelocity(type_street, n,  m, front);
                    return distance;
                }
                distance++;
            }
        }
        else {

            /*
            if (n == 1 && m == 2 && type_street == 'H') {

                qDebug() << "Tss" << x;
            }*/

            for (front = x - 1; front >= 0; front--) {
                if (GetVirtualCellValue(type_street, n, m, front) >= 1){
                    *velocity = GetVirtualCellVelocity(type_street, n,  m, front);
                    return distance;
                }
                distance++;
            }
        }

        distance = d_side_block;//No hay vehiculo enfrente

    }

    return distance;
}

SResults2 countVirtualEnvironment(char type_street, int n, int m)
{

    SResults2 results;

    results.n = 0;
    results.m = 0;
    results.n_z = 0;


    if (type_street == 'H') {

        char direction;
        int pos_t;
        int p_d;
        int p_r;
        int p_z;

        direction = (n % 2) == 0 ? 'R' : 'L';
        // pos_t = GetPositionTrafficLightSO('H', n, m);

        if (direction == 'R'){

            pos_t = d_side_block - (d_sensores[n][m].horizontal.distance_e - 1);
            p_d = (pos_t - d_sensores[n][m].horizontal.distance_d) + 1;
            p_r = pos_t - d_sensores[n][m].horizontal.distance_r;

            for (int i = p_d; i <= pos_t; i++) {
                if (d_sensores[n][m].horizontal.pr_street[i].value == 1) {//Only front
                    results.n++;
                    if (i >= p_r)
                        results.m++;
                }
            }

            p_z = pos_t + d_sensores[n][m].horizontal.distance_z + 1;
            for (int i = pos_t + 1; i <= p_z; i++) {
                if (d_sensores[n][m].horizontal.pr_street[i].value == 1) {//Only front
                    results.n_z++;
                }
            }
        }
        else {

            pos_t = d_sensores[n][m].horizontal.distance_e + 1;
            p_d = (pos_t + d_sensores[n][m].horizontal.distance_d) - 1;
            p_r = pos_t + d_sensores[n][m].horizontal.distance_r;

            for (int i = p_d; i >= pos_t; i--) {
                if (d_sensores[n][m].horizontal.pr_street[i].value == 1) {//Only front
                    results.n++;
                    if (i <= p_r)
                        results.m++;
                }
            }

            p_z = pos_t - d_sensores[n][m].horizontal.distance_z - 1;
            for (int i = pos_t - 1; i >= p_z; i--) {
                if (d_sensores[n][m].horizontal.pr_street[i].value == 1) {//Only front
                    results.n_z++;
                }
            }
        }
    }
    else {

        char direction;
        int pos_t;
        int p_d;
        int p_r;
        int p_z;


        direction = (m % 2) == 0 ? 'R' : 'L';
        //pos_t = GetPositionTrafficLightSO('V', n, m);

        if (direction == 'R'){

            pos_t = d_side_block - (d_sensores[n][m].vertical.distance_e - 1);
            p_d = (pos_t - d_sensores[n][m].vertical.distance_d) + 1;
            p_r = pos_t - d_sensores[n][m].vertical.distance_r;

            for (int i = p_d; i <= pos_t; i++) {

                // qDebug() << "sensors : " << v_sensores[m][n].pr_street[i].value;
                if (d_sensores[n][m].vertical.pr_street[i].value == 1) {//Only front
                    results.n++;
                    if (i >= p_r)
                        results.m++;
                }
            }

            p_z = pos_t + d_sensores[n][m].vertical.distance_z + 1;
            for (int i = pos_t + 1; i <= p_z; i++) {
                if (d_sensores[n][m].vertical.pr_street[i].value == 1) {//Only front
                    results.n_z++;
                }
            }
        }
        else {

            pos_t = d_sensores[n][m].vertical.distance_e + 1;
            p_d = (pos_t + d_sensores[n][m].vertical.distance_d) - 1;
            p_r = pos_t + d_sensores[n][m].vertical.distance_r;

            for (int i = p_d; i >= pos_t; i--) {
                if (d_sensores[n][m].vertical.pr_street[i].value == 1) {//Only front
                    results.n++;
                    if (i <= p_r)
                        results.m++;
                }
            }

            p_z = pos_t - d_sensores[n][m].vertical.distance_z - 1;
            for (int i = pos_t - 1; i >= p_z; i--) {
                if (d_sensores[n][m].vertical.pr_street[i].value == 1) {//Only front
                    results.n_z++;
                }
            }
        }
    }

    return results;

}

void resetAllDetectVehicle()
{

    int n, m;

    for (n = 0; n < n_hor_streets; n++){
        for (m = 0; m < m_ver_streets; m++){

            //No debes resetear el ID
            d_sensores[n][m].horizontal.sensed_vehicle.velocity = INVALID;
            d_sensores[n][m].horizontal.sensed_vehicle.position = INVALID;
            d_sensores[n][m].horizontal.sensed_vehicle.direction = INVALID;
            d_sensores[n][m].horizontal.sensed_vehicle.visible = true;

            //No debes resetear el ID
            d_sensores[n][m].vertical.sensed_vehicle.velocity = INVALID;
            d_sensores[n][m].vertical.sensed_vehicle.position = INVALID;
            d_sensores[n][m].vertical.sensed_vehicle.direction = INVALID;
            d_sensores[n][m].vertical.sensed_vehicle.visible = true;

        }
    }

}

void resetOneDetectVehicle(char type_street, int n, int m)
{

    if (type_street == 'H') {
        //No debes resetear el ID
        d_sensores[n][m].horizontal.sensed_vehicle.velocity = INVALID;
        d_sensores[n][m].horizontal.sensed_vehicle.position = INVALID;
        d_sensores[n][m].horizontal.sensed_vehicle.direction = INVALID;
        d_sensores[n][m].horizontal.sensed_vehicle.visible = true;

    } else {
        //No debes resetear el ID
        d_sensores[n][m].vertical.sensed_vehicle.velocity = INVALID;
        d_sensores[n][m].vertical.sensed_vehicle.position = INVALID;
        d_sensores[n][m].vertical.sensed_vehicle.direction = INVALID;
        d_sensores[n][m].vertical.sensed_vehicle.visible = true;
    }

}


void FillVehicles(int n, int m, int x, int y)
{

    int i;

    if (n % 2 == 0) {
        for (int k = 0; k < d_side_block / SPLIT_BLOCK; k+=ls) {
            VirtualVehicle veh;
            if (x == 0) {
                veh.position = d_hor_street - k - 1;
                veh.direction = 'R';
                veh.id = k;
                veh.velocity = 0;

                d_sensores[n][m].horizontal.vehicles_read.push_back(veh);
                i = localVirtualVehiclePosition('H', veh.direction,  n, m, veh.position);

                SetVirtualVehicleCell('H', veh.direction, n, m, i, 0);
            }
            else {
                veh.position = x - k - 1;
                veh.direction = 'R';
                veh.id = k;
                veh.velocity = 0;

                d_sensores[n][m].horizontal.vehicles_read.push_back(veh);
                i = localVirtualVehiclePosition('H', veh.direction, n, m, veh.position);

                SetVirtualVehicleCell('H', veh.direction, n, m, i, 0);
            }
        }
    }
    else {
        for (int k = 0; k < d_side_block / SPLIT_BLOCK; k+=ls) {
            VirtualVehicle veh;

            veh.position = x + k + 1;
            veh.direction = 'L';
            veh.id = k;
            veh.velocity = 0;

            d_sensores[n][m].horizontal.vehicles_read.push_back(veh);
            i = localVirtualVehiclePosition('H', veh.direction, n, m, veh.position);
            SetVirtualVehicleCell('H', veh.direction, n, m, i, 0);
        }
    }

    SwitchVirtualCellRW('H', n, m);

    if (m % 2 == 0) {
        for (int k = 0; k < d_side_block / SPLIT_BLOCK; k+=ls) {
            VirtualVehicle veh;
            if (y == 0) {
                veh.position = d_ver_street - k - 1;
                veh.direction = 'R';
                veh.id = k;
                veh.velocity = 0;

                d_sensores[n][m].vertical.vehicles_read.push_back(veh);
                i = localVirtualVehiclePosition('V', veh.direction, n, m, veh.position);

                SetVirtualVehicleCell('V', veh.direction, n, m, i, 0);
            }
            else {
                veh.position = y - k - 1;
                veh.direction = 'R';
                veh.id = k;
                veh.velocity = 0;

                d_sensores[n][m].vertical.vehicles_read.push_back(veh);
                i = localVirtualVehiclePosition('V', veh.direction, n, m, veh.position);

                SetVirtualVehicleCell('V', veh.direction, n, m, i, 0);
            }
        }
    }
    else {
        for (int k = 0; k < d_side_block / SPLIT_BLOCK; k+=ls) {
            VirtualVehicle veh;

            veh.position = y + k + 1;
            veh.direction = 'L';
            veh.id = k;
            veh.velocity = 0;

            d_sensores[n][m].vertical.vehicles_read.push_back(veh);
            i = localVirtualVehiclePosition('V', veh.direction, n, m, veh.position);
            SetVirtualVehicleCell('V', veh.direction, n, m, i, 0);
        }
    }

    SwitchVirtualCellRW('V', n, m);

}

void DetectVehicle(char type_street, char direction, int n, int m, int current_x, int current_rear_x, int previous_x, int velocity, int distance_street, int id)
{

    int x, y;

    if (metodo_sensado == 2) {
        if (type_street == 'H'){
            x = GetPositionSensor('H', n, m);

            if (direction == 'R') {
                if ((x >= current_rear_x && x <= current_x) || (x > previous_x && x < current_rear_x)){
                    d_sensores[n][m].horizontal.sensed_vehicle.velocity = velocity;
                    d_sensores[n][m].horizontal.sensed_vehicle.position = current_x;
                    d_sensores[n][m].horizontal.sensed_vehicle.direction = direction;
                    d_sensores[n][m].horizontal.sensed_vehicle.id = id;
                }
            }
            else {
                if ((x <= current_rear_x && x >= current_x) || (x < previous_x && x > current_rear_x)){
                    d_sensores[n][m].horizontal.sensed_vehicle.velocity = velocity;
                    d_sensores[n][m].horizontal.sensed_vehicle.position = current_x;
                    d_sensores[n][m].horizontal.sensed_vehicle.direction = direction;
                    d_sensores[n][m].horizontal.sensed_vehicle.id = id;
                }
            }
        }
        else {
            x = GetPositionSensor('V', n, m);

            if (direction == 'R') {
                if ((x >= current_rear_x && x <= current_x) || (x > previous_x && x < current_rear_x)){
                    d_sensores[n][m].vertical.sensed_vehicle.velocity = velocity;
                    d_sensores[n][m].vertical.sensed_vehicle.position = current_x;
                    d_sensores[n][m].vertical.sensed_vehicle.direction = direction;
                    d_sensores[n][m].vertical.sensed_vehicle.id = id;
                }
            }
            else {
                if ((x <= current_rear_x && x >= current_x) || (x < previous_x && x > current_rear_x)){
                    d_sensores[n][m].vertical.sensed_vehicle.velocity = velocity;
                    d_sensores[n][m].vertical.sensed_vehicle.position = current_x;
                    d_sensores[n][m].vertical.sensed_vehicle.direction = direction;
                    d_sensores[n][m].vertical.sensed_vehicle.id = id;
                }
            }
        }
    }

}




//////////////////////////////////////////////////////////////////////////////

void InializedDistributedSensors(int distance_d, int distance_r, int distance_e, int distance_z)
{
    int n, m;
    int x;
    int y;

    if (!(distance_d >= 0 && distance_d <= d_side_block)) {
        qDebug() << "ERROR: d_side_block" << __PRETTY_FUNCTION__;
        distance_d = d_side_block;
    }

    if (!(distance_r >= 0 && distance_r <= distance_d)) {
        qDebug() << "ERROR: distance_r" << __PRETTY_FUNCTION__;
        distance_r = distance_d;
    }

    if (!(distance_e >= 0 && distance_e <= d_side_block - (ls - 1))){
        qDebug() << "ERROR: distance_e" << __PRETTY_FUNCTION__;
        //distance_e = d_side_block - (ls - 1);
        distance_e = 0;
    }

    if (!(distance_z >= 0 && distance_z <= distance_e)){
        qDebug() << "ERROR: distance_z" << __PRETTY_FUNCTION__;
        distance_z = distance_e;
    }

    allocateMemoryDistributedSensors();

    for (n = 0; n < n_hor_streets; n++){
        for (m = 0; m < m_ver_streets; m++){

            x = m * d_side_block + m;
            y = n * d_side_block + n;

            distributed_sensores[n][m].horizontal.position = GetPositionTrafficLightSO('H', n, m);
            distributed_sensores[n][m].vertical.position = GetPositionTrafficLightSO('V', n, m);

            distributed_sensores[n][m].horizontal.distance_d = distance_d;
            distributed_sensores[n][m].vertical.distance_d = distance_d;

            distributed_sensores[n][m].horizontal.distance_r = distance_r;
            distributed_sensores[n][m].vertical.distance_r = distance_r;

            distributed_sensores[n][m].horizontal.distance_e = distance_e;
            distributed_sensores[n][m].vertical.distance_e = distance_e;

            if (n % 2 == 0) {

                distributed_sensores[n][m].horizontal.direction = 'R';

                if (x == 0) {
                    distributed_sensores[n][m].horizontal.start = d_hor_street - distance_d;
                    distributed_sensores[n][m].horizontal.end = distance_e;
                }
                else {
                    distributed_sensores[n][m].horizontal.start = x - distance_d;
                    distributed_sensores[n][m].horizontal.end = x + distance_e;
                }
            }
            else {
                distributed_sensores[n][m].horizontal.direction = 'L';

                if (x == 0) {
                    distributed_sensores[n][m].horizontal.start = distance_d;
                    distributed_sensores[n][m].horizontal.end = d_hor_street - distance_e;
                }
                else {
                    distributed_sensores[n][m].horizontal.start = x + distance_d;
                    distributed_sensores[n][m].horizontal.end = x - distance_e;
                }
            }

            if (m % 2 == 0) {
                distributed_sensores[n][m].vertical.direction = 'R';

                if (y == 0) {
                    distributed_sensores[n][m].vertical.start = d_ver_street - distance_d;
                    distributed_sensores[n][m].vertical.end = distance_e;
                }
                else {
                    distributed_sensores[n][m].vertical.start = y - distance_d;
                    distributed_sensores[n][m].vertical.end = y + distance_e;
                }
            }
            else {
                distributed_sensores[n][m].vertical.direction = 'L';

                if (y == 0) {
                    distributed_sensores[n][m].vertical.start = distance_d;
                    distributed_sensores[n][m].vertical.end = d_ver_street - distance_e;
                }
                else {
                    distributed_sensores[n][m].vertical.start = y + distance_d;
                    distributed_sensores[n][m].vertical.end = y - distance_e;
                }
            }
        }
    }

    regionDistributedSensores();

}

int allocateMemoryDistributedSensors()
{
    distributed_sensores = NULL;

    h_regionDistributedSensors[0] = NULL;
    h_regionDistributedSensors[1] = NULL;

    v_regionDistributedSensors[0] = NULL;
    v_regionDistributedSensors[1] = NULL;

    distributed_sensores = new SDistributedSensor*[n_hor_streets];
    for (int i = 0; i < n_hor_streets; i++)
        distributed_sensores[i] = new SDistributedSensor[m_ver_streets];

    h_regionDistributedSensors[0] = new int[d_hor_street];
    h_regionDistributedSensors[1] = new int[d_hor_street];

    v_regionDistributedSensors[0] = new int[d_ver_street];
    v_regionDistributedSensors[1] = new int[d_ver_street];

    return 0;
}

void freeDistributedSensors()
{
    if (v_regionDistributedSensors[0] != NULL){
        delete [] v_regionDistributedSensors[0];
        v_regionDistributedSensors[0] = NULL;
    }

    if (v_regionDistributedSensors[1] != NULL){
        delete [] v_regionDistributedSensors[1];
        v_regionDistributedSensors[1] = NULL;
    }

    if (h_regionDistributedSensors[0] != NULL){
        delete [] h_regionDistributedSensors[0];
        h_regionDistributedSensors[0] = NULL;
    }

    if (h_regionDistributedSensors[1] != NULL){
        delete [] h_regionDistributedSensors[1];
        h_regionDistributedSensors[1] = NULL;
    }

    if (distributed_sensores != NULL){
        for (int i = 0; i < n_hor_streets; i++)
            delete [] distributed_sensores[i];
        delete [] distributed_sensores;
        distributed_sensores = NULL;
    }

}

void regionDistributedSensores()
{

    //Create region for each traffic light
    int k, i, m, n;

    for (k = 0; k < 2; k++){

        for (i = 0; i < d_hor_street; i++)
            h_regionDistributedSensors[k][i] = -1;

        for (i = 0; i < d_ver_street; i++)
            v_regionDistributedSensors[k][i] = -1;
    }

    int limit_n = n_hor_streets > 1 ? 2 : 1;

    for (n = 0; n < limit_n; n++){
        for (m = 0; m < m_ver_streets; m++){

            if (distributed_sensores[n][m].horizontal.direction == 'R') {

                if (m == 0) {
                    for (i = distributed_sensores[n][m].horizontal.start; i < d_hor_street; i++)
                        h_regionDistributedSensors[n][i] = m;

                    for (i = 0; i <= distributed_sensores[n][m].horizontal.end; i++)
                        h_regionDistributedSensors[n][i] = m;
                }
                else {
                    for (i = distributed_sensores[n][m].horizontal.start; i <= distributed_sensores[n][m].horizontal.end; i++)
                        h_regionDistributedSensors[n][i] = m;
                }
            }
            else {

                if (m == 0) {

                    for (i = distributed_sensores[n][m].horizontal.start; i >= 0; i--)
                        h_regionDistributedSensors[n][i] = m;

                    for (i =  d_hor_street - 1; i >= distributed_sensores[n][m].horizontal.end; i--)
                        h_regionDistributedSensors[n][i] = m;

                }
                else {
                    for (i = distributed_sensores[n][m].horizontal.start; i >= distributed_sensores[n][m].horizontal.end; i--)
                        h_regionDistributedSensors[n][i] = m;
                }
            }
        }
    }

    int limit_m = m_ver_streets > 1 ? 2 : 1;

    for (m = 0; m < limit_m; m++) {
        for (n = 0; n < n_hor_streets; n++) {

            if (distributed_sensores[n][m].vertical.direction == 'R') {

                if (n == 0) {
                    for (i = distributed_sensores[n][m].vertical.start; i < d_ver_street; i++)
                        v_regionDistributedSensors[m][i] = n;

                    for (i = 0; i <= distributed_sensores[n][m].vertical.end; i++)
                        v_regionDistributedSensors[m][i] = n;
                }
                else {
                    for (i = distributed_sensores[n][m].vertical.start; i <= distributed_sensores[n][m].vertical.end; i++)
                        v_regionDistributedSensors[m][i] = n;
                }
            }
            else {

                if (n == 0) {

                    for (i = distributed_sensores[n][m].vertical.start; i >= 0; i--)
                        v_regionDistributedSensors[m][i] = n;


                    for (i = d_ver_street - 1; i >= distributed_sensores[n][m].vertical.end; i--)
                        v_regionDistributedSensors[m][i] = n;

                }
                else {
                    for (i = distributed_sensores[n][m].vertical.start; i >= distributed_sensores[n][m].vertical.end; i--)
                        v_regionDistributedSensors[m][i] = n;
                }
            }
        }
    }


    /*
    for (k = 0; k < 2; k++){
       for (i = 0; i < d_hor_street; i++)
            qDebug() << h_regionSensoresT[k][i];
         qDebug() << endl;

        for (i = 0; i < d_ver_street; i++)
            qDebug() << v_regionSensoresT[k][i];

        qDebug() << endl;

    }
    */

}

int GetIndexDistributedSensors(char type_street, char direction, int x)
{
    if (type_street == 'H') {
        if (direction == 'R')
            return h_regionDistributedSensors[0][x];
        else
            return h_regionDistributedSensors[1][x];
    }
    else {
        if (direction == 'R')
            return v_regionDistributedSensors[0][x];
        else
            return v_regionDistributedSensors[1][x];
    }

    return INVALID;//INVALID significa que no esta en un area de sensado
}

int searchDistanceLeaderVehicle(char type_street, char direction, int n, int m)
{

    int pos;
    int pos_before_intersection = GetPositionDistributedControl(type_street, n, m);
    int num_intersections;
    int y;

    if (type_street == 'H') {
        y = n;
        num_intersections = m_ver_streets;
    }
    else {
        y = m;
        num_intersections = n_hor_streets;
    }

    if (num_intersections > 1) {
        if (direction == 'R') {
            for (int dis = 0; dis < d_side_block; dis++){
                pos = pos_before_intersection - dis;
                if (GetValueCellStreet(type_street, y, pos) == 1){
                    return dis;
                }
            }
        }else {
            for (int dis = 0; dis < d_side_block; dis++){
                pos = pos_before_intersection + dis;
                if (GetValueCellStreet(type_street, y, pos) == 1){
                    return dis;
                }
            }
        }
    }


    return INVALID;
}

void DistributedSensing(int n, int m)
{
    int x;
    char direction;
    int value;
    int pos_t;
    int distance_e;
    int distance_d;
    int distance_r;

    //Horizontales ///////////////////////////////////////////////////////////////////////////
    distributed_control[n][m].horizontal.n_vehicles = 0;
    distributed_control[n][m].horizontal.m_vehicles = 0;
    distributed_control[n][m].horizontal.vehicle_stop = false;

    direction = (n % 2) == 0 ? 'R' : 'L';

    bool there_is_leader_h = false;
    int distance_leader_h = searchDistanceLeaderVehicle('H', direction, n, m);

    if (distance_leader_h != INVALID && distance_leader_h <= distributed_sensores[n][m].horizontal.distance_d / 2){
        there_is_leader_h = true;
    }

    if (there_is_leader_h == true){

        pos_t = distributed_sensores[n][m].horizontal.position;

        distance_d = distance_leader_h + (distributed_sensores[n][m].horizontal.distance_d / 2);
        distance_r = distributed_sensores[n][m].horizontal.distance_r;

        distance_e = (distributed_sensores[n][m].horizontal.distance_d / 2) - distance_leader_h;
        if (distance_e > distributed_sensores[n][m].horizontal.distance_e)
            distance_e = distributed_sensores[n][m].horizontal.distance_e;

        distributed_control[n][m].horizontal.vehicle_stop = VehiclesStoppedDistance_e('H', n, m, distance_e);

        //if (n == 0 && m == 0)
        //qDebug() << "EH: " << traffic_light_so[n][m].horizontal.e_vehicles;

        if (direction == 'R'){

            for (x = (pos_t - distance_d) + 1; x <= pos_t; x++){
                //Regla 1 y 3
                value = 0;
                if (GetValueCellStreet('H', n, x) == 1) //Front vehicle
                    if (GetVisibleCellStreet('H', n, x) == true)
                        value = 1;

                distributed_control[n][m].horizontal.n_vehicles+=value;

                if (x > pos_t - distance_r)
                    distributed_control[n][m].horizontal.m_vehicles+= value;
            }

            distributed_control[n][m].horizontal.n_sum_veh+= distributed_control[n][m].horizontal.n_vehicles;
        }
        else{

            for (x = (pos_t + distance_d) - 1; x >= pos_t; x--){
                //Regla 1 y 3
                value = 0;
                if (GetValueCellStreet('H', n, x) == 1) //Front vehicle
                    if (GetVisibleCellStreet('H', n, x) == true)
                        value = 1;

                distributed_control[n][m].horizontal.n_vehicles+=value;

                if (x <= pos_t + distance_r)
                    distributed_control[n][m].horizontal.m_vehicles+= value;
            }

            distributed_control[n][m].horizontal.n_sum_veh+= distributed_control[n][m].horizontal.n_vehicles;
        }
    }
    else
        distributed_control[n][m].horizontal.n_sum_veh = 0;

    //Verticales ///////////////////////////////////////////////////////////////////////////

    distributed_control[n][m].vertical.n_vehicles = 0;
    distributed_control[n][m].vertical.m_vehicles = 0;
    distributed_control[n][m].vertical.vehicle_stop = false;

    direction = (m % 2) == 0 ? 'R' : 'L';

    bool there_is_leader_v = false;
    int distance_leader_v = searchDistanceLeaderVehicle('V', direction, n, m);

    if (distance_leader_v != INVALID && distance_leader_v <= distributed_sensores[n][m].vertical.distance_d / 2){
        there_is_leader_v = true;
    }

    if (there_is_leader_v == true){

        pos_t = distributed_sensores[n][m].vertical.position;

        distance_d = distance_leader_v + (distributed_sensores[n][m].vertical.distance_d / 2);
        distance_r = distributed_sensores[n][m].vertical.distance_r;

        distance_e = (distributed_sensores[n][m].vertical.distance_d / 2) - distance_leader_v;
        if (distance_e > distributed_sensores[n][m].vertical.distance_e)
            distance_e = distributed_sensores[n][m].vertical.distance_e;

        distributed_control[n][m].vertical.vehicle_stop = VehiclesStoppedDistance_e('V', n, m, distance_e);

        //qDebug() << "EV: " << v_traffic_light_so[m][n].e_vehicles;

        if (direction == 'R'){

            for (x = (pos_t - distance_d) + 1; x <= pos_t; x++){
                //Regla 1 y 3

                value = 0;
                if (GetValueCellStreet('V', m, x) == 1)//Front vehicle
                    if (GetVisibleCellStreet('V', m, x) == true)
                        value = 1;

                distributed_control[n][m].vertical.n_vehicles+=value;

                if (x > pos_t - distance_r)
                    distributed_control[n][m].vertical.m_vehicles+= value;
            }
            distributed_control[n][m].vertical.n_sum_veh+= distributed_control[n][m].vertical.n_vehicles;

        }
        else{

            for (x = (pos_t + distance_d) - 1; x >= pos_t; x--){
                //Regla 1 y 3
                value = 0;
                if (GetValueCellStreet('V', m, x) == 1)//Front vehicle
                    if (GetVisibleCellStreet('V', m, x) == true)
                        value = 1;

                distributed_control[n][m].vertical.n_vehicles+=value;

                if (x <= pos_t + distance_r)
                    distributed_control[n][m].vertical.m_vehicles+= value;
            }
            distributed_control[n][m].vertical.n_sum_veh+= distributed_control[n][m].vertical.n_vehicles;
        }
    }
    else
        distributed_control[n][m].vertical.n_sum_veh = 0;


}

#if 0

void DetectVehicle(char type_street, char direction, int n, int m, int current_x, int current_rear_x, int previous_x, int velocity, int distance_street, int id)
{

    int x, y;

    if (metodo_sensado == 2) {

        /*if (model == 1) {

            if (type_street == 'H') {
                y = n;
                x = GetPositionSensor('H', n, m);
                if (x == current_x){
                    d_sensores[n][m].horizontal.sensed_vehicle.velocity = 0;
                    d_sensores[n][m].horizontal.sensed_vehicle.position = current_x;
                    d_sensores[n][m].horizontal.sensed_vehicle.direction = direction;
                    d_sensores[n][m].horizontal.sensed_vehicle.id = id;
                }
            }
            else {
                y = m;
                x = GetPositionSensor('V', n, m);
                if (x == current_x){
                    d_sensores[n][m].vertical.sensed_vehicle.velocity = 0;
                    d_sensores[n][m].vertical.sensed_vehicle.position = current_x;
                    d_sensores[n][m].vertical.sensed_vehicle.direction = direction;
                    d_sensores[n][m].vertical.sensed_vehicle.id = id;
                }
            }
        }
        else {*/



        if (type_street == 'H'){
            x = GetPositionSensor('H', n, m);

            if (direction == 'R') {
                if ((x >= current_rear_x && x <= current_x) || (x > previous_x && x < current_rear_x)){
                    d_sensores[n][m].horizontal.sensed_vehicle.velocity = velocity;
                    d_sensores[n][m].horizontal.sensed_vehicle.position = current_x;
                    d_sensores[n][m].horizontal.sensed_vehicle.direction = direction;
                    d_sensores[n][m].horizontal.sensed_vehicle.id = id;
                }
            }
            else {
                if ((x <= current_rear_x && x >= current_x) || (x < previous_x && x > current_rear_x)){
                    d_sensores[n][m].horizontal.sensed_vehicle.velocity = velocity;
                    d_sensores[n][m].horizontal.sensed_vehicle.position = current_x;
                    d_sensores[n][m].horizontal.sensed_vehicle.direction = direction;
                    d_sensores[n][m].horizontal.sensed_vehicle.id = id;
                }
            }


            //if (d_sensores[n][m].horizontal.sensed_vehicle.id != id) {//OJO, si es un solo vehiculo ya no lo captara posteriormente, no importa porque no es un caso importante, lo importante es no repetir eventos
            /*if (previous_x != x) {
                    int dis_sensor = calculateDistance(d_sensores[n][m].horizontal.position, current_x, direction, distance_street);
                    if (dis_sensor < d_side_block - 1){//solo interesa los vehiculos que cruzan el sensor
                        int dis_previous = calculateDistance(previous_x, current_x, direction, distance_street);
                        //  qDebug() << direction << d_sensores[n][m].horizontal.position << current_x << dis_sensor;
                        if (dis_previous < vmax) {
                            // qDebug() << direction <<  previous_x << current_x << dis_previous;// << rand();
                            if (dis_previous >= dis_sensor){

                                //if (n == 3 && m == 1 && type_street == 'H')
                                //qDebug() << "Sensado en estructura: " << "curr_x:" << current_x << "velocity:" << velocity << "id:" << id;// << n << m;

                                d_sensores[n][m].horizontal.sensed_vehicle.velocity = velocity;
                                d_sensores[n][m].horizontal.sensed_vehicle.position = current_x;
                                d_sensores[n][m].horizontal.sensed_vehicle.direction = direction;
                                d_sensores[n][m].horizontal.sensed_vehicle.id = id;
                            }
                        }
                    }
                }*/


        }
        else {

            x = GetPositionSensor('V', n, m);

            if (direction == 'R') {
                if ((x >= current_rear_x && x <= current_x) || (x > previous_x && x < current_rear_x)){
                    d_sensores[n][m].vertical.sensed_vehicle.velocity = velocity;
                    d_sensores[n][m].vertical.sensed_vehicle.position = current_x;
                    d_sensores[n][m].vertical.sensed_vehicle.direction = direction;
                    d_sensores[n][m].vertical.sensed_vehicle.id = id;
                }
            }
            else {
                if ((x <= current_rear_x && x >= current_x) || (x < previous_x && x > current_rear_x)){
                    d_sensores[n][m].vertical.sensed_vehicle.velocity = velocity;
                    d_sensores[n][m].vertical.sensed_vehicle.position = current_x;
                    d_sensores[n][m].vertical.sensed_vehicle.direction = direction;
                    d_sensores[n][m].vertical.sensed_vehicle.id = id;
                }
            }


            /*
                //if (d_sensores[n][m].vertical.sensed_vehicle.id != id) {
                x = GetPositionSensor('V', n, m);
                if (previous_x != x) {
                    int dis_sensor = calculateDistance(d_sensores[n][m].vertical.position, current_x, direction, distance_street);
                    if (dis_sensor < d_side_block - 1){//solo interesa los vehiculos que cruzan el sensor
                        int dis_previous = calculateDistance(previous_x, current_x, direction, distance_street);
                        //       qDebug() << direction << d_sensores[n][m].vertical.position << current_x << dis_sensor;
                        if (dis_previous < vmax) {
                            // qDebug() << direction <<  previous_x << current_x << dis_previous;// << rand();
                            if (dis_previous >= dis_sensor){
                                d_sensores[n][m].vertical.sensed_vehicle.velocity = velocity;
                                d_sensores[n][m].vertical.sensed_vehicle.position = current_x;
                                d_sensores[n][m].vertical.sensed_vehicle.direction = direction;
                                d_sensores[n][m].vertical.sensed_vehicle.id = id;
                            }
                        }
                    }
                }*/
            //}
        }
    }

}

#endif

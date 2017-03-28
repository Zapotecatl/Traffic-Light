#include "traffic_light.h"
#include "sensor.h"
#include "vehicle.h"

SSensorTraditional **v_sensores_t;
SSensorTraditional **h_sensores_t;

SSensor **v_sensores;
SSensor **h_sensores;

int *h_regionSensoresT[2];
int *v_regionSensoresT[2];

int *h_regionSensoresNew[2];
int *v_regionSensoresNew[2];

float precision_sensor;
int metodo_sensado;

//////////////////////////////////////////////////////////////////////////////

void InializedSensores(int metodo_s, float precision, int distance_d, int distance_r, int distance_e)
{

    //sensores
    metodo_sensado = metodo_s; //1 cerca de interseccion 2 inicio de cuadras
    precision_sensor = precision; //1.0

    InializedSensoresTraditional(distance_d, distance_r, distance_e);
    InializedSensoresDeliberative(distance_d, distance_r, distance_e);

}

int allocateMemorySensorsTraditional()
{
    int i;

    //reserved by horizontal
    h_sensores_t = new SSensorTraditional*[n_hor_streets];
    for (i = 0; i < n_hor_streets; i++)
        h_sensores_t[i] = new SSensorTraditional[m_ver_streets];

    //reserved by vertical
    v_sensores_t = new SSensorTraditional*[m_ver_streets];
    for (i = 0; i < m_ver_streets; i++)
        v_sensores_t[i] = new SSensorTraditional[n_hor_streets];

    h_regionSensoresT[0] = new int[d_hor_street];
    h_regionSensoresT[1] = new int[d_hor_street];

    v_regionSensoresT[0] = new int[d_ver_street];
    v_regionSensoresT[1] = new int[d_ver_street];

    return 0;
}

void freeSensorsTraditional()
{
    int i;

    delete [] v_regionSensoresT[0];
    delete [] v_regionSensoresT[1];

    delete [] h_regionSensoresT[0];
    delete [] h_regionSensoresT[1];

    for (i = 0; i < m_ver_streets; i++)
        delete [] v_sensores_t[i];
    delete [] v_sensores_t;

    for (i = 0; i < n_hor_streets; i++)
        delete [] h_sensores_t[i];
    delete [] h_sensores_t;

}

void regionSensoresTraditional()
{

    //Create region for each traffic light
    int k, i, m, n;

    for (k = 0; k < 2; k++){

        for (i = 0; i < d_hor_street; i++)
            h_regionSensoresT[k][i] = -1;

        for (i = 0; i < d_ver_street; i++)
            v_regionSensoresT[k][i] = -1;
    }

    int limit_n = n_hor_streets > 1 ? 2 : 1;

    for (n = 0; n < limit_n; n++){
        for (m = 0; m < m_ver_streets; m++){

            if (h_sensores_t[n][m].direction == 'R') {

                if (m == 0) {
                    for (i = h_sensores_t[n][m].start; i < d_hor_street; i++)
                        h_regionSensoresT[n][i] = m;

                    for (i = 0; i <= h_sensores_t[n][m].end; i++)
                        h_regionSensoresT[n][i] = m;
                }
                else {
                    for (i = h_sensores_t[n][m].start; i <= h_sensores_t[n][m].end; i++)
                        h_regionSensoresT[n][i] = m;
                }
            }
            else {

                if (m == 0) {

                    for (i = h_sensores_t[n][m].start; i >= 0; i--)
                        h_regionSensoresT[n][i] = m;

                    for (i =  d_hor_street - 1; i >= h_sensores_t[n][m].end; i--)
                        h_regionSensoresT[n][i] = m;

                }
                else {
                    for (i = h_sensores_t[n][m].start; i >= h_sensores_t[n][m].end; i--)
                        h_regionSensoresT[n][i] = m;
                }
            }
        }
    }

    int limit_m = m_ver_streets > 1 ? 2 : 1;

    for (m = 0; m < limit_m; m++) {
        for (n = 0; n < n_hor_streets; n++) {

            if (v_sensores_t[m][n].direction == 'R') {

                if (n == 0) {

                    for (i = v_sensores_t[m][n].start; i < d_ver_street; i++)
                        v_regionSensoresT[m][i] = n;

                    for (i = 0; i <= v_sensores_t[m][n].end; i++)
                        v_regionSensoresT[m][i] = n;
                }
                else {
                    for (i = v_sensores_t[m][n].start; i <= v_sensores_t[m][n].end; i++)
                        v_regionSensoresT[m][i] = n;
                }
            }
            else {

                if (n == 0) {

                    for (i = v_sensores_t[m][n].start; i >= 0; i--)
                        v_regionSensoresT[m][i] = n;


                    for (i = d_ver_street - 1; i >= v_sensores_t[m][n].end; i--)
                        v_regionSensoresT[m][i] = n;

                }
                else {
                    for (i = v_sensores_t[m][n].start; i >= v_sensores_t[m][n].end; i--)
                        v_regionSensoresT[m][i] = n;
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

int GetSensoresTraditional(char type_street, char direction, int x)
{

    int value = -1; //-1 significa que no esta en un area de sensado

    if (type_street == 'H') {

        if (direction == 'R')
            value = h_regionSensoresT[0][x];
        else
            value = h_regionSensoresT[1][x];
    }
    else {

        if (direction == 'R')
            value = v_regionSensoresT[0][x];
        else
            value = v_regionSensoresT[1][x];
    }

    return value;

}


void SensingSelfOrganizing(int n, int m)
{
    int x;
    char direction;
    int value;

    int pos_t;
    int distance_e;
    int distance_d;
    int distance_r;

    //Horizontales ///////////////////////////////////////////////////////////////////////////
    h_traffic_light_so[n][m].n_vehicles = 0;
    h_traffic_light_so[n][m].m_vehicles = 0;

    direction = (n % 2) == 0 ? 'R' : 'L';

    pos_t = h_sensores_t[n][m].position;

    distance_d = h_sensores_t[n][m].distance_d;
    distance_r = h_sensores_t[n][m].distance_r;
    distance_e = h_sensores_t[n][m].distance_e;

    h_traffic_light_so[n][m].vehicle_stop = VehiclesStoppedDistance_e('H', n, m, distance_e);


    if (direction == 'R'){

        for (x = (pos_t - distance_d) + 1; x <= pos_t; x++){
            //Regla 1 y 3
            if (GetVisibleCellStreet('H', n, x) == true)
                value = GetValueCellStreet('H', n, x);
            else
                value = 0;

            h_traffic_light_so[n][m].n_vehicles+=value;



            if (x > pos_t - distance_r)
                h_traffic_light_so[n][m].m_vehicles+= value;
        }

        h_traffic_light_so[n][m].n_sum_veh+= h_traffic_light_so[n][m].n_vehicles;
        h_traffic_light_so[n][m].theta_vehicles = h_traffic_light_so[n][m].n_vehicles;

    }
    else{

        for (x = (pos_t + distance_d) - 1; x >= pos_t; x--){
            //Regla 1 y 3

            if (GetVisibleCellStreet('H', n, x) == true)
                value = GetValueCellStreet('H', n, x);
            else
                value = 0;

            h_traffic_light_so[n][m].n_vehicles+=value;

            if (x <= pos_t + distance_r)
                h_traffic_light_so[n][m].m_vehicles+= value;
        }

        h_traffic_light_so[n][m].n_sum_veh+= h_traffic_light_so[n][m].n_vehicles;
        h_traffic_light_so[n][m].theta_vehicles = h_traffic_light_so[n][m].n_vehicles;
    }

    //Verticales ///////////////////////////////////////////////////////////////////////////

    v_traffic_light_so[m][n].n_vehicles = 0;
    v_traffic_light_so[m][n].m_vehicles = 0;

    direction = (m % 2) == 0 ? 'R' : 'L';

    pos_t = v_sensores_t[m][n].position;

    distance_d = v_sensores_t[m][n].distance_d;
    distance_r = v_sensores_t[m][n].distance_r;
    distance_e = v_sensores_t[m][n].distance_e;

    v_traffic_light_so[m][n].vehicle_stop = VehiclesStoppedDistance_e('V', n, m, distance_e);

    if (direction == 'R'){

        for (x = (pos_t - distance_d) + 1; x <= pos_t; x++){
            //Regla 1 y 3

            if (GetVisibleCellStreet('V', m, x) == true)
                value = GetValueCellStreet('V', m, x);
            else
                value = 0;

            v_traffic_light_so[m][n].n_vehicles+=value;

            if (x > pos_t - distance_r)
                v_traffic_light_so[m][n].m_vehicles+= value;
        }
        v_traffic_light_so[m][n].n_sum_veh+= v_traffic_light_so[m][n].n_vehicles;
        v_traffic_light_so[m][n].theta_vehicles = v_traffic_light_so[m][n].n_vehicles;
    }
    else{

        for (x = (pos_t + distance_d) - 1; x >= pos_t; x--){
            //Regla 1 y 3

            if (GetVisibleCellStreet('V', m, x) == true)
                value = GetValueCellStreet('V', m, x);
            else
                value = 0;

            v_traffic_light_so[m][n].n_vehicles+=value;

            if (x <= pos_t + distance_r)
                v_traffic_light_so[m][n].m_vehicles+= value;
        }
        v_traffic_light_so[m][n].n_sum_veh+= v_traffic_light_so[m][n].n_vehicles;
        v_traffic_light_so[m][n].theta_vehicles = v_traffic_light_so[m][n].n_vehicles;
    }
}


bool determineVisible(char type_street, char direction, int x, bool prev_visible)
{

    bool visible = false;
    int pos_t;

    if (metodo_sensado == 1) {

        pos_t = GetSensoresTraditional(type_street, direction, x);

        if (pos_t == -1) {//no esta en zona de sensado por lo que el estado por omision es true
            visible = true;
        }
        else if (prev_visible != false) {

            if (frand() <= precision_sensor)
                visible = true;
        }
    }
    else {

        pos_t = GetSensoresRegion(type_street, direction, x);

        if (pos_t == -1) //no esta en zona de sensado por lo que el estado por omision es true
            visible = true;
        else if (prev_visible != false) {

            if (frand() <= precision_sensor)
                visible = true;
        }
    }

   // qDebug() << new_visible;// << precision_sensor;

    return visible;

}




//Deliberative //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int allocateMemorySensors()
{
    int i, m, n;

    //Sensores //////////////////////////////////////////////////////////////
    //reserved by horizontal
    h_sensores = new SSensor*[n_hor_streets];
    for (i = 0; i < n_hor_streets; i++)
        h_sensores[i] = new SSensor[m_ver_streets];

    //reserved by vertical
    v_sensores = new SSensor*[m_ver_streets];
    for (i = 0; i < m_ver_streets; i++)
        v_sensores[i] = new SSensor[n_hor_streets];

    for (n = 0; n < n_hor_streets; n++) {
        for (m = 0; m < m_ver_streets; m++) {

            h_sensores[n][m].pr_street = NULL;
            h_sensores[n][m].pw_street = NULL;
            v_sensores[m][n].pr_street = NULL;
            v_sensores[m][n].pw_street = NULL;

            h_sensores[n][m].virtualCell_A = new SVirtualCell[d_side_block + 1];
            h_sensores[n][m].virtualCell_Z = new SVirtualCell[d_side_block + 1];
            v_sensores[m][n].virtualCell_A = new SVirtualCell[d_side_block + 1];
            v_sensores[m][n].virtualCell_Z = new SVirtualCell[d_side_block + 1];
        }
    }

    h_regionSensoresNew[0] = new int[d_hor_street];
    h_regionSensoresNew[1] = new int[d_hor_street];

    v_regionSensoresNew[0] = new int[d_ver_street];
    v_regionSensoresNew[1] = new int[d_ver_street];

   return 0;

}

void freeSensors()
{
    int i, n, m;

    delete [] v_regionSensoresNew[0];
    delete [] v_regionSensoresNew[1];

    delete [] h_regionSensoresNew[0];
    delete [] h_regionSensoresNew[1];

    for (n = 0; n < n_hor_streets; n++) {
        for (m = 0; m < m_ver_streets; m++) {

            delete [] v_sensores[m][n].virtualCell_Z;
            delete [] v_sensores[m][n].virtualCell_A;
            delete [] h_sensores[n][m].virtualCell_Z;
            delete [] h_sensores[n][m].virtualCell_A;
        }
    }

    for (i = 0; i < m_ver_streets; i++)
        delete [] v_sensores[i];
    delete [] v_sensores;

    for (i = 0; i < n_hor_streets; i++)
        delete [] h_sensores[i];
    delete [] h_sensores;

}

Package1 ReceivesMessageFrontCT(char type_street, int n, int m, int tmp)
{
    Package1 pack1;

    pack1.light_state = 0;
    pack1.reived_front = 0;
    pack1.stop_front = false;

    if (type_street == 'H') {

        pack1.light_state = h_traffic_light_so[n][m].light;
        pack1.stop_front = h_sensores[n][tmp].my_stop;
        pack1.reived_front = h_sensores[n][tmp].received;

    }
    else
    {
        pack1.light_state = v_traffic_light_so[m][n].light;
        pack1.stop_front = v_sensores[m][tmp].my_stop;
        pack1.reived_front = v_sensores[m][tmp].received;

    }

    return pack1;
}

void SendMessageFrontCT(char type_street, Package2 &package2, int n, int m)
{

    if (type_street == 'H') {

        h_traffic_light_so[n][m].n_sum_veh+= package2.n;
        h_traffic_light_so[n][m].n_vehicles = package2.n;
        h_traffic_light_so[n][m].m_vehicles = package2.m;
        h_traffic_light_so[n][m].vehicle_stop = package2.stop;

        //Zapo semaforo
        h_traffic_light_so[n][m].theta_vehicles = h_traffic_light_so[n][m].n_vehicles;
    }
    else {

        v_traffic_light_so[m][n].n_sum_veh+= package2.n;
        v_traffic_light_so[m][n].n_vehicles = package2.n;
        v_traffic_light_so[m][n].m_vehicles = package2.m;
        v_traffic_light_so[m][n].vehicle_stop = package2.stop;

        v_traffic_light_so[m][n].theta_vehicles = v_traffic_light_so[m][n].n_vehicles;

    }

}

void SendMessageBackCT(char type_street, Package3 &package3, int n, int m)
{
    //No es necesario enviar datos
}

bool ReceivesMessageBackCT(char type_street, int n, int m)
{

    //Para hacer la implementacion mas sencilla con los datos que se tiene se realiza a la inversa
    if (type_street == 'H') {
        if (h_sensores[n][m].state_light == 1 &&  h_traffic_light_so[n][m].changed == true)
            return true;
    }
    else {
        if (v_sensores[m][n].state_light == 1 &&  v_traffic_light_so[m][n].changed == true)
            return true;
    }

     return false;

}

void SensingSelfOrganizingSensor(int n, int m)
{

    int x;
    int y;
    bool light_change;
    int k;

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
    pack2.stop = false;

    pack3.mystop = false;
    pack3.received = 0;

    results.arrived_sended = false;
    results.received_stop = false;
    results.sent_stop = false;

    results2.m = 0;
    results2.n = 0;

    int m_tmp;
    int vehicle_state_h;
    int virtual_state_h;

    m_tmp = PositionFrontSensor('H', n, m);
    x = GetPositionSensor('H', n, m);
    y = n;

    h_sensores[n][m].my_stop = false;
    h_sensores[n][m].stop_front = false;


    if (GetVisibleCellStreet('H', y, x) == true) {//Simula errores de sensado

        vehicle_state_h = GetValueCellStreet('H', y, x);

        k = localVirtualVehiclePosition('H', h_sensores[n][m].direction, n, m, x);
        virtual_state_h = GetVirtualCellValue('H', n, m, k);

        if (vehicle_state_h == 1) {

            if (virtual_state_h == 0) {

                  VirtualVehicle veh;

                  veh.position = x;
                  veh.velocity = 0;
                  veh.direction = h_sensores[n][m].direction;

                  SetVirtualCellValue('H', n, m, k, 1);
                  SetVirtualCellStop('H', n, m, k, veh.velocity);
                  h_sensores[n][m].vehicles_read.push_back(veh);
            }

            bool vehicle_stop = GetVehiclesStopped('H', y, x);

            if (vehicle_stop == true) {

                h_sensores[n][m].my_stop = true;

            }
            else
                h_sensores[n][m].received++;
        }
    }


    pack1 = ReceivesMessageFrontCT('H', n, m, m_tmp);

    h_sensores[n][m].state_light = pack1.light_state;
    h_sensores[n][m].stop_front = pack1.stop_front;


    results = SimulateVirtualEnvironment('H', n, m);


    if (results.arrived_sended == true)
        h_sensores[n][m].sended++;

    if (results.received_stop == true)
        h_sensores[n][m].my_stop = true;

    if (results.sent_stop == true)
        h_sensores[n][m].stop_front = true;


    //Correction in the count of the senden and received
    h_sensores[n][m].epsilon = 0;
    if (h_sensores[n][m].state_light == 1 &&  h_traffic_light_so[n][m].changed == true) {

        h_sensores[n][m].epsilon =  abs(h_sensores[n][m_tmp].received - h_sensores[n][m].sended);
        h_sensores[n][m].sended = 0;
    }

    results2 = countVirtualEnvironment('H', n, m);

    pack2.n = results2.n + h_sensores[n][m].epsilon;
    pack2.m = results2.m;
    pack2.stop = h_sensores[n][m].stop_front;

    SendMessageFrontCT('H', pack2, n, m);

    pack3.mystop = h_sensores[n][m].my_stop;
    pack3.received =  h_sensores[n][m].received;

    SendMessageBackCT('H', pack3, n, m);

    light_change = ReceivesMessageBackCT('H', n, m);

    //Para hacer la implementacion mas sencilla con los datos que se tiene se realiza a la inversa
    if (light_change == true)
      h_sensores[n][m_tmp].received = 0;


    //Vertical /////////////////////////////////////////////////////////////////////////////////////////////////

    pack1.light_state = 0;
    pack1.reived_front = 0;
    pack1.stop_front = false;

    pack2.m = 0;
    pack2.n = 0;
    pack2.stop = false;

    pack3.mystop = false;
    pack3.received = 0;

    results.arrived_sended = false;
    results.received_stop = false;
    results.sent_stop = false;

    results2.m = 0;
    results2.n = 0;

    int n_tmp;
    int vehicle_state_v;
    int virtual_state_v;

    n_tmp = PositionFrontSensor('V', n, m);
    x = GetPositionSensor('V', n, m);
    y = m;

    v_sensores[m][n].my_stop = false;
    v_sensores[m][n].stop_front = false;


    if (GetVisibleCellStreet('V', y, x) == true) {//Simula errores de sensado

        vehicle_state_v = GetValueCellStreet('V', y, x);

        k = localVirtualVehiclePosition('V', v_sensores[m][n].direction, n, m, x);
        virtual_state_v = GetVirtualCellValue('V', n, m, k);

        if (vehicle_state_v == 1) {

            if (virtual_state_v == 0) {

                  VirtualVehicle veh;

                  veh.position = x;
                  veh.velocity = 0;
                  veh.direction = v_sensores[m][n].direction;

                  SetVirtualCellValue('V', n, m, k, 1);
                  SetVirtualCellStop('V', n, m, k, veh.velocity);
                  v_sensores[m][n].vehicles_read.push_back(veh);
            }

            bool vehicle_stop = GetVehiclesStopped('V', y, x);

            if (vehicle_stop == true)
                v_sensores[m][n].my_stop = true;
            else
                v_sensores[m][n].received++;
        }
   }

    pack1 = ReceivesMessageFrontCT('V', n, m, n_tmp);

    v_sensores[m][n].state_light = pack1.light_state;
    v_sensores[m][n].stop_front = pack1.stop_front;


    results = SimulateVirtualEnvironment('V', n, m);


    if (results.arrived_sended == true)
        v_sensores[m][n].sended++;

    if (results.received_stop == true)
        v_sensores[m][n].my_stop = true;

    if (results.sent_stop == true)
        v_sensores[m][n].stop_front = true;


    //Correction in the count of the senden and received
    v_sensores[m][n].epsilon = 0;
    if (v_sensores[m][n].state_light == 1 &&  v_traffic_light_so[m][n].changed == true) {

        v_sensores[m][n].epsilon =  abs(v_sensores[m][n_tmp].received - v_sensores[m][n].sended);
        v_sensores[m][n].sended = 0;
    }

    results2 = countVirtualEnvironment('V', n, m);

    pack2.n = results2.n + v_sensores[m][n].epsilon;
    pack2.m = results2.m;
    pack2.stop = v_sensores[m][n].stop_front;

    SendMessageFrontCT('V', pack2, n, m);

    pack3.mystop = v_sensores[m][n].my_stop;
    pack3.received =  v_sensores[m][n].received;

    SendMessageBackCT('V', pack3, m, n);

    light_change = ReceivesMessageBackCT('V', n, m);

    //Para hacer la implementacion mas sencilla con los datos que se tiene se realiza a la inversa
    if (light_change == true)
      v_sensores[m][n_tmp].received = 0;


}

void InializedSensoresTraditional(int distance_d, int distance_r, int distance_e)
{
    int n, m;
    int x;
    int y;

    if (!(distance_d >= 0 && distance_d <= d_side_block))
        distance_d = d_side_block;

    if (!(distance_r >= 0 && distance_r <= distance_d))
        distance_r = distance_d;

    if (!(distance_e >= 0 && distance_e <= d_side_block))
        distance_e = d_side_block;

    allocateMemorySensorsTraditional();


    for (n = 0; n < n_hor_streets; n++){
        for (m = 0; m < m_ver_streets; m++){

            x = m * d_side_block + m;
            y = n * d_side_block + n;

            h_sensores_t[n][m].position = GetPositionTrafficLightSO('H', n, m);
            v_sensores_t[m][n].position = GetPositionTrafficLightSO('V', n, m);

            h_sensores_t[n][m].distance_d = distance_d;
            v_sensores_t[m][n].distance_d = distance_d;

            h_sensores_t[n][m].distance_r = distance_r;
            v_sensores_t[m][n].distance_r = distance_r;

            h_sensores_t[n][m].distance_e = distance_e;
            v_sensores_t[m][n].distance_e = distance_e;


            if (n % 2 == 0) {

                h_sensores_t[n][m].direction = 'R';

                if (x == 0) {
                    h_sensores_t[n][m].start = d_hor_street - distance_d;
                    h_sensores_t[n][m].end = distance_e;
                }
                else {
                    h_sensores_t[n][m].start = x - distance_d;
                    h_sensores_t[n][m].end = x + distance_e;
                }
            }
            else {

                h_sensores_t[n][m].direction = 'L';

                if (x == 0) {
                    h_sensores_t[n][m].start = distance_d;
                    h_sensores_t[n][m].end = d_hor_street - distance_e;
                }
                else {
                    h_sensores_t[n][m].start = x + distance_d;
                    h_sensores_t[n][m].end = x - distance_e;
                }
            }


            if (m % 2 == 0) {

                v_sensores_t[m][n].direction = 'R';

                if (y == 0) {
                    v_sensores_t[m][n].start = d_ver_street - distance_d;
                    v_sensores_t[m][n].end = distance_e;
                }
                else {
                    v_sensores_t[m][n].start = y - distance_d;
                    v_sensores_t[m][n].end = y + distance_e;
                }
            }
            else {

                v_sensores_t[m][n].direction = 'L';

                if (y == 0) {
                    v_sensores_t[m][n].start = distance_d;
                    v_sensores_t[m][n].end = d_ver_street - distance_e;
                }
                else {
                    v_sensores_t[m][n].start = y + distance_d;
                    v_sensores_t[m][n].end = y - distance_e;
                }
            }
        }
    }

    regionSensoresTraditional();

}


void InializedSensoresDeliberative(int distance_d, int distance_r, int distance_e)
{
    int n, m;
    int x;
    int y;


    if (!(distance_d >= 0 && distance_d <= d_side_block))
        distance_d = d_side_block;

    if (!(distance_r >= 0 && distance_r <= distance_d))
        distance_r = distance_d;

    if (!(distance_e >= 0 && distance_e <= d_side_block))
        distance_e = d_side_block;


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

            h_sensores[n][m].state_light = h_traffic_light_so[n][m].light;
            v_sensores[m][n].state_light = v_traffic_light_so[m][n].light;

            h_sensores[n][m].p_traffic_light = h_traffic_light_so[n][m].position;
            v_sensores[m][n].p_traffic_light = v_traffic_light_so[m][n].position;

            h_sensores[n][m].received = 0;
            v_sensores[m][n].received = 0;

            h_sensores[n][m].sended = 0;
            v_sensores[m][n].sended = 0;

            h_sensores[n][m].switch_virtual_cell = false;
            v_sensores[m][n].switch_virtual_cell = false;

            h_sensores[n][m].stop_front = false;
            v_sensores[m][n].stop_front = false;

            h_sensores[n][m].my_stop = false;
            v_sensores[m][n].my_stop = false;

            h_sensores[n][m].stop_sended = false;
            v_sensores[m][n].stop_sended = false;

            h_sensores[n][m].epsilon = 0;
            v_sensores[m][n].epsilon = 0;

            h_sensores[n][m].distance_d = distance_d;
            v_sensores[m][n].distance_d = distance_d;

            h_sensores[n][m].distance_r = distance_r;
            v_sensores[m][n].distance_r = distance_r;

            h_sensores[n][m].distance_e = distance_e;
            v_sensores[m][n].distance_e = distance_e;


            if (n % 2 == 0) {

                h_sensores[n][m].direction = 'R';

                if (x == 0)
                    h_sensores[n][m].position = d_hor_street - d_side_block + distance_e;
                else
                    h_sensores[n][m].position = x - d_side_block + distance_e;
            }
            else {

                h_sensores[n][m].direction = 'L';
                h_sensores[n][m].position = x + d_side_block - distance_e;
            }


            if (m % 2 == 0) {

                v_sensores[m][n].direction = 'R';

                if (y == 0)
                    v_sensores[m][n].position = d_ver_street - d_side_block + distance_e;
                else
                    v_sensores[m][n].position = y - d_side_block + distance_e;
            }
            else {

                v_sensores[m][n].direction = 'L';
                v_sensores[m][n].position = y + d_side_block - distance_e;
            }

            FillVehicles(n, m, x, y);
        }
    }

    for (n = 0; n < n_hor_streets; n++){
        for (m = 0; m < m_ver_streets; m++){

            int m_tmp = PositionFrontSensor('H', n, m);
            h_sensores[n][m].p_sensor_f = h_sensores[n][m_tmp].position;//Interseccion y distanica e comienza des de 0

            int n_tmp = PositionFrontSensor('V', n, m);
            v_sensores[m][n].p_sensor_f = v_sensores[m][n_tmp].position;//Interseccion y distanica e comienza des de 0
        }
    }

    regionSensoresNew();
}


void FillVehicles(int n, int m, int x, int y)
{

    int i;

    if (n % 2 == 0) {

        for (int k = 0; k < d_side_block / 2; k++) {

            VirtualVehicle veh;
            //   veh.t = 0;

            if (x == 0) {
                veh.position = d_hor_street - k - 1;
                veh.direction = 'R';

                h_sensores[n][m].vehicles_read.push_back(veh);
                i = localVirtualVehiclePosition('H', veh.direction,  n, m, veh.position);
                SetVirtualCellValue('H', n, m, i, 1);
                SetVirtualCellStop('H', n, m, i, 0);
            }
            else {

                veh.position = x - k - 1;
                veh.direction = 'R';

                h_sensores[n][m].vehicles_read.push_back(veh);
                i = localVirtualVehiclePosition('H', veh.direction, n, m, veh.position);
                SetVirtualCellValue('H', n, m, i, 1);
                SetVirtualCellStop('H', n, m, i, 0);
            }
        }
    }
    else {

        for (int k = 0; k < d_side_block / 2; k++) {

            VirtualVehicle veh;

            veh.position = x + k + 1;
            veh.direction = 'L';
            h_sensores[n][m].vehicles_read.push_back(veh);

            i = localVirtualVehiclePosition('H', veh.direction, n, m, veh.position);
            SetVirtualCellValue('H', n, m, i, 1);
            SetVirtualCellStop('H', n, m, i, 0);

        }
    }

    SwitchVirtualCellRW('H', n, m);


    if (m % 2 == 0) {

        for (int k = 0; k < d_side_block / 2; k++) {

            VirtualVehicle veh;
            //   veh.t = 0;

            if (y == 0) {

                veh.position = d_ver_street - k - 1;
                veh.direction = 'R';

                v_sensores[m][n].vehicles_read.push_back(veh);
                i = localVirtualVehiclePosition('V', veh.direction, n, m, veh.position);
                SetVirtualCellValue('V', n, m, i, 1);
                SetVirtualCellStop('V', n, m, i, 0);
            }
            else {

                veh.position = y - k - 1;
                veh.direction = 'R';

                v_sensores[m][n].vehicles_read.push_back(veh);
                i = localVirtualVehiclePosition('V', veh.direction, n, m, veh.position);
                SetVirtualCellValue('V', n, m, i, 1);
                SetVirtualCellStop('V', n, m, i, 0);

            }
        }
    }
    else {

        for (int k = 0; k < d_side_block / 2; k++) {

            VirtualVehicle veh;

            veh.position = y + k + 1;
            veh.direction = 'L';
            v_sensores[m][n].vehicles_read.push_back(veh);

            i = localVirtualVehiclePosition('V', veh.direction, n, m, veh.position);
            SetVirtualCellValue('V', n, m, i, 1);
            SetVirtualCellStop('V', n, m, i, 0);
        }
    }

    SwitchVirtualCellRW('V', n, m);

}


int GetPositionSensor(char type_street, int n, int m)
{
    int pos;

    if (type_street == 'H')
        pos = h_sensores[n][m].position;
    else
        pos = v_sensores[m][n].position;

    return pos;
}

char GetDirectionSensor(char type_street, int n, int m)
{
    char dir;

    if (type_street == 'H')
        dir = h_sensores[n][m].direction;
    else
        dir = v_sensores[m][n].direction;

    return dir;
}

int GetOffsetSensor(char type_street, int n, int m)
{
    int d_e;

    if (type_street == 'H')
        d_e = h_sensores[n][m].distance_e;
    else
        d_e = v_sensores[m][n].distance_e;

    return d_e;
}

int GetSizeReadSensor(char type_street, int n, int m)
{
    int size;

    if (type_street == 'H')
        size = h_sensores[n][m].vehicles_read.size();
    else
        size = v_sensores[m][n].vehicles_read.size();

    return size;
}

int GetPositionVirtualVehicleSensor(char type_street, int n, int m, int i)
{
    int pos;

    if (type_street == 'H') {

        if (!(i >= 0 && i < h_sensores[n][m].vehicles_read.size()))
            i = 0;

        pos = h_sensores[n][m].vehicles_read[i].position;
    }
    else {

        if (!(i >= 0 && i < v_sensores[m][n].vehicles_read.size()))
            i = 0;

        pos = v_sensores[m][n].vehicles_read[i].position;
    }

    return pos;
}

int PositionFrontSensor(char type_street, int n, int m)
{

    int s_f = 0;

    if (type_street == 'H') {

        if (h_sensores[n][m].direction == 'R') {

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

        if (v_sensores[m][n].direction == 'R') {

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



void regionSensoresNew()
{

    //Create region for each traffic light
    int k, i, m, n;

    for (k = 0; k < 2; k++){

        for (i = 0; i < d_hor_street; i++)
            h_regionSensoresNew[k][i] = -1;

        for (i = 0; i < d_ver_street; i++)
            v_regionSensoresNew[k][i] = -1;
    }

    int limit_n = n_hor_streets > 1 ? 2 : 1;

    for (n = 0; n < limit_n; n++)
        for (m = 0; m < m_ver_streets; m++){
            h_regionSensoresNew[n][h_sensores[n][m].position] = m;
        }

    int limit_m = m_ver_streets > 1 ? 2 : 1;

    for (m = 0; m < limit_m; m++)
        for (n = 0; n < n_hor_streets; n++){
            v_regionSensoresNew[m][v_sensores[m][n].position] = n;
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

int GetSensoresRegion(char type_street, int direction, int x)
{

    int value = -1; //-1 significa que no esta en un area de sensado

    if (type_street == 'H') {

        if (direction == 'R')
            value = h_regionSensoresNew[0][x];
        else
            value = h_regionSensoresNew[1][x];
    }
    else {

        if (direction == 'R')
            value = v_regionSensoresNew[0][x];
        else
            value = v_regionSensoresNew[1][x];
    }

  return value;

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

    v = 0;

    h_sensores[n][m].stop_sended = false; //Muy importante para no trabar en falso
    h_sensores[n][m].vehicles_write.clear();

   if (h_sensores[n][m].my_stop == true)
      h_sensores[n][m].stop_received = true;
   else
      h_sensores[n][m].stop_received = false;


   for (i = 0; i < h_sensores[n][m].vehicles_read.size(); i++) {

       x = h_sensores[n][m].vehicles_read[i].position;
       direction = h_sensores[n][m].vehicles_read[i].direction;
       h_sensores[n][m].state_light = GetValueTrafficLightSO('H', n, m);

       if (h_sensores[n][m].direction == 'R') {

           //Received
           if (x >= h_sensores[n][m].position && x <= h_sensores[n][m].p_traffic_light) {

               if (h_sensores[n][m].state_light == 0 && x == h_sensores[n][m].p_traffic_light) {
                   k = localVirtualVehiclePosition('H', 'R', n, m, x);
                   v = Virtual_Rule_252('H', 'R', n, m, k);
               }
               else {

                   k = localVirtualVehiclePosition('H', 'R', n, m, x);
                   v = Virtual_Rule_184('H', 'R', n, m, k);
               }

               if (GetReceivedStopVirtualVehicles('H', 'R', n, m) == true)
                 h_sensores[n][m].stop_received = true;
           }
           else {

               //Sended
               int m_tmp = PositionFrontSensor('H', n, m);
               bool stop_received = h_sensores[n][m_tmp].stop_received;

               if (m == 0) {
                   if ( x >= 0 && x < h_sensores[n][m].p_sensor_f) {

                       if (stop_received == true && x == h_sensores[n][m].p_sensor_f - 1){

                           k = localVirtualVehiclePosition('H', h_sensores[n][m].direction, n, m, x);
                           v = Virtual_Rule_252('H', 'R', n, m, k);
                       }
                       else {

                           k = localVirtualVehiclePosition('H', h_sensores[n][m].direction, n, m, x);
                           v = Virtual_Rule_184('H', 'R', n, m, k);
                       }
                   }
               }
               else {
                   if ( x >  h_sensores[n][m].p_traffic_light && x < h_sensores[n][m].p_sensor_f) {

                       if (stop_received == true && x == h_sensores[n][m].p_sensor_f - 1) {

                           k = localVirtualVehiclePosition('H', h_sensores[n][m].direction, n, m, x);
                           v = Virtual_Rule_252('H', 'R', n, m, k);
                       }
                       else {

                           k = localVirtualVehiclePosition('H', h_sensores[n][m].direction, n, m, x);
                           v = Virtual_Rule_184('H', 'R', n, m, k);
                       }
                   }
               }

               if (GetSendedStopVirtualVehicles('H', 'R', n, m) == true)
                  h_sensores[n][m].stop_sended = true;

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


           if ((veh.position < h_sensores[n][m].p_sensor_f) ||
               (m == 0 && ((veh.position >= 0 && veh.position < h_sensores[n][m].p_sensor_f) ||
               (x >= h_sensores[n][m].position && x <= h_sensores[n][m].p_traffic_light)))) {

               k = localVirtualVehiclePosition('H', 'R', n, m, veh.position);
               SetVirtualCellValue('H', n, m, k, 1);
               SetVirtualCellStop('H', n, m, k, veh.velocity);

               h_sensores[n][m].vehicles_write.push_back(veh);
           }
           else {

               results.arrived_sended = true;
               //h_sensores[n][m].sended++;//TODO ...
           }
       }

       else { //Direction L ////////////////////////////////////////////////////////////////////////////


           //Received
           if (x <= h_sensores[n][m].position && x >= h_sensores[n][m].p_traffic_light) {


               if (h_sensores[n][m].state_light == 0 && x == h_sensores[n][m].p_traffic_light) {

                   k = localVirtualVehiclePosition('H', h_sensores[n][m].direction, n, m, x);
                   v = Virtual_Rule_252('H', 'L', n, m, k);
               }
               else {

                   k = localVirtualVehiclePosition('H', 'L', n, m, x);
                   v = Virtual_Rule_184('H', 'L', n, m, k);
               }


               if (GetReceivedStopVirtualVehicles('H', 'L', n, m) == true)
                   h_sensores[n][m].stop_received = true;

           }
           else {

               //Sended

               int m_tmp = PositionFrontSensor('H', n, m);
               bool stop_received = h_sensores[n][m_tmp].stop_received;

               if (m == 0) {
                   if ( (x < d_hor_street && x > h_sensores[n][m].p_sensor_f) || x == 0) {

                       if (stop_received == true && x == h_sensores[n][m].p_sensor_f - 1){

                           k = localVirtualVehiclePosition('H', 'L', n, m, x);
                           v = Virtual_Rule_252('H', 'L', n, m, k);
                       }
                       else {

                           k = localVirtualVehiclePosition('H', h_sensores[n][m].direction, n, m, x);
                           v = Virtual_Rule_184('H', 'L', n, m, k);
                       }
                   }
               }
               else {
                   if ( x < h_sensores[n][m].p_traffic_light && x > h_sensores[n][m].p_sensor_f) {

                       if (stop_received == true && x == h_sensores[n][m].p_sensor_f + 1) {

                           k = localVirtualVehiclePosition('H', h_sensores[n][m].direction, n, m, x);
                           v = Virtual_Rule_252('H', 'L', n, m, k);
                       }
                       else {

                           k = localVirtualVehiclePosition('H', h_sensores[n][m].direction, n, m, x);
                           v = Virtual_Rule_184('H', 'L', n, m, k);
                       }
                   }
               }

               if (GetSendedStopVirtualVehicles('H', 'L', n, m) == true)
                   h_sensores[n][m].stop_sended = true;
           }

           if ((x - v) < 0) {
               x = d_hor_street - 1;
           }
           else
               x = x - v;

           VirtualVehicle veh;

           veh.position = x;
           veh.velocity = v;
           veh.direction = 'L';


           if ((veh.position > h_sensores[n][m].p_sensor_f) ||
               (m == 0 && ((veh.position < d_hor_street && veh.position > h_sensores[n][m].p_sensor_f) || veh.position == 0) ||
               (x <= h_sensores[n][m].position && x >= h_sensores[n][m].p_traffic_light))) {

               k = localVirtualVehiclePosition('H', 'L', n, m, veh.position);
               SetVirtualCellValue('H', n, m, k, 1);
               SetVirtualCellStop('H', n, m, k, veh.velocity);

               h_sensores[n][m].vehicles_write.push_back(veh);
           }
           else {

               results.arrived_sended = true;
               //h_sensores[n][m].sended++;//TODO ...
           }
       }
   }

   h_sensores[n][m].vehicles_read = h_sensores[n][m].vehicles_write;//Esta operacion creo que es costosa...
   SwitchVirtualCellRW('H', n, m);

   results.sent_stop = h_sensores[n][m].stop_sended;
   results.received_stop = h_sensores[n][m].stop_received;

}

void SimulationVertical(int n, int m, SResults &results)
{

    int i;
    int k;
    //char type_street;
    char direction;
    int x;
    int v;
    v = 0;

    v_sensores[m][n].stop_sended = false; //Muy importante para no trabar en falso
    v_sensores[m][n].vehicles_write.clear();

   if (v_sensores[m][n].my_stop == true)
      v_sensores[m][n].stop_received = true;
   else
      v_sensores[m][n].stop_received = false;

   for (i = 0; i < v_sensores[m][n].vehicles_read.size(); i++) {

       x = v_sensores[m][n].vehicles_read[i].position;
       direction = v_sensores[m][n].vehicles_read[i].direction;
       v_sensores[m][n].state_light = GetValueTrafficLightSO('V', n, m);

       if (v_sensores[m][n].direction == 'R') {
           //Received
           if (x >= v_sensores[m][n].position && x <= v_sensores[m][n].p_traffic_light) {

               if (v_sensores[m][n].state_light == 0 && x == v_sensores[m][n].p_traffic_light) {

                   k = localVirtualVehiclePosition('V', 'R', n, m, x);
                   v = Virtual_Rule_252('V', 'R', n, m, k);
               }
               else {

                   k = localVirtualVehiclePosition('V', 'R', n, m, x);
                   v = Virtual_Rule_184('V', 'R', n, m, k);
               }

               if (GetReceivedStopVirtualVehicles('V', 'R', n, m) == true)
                   v_sensores[m][n].stop_received = true;
           }
           else {
               //Sended
               int n_tmp = PositionFrontSensor('V', n, m);
               bool stop_received = v_sensores[m][n_tmp].stop_received;

               if (n == 0) {
                   if ( x >= 0 && x < v_sensores[m][n].p_sensor_f) {
                       if (stop_received == true && x == v_sensores[m][n].p_sensor_f - 1){

                           k = localVirtualVehiclePosition('V', v_sensores[m][n].direction, n, m, x);
                           v = Virtual_Rule_252('V', 'R', n, m, k);
                       }
                       else {

                           k = localVirtualVehiclePosition('V', v_sensores[m][n].direction, n, m, x);
                           v = Virtual_Rule_184('V', 'R', n, m, k);
                       }
                   }
               }
               else {
                   if ( x >  v_sensores[m][n].p_traffic_light && x < v_sensores[m][n].p_sensor_f) {

                       if (stop_received == true && x == v_sensores[m][n].p_sensor_f - 1) {

                           k = localVirtualVehiclePosition('V', v_sensores[m][n].direction, n, m, x);
                           v = Virtual_Rule_252('V', 'R', n, m, k);
                       }
                       else {

                           k = localVirtualVehiclePosition('V', v_sensores[m][n].direction, n, m, x);
                           v = Virtual_Rule_184('V', 'R', n, m, k);
                       }
                   }
               }

               if (GetSendedStopVirtualVehicles('V', 'R', n, m) == true)
                   v_sensores[m][n].stop_sended = true;
            }

           if ((x + v) >= d_ver_street) {
               x = (x + v) % d_ver_street;
           }
           else
               x = x + v;

           VirtualVehicle veh;

           veh.position = x;
           veh.velocity = v;
           veh.direction = 'R';

           if ((veh.position < v_sensores[m][n].p_sensor_f) ||
               (n == 0 && ((veh.position >= 0 && veh.position < v_sensores[m][n].p_sensor_f) ||
               (x >= v_sensores[m][n].position && x <= v_sensores[m][n].p_traffic_light)))) {

               k = localVirtualVehiclePosition('V', 'R', n, m, veh.position);
               SetVirtualCellValue('V', n, m, k, 1);
               SetVirtualCellStop('V', n, m, k, veh.velocity);

               v_sensores[m][n].vehicles_write.push_back(veh);
           }
           else {
               results.arrived_sended = true;
               //v_sensores[m][n].sended++;//TODO ponte chido...
           }
       }
       else { //Direction L ////////////////////////////////////////////////////////////////////////////

           //Received
           if (x <= v_sensores[m][n].position && x >= v_sensores[m][n].p_traffic_light) {
               if (v_sensores[m][n].state_light == 0 && x == v_sensores[m][n].p_traffic_light) {

                   k = localVirtualVehiclePosition('V', v_sensores[m][n].direction, n, m, x);
                   v = Virtual_Rule_252('V', 'L', n, m, k);
               }
               else {

                   k = localVirtualVehiclePosition('V', 'L', n, m, x);
                   v = Virtual_Rule_184('V', 'L', n, m, k);
               }

               if (GetReceivedStopVirtualVehicles('V', 'L', n, m) == true)
                   v_sensores[m][n].stop_received = true;
           }
           else {
               //Sended

               int n_tmp = PositionFrontSensor('V', n, m);
               bool stop_received = v_sensores[m][n_tmp].stop_received;

               if (n == 0) {
                   if ( (x < d_ver_street && x > v_sensores[m][n].p_sensor_f) || x == 0) {

                       if (stop_received == true && x == v_sensores[m][n].p_sensor_f - 1){

                           k = localVirtualVehiclePosition('V', 'L', n, m, x);
                           v = Virtual_Rule_252('V', 'L', n, m, k);
                       }
                       else {

                           k = localVirtualVehiclePosition('V', 'L', n, m, x);
                           v = Virtual_Rule_184('V', 'L', n, m, k);
                       }
                   }
               }
               else {
             //  v_traffic_light_so[m][n].n_vehicles = 5;

                   if ( x < v_sensores[m][n].p_traffic_light && x > v_sensores[m][n].p_sensor_f) {

                       if (stop_received == true && x == v_sensores[m][n].p_sensor_f + 1) {

                           k = localVirtualVehiclePosition('V', 'L', n, m, x);
                           v = Virtual_Rule_252('V', 'L', n, m, k);
                       }
                       else {

                           k = localVirtualVehiclePosition('V', 'L', n, m, x);
                           v = Virtual_Rule_184('V', 'L', n, m, k);
                       }
                   }
               }

               if (GetSendedStopVirtualVehicles('V', 'L', n, m) == true)
                   v_sensores[m][n].stop_sended = true;
           }

           if ((x - v) < 0) {
               x = d_ver_street - 1;
           }
           else
               x = x - v;

           VirtualVehicle veh;

           veh.position = x;
           veh.velocity = v;
           veh.direction = 'L';

           if ((veh.position > v_sensores[m][n].p_sensor_f) ||
               (n == 0 && ((veh.position < d_ver_street && veh.position > v_sensores[m][n].p_sensor_f) || veh.position == 0) ||
               (x <= v_sensores[m][n].position && x >= v_sensores[m][n].p_traffic_light))) {

               k = localVirtualVehiclePosition('V', 'L', n, m, veh.position);
               SetVirtualCellValue('V', n, m, k, 1);
               SetVirtualCellStop('V', n, m, k, veh.velocity);

               v_sensores[m][n].vehicles_write.push_back(veh);
           }
           else {
               results.arrived_sended = true;
               //v_sensores[m][n].sended++;//TODO ponte chido...
           }
       }
   }

   v_sensores[m][n].vehicles_read = v_sensores[m][n].vehicles_write;//Esta operacion creo que es costosa...
   SwitchVirtualCellRW('V', n, m);

   results.sent_stop = v_sensores[m][n].stop_sended;
   results.received_stop = v_sensores[m][n].stop_received;

   //qDebug() << "Run simulation";

//#endif

}


int localVirtualVehiclePosition(char type_street, char direction, int n, int m, int x)
{

    int i = 0;

    if (type_street == 'H') {

        if (direction == 'R') {

            if (m == 0) {
                if (x >= h_sensores[n][m].position && x <= h_sensores[n][m].p_traffic_light)
                    i = x - h_sensores[n][m].position;
                else
                    i = (d_hor_street - h_sensores[n][m].position) + x;
            }
            else {
                i = x - h_sensores[n][m].position;
            }
        }
        else if (direction == 'L') {

            if (m == 0) {

                if (x <= h_sensores[n][m].position && x >= (h_sensores[n][m].p_traffic_light - 1)) //cuenta tambien la interseccion
                    i = d_side_block - (h_sensores[n][m].position - x);
                else
                    i = h_sensores[n][m].distance_e - (d_hor_street - x);
            }
            else {
                i = d_side_block - (h_sensores[n][m].position - x);
            }
        }
    }
    else {

        if (direction == 'R') {

            if (n == 0) {
                if (x >= v_sensores[m][n].position && x <= v_sensores[m][n].p_traffic_light)
                    i = x - v_sensores[m][n].position;
                else
                    i = (d_ver_street - v_sensores[m][n].position) + x;
            }
            else {
                i = x - v_sensores[m][n].position;
            }
        }
        else if (direction == 'L') {

            if (n == 0) {

                if (x <= v_sensores[m][n].position && x >= (v_sensores[m][n].p_traffic_light - 1)) //cuenta tambien la interseccion
                    i = d_side_block - (v_sensores[m][n].position - x);
                else
                    i = v_sensores[m][n].distance_e - (d_ver_street - x);
            }
            else {
                i = d_side_block - (v_sensores[m][n].position - x);
            }
        }
    }

    return i;
}


void SwitchVirtualCellRW(char type_street, int n, int m)
{
    if (type_street == 'H') {

        if (h_sensores[n][m].switch_virtual_cell == true){

            //Inicializa matriz de lectura
            h_sensores[n][m].pr_street = h_sensores[n][m].virtualCell_A;
            h_sensores[n][m].pw_street = h_sensores[n][m].virtualCell_Z;
        }
        else {

            //Inicializa matriz de lectura
            h_sensores[n][m].pr_street = h_sensores[n][m].virtualCell_Z;
            h_sensores[n][m].pw_street = h_sensores[n][m].virtualCell_A;

        }

        h_sensores[n][m].switch_virtual_cell = !(h_sensores[n][m].switch_virtual_cell);
    }
    else {

        if (v_sensores[m][n].switch_virtual_cell == true){

            //Inicializa matriz de lectura
            v_sensores[m][n].pr_street = v_sensores[m][n].virtualCell_A;
            v_sensores[m][n].pw_street = v_sensores[m][n].virtualCell_Z;
        }
        else {

            //Inicializa matriz de lectura
            v_sensores[m][n].pr_street = v_sensores[m][n].virtualCell_Z;
            v_sensores[m][n].pw_street = v_sensores[m][n].virtualCell_A;

        }

        v_sensores[m][n].switch_virtual_cell = !(v_sensores[m][n].switch_virtual_cell);

    }

}

void SetVirtualCellValue(char type_street, int n, int m, int i, int value)
{

    if (n < 0 || n >= n_hor_streets)
        n = 0;

    if (m < 0 || m >= m_ver_streets)
        m = 0;

    if (i < 0 || i > d_side_block)
        i = 0;

    if (type_street == 'H'){

        h_sensores[n][m].pw_street[i].value = value;

    }
    else {

        v_sensores[m][n].pw_street[i].value = value;
    }

}



void SetVirtualCellStop(char type_street, int n, int m, int i, int vel)
{

    if (n < 0 || n >= n_hor_streets)
        n = 0;

    if (m < 0 || m >= m_ver_streets)
        m = 0;

    if (i < 0 || i > d_side_block)
        i = 0;

    if (type_street == 'H'){

        if (vel == 0)
            h_sensores[n][m].pw_street[i].stop_cell = 1;
        else
            h_sensores[n][m].pw_street[i].stop_cell = 0;

    }
    else {

        if (vel == 0)
            v_sensores[m][n].pw_street[i].stop_cell = 1;
        else
            v_sensores[m][n].pw_street[i].stop_cell = 0;
    }

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
        value = h_sensores[n][m].pr_street[i].value;
    }
    else {
        value = v_sensores[m][n].pr_street[i].value;
    }

    return value;
}



int GetVirtualCellStop(char type_street, int n, int m, int i)
{

    int value = 0;

    if (n < 0 || n >= n_hor_streets)
        n = 0;

    if (m < 0 || m >= m_ver_streets)
        m = 0;

    if (i < 0 || i > d_side_block)
        i = 0;


    if (type_street == 'H'){

        value = h_sensores[n][m].pr_street[i].stop_cell;
    }
    else {

        value = v_sensores[m][n].pr_street[i].stop_cell;
    }

    return value;
}


bool GetReceivedStopVirtualVehicles(char type_street, char direction, int n, int m)
{

    bool tmp_stop = false;

    if (n < 0 || n >= n_hor_streets)
        n = 0;

    if (m < 0 || m >= m_ver_streets)
        m = 0;


    if (type_street == 'H'){

        if (direction == 'R') {

                if (h_sensores[n][m].pr_street[0].stop_cell == 1)
                    return true;
        }
        else {
                if (h_sensores[n][m].pr_street[d_side_block].stop_cell == 1)
                    return true;
        }
    }
    else {

        if (direction == 'R') {

                if (v_sensores[m][n].pr_street[0].stop_cell == 1)
                    return true;
        }
        else {

               if (v_sensores[m][n].pr_street[d_side_block].stop_cell == 1)
                    return true;
        }

    }

    return tmp_stop;
}


bool GetSendedStopVirtualVehicles(char type_street, char direction, int n, int m)
{


    if (n < 0 || n >= n_hor_streets)
        n = 0;

    if (m < 0 || m >= m_ver_streets)
        m = 0;


    if (type_street == 'H'){

        if (direction == 'R') {

            for (int i = (d_side_block - h_sensores[n][m].distance_e); i <= d_side_block; i++)
                if (h_sensores[n][m].pr_street[i].stop_cell == 1)
                    return true;
        }
        else {

            for (int i = h_sensores[n][m].distance_e; i >= 0; i--)
                if (h_sensores[n][m].pr_street[i].stop_cell == 1)
                    return true;

        }

    }
    else {

        if (direction == 'R') {

            for (int i = (d_side_block - v_sensores[m][n].distance_e); i <= d_side_block; i++)
                if (v_sensores[m][n].pr_street[i].stop_cell == 1)
                    return true;
        }
        else {

            for (int i = v_sensores[m][n].distance_e; i >= 0; i--)
                if (v_sensores[m][n].pr_street[i].stop_cell == 1)
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

            for (int i = 0; i <= (d_side_block - h_sensores[n][m].distance_e - 1); i++)
                size+= h_sensores[n][m].pr_street[i].value;
        }
        else {

            for (int i = d_side_block; i >= (h_sensores[n][m].distance_e + 1); i--)
                size+= h_sensores[n][m].pr_street[i].value;

        }
    }
    else {

        if (direction == 'R') {

            for (int i = 0; i <= (d_side_block - v_sensores[m][n].distance_e - 1); i++)
                size+= v_sensores[m][n].pr_street[i].value;
        }
        else {

            for (int i = d_side_block; i >= (v_sensores[m][n].distance_e + 1); i--)
                size+= v_sensores[m][n].pr_street[i].value;

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

            for (int i = (d_side_block - h_sensores[n][m].distance_e); i <= d_side_block; i++)
                size+= h_sensores[n][m].pr_street[i].value;
        }
        else {

            for (int i = h_sensores[n][m].distance_e; i >= 0; i--)
                size+= h_sensores[n][m].pr_street[i].value;

        }

    }
    else {

        if (direction == 'R') {

            for (int i = (d_side_block - v_sensores[m][n].distance_e); i <= d_side_block; i++)
                size+= v_sensores[m][n].pr_street[i].value;
        }
        else {

            for (int i = v_sensores[m][n].distance_e; i >= 0; i--)
                size+= v_sensores[m][n].pr_street[i].value;

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

            h_sensores[n][m].pw_street[i].value = 0;
            h_sensores[n][m].pw_street[i].stop_cell = 0;
        }

    }
    else {

        for (int i = 0; i <= d_side_block; i++) {

            v_sensores[m][n].pw_street[i].value = 0;
            v_sensores[m][n].pw_street[i].stop_cell = 0;
        }

    }

}


int Virtual_Rule_184(char type_street, char direction, int n, int m, int x)
{

    int front = 0;
    int speed = 0;

    if (type_street == 'H') {

        if (x < 0 || x > d_side_block)
            x = 0;

        if (direction == 'R')
            front = x + 1;
        else
            front = x - 1;

    }
    else if (type_street == 'V') {

        if (x < 0 || x > d_side_block)
            x = 0;

        if (direction == 'R')
            front = x + 1;
        else
            front = x - 1;

    }


    if (front > d_side_block || front < 0) {
        speed = 1;

        //qDebug() << "No entrar!!" << x << front;
    }
    else {

        if (GetVirtualCellValue(type_street, n, m, front) == 1)
            speed = 0;
        else
            speed = 1;
    }


    return speed;
}

int Virtual_Rule_252(char type_street, char direction, int n, int m, int x)
{
    int speed;

    speed = 0;

    return speed;

}



SResults2 countVirtualEnvironment(char type_street, int n, int m)
{

    SResults2 results;

    results.n = 0;
    results.m = 0;


    if (type_street == 'H') {

        char direction;
        int pos_t;
        int p_d;
        int p_r;
        int i;

        direction = (n % 2) == 0 ? 'R' : 'L';
        // pos_t = GetPositionTrafficLightSO('H', n, m);

        if (direction == 'R'){

            pos_t = d_side_block - (h_sensores[n][m].distance_e - 1);
            p_d = (pos_t - h_sensores[n][m].distance_d) + 1;
            p_r = pos_t - h_sensores[n][m].distance_r;

            for (int i = p_d; i <= pos_t; i++) {

                if (h_sensores[n][m].pr_street[i].value == 1) {
                    results.n++;
                    if (i >= p_r)
                        results.m++;
                }
            }
        }
        else {

            pos_t = h_sensores[n][m].distance_e + 1;
            p_d = (pos_t + h_sensores[n][m].distance_d) - 1;
            p_r = pos_t + h_sensores[n][m].distance_r;

            for (int i = p_d; i >= pos_t; i--) {

                if (h_sensores[n][m].pr_street[i].value == 1) {
                    results.n++;
                    if (i <= p_r)
                        results.m++;
                }
            }
        }
    }
    else {

        char direction;
        int pos_t;
        int p_d;
        int p_r;
        int i;

        direction = (m % 2) == 0 ? 'R' : 'L';
        //pos_t = GetPositionTrafficLightSO('V', n, m);

        if (direction == 'R'){

            pos_t = d_side_block - (v_sensores[m][n].distance_e - 1);
            p_d = (pos_t - v_sensores[m][n].distance_d) + 1;
            p_r = pos_t - v_sensores[m][n].distance_r;

            for (int i = p_d; i <= pos_t; i++) {

               // qDebug() << "sensors : " << v_sensores[m][n].pr_street[i].value;

                if (v_sensores[m][n].pr_street[i].value == 1) {
                    results.n++;
                    if (i >= p_r)
                        results.m++;
                }
            }
        }
        else {

            pos_t = v_sensores[m][n].distance_e + 1;
            p_d = (pos_t + v_sensores[m][n].distance_d) - 1;
            p_r = pos_t + v_sensores[m][n].distance_r;

            for (int i = p_d; i >= pos_t; i--) {

                if (v_sensores[m][n].pr_street[i].value == 1) {
                    results.n++;
                    if (i <= p_r)
                        results.m++;
                }
            }
        }
    }

    return results;

}

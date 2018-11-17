#ifndef SLOT_SYSTEM_CPP
#define SLOT_SYSTEM_CPP

#include "slot_system.h"

SSlotBasedSystem **slot_system;

int calculateT1()
{
    int T1 = 1;
    return T1;
}

int calculateT2()
{
    int T2 = 0;

    if (model == 1) {//184 ACE
        float v_asterisk = minimum(vmax, sqrt(2 * 1 * (ls + 1)));
        if (v_asterisk > 0)
            T2 = ceil((v_asterisk / (2.0 * 1)) + ((ls + 1.0) / v_asterisk));
        else
            qDebug() << "ERROR T2, v_asterisk: " << v_asterisk;

        // qDebug() << "v*: " << v_asterisk  << "T2: " << T2;
    }
    else {//LAI AC
        float t_res = 0.0;
        float v_asterisk = minimum(vmax, sqrt(2 * M * (ls + 1)));
        if (v_asterisk > 0)
            T2 = ceil(t_res + (v_asterisk / (2.0 * M)) + ((ls + 1.0) / v_asterisk));
        else
            qDebug() << "ERROR T2, v_asterisk: " << v_asterisk;

        //qDebug() << "v*: " << v_asterisk  << "T2: " << T2;
    }

    // T2 = 1;

    return T2;
}

int calculateATV(int id, int position_intersection, char dir, int distance_street, int atv1)
{
    int ATV;

    //184 ACE
    if (model == 1) {
        ATV = calculateDistance(GetVehicle(id).position.x, position_intersection, dir, distance_street) + 1;
    }
    else {//LAI AC

        int vel2;
        int distance2;
        int atv2;
        int vx2;

        atv2 = 0;
        vx2 = 0;

        vel2 = GetVehicle(id).speed;
        distance2 = calculateDistance(GetVehicle(id).position.x, position_intersection, dir, distance_street) + 1;
        vx2 =  minimum(vmax , (int) ceil(sqrt(vel2 * vel2 + (2.0 * delta_v * distance2))));//Para el caso donde LAI solo acelera se puede considerar como aceleracion constate

        if ((vel2 + vx2) > 0)
            atv2 = (int) ceil((float) distance2 / ((float)(vel2 + vx2) / 2.0));
        else
            atv2 = 0;

        int T1 = 1;
        ATV = maximum(atv2, atv1 + T1);

        if (atv1 > ATV)
            qDebug() << "Atv: " << atv1 << atv2;

    }


    return ATV;
}

void InializedSlotSystem(int N)
{
    int n, m;
    int x, y;

    allocateMemorySlotSystem();

    for (n = 0; n < n_hor_streets; n++){
        for (m = 0; m < m_ver_streets; m++){

            slot_system[n][m].tl = 0;
            slot_system[n][m].Tsep = 0;
            slot_system[n][m].N = N;//N = 1 es FAIR, N > 1 es BATCH
            slot_system[n][m].dl = 'H';
            slot_system[n][m].occupancy_id = INVALID;

            slot_system[n][m].T1 = calculateT1();//1;//
            slot_system[n][m].T2 = calculateT2();//ceil((v / (2 * a_brake)) + ((l + s) / v));

            slot_system[n][m].batch.clear();
            slot_system[n][m].ordered_list.clear();

            slot_system[n][m].id_slot_last_h = INVALID;
            slot_system[n][m].id_slot_last_v = INVALID;

            slot_system[n][m].state = 0; //0.-Sin bloqueos 3.- Ambos bloqueados 2.- Bloqueado en H 1.- Bloqueado en V

            x = m * d_side_block + m;
            y = n * d_side_block + n;

            if (n % 2 == 0) {
                slot_system[n][m].direction_h = 'R';
                if (x == 0) {
                    slot_system[n][m].pos_h = d_hor_street - 1;
                    slot_system[n][m].pos_intersection_h = 0;
                }
                else {
                    slot_system[n][m].pos_h = x - 1;
                    slot_system[n][m].pos_intersection_h = x;
                }
            }
            else {
                slot_system[n][m].direction_h = 'L';
                slot_system[n][m].pos_h = x + 1;
                slot_system[n][m].pos_intersection_h = x;
            }

            if (m % 2 == 0) {
                slot_system[n][m].direction_v = 'R';
                if (y == 0){
                    slot_system[n][m].pos_v = d_ver_street - 1;
                    slot_system[n][m].pos_intersection_v = 0;
                }
                else {
                    slot_system[n][m].pos_v = y - 1;
                    slot_system[n][m].pos_intersection_v = y;
                }
            }
            else {
                slot_system[n][m].direction_v = 'L';
                slot_system[n][m].pos_v = y + 1;
                slot_system[n][m].pos_intersection_v = y;
            }
        }
    }
}

int GetValueSlotBasedSystem(char type_street, int n, int m, int id)
{

    if (n < 0 || m < 0)
        return -1;//no definido

    if (n >= n_hor_streets)
        n = 0;

    if (m >= m_ver_streets)
        m = 0;

    if (slot_system[n][m].state == 3) //Bloqueo en ambos sentidos
        return 0;
    else if (slot_system[n][m].state == 2) {
        if (type_street == 'H')
            return 0;
        else
            return 1;

    } else if (slot_system[n][m].state == 1){
        if (type_street == 'V')
            return 0;
        else
            return 1;
    }

    int state_light = GetOccupancyIdSlotSystem(n, m) == id ? 1 : 0;





    /*
    if (type_street == 'H')
        return 1;
    else
        return 0;*/



    return state_light;
}


void slotBasedSystem(int n, int m)
{
    std::list<SSlot> batch_H;
    std::list<SSlot> batch_V;
    std::list<SSlot>::iterator it;
    SSlot slot;

    if (slot_rule_e(n, m) < 0)
        return;

    insertSlotsList(n, m);



    /*
    it = slot_system[n][m].ordered_list.begin();
    while (it != slot_system[n][m].ordered_list.end()) {
        if (isIDStillValid(n, m, (*it).id) == false) {
            it = slot_system[n][m].ordered_list.erase(it); // alternatively, items.erase(it++);
        }
        //else {
          //  break;
        //}

        it++;
    }*/

    /*
    if (slot_system[n][m].ordered_list.empty() != true) {
        if (isIDStillValid(n, m, slot_system[n][m].ordered_list.front().id) == false)
            slot_system[n][m].ordered_list.pop_front();
    }


    if (n == 1 && m == 1) {
        //printBatch(n, m);
        //calculateOrderedList(n, m);
        updateAtv(n, m);
        printList(n, m);
    }
*/


#if 1

    if (slot_system[n][m].ordered_list.empty() != true) {
        if (slot_system[n][m].batch.empty() == true) {

            calculateOrderedList(n, m);

            //printList(n, m);

            slot = slot_system[n][m].ordered_list.front();

            if (slot.type_street == slot_system[n][m].dl)
                slot_system[n][m].Tsep = slot_system[n][m].T1;
            else
                slot_system[n][m].Tsep = slot_system[n][m].T2;

            slot_system[n][m].tl = maximum(slot_system[n][m].Tsep + slot_system[n][m].tl, slot.atv);

            batch_H.clear();
            batch_V.clear();

            int counting_batch = 0;
            for (it=slot_system[n][m].ordered_list.begin(); it != slot_system[n][m].ordered_list.end(); ++it){
                if ((*it).atv <= slot_system[n][m].tl && counting_batch < slot_system[n][m].N) {
                    if ((*it).type_street == 'H')
                        batch_H.push_back((*it));
                    else
                        batch_V.push_back((*it));

                    counting_batch++;
                }
                else
                    break;
            }

            if (counting_batch > 0)
                slot_system[n][m].ordered_list.erase(slot_system[n][m].ordered_list.begin(), it);

            int tf = slot_system[n][m].tl;

            if (slot.type_street == 'H') {
                for (it = batch_H.begin(); it != batch_H.end(); ++it){
                    (*it).tv = tf;
                    if ((*it).id == batch_H.back().id) {
                        if (batch_V.empty() != true)
                            tf = tf + slot_system[n][m].T2;
                    }
                    else
                        tf = tf + slot_system[n][m].T1;
                }

                for (it = batch_V.begin(); it != batch_V.end(); ++it){
                    (*it).tv = tf;
                    tf = tf + slot_system[n][m].T1;
                    //No he puesto la condicion final del algortimo del paper porque me parece es innecesaria
                }

                it = slot_system[n][m].batch.begin();
                slot_system[n][m].batch.insert(it, batch_H.begin(), batch_H.end());

                if (batch_V.empty() != true) {
                    it = slot_system[n][m].batch.end();
                    slot_system[n][m].batch.insert(it, batch_V.begin(), batch_V.end());
                }
            }
            else {

                for (it = batch_V.begin(); it != batch_V.end(); ++it){
                    (*it).tv = tf;
                    if ((*it).id == batch_V.back().id) {
                        if (batch_H.empty() != true)
                            tf = tf + slot_system[n][m].T2;
                    }
                    else
                        tf = tf + slot_system[n][m].T1;
                }

                for (it = batch_H.begin(); it != batch_H.end(); ++it){
                    (*it).tv = tf;
                    tf = tf + slot_system[n][m].T1;
                    //No he puesto la condicion final del algortimo del paper porque me pare es innecesaria
                }

                it = slot_system[n][m].batch.begin();
                slot_system[n][m].batch.insert(it, batch_V.begin(), batch_V.end());

                if (batch_H.empty() != true) {
                    it = slot_system[n][m].batch.end();
                    slot_system[n][m].batch.insert(it, batch_H.begin(), batch_H.end());
                }
            }
            //slot_system[n][m].batch.sort(compare_tv);//No es necesario
            slot_system[n][m].tl = slot_system[n][m].batch.back().tv;
            slot_system[n][m].dl = slot_system[n][m].batch.back().type_street;
        }
    }


    //pensandolo detenidamente porque verificar todos?
    it = slot_system[n][m].batch.begin();
    while (it != slot_system[n][m].batch.end()) {
        if (isIDStillValid(n, m, (*it).id) == false) {
            it = slot_system[n][m].batch.erase(it); // alternatively, items.erase(it++);
        }
        //else {
        //  break;
        //}
        it++;
    }

    /*
    if (slot_system[n][m].batch.empty() != true) {
        if (isIDStillValid(n, m, slot_system[n][m].batch.front().id) == false)
            slot_system[n][m].batch.pop_front();
    }*/

    slot_system[n][m].occupancy_id = INVALID;
    if (slot_system[n][m].batch.empty() != true) {
        slot_system[n][m].occupancy_id = slot_system[n][m].batch.front().id;
    }


    //bool vehicle_stop_H2 = GetVehiclesStopped('H', n, slot_system[n][m].pos_h);
    // bool vehicle_stop_V2 = GetVehiclesStopped('V', m, slot_system[n][m].pos_v);



    /* if (n == 1 && m == 1) {
        qDebug() << "ID: " << slot_system[n][m].occupancy_id;// << "H: " << GetIDCellStreet('H', n, slot_system[n][m].pos_h) << "V: " << GetIDCellStreet('V', m, slot_system[n][m].pos_v);

        printBatch(n, m);
        printList(n, m);
    }*/


    /*
    if (vehicle_stop_H2 == true && vehicle_stop_V2 == true) {


        if (slot_system[n][m].occupancy_id != GetIDCellStreet('H', n, slot_system[n][m].pos_h) && slot_system[n][m].occupancy_id != GetIDCellStreet('V', m, slot_system[n][m].pos_v)) {
            qDebug() << "ID: " << slot_system[n][m].occupancy_id << "H: " << GetIDCellStreet('H', n, slot_system[n][m].pos_h) << "V: " << GetIDCellStreet('V', m, slot_system[n][m].pos_v);

            if (slot_system[n][m].occupancy_id != INVALID) {

                qDebug() << "Pos ID: " << slot_system[n][m].occupancy_id << GetVehicle(slot_system[n][m].occupancy_id).position.x << GetVehicle(slot_system[n][m].occupancy_id).type_street;

                printBatch(n, m);
                printList(n, m);


            }
        }
    }*/


#endif

}

bool isIDStillValid(int n, int m, int id)
{
    SVehicle vehicle =  GetVehicle(id);
    int k_intersection = GetIndexIntersection(vehicle.type_street, vehicle.position.x, vehicle.direction);

    if (vehicle.type_street == 'H') {
        if (m == k_intersection)
            return true;
    }
    else {
        if (n == k_intersection)
            return true;
    }

    return false;

}

#if 0
bool isIDStillValid(int n, int m, int id)
{
    SVehicle vehicle =  GetVehicle(id);
    int x = vehicle.position.x;

    int pos_inter_x;
    int d_street;
    int number_intersections;

    if (vehicle.type_street == 'H') {
        d_street = d_hor_street;
        number_intersections = m_ver_streets;
        pos_inter_x = slot_system[n][m].pos_intersection_h;
    }
    else {
        d_street = d_ver_street;
        number_intersections = n_hor_streets;
        pos_inter_x = slot_system[n][m].pos_intersection_v;
    }

    if (number_intersections > 1) {//Evitar el caso donde la entrada es lo mismo que la salida (condiciones periodicas en la frontera)

        if (vehicle.direction == 'R') {//direccion derecha
            if (pos_inter_x == 0) {
                if (x > pos_inter_x && x < (d_street - d_side_block))
                    return false;
            } else {
                if (x > pos_inter_x)
                    return false;
            }
        }
        else if (vehicle.direction == 'L') {//direccion izquierda
            if (pos_inter_x == 0) {
                if (x < d_street && x >= (d_street - d_side_block))
                    return false;
            } else {
                if (x < pos_inter_x)
                    return false;
            }
        }
    }

    return true;
}
#endif


int slot_rule_e(int n, int m)
{
    bool vehicle_stop_H, vehicle_stop_V;
    vehicle_stop_H = VehiclesStoppedDistance_e('H', n, m, stopped_distance);
    vehicle_stop_V = VehiclesStoppedDistance_e('V', n, m, stopped_distance);

    //qDebug() << "Que pues" << vehicle_stop_H << vehicle_stop_V << rand();

    slot_system[n][m].state = 0;//Sin bloqueos
    if (vehicle_stop_H == true && vehicle_stop_V == true)
        slot_system[n][m].state = 3; //Bloqueo en ambos sentidos
    else if(vehicle_stop_H == true)
        slot_system[n][m].state = 2; //Bloqueo en H
    else if(vehicle_stop_V == true)
        slot_system[n][m].state = 1; //Bloqueo en V

    if ((vehicle_stop_H == true || vehicle_stop_V == true)){
        slot_system[n][m].batch.clear();
        slot_system[n][m].ordered_list.clear();
        slot_system[n][m].id_slot_last_h = INVALID;
        slot_system[n][m].id_slot_last_v = INVALID;

        return -1;
    }

    return 0;

}

void insertSlotsList(int n, int m)
{
    int id_vehicle;
    int d_street;
    int pos_inter_x;
    int number_intersections;
    int start, end;

    SSlot slot;
    SVehicle vehicle;
    std::list<SSlot> list_H;
    std::list<SSlot> list_V;

    char type_street_intersection = INVALID;

    //slot_system[n][m].id_slot_last_h = INVALID;
    //slot_system[n][m].id_slot_last_v = INVALID;

    if (slot_system[n][m].id_slot_last_h != INVALID) {
        if (isIDStillValid(n, m, slot_system[n][m].id_slot_last_h) == false) {
            slot_system[n][m].id_slot_last_h = INVALID;
        }
    }

    number_intersections = m_ver_streets;
    if (number_intersections > 1) {//Evitar el caso donde la entrada es lo mismo que la salida (condiciones periodicas en la frontera)
        d_street = d_hor_street;
        pos_inter_x = slot_system[n][m].pos_intersection_h;

        if (slot_system[n][m].direction_h == 'R') {
            if (slot_system[n][m].id_slot_last_h != INVALID) {
                vehicle = GetVehicle(slot_system[n][m].id_slot_last_h);
                start = (slot_system[n][m].pos_h - d_side_block) + 1;
                if (vehicle.position.x == 0)
                    end = d_street - vehicle.rear_position - 1;
                else
                    end = vehicle.rear_position - 1;
            } else {
                start = slot_system[n][m].pos_h - d_side_block + 1;
                end = slot_system[n][m].pos_h;
                type_street_intersection = 'H';
            }

            rightInsert('H', n, start, end, pos_inter_x, d_street, list_H);
        }
        else {//direccion izquierda
            if (slot_system[n][m].id_slot_last_h != INVALID) {
                vehicle = GetVehicle(slot_system[n][m].id_slot_last_h);
                start = vehicle.rear_position + 1;
                end = (slot_system[n][m].pos_h + d_side_block) - 1;
            }
            else {
                start = slot_system[n][m].pos_h;
                end = slot_system[n][m].pos_h + d_side_block - 1;
                type_street_intersection = 'H';
            }

            leftInsert('H', n, start, end, pos_inter_x, d_street, list_H);
        }
    }

    if (slot_system[n][m].id_slot_last_v != INVALID) {
        if (isIDStillValid(n, m, slot_system[n][m].id_slot_last_v) == false)
            slot_system[n][m].id_slot_last_v = INVALID;
    }

    number_intersections = n_hor_streets;
    if (number_intersections > 1) {//Evitar el caso donde la entrada es lo mismo que la salida (condiciones periodicas en la frontera)
        d_street = d_ver_street;
        pos_inter_x = slot_system[n][m].pos_intersection_v;
        if (slot_system[n][m].direction_v == 'R') {
            if (slot_system[n][m].id_slot_last_v != INVALID) {
                vehicle = GetVehicle(slot_system[n][m].id_slot_last_v);
                start = (slot_system[n][m].pos_v - d_side_block) + 1;
                if (vehicle.position.x == 0)
                    end = d_street - vehicle.rear_position - 1;
                else
                    end = vehicle.rear_position - 1;
            } else {
                start = slot_system[n][m].pos_v - d_side_block + 1;
                end = slot_system[n][m].pos_v;
                type_street_intersection = 'V';
            }

            rightInsert('V', m, start, end, pos_inter_x, d_street, list_V);
        }
        else {//direccion izquierda
            if (slot_system[n][m].id_slot_last_v != INVALID) {
                vehicle = GetVehicle(slot_system[n][m].id_slot_last_v);
                start = vehicle.rear_position + 1;
                end = (slot_system[n][m].pos_v + d_side_block) - 1;
            }
            else {
                start = slot_system[n][m].pos_v;
                end = slot_system[n][m].pos_v + d_side_block - 1;
                type_street_intersection = 'V';
            }

            leftInsert('V', m, start, end, pos_inter_x, d_street, list_V);
        }
    }

    //Vehiculo en interseccion
    if (type_street_intersection == 'H') {
        pos_inter_x = slot_system[n][m].pos_intersection_h;
        if (GetValueCellStreet('H', n, pos_inter_x) == 1) {
            if (GetVisibleCellStreet('H', n, pos_inter_x) == true){
                id_vehicle = GetIDCellStreet('H', n, pos_inter_x);
                if (GetVehicle(id_vehicle).type_street == 'H') {
                    slot.id = id_vehicle;
                    slot.type_street = 'H';
                    slot.atv = 0;
                    slot.tv = 0;
                    list_H.push_front(slot);
                }
            }
        }
    }else if (type_street_intersection == 'V') {
        pos_inter_x = slot_system[n][m].pos_intersection_v;
        if (GetValueCellStreet('V', m, pos_inter_x) == 1) {
            if (GetVisibleCellStreet('V', m, pos_inter_x) == true){
                id_vehicle = GetIDCellStreet('V', m, pos_inter_x);
                if (GetVehicle(id_vehicle).type_street == 'V'){
                    slot.id = id_vehicle;
                    slot.type_street = 'V';
                    slot.atv = 0;
                    slot.tv = 0;
                    list_V.push_front(slot);
                }
            }
        }
    }


    if (list_H.empty() != true) {
        slot_system[n][m].ordered_list.insert(slot_system[n][m].ordered_list.end(), list_H.begin(), list_H.end());
        slot_system[n][m].id_slot_last_h = list_H.back().id;
        list_H.clear();
    }

    if (list_V.empty() != true) {
        slot_system[n][m].ordered_list.insert(slot_system[n][m].ordered_list.end(), list_V.begin(), list_V.end());
        slot_system[n][m].id_slot_last_v = list_V.back().id;
        list_V.clear();
    }


}

void leftInsert(char type_street, int y, int _start, int _end, int _pos_inter_x, int _d_street, std::list<SSlot>& list)
{
    SSlot slot;

    for (int pos_x = _start; pos_x <= _end; pos_x++) {
        if (GetValueCellStreet(type_street, y, pos_x) == 1) {
            if (GetVisibleCellStreet(type_street, y, pos_x) == true){
                int id_vehicle = GetIDCellStreet(type_street, y, pos_x);
                slot.id = id_vehicle;
                slot.type_street = type_street;
                slot.atv = 0;//calculateATV(id_vehicle, _pos_inter_x, 'L', _d_street, 0);
                slot.tv = 0;
                list.push_back(slot);
            }
        }
    }
}

void rightInsert(char type_street, int y, int _start, int _end, int _pos_inter_x, int _d_street, std::list<SSlot>& list)
{
    SSlot slot;

    for (int pos_x = _end; pos_x >= _start; pos_x--) {
        if (GetValueCellStreet(type_street, y, pos_x) == 1) {
            if (GetVisibleCellStreet(type_street, y, pos_x) == true){
                int id_vehicle = GetIDCellStreet(type_street, y, pos_x);
                slot.id = id_vehicle;
                slot.type_street = type_street;
                slot.atv = 0; //calculateATV(id_vehicle, _pos_inter_x, 'R', _d_street, 0);
                slot.tv = 0;
                list.push_back(slot);
            }
        }
    }
}

void updateAtv(int n, int m)
{
    int d_street;
    int pos_inter_x;
    SVehicle tmp_vehicle;

    int atv_h1;
    int atv_v1;
    bool first_h = true;
    bool first_v = true;

    atv_h1 = 0;
    atv_v1 = 0;
    std::list<SSlot>::iterator it;
    for (it=slot_system[n][m].ordered_list.begin(); it!=slot_system[n][m].ordered_list.end(); ++it) {
        tmp_vehicle = GetVehicle((*it).id);
        if (tmp_vehicle.type_street == 'H'){
            d_street = d_hor_street;
            pos_inter_x = slot_system[n][m].pos_intersection_h;

            if (first_h == true) {
                int id_front_vehicle = searchVehicleFrontSameBlockID(tmp_vehicle.id);
                if (id_front_vehicle != INVALID)
                    atv_h1 = calculateATV(id_front_vehicle, pos_inter_x, GetVehicle(id_front_vehicle).direction, d_street, 0);//Como el vehiculo frontal esta en el batch y no en la lista, entonces se asume el atv1 a 0
                first_h = false;
            }

            (*it).atv = calculateATV(tmp_vehicle.id, pos_inter_x, tmp_vehicle.direction, d_street, atv_h1);
            atv_h1 = (*it).atv;
        }
        else {
            d_street = d_ver_street;
            pos_inter_x = slot_system[n][m].pos_intersection_v;

            if (first_v == true) {
                int id_front_vehicle = searchVehicleFrontSameBlockID(tmp_vehicle.id);
                if (id_front_vehicle != INVALID)
                    atv_v1 = calculateATV(id_front_vehicle, pos_inter_x, GetVehicle(id_front_vehicle).direction, d_street, 0);//Como el vehiculo frontal esta en el batch y no en la lista, entonces se asume el atv1 a 0
                first_v = false;
            }

            (*it).atv = calculateATV(tmp_vehicle.id, pos_inter_x, tmp_vehicle.direction, d_street, atv_v1);
            atv_v1 = (*it).atv;
        }
    }

}

void calculateOrderedList(int n, int m)
{
    updateAtv(n, m);
    slot_system[n][m].ordered_list.sort(compare_atv);
}

int GetOccupancyIdSlotSystem(int n, int m)
{
    return slot_system[n][m].occupancy_id;
}

int GetPositionDirectionSlotSystem(char type_street, int n, int m)
{
    if (type_street == 'H')
        return slot_system[n][m].pos_h;
    else
        return slot_system[n][m].pos_v;

    return INVALID;
}

int GetPositionIntersectionSlotSystem(char type_street, int n, int m)
{
    if (type_street == 'H')
        return slot_system[n][m].pos_intersection_h;
    else
        return slot_system[n][m].pos_intersection_v;

    return INVALID;
}

bool compare_atv(const SSlot &a, const SSlot &b)
{
    return a.atv < b.atv;
}

bool compare_tv(const SSlot &a, const SSlot &b)
{
    return a.tv < b.tv;
}

void printList(int n, int m)
{
    qDebug() << "*********Ordenado, n x m: " << n << " " << m << "size: " << slot_system[n][m].ordered_list.size();

    std::list<SSlot>::iterator it;
    for (it=slot_system[n][m].ordered_list.begin(); it!=slot_system[n][m].ordered_list.end(); ++it)
        qDebug() << "ORDER LIST ID:" << (*it).id << (*it).type_street << "Atv:" << (*it).atv << "Pos:" << GetVehicle((*it).id).position.x;

    qDebug() << "******************************************";
}

void printBatch(int n, int m)
{
    qDebug() << "*********BATCH, n x m: " << n << " " << m << "size: " << slot_system[n][m].batch.size();

    std::list<SSlot>::iterator it;
    for (it=slot_system[n][m].batch.begin(); it!=slot_system[n][m].batch.end(); ++it)
        qDebug() << "ORDER BATCH ID:" << (*it).id << (*it).type_street << "Atv:" << (*it).atv << "Tv:" << (*it).tv << "Type:" << (*it).type_street;

    qDebug() << "******************************************";
}


void allocateMemorySlotSystem()
{
    slot_system = NULL;

    slot_system = new SSlotBasedSystem*[n_hor_streets];
    for (int i = 0; i < n_hor_streets; i++)
        slot_system[i] = new SSlotBasedSystem[m_ver_streets];
}

void freeSlotSystem()
{
    if (slot_system != NULL) {
        for (int i = 0; i < n_hor_streets; i++)
            delete [] slot_system[i];
        delete [] slot_system;
        slot_system = NULL;
    }
}

#endif // SLOT_SYSTEM_CPP

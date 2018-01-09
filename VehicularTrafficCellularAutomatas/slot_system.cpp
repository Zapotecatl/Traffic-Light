#ifndef SLOT_SYSTEM_CPP
#define SLOT_SYSTEM_CPP

#include "slot_system.h"

SSlotBasedSystem **slot_system;

int calculateT1()
{
    int T1;
    //184 ACE
    if (model == 1) {
        T1 = 1;
    }
    else {//LAI AC
        T1 = 1;
    }
    return T1;
}

int calculateT2(int vel)
{
    int T2;
    //184 ACE
    if (model == 1) {
        T2 = ls;
    }
    else {//LAI AC

        //T2 = 1;
        if (M > 0 && vel >= M)
            T2 = (vel - M) / M + ls;
        else
            T2 = ls;
    }
    return T2;
}

int calculateATV(int vel, int position_1, int position_2, char dir, int distance_street)
{
    int ATV;



    //184 ACE
    if (model == 1) {
        ATV = calculateDistance(position_1, position_2, dir, distance_street) + 1;
    }
    else {//LAI AC

        ATV = calculateDistance(position_1, position_2, dir, distance_street) + 1;
        /*
        if (vel > 1)
            ATV = ceil((float)(calculateDistance(position_1, position_2, dir, distance_street) + 1.0) / (float)vel);
        else
            ATV = calculateDistance(position_1, position_2, dir, distance_street) + 1;
            */
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

            slot_system[n][m].t_res = 0.5;
            slot_system[n][m].tl = 0;
            slot_system[n][m].Tsep = 0;
            slot_system[n][m].N = N;//N = 1 es FAIR, N > 1 es BATCH
            slot_system[n][m].dl = 'H';
            slot_system[n][m].occupancy_id = INVALID;

            slot_system[n][m].T1 = calculateT1();//1;//ceil(slot_system[n][m].t_res + 0.5);
            slot_system[n][m].T2 = calculateT2(0);//ceil(slot_system[n][m].t_res + (v / (2 * a_brake)) + ((l + s) / v));

            slot_system[n][m].batch.clear();
            slot_system[n][m].ordered_list.clear();
            slot_system[n][m].list_H.clear();
            slot_system[n][m].list_V.clear();

            slot_system[n][m].slot_last_h.id = INVALID;
            slot_system[n][m].slot_last_v.id = INVALID;

            slot_system[n][m].vehicle_stop_H = false;
            slot_system[n][m].vehicle_stop_V = false;

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

    return state_light;
}


void slotBasedSystem(int n, int m)
{
    std::list<SSlot> batch_H;
    std::list<SSlot> batch_V;
    std::list<SSlot>::iterator it;
    SSlot slot;

    bool vehicle_stop_H, vehicle_stop_V;

    vehicle_stop_H = VehiclesStoppedDistance_e('H', n, m, stopped_distance);
    vehicle_stop_V = VehiclesStoppedDistance_e('V', n, m, stopped_distance);
    slot_system[n][m].state = 0;//Sin bloqueos

    //qDebug() << "Que pues" << vehicle_stop_H << vehicle_stop_V << rand();

    if (vehicle_stop_H == true && vehicle_stop_V == true) {
        slot_system[n][m].state = 3; //Bloqueo en ambos sentidos
        slot_system[n][m].vehicle_stop_H = vehicle_stop_H;
        slot_system[n][m].vehicle_stop_V = vehicle_stop_V;
        return;
    }
    else if(vehicle_stop_H == true) {
        slot_system[n][m].state = 2; //Bloqueo en H
        slot_system[n][m].vehicle_stop_H = vehicle_stop_H;
        slot_system[n][m].vehicle_stop_V = vehicle_stop_V;
        return;
    }
    else if(vehicle_stop_V == true) {
        slot_system[n][m].state = 1; //Bloqueo en V
        slot_system[n][m].vehicle_stop_H = vehicle_stop_H;
        slot_system[n][m].vehicle_stop_V = vehicle_stop_V;
        return;
    }

    //Reiniciar?
    if (slot_system[n][m].vehicle_stop_H == true || slot_system[n][m].vehicle_stop_V == true){
        slot_system[n][m].T1 = calculateT1();
        slot_system[n][m].T2 = calculateT2(0);//TODO que pasa con el 0?
        slot_system[n][m].batch.clear();
        slot_system[n][m].ordered_list.clear();
        slot_system[n][m].list_H.clear();
        slot_system[n][m].list_V.clear();
        slot_system[n][m].slot_last_h.id = INVALID;
        slot_system[n][m].slot_last_v.id = INVALID;
        slot_system[n][m].vehicle_stop_H = false;
        slot_system[n][m].vehicle_stop_V = false;
    }

#if 1

    insertSlotsList(n, m);
    calculateOrderedList(n, m);

    if (slot_system[n][m].ordered_list.empty() != true) {

        if (slot_system[n][m].batch.empty() == true) {
            slot = slot_system[n][m].ordered_list.front();

            if (slot.type_street == slot_system[n][m].dl)
                slot_system[n][m].Tsep = slot_system[n][m].T1;
            else {
                slot_system[n][m].T2 = calculateT2(GetVehicle(slot.id).speed);
                slot_system[n][m].Tsep = slot_system[n][m].T2;
            }

            slot_system[n][m].tl = max(slot_system[n][m].Tsep + slot_system[n][m].tl, slot.atv);

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

    int d_street, number_intersections;
    int pos_inter_x;
    int id_vehicle;
    int x;

    it = slot_system[n][m].batch.begin();
    while (it != slot_system[n][m].batch.end()) {
        id_vehicle = (*it).id;
        SVehicle vehicle = GetVehicle(id_vehicle);
        x = vehicle.position.x;

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
            bool pop = false;
            if (vehicle.direction == 'R') {//direccion derecha
                if (pos_inter_x == 0) {
                    if (x > pos_inter_x && x < (d_street - d_side_block))
                        pop = true;
                } else {
                    if (x > pos_inter_x)
                        pop = true;
                }
            }
            else if (vehicle.direction == 'L') {//direccion izquierda
                if (pos_inter_x == 0) {
                    if (x < d_street && x >= (d_street - d_side_block))
                        pop = true;
                } else {
                    if (x < pos_inter_x)
                        pop = true;
                }
            }

            if (pop == true) {
                it = slot_system[n][m].batch.erase(it); // alternatively, items.erase(it++);

            }
            else {
                break;
            }
        }
    }

    slot_system[n][m].occupancy_id = INVALID;
    if (slot_system[n][m].batch.empty() != true) {
        slot_system[n][m].occupancy_id = slot_system[n][m].batch.front().id;
    }

    /*if (n == 0 && m == 0)
        qDebug() << GetVehicle(slot_system[n][m].occupancy_id).type_street << "ID: "<<slot_system[n][m].occupancy_id << "Pos:" << GetVehicle(slot_system[n][m].occupancy_id).position.x << slot_system[n][m].batch.empty();
    if (n == 0 && m == 0 && slot_system[n][m].batch.empty() == false) {
        qDebug() << "*********Batch, n x m: " << n << " " << m << "size: " << slot_system[n][m].batch.size();
        for (it=slot_system[n][m].batch.begin(); it!=slot_system[n][m].batch.end(); ++it)
            qDebug() << (*it).type_street << "ID:" << (*it).id << "Tv:" << (*it).tv;
        qDebug() << "******************************************";
    }*/

#endif

}


void insertSlotsList(int n, int m)
{
    int id_vehicle;
    int d_street;
    int pos_inter_x;
    int number_intersections;
    int start, end;
    char type_street_intersection = INVALID;
    SSlot slot;
    SVehicle vehicle;

    number_intersections = m_ver_streets;
    if (number_intersections > 1) {//Evitar el caso donde la entrada es lo mismo que la salida (condiciones periodicas en la frontera)
        d_street = d_hor_street;
        pos_inter_x = slot_system[n][m].pos_intersection_h;

        if (slot_system[n][m].direction_h == 'R') {
            if (slot_system[n][m].slot_last_h.id != INVALID) {
                vehicle = GetVehicle(slot_system[n][m].slot_last_h.id);
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

            for (int pos_x = end; pos_x >= start; pos_x--) {
                if (GetValueCellStreet('H', n, pos_x) == 1) {
                    if (GetVisibleCellStreet('H', n, pos_x) == true){
                        id_vehicle = GetIDCellStreet('H', n, pos_x);
                        slot.id = id_vehicle;
                        slot.type_street = 'H';
                        slot.atv = calculateATV(GetVehicle(id_vehicle).speed, pos_x, pos_inter_x, slot_system[n][m].direction_h, d_street);
                        slot.tv = 0;
                        slot_system[n][m].list_H.push_back(slot);
                    }
                }
            }
        }
        else {//direccion izquierda
            if (slot_system[n][m].slot_last_h.id != INVALID) {
                vehicle = GetVehicle(slot_system[n][m].slot_last_h.id);
                start = vehicle.rear_position + 1;
                end = (slot_system[n][m].pos_h + d_side_block) - 1;
            }
            else {
                start = slot_system[n][m].pos_h;
                end = slot_system[n][m].pos_h + d_side_block - 1;
                type_street_intersection = 'H';
            }

            for (int pos_x = start; pos_x <= end; pos_x++) {
                if (GetValueCellStreet('H', n, pos_x) == 1) {
                    if (GetVisibleCellStreet('H', n, pos_x) == true){
                        id_vehicle = GetIDCellStreet('H', n, pos_x);
                        slot.id = id_vehicle;
                        slot.type_street = 'H';
                        slot.atv = calculateATV(GetVehicle(id_vehicle).speed, pos_x, pos_inter_x, slot_system[n][m].direction_h, d_street);
                        slot.tv = 0;
                        slot_system[n][m].list_H.push_back(slot);
                    }
                }
            }
        }
    }

    number_intersections = n_hor_streets;
    if (number_intersections > 1) {//Evitar el caso donde la entrada es lo mismo que la salida (condiciones periodicas en la frontera)
        d_street = d_ver_street;
        pos_inter_x = slot_system[n][m].pos_intersection_v;
        if (slot_system[n][m].direction_v == 'R') {
            if (slot_system[n][m].slot_last_v.id != INVALID) {
                vehicle = GetVehicle(slot_system[n][m].slot_last_v.id);
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

            for (int pos_x = end; pos_x >= start; pos_x--) {
                if (GetValueCellStreet('V', m, pos_x) == 1) {
                    if (GetVisibleCellStreet('V', m, pos_x) == true){
                        id_vehicle = GetIDCellStreet('V', m, pos_x);
                        slot.id = id_vehicle;
                        slot.type_street = 'V';
                        slot.atv = calculateATV(GetVehicle(id_vehicle).speed, pos_x, pos_inter_x, slot_system[n][m].direction_v, d_street);
                        slot.tv = 0;
                        slot_system[n][m].list_V.push_back(slot);
                    }
                }
            }
        }
        else {//direccion izquierda
            if (slot_system[n][m].slot_last_v.id != INVALID) {
                vehicle = GetVehicle(slot_system[n][m].slot_last_v.id);
                start = vehicle.rear_position + 1;
                end = (slot_system[n][m].pos_v + d_side_block) - 1;
            }
            else {
                start = slot_system[n][m].pos_v;
                end = slot_system[n][m].pos_v + d_side_block - 1;
                type_street_intersection = 'V';
            }

            for (int pos_x = start; pos_x <= end; pos_x++) {
                if (GetValueCellStreet('V', m, pos_x) == 1) {
                    if (GetVisibleCellStreet('V', m, pos_x) == true){
                        id_vehicle = GetIDCellStreet('V', m, pos_x);
                        slot.id = id_vehicle;
                        slot.type_street = 'V';
                        slot.atv = calculateATV(GetVehicle(id_vehicle).speed, pos_x, pos_inter_x, slot_system[n][m].direction_v, d_street);
                        slot.tv = 0;
                        slot_system[n][m].list_V.push_back(slot);
                    }
                }
            }
        }
    }

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
                    slot_system[n][m].list_H.push_front(slot);
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
                    slot_system[n][m].list_V.push_front(slot);
                }
            }
        }
    }

}

void updateAtv(int n, int m)
{
    int d_street;
    int pos_inter_x;
    SVehicle tmp_vehicle;

    std::list<SSlot>::iterator it;
    for (it=slot_system[n][m].ordered_list.begin(); it!=slot_system[n][m].ordered_list.end(); ++it) {
        tmp_vehicle = GetVehicle((*it).id);
        if (tmp_vehicle.type_street == 'H'){
            d_street = d_hor_street;
            pos_inter_x = slot_system[n][m].pos_intersection_h;
        }
        else {
            d_street = d_ver_street;
            pos_inter_x = slot_system[n][m].pos_intersection_v;
        }
        (*it).atv = calculateATV(tmp_vehicle.speed, tmp_vehicle.position.x, pos_inter_x, tmp_vehicle.direction, d_street);
    }
}

void calculateOrderedList(int n, int m)
{

    if (slot_system[n][m].list_H.empty() != true) {
        slot_system[n][m].ordered_list.insert(slot_system[n][m].ordered_list.begin(), slot_system[n][m].list_H.begin(), slot_system[n][m].list_H.end());
        slot_system[n][m].slot_last_h = slot_system[n][m].list_H.back();
        slot_system[n][m].list_H.clear();
    }
    else {
        if (slot_system[n][m].slot_last_h.id != INVALID) {
            SVehicle vehicle =  GetVehicle(slot_system[n][m].slot_last_h.id);
            int x = vehicle.position.x;
            int d_street = d_hor_street;
            int pos_inter_x = slot_system[n][m].pos_intersection_h;
            bool invalid = false;

            if (vehicle.direction == 'R') {//direccion derecha
                if (pos_inter_x == 0) {
                    if (x > pos_inter_x && x < (d_street - d_side_block))
                        invalid = true;
                } else {
                    if (x > pos_inter_x)
                        invalid = true;
                }
            }
            else if (vehicle.direction == 'L') {//direccion izquierda
                if (pos_inter_x == 0) {
                    if (x < d_street && x >= (d_street - d_side_block))
                        invalid = true;
                } else {
                    if (x < pos_inter_x)
                        invalid = true;
                }
            }
            if (invalid == true) {
                slot_system[n][m].slot_last_h.id = INVALID;
                slot_system[n][m].slot_last_h.type_street = INVALID;
                slot_system[n][m].slot_last_h.atv = INVALID;
                slot_system[n][m].slot_last_h.tv = INVALID;
            }
        }
    }

    if (slot_system[n][m].list_V.empty() != true) {
        slot_system[n][m].ordered_list.insert(slot_system[n][m].ordered_list.end(), slot_system[n][m].list_V.begin(), slot_system[n][m].list_V.end());
        slot_system[n][m].slot_last_v = slot_system[n][m].list_V.back();
        slot_system[n][m].list_V.clear();
    }
    else {
        if (slot_system[n][m].slot_last_v.id != INVALID) {
            SVehicle vehicle =  GetVehicle(slot_system[n][m].slot_last_v.id);
            int x = vehicle.position.x;
            int d_street = d_ver_street;
            int pos_inter_x = slot_system[n][m].pos_intersection_v;

            bool invalid = false;
            if (vehicle.direction == 'R') {//direccion derecha
                if (pos_inter_x == 0) {
                    if (x > pos_inter_x && x < (d_street - d_side_block))
                        invalid = true;
                } else {
                    if (x > pos_inter_x)
                        invalid = true;
                }
            }
            else if (vehicle.direction == 'L') {//direccion izquierda
                if (pos_inter_x == 0) {
                    if (x < d_street && x >= (d_street - d_side_block))
                        invalid = true;
                } else {
                    if (x < pos_inter_x)
                        invalid = true;
                }
            }

            if (invalid == true) {
                slot_system[n][m].slot_last_v.id = INVALID;
                slot_system[n][m].slot_last_v.type_street = INVALID;
                slot_system[n][m].slot_last_v.atv = INVALID;
                slot_system[n][m].slot_last_v.tv = INVALID;
            }
        }
    }

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
        qDebug() << "ORDER LIST ID:" << (*it).id << (*it).type_street << "Atv:" << (*it).atv;

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

#include "traffic_light.h"
#include "vehicle.h"
#include "sensor.h"
#include "measures.h"

#include <qdebug.h>


void SetPositionVehicle(char type_street, int y, int x, int id)
{

    if (id < 0 || id >= size_vehicles)
        id = 0;

    if (type_street == 'H'){

        if (y < 0 || y >= n_hor_streets)
            y = 0;

        if (x < 0 || x >= d_hor_street)
            x = 0;

        pw_vehicles[id].position.x = x;
        pw_vehicles[id].position.y = y;
    }
    else {

        if (y < 0 || y >= m_ver_streets)
            y = 0;

        if (x < 0 || x >= d_ver_street)
            x = 0;

        pw_vehicles[id].position.x = x;
        pw_vehicles[id].position.y = y;
    }
}

SPosition GetPositionVehicle(int id)
{
   SPosition pos;

   if (id < 0 || id >= size_vehicles)
       id = 0;

   pos = pr_vehicles[id].position;

   return pos;
}

void SetVelocityVehicle(int spd, int id)
{
    if (id < 0 || id >= size_vehicles)
        id = 0;

     pw_vehicles[id].speed = spd;
}

int GetVelocityVehicle(int id)
{
    int speed;

    if (id < 0 || id >= size_vehicles)
        id = 0;

    speed = pr_vehicles[id].speed;

    return speed;
}

void SetTypeStreetVehicle(char type, int id)
{
    if (id < 0 || id >= size_vehicles)
        id = 0;

    pw_vehicles[id].type_street = type;
}

void SetDirectionVehicle(char dir, int id)
{
    if (id < 0 || id >= size_vehicles)
        id = 0;

     pw_vehicles[id].direction = dir;
}

char GetDirectionVehicle(int id)
{
    char direction;

    if (id < 0 || id >= size_vehicles)
        id = 0;

    direction = pr_vehicles[id].direction;

    return direction;
}

void SetColorVehicle(struct Color color, int id)
{
    if (id < 0 || id >= size_vehicles)
        id = 0;

     pw_vehicles[id].color = color;
}

struct Color GetColorVehicle(int id)
{
    struct Color col;

    if (id < 0 || id >= size_vehicles)
        id = 0;

    col = pr_vehicles[id].color;

    return col;
}



char GetTypeStreetVehicle(int id)
{
    char type_street;

    if (id < 0 || id >= size_vehicles)
        id = 0;

    type_street = pr_vehicles[id].type_street;

    return type_street;
}


void SetVisibleVehicle(int id, bool visible)
{
    if (id < 0 || id >= size_vehicles)
        id = 0;

    pw_vehicles[id].visible = visible;

}

bool GetVisibleVehicle(int id)
{
    bool visible;

    if (id < 0 || id >= size_vehicles)
        id = 0;

    visible = pr_vehicles[id].visible;

    return visible;
}

////////////////////////////////////////////////////////////////////////////////////////////////

void SetValueCellStreet(char type_street, int y, int x, int value, bool visible, int id)
{
    if (type_street == 'H'){

        if (y < 0 || y >= n_hor_streets)
            y = 0;

        if (x < 0 || x >= d_hor_street)
            x = 0;

        if (pw_horizontal_streets[y][x].value == 2) {

            int m = x / (1 + d_side_block);

            if (pw_intersections[y][m].value == 1){
                collisions++;
                //if (PRINT_COL == 1)
                //qDebug() << "No. Choques: " << collisions;
             //   printf ("No. Choques: %d", collisions);
            }
            //else {

            pw_intersections[y][m].value = value;
            pw_intersections[y][m].id = id;
            pw_intersections[y][m].visible = visible;
            //}
        }
        else {
            pw_horizontal_streets[y][x].value = value;
            pw_horizontal_streets[y][x].id = id;

            pw_horizontal_streets[y][x].visible = visible;
        }

    }
    else if (type_street == 'V'){

        if (y < 0 || y >= m_ver_streets)
            y = 0;

        if (x < 0 || x >= d_ver_street)
            x = 0;

        if (pw_vertical_streets[y][x].value == 2){

            int n = x / (1 + d_side_block);

            if (pw_intersections[n][y].value == 1){
                collisions++;
                //if (PRINT_COL == 1)
                //  qDebug() << "No. Choques: " << collisions;
                //printf ("No. Choques: %d", collisions);
            }

            pw_intersections[n][y].value = value;
            pw_intersections[n][y].id = id;
            pw_intersections[n][y].visible = visible;

            //pw_intersections[n][y].value = 0;
            //pw_intersections[n][y].id = id;
        }
        else{
            pw_vertical_streets[y][x].value = value;
            pw_vertical_streets[y][x].id = id;

            pw_vertical_streets[y][x].visible = visible;
        }
    }

}




int GetValueCellStreet(char type_street, int y, int x)
{
    int value;

    value = 0;
    if (type_street == 'H'){

        if (y < 0 || y >= n_hor_streets) {
            printf ("Error en H, y = ", y);
            return 0;
        }

        if (x < 0 || x >= d_hor_street) {
            printf ("Error en H, x = ", x);
            return 0;
        }

        if (pr_horizontal_streets[y][x].value == 2) {

            int m = x / (1 + d_side_block);
            value = pr_intersections[y][m].value;
        }
        else
            value = pr_horizontal_streets[y][x].value;

    }
    else if (type_street == 'V'){

        if (y < 0 || y >= m_ver_streets) {
            printf ("Error en V, y = ", y);
            return 0;
        }

        if (x < 0 || x >= d_ver_street) {
            printf ("Error en V, x = ", x);
            return 0;
        }

        if (pr_vertical_streets[y][x].value == 2) {
            int n = x / (1 + d_side_block);
            value = pr_intersections[n][y].value;
        }
        else
            value = pr_vertical_streets[y][x].value;
    }

    return value;
}

void SetIDCellStreet(char type_street, int y, int x, int id)
{

    if (type_street == 'H'){
        if (y < 0 || y >= n_hor_streets) {
            printf ("Error en H, y = ", y);
            return;
        }

        if (x < 0 || x >= d_hor_street) {
            printf ("Error en H, x = ", x);
            return;
        }

        if (pr_horizontal_streets[y][x].value == 2) {
            int m = x / (1 + d_side_block);
            pw_intersections[y][m].id = id;
        }
        else
            pw_horizontal_streets[y][x].id = id;
    }
    else if (type_street == 'V'){
        if (y < 0 || y >= m_ver_streets) {
            printf ("Error en V, y = ", y);
            return;
        }

        if (x < 0 || x >= d_ver_street) {
            printf ("Error en V, x = ", x);
            return;
        }

        if (pr_vertical_streets[y][x].value == 2) {
            int n = x / (1 + d_side_block);
            pw_intersections[n][y].id = id;
        }
        else
            pw_vertical_streets[y][x].id = id;
    }

}

int GetIDCellStreet(char type_street, int y, int x)
{
    int id;

    id = -1;
    if (type_street == 'H'){
        if (y < 0 || y >= n_hor_streets) {
            printf ("Error en H, y = ", y);
            return 0;
        }

        if (x < 0 || x >= d_hor_street) {
            printf ("Error en H, x = ", x);
            return 0;
        }

        if (pr_horizontal_streets[y][x].value == 2) {
            int m = x / (1 + d_side_block);
            id = pr_intersections[y][m].id;
        }
        else
            id = pr_horizontal_streets[y][x].id;
    }
    else if (type_street == 'V'){
        if (y < 0 || y >= m_ver_streets) {
            printf ("Error en V, y = ", y);
            return 0;
        }

        if (x < 0 || x >= d_ver_street) {
            printf ("Error en V, x = ", x);
            return 0;
        }

        if (pr_vertical_streets[y][x].value == 2) {
            int n = x / (1 + d_side_block);
            id = pr_intersections[n][y].id;
        }
        else
            id = pr_vertical_streets[y][x].id;
    }

    return id;
}


bool GetVisibleCellStreet(char type_street, int y, int x)
{
    bool visible;

    if (type_street == 'H'){
        if (y < 0 || y >= n_hor_streets) {
            printf ("Error en H, y = ", y);
            return 0;
        }

        if (x < 0 || x >= d_hor_street) {
            printf ("Error en H, x = ", x);
            return 0;
        }

        if (pr_horizontal_streets[y][x].value == 2) {
            int m = x / (1 + d_side_block);
            visible = pr_intersections[y][m].visible;
        }
        else
            visible = pr_horizontal_streets[y][x].visible;
    }
    else if (type_street == 'V'){
        if (y < 0 || y >= m_ver_streets) {
            printf ("Error en V, y = ", y);
            return 0;
        }

        if (x < 0 || x >= d_ver_street) {
            printf ("Error en V, x = ", x);
            return 0;
        }

        if (pr_vertical_streets[y][x].value == 2) {
            int n = x / (1 + d_side_block);
            visible = pr_intersections[n][y].visible;
        }
        else
            visible = pr_vertical_streets[y][x].visible;
    }

    return visible;
}

int Rule_184(char type_street, int y, int x, char direction)
{

    int front;
    int speed;

    if (type_street == 'H') {
        if (y < 0 || y >= n_hor_streets)
            y = 0;
        if (x < 0 || x >= d_hor_street)
            x= 0;

        if (direction == 'R'){
            if (x == d_hor_street - 1)
                front = 0;
            else
                front = x + 1;
        }
        else {
            if (x == 0)
                front = d_hor_street - 1;
            else
                front = x - 1;
        }
    }
    else if (type_street == 'V') {
        if (y < 0 || y >= m_ver_streets)
            y = 0;
        if (x < 0 || x >= d_ver_street)
            x= 0;

        if (direction == 'R'){
            if (x == d_ver_street - 1)
                front = 0;
            else
                front = x + 1;
        }
        else {
            if (x == 0)
                front = d_ver_street - 1;
            else
                front = x - 1;
        }
    }

    if (GetValueCellStreet(type_street, y, front) == 1)
        speed = 0;
    else
        speed = 1;

    return speed;
}

int Rule_252(char type_street, int y, int x, char direction)
{
    int speed;

    speed = 0;

    return speed;
}




void SwitchVehiclesRW()
{

    int i;

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

void resetCityWrite()
{
    int i;
    int m, n;

    for (n = 0; n < n_hor_streets; n++){
        for (i = 0; i < d_hor_street; i++){
            pw_horizontal_streets[n][i].value = 0;
            pw_horizontal_streets[n][i].id = 0;
            pw_horizontal_streets[n][i].visible = true;
        }
    }

    for (m = 0; m < m_ver_streets; m++){
        for (i = 0; i < d_ver_street; i++){
            pw_vertical_streets[m][i].value = 0;
            pw_vertical_streets[m][i].id = 0;
            pw_vertical_streets[m][i].visible = true;
        }
    }

    int x_h;
    int x_v;


    for (n = 0; n < n_hor_streets; n++)
        for (m = 0; m < m_ver_streets; m++){

            x_h = m * d_side_block + m;
            x_v = n * d_side_block + n;

            pw_intersections[n][m].value = 0;
            pw_intersections[n][m].id = 0;
            pw_intersections[n][m].visible = true;

            pw_horizontal_streets[n][x_h].value = 2;
            pw_vertical_streets[m][x_v].value = 2;

            pw_horizontal_streets[n][x_h].visible = false;
            pw_vertical_streets[m][x_v].visible = false;

        }

}

void printCity()
{

    int i, j;

    printf ("---Read----\n");

    for (i = 0; i < n_hor_streets; i++){
        for (j = 0; j < d_hor_street; j++)
            printf ("%d ", pr_horizontal_streets[i][j]);
        printf ("\n");
    }
    printf ("\n");

    for (j = 0; j < d_ver_street; j++){
        for (i = 0; i < m_ver_streets; i++)
            printf ("%d ", pr_vertical_streets[i][j]);
        printf ("\n");
    }
    printf ("\n");

    printf ("---Write----\n");

    for (i = 0; i < n_hor_streets; i++){
        for (j = 0; j < d_hor_street; j++)
            printf ("%d ", pw_horizontal_streets[i][j]);
        printf ("\n");

    }
    printf ("\n");

    for (j = 0; j < d_ver_street; j++){
        for (i = 0; i < m_ver_streets; i++)
            printf ("%d ", pw_vertical_streets[i][j]);
        printf ("\n");
    }
    printf ("\n");
}


void SwitchMatricesRW()
{

    int i;

    if (switch_matriz == true){

        //Inicializa matriz de lectura
        for (i = 0; i < n_hor_streets; i++)
            pr_horizontal_streets[i] = horizontal_streets_A[i];

        for (i = 0; i < m_ver_streets; i++)
            pr_vertical_streets[i] = vertical_streets_A[i];

        for (i = 0; i < n_hor_streets; i++)
            pr_intersections[i] = intersections_A[i];

        //Inicializa matriz de escritura
        for (i = 0; i < n_hor_streets; i++)
            pw_horizontal_streets[i] = horizontal_streets_Z[i];

        for (i = 0; i < m_ver_streets; i++)
            pw_vertical_streets[i] = vertical_streets_Z[i];

        for (i = 0; i < n_hor_streets; i++)
            pw_intersections[i] = intersections_Z[i];
    }
    else {

        //Inicializa matriz de lectura
        for (i = 0; i < n_hor_streets; i++)
            pr_horizontal_streets[i] = horizontal_streets_Z[i];

        for (i = 0; i < m_ver_streets; i++)
            pr_vertical_streets[i] = vertical_streets_Z[i];

        for (i = 0; i < n_hor_streets; i++)
            pr_intersections[i] = intersections_Z[i];

        //Inicializa matriz de escritura
        for (i = 0; i < n_hor_streets; i++)
            pw_horizontal_streets[i] = horizontal_streets_A[i];

        for (i = 0; i < m_ver_streets; i++)
            pw_vertical_streets[i] = vertical_streets_A[i];

        for (i = 0; i < n_hor_streets; i++)
            pw_intersections[i] = intersections_A[i];

    }

    switch_matriz = !switch_matriz;

}

bool GetVehiclesStopped(char type_street, int y, int x)
{

    int id;

    if (GetValueCellStreet(type_street, y, x) == 1) {

         id = GetIDCellStreet(type_street, y, x);

        if (GetTypeStreetVehicle(id) == type_street)
            if (GetVelocityVehicle(id) == 0)
                return true;

    }

    return false;

}

bool isVehiclesStoppedIntersection(int n, int m)
{
   int x, y;

   y = n;
   x =  m * d_side_block + m;

   if (GetVehiclesStopped('H', y, x) == true)
       return true;

   y = m;
   x = n * d_side_block + n;

    if (GetVehiclesStopped('V', y, x) == true)
        return true;

    return false;
}

bool VehiclesStoppedDistance_e(char type_street, int n, int m, int d_e)
{
    int x, y;
    int direction;
    int e;

    if (type_street == 'H') {

        y = n;
        direction = (n % 2) == 0 ? 'R' : 'L';

        if (direction == 'R') {

            for (e = d_e; e > 0; e--) {
                x = (m * d_side_block + m) + e;

                if (GetVisibleCellStreet('H', n, x) == true){
                    if (GetVehiclesStopped(type_street, y, x) == true)
                        return true;
                }
            }
        }
        else {

            if (m > 0) {
                for (e = d_e; e > 0; e--) {
                    x = (m * d_side_block + m) - e;

                    if (GetVisibleCellStreet('H', n, x) == true){

                        if (GetVehiclesStopped(type_street, y, x) == true)
                            return true;
                    }
                }
            }
            else {

                for (e = d_e; e > 0; e--) {
                    x = d_hor_street - e;

                    if (GetVisibleCellStreet('H', n, x) == true){

                        if (GetVehiclesStopped(type_street, y, x) == true)
                            return true;
                    }
                }
            }
        }
    } else if (type_street == 'V') {

        y = m;
        direction = (m % 2) == 0 ? 'R' : 'L';

        if (direction == 'R') {

            for (e = d_e; e > 0; e--) {
                x = (n * d_side_block + n) + e;

                if (GetVisibleCellStreet('V', m, x) == true){

                    if (GetVehiclesStopped(type_street, y, x) == true)
                        return true;
                }
            }
        }
        else {

            if (n > 0) {
                for (e = d_e; e > 0; e--) {
                    x = (n * d_side_block + n) - e;

                    if (GetVisibleCellStreet('V', m, x) == true){

                        if (GetVehiclesStopped(type_street, y, x) == true)
                            return true;
                    }
                }
            }
            else {
                for (e = d_e; e > 0; e--) {

                    x = d_ver_street - e;

                    if (GetVisibleCellStreet('V', m, x) == true){

                        if (GetVehiclesStopped(type_street, y, x) == true)
                            return true;
                    }
                }
            }
        }
    }

    return false;

}


void RunSimulationGreenWave(int tick)
{

    int i;

    SPosition pos;
    char type_street;
    char direction;


    int x, y;
    int n, m;
    int v;
    int d_street;

    resetCityWrite();

    for (i = 0; i < size_vehicles; i++) {

        pos = GetPositionVehicle(i);
        x = pos.x;
        y = pos.y;
        type_street = GetTypeStreetVehicle(i);
        direction = GetDirectionVehicle(i);

        int pos_t = 0;
        pos_t = GetRegionTrafficLight(type_street, x, direction);

        if (type_street == 'H') {
            n = y;
            m = pos_t;
            d_street = d_hor_street;
        }
        else {
            n = pos_t;
            m = y;
            d_street = d_ver_street;
        }

        if (GetValueTrafficLight(type_street, n, m) == 0 && GetPositionTrafficLight(type_street, n, m) == x)
            v = Rule_252(type_street, y, x, direction);
        else
            v = Rule_184(type_street, y, x, direction);

        if (direction == 'R') {//direccion derecha

            if ((x + v) >= d_street)
                x = 0;
            else
                x = x + v;
        }
        else if (direction == 'L') {//direccion izquierda

            if ((x - v) < 0)
                x = d_street - 1;
            else
                x = x - v;
        }

        SetPositionVehicle(type_street, y, x, i);
        SetVelocityVehicle(v, i);
        SetDirectionVehicle(direction, i);

        if (frand() < p_turn) {
          turn(type_street, y, x, v, direction, true, i);
        }


        SetValueCellStreet(type_street, y, x, 1, true, i);

    }

    for (n = 0; n < n_hor_streets; n++)
        for (m = 0; m < m_ver_streets; m++)
            RunTrafficLight(n, m);

   //printf("Colision: %d \n", collisions);
   // qDebug() << "Colision: " << collisions;

    if (tick < n_ticks) {
        Velocity();
        Flux(density);

    }

    SwitchMatricesRW();
    SwitchVehiclesRW();

    //qDebug() << "Run simulation";
}

bool turn(char &type_street, int &y, int &x, int &v, char &direction, bool visible, int id)
{

        if (type_street == 'H'){

            if (y < 0 || y >= n_hor_streets) {
                printf ("Error en H, y = ", y);
                return false;
            }

            if (x < 0 || x >= d_hor_street) {
                printf ("Error en H, x = ", x);
                return false;
            }

            if (pr_horizontal_streets[y][x].value == 2) {

                int y_new = x / (1 + d_side_block);
                int x_new = y * d_side_block + y;

                y = y_new;
                x = x_new;

                type_street = 'V';
                SetTypeStreetVehicle('V', id);

                SetPositionVehicle(type_street, y, x, id);

                if ((y % 2) == 0)
                   direction = 'R';
                else
                   direction = 'L';

                SetDirectionVehicle(direction, id);

                //v = Rule_184(type_street, y, x, direction);
                SetVelocityVehicle(v, id);

                SetVisibleVehicle(id, visible);

                return true;
           }
        }


        else if (type_street == 'V'){

            if (y < 0 || y >= m_ver_streets) {
                printf ("Error en V, y = ", y);
                return false;
            }

            if (x < 0 || x >= d_ver_street) {
                printf ("Error en V, x = ", x);
                return false;
            }

            if (pr_vertical_streets[y][x].value == 2) {

                int y_new = x / (1 + d_side_block);
                int x_new = y * d_side_block + y;

                y = y_new;
                x = x_new;

                type_street = 'H';
                SetTypeStreetVehicle('H', id);

                SetPositionVehicle(type_street, y, x, id);

                if ((y % 2) == 0)
                   direction = 'R';
                else
                   direction = 'L';

                SetDirectionVehicle(direction, id);

                //v = Rule_184(type_street, y, x, direction);
                SetVelocityVehicle(v, id);

                SetVisibleVehicle(id, visible);

                return true;

            }
        }

        return false;
}


void RunSimulationSelfOrganization(int tick)
{

    int i;

    SPosition pos;
    char type_street;
    char direction;
    bool prev_visible;

    int x, y;
    int n, m;
    int v;
    int d_street;
    bool visible;

    resetCityWrite();

    for (i = 0; i < size_vehicles; i++) {

        pos = GetPositionVehicle(i);
        x = pos.x;
        y = pos.y;
        type_street = GetTypeStreetVehicle(i);
        direction = GetDirectionVehicle(i);
        prev_visible = GetVisibleVehicle(i);

        int pos_t = 0;

        pos_t = GetRegionTrafficLightSO(type_street, x, direction);

        if (type_street == 'H') {
            n = y;
            m = pos_t;
            d_street = d_hor_street;
        }
        else {
            n = pos_t;
            m = y;
            d_street = d_ver_street;
        }

        if (GetValueTrafficLightSO(type_street, n, m) == 0 && GetPositionTrafficLightSO(type_street, n, m) == x)
            v = Rule_252(type_street, y, x, direction);
        else
            v = Rule_184(type_street, y, x, direction);


        if (direction == 'R') {//direccion derecha

            if ((x + v) >= d_street)
                x = 0;
            else
                x = x + v;
        }
        else if (direction == 'L') {//direccion izquierda

            if ((x - v) < 0)
                x = d_street - 1;
            else
                x = x - v;
        }



        visible = determineVisible(type_street, direction, x, prev_visible);//Visibilidad

        SetTypeStreetVehicle(type_street, i);
        SetPositionVehicle(type_street, y, x, i);
        SetVelocityVehicle(v, i);
        SetDirectionVehicle(direction, i);
        SetVisibleVehicle(i, visible);


        if (frand() < p_turn) {
          turn(type_street, y, x, v, direction, visible, i);
        }

         SetValueCellStreet(type_street, y, x, 1, visible, i);

    }


    for (n = 0; n < n_hor_streets; n++)
        for (m = 0; m < m_ver_streets; m++)
            RunTrafficLight(n, m);


    if (tick < n_ticks) {
        Velocity();
        Flux(density);
        averageValueIntersections();
    }

    SwitchMatricesRW();
    SwitchVehiclesRW();

    //qDebug() << "Run simulation";

}



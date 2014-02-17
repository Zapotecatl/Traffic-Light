/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Description: This program implements the algorithms of the Green Wave and Self Organization to control traffic ligths in a city proposed by
Carlos Gershenson and David A. Rosenblueth. There have been several highway traffic models proposed based on cellular au-
tomata. The simplest one is elementary cellular automaton rule 184. This model to city traffic with cellular automata coupled at
intersections using only rules 184, 252, and 136. The simplicity of the model offers a clear understanding of the main properties
of city traffc and its phase transitions.

The green-wave method that tries to optimize phases according to expected flows and a self-organizing method that adapts to the
current traffic conditions. The self-organizing method delivers considerable improvements over the green-wave method.

Autor: Jorge Luis Zapotecatl López

Date: 01/31/2014

*///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <iostream>
#include <algorithm>
#include <vector>

#include <QDebug>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TRAFFIC LIGHTS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct SPosCell{
    char type_street;
    int n_inter;
    int m_inter;
    int pos_cell;
};

struct STrafficLight{
    int light;//1.- Green 0.- Red
    int time;

    int offset_t;
    bool offset_applied;

};

struct STrafficSelfOrganizing{

    int light;//1.- Green 0.- Red
    int time_u;
    bool vehicle_stop;
    bool inter_ocup;

    int n_vehicles;
    int m_vehicles;
};

bool switch_matriz;

int n_hor_streets; //An integer that represents the amount of horizontal streets
int m_ver_streets; //An integer that represents the amount of vertical streets
int d_hor_street; //horizontal distance of the streets
int d_ver_street; //vertical distance of the streets

int d_side_block; //distance of the smallest area that is surrounded by streets
int n_blocks; //An integer that represents the amount of blocks


//Pointers for read
unsigned int **pr_horizontal_streets;
unsigned int **pr_vertical_streets;
unsigned int **pr_intersections;

//Pointers for write
unsigned int **pw_horizontal_streets;
unsigned int **pw_vertical_streets;
unsigned int **pw_intersections;

unsigned int **horizontal_streets_A;//Two-dimensional array representing the streets in horizontal direction
unsigned int **vertical_streets_A;//Two-dimensional array representing the streets in vertical direction
unsigned int **intersections_A;//Two dimensional array that represents the intersections between horizontal and vertical streets

unsigned int **horizontal_streets_Z;//Two-dimensional array representing the streets in horizontal direction
unsigned int **vertical_streets_Z;//Two-dimensional array representing the streets in vertical direction
unsigned int **intersections_Z;//Two dimensional array that represents the intersections between horizontal and vertical streets


STrafficLight **h_traffic_light;
STrafficLight **v_traffic_light;

STrafficSelfOrganizing **h_traffic_light_so;
STrafficSelfOrganizing **v_traffic_light_so;

vector<SPosCell> random_pos;

float T;
float densitiy;

int maxim_n;
int maxim_m;
int min_time;

int max_time_u;
int distance_d;
int distance_r;
int distance_e;

int total_cell;
float velocity;
float flux;

float velocity_total;
float flux_total;
int n_ticks;
int limit;

FILE *fp;

int metodo;
int inicio;

int CityBuilding(int n_hor, int m_ver, int d_blk);
void CityDestroying();
void printCity();
int GetValueCellStreet(char type_street, int n, int m, int pos_cell);
int SetValueCellStreet(char type_street, int n, int m, int pos_cell, int value);
void VehiclesStopped(char type_street, int n, int m, int pos_cell);
bool GetVehiclesStopped(char type_street, int n, int m, int pos_cell);
int RandomizeVehicles(float density);
int LeftNeighbor(char type_street, int n, int m, int pos_cell, SPosCell *neighbor_left);
int RightNeighbor(char type_street, int n, int m, int pos_cell, SPosCell *neighbor_right);
int Rule_184(char type_street, int n, int m, int pos_cell, char direction);
int Rule_252(char type_street, int n, int m, int pos_cell, char direction);
int Rule_136(char type_street, int n, int m, int pos_cell, char direction);
void SwitchMatricesRW();


void RunCityTrafficLightGreenWave();
void InializedTrafficLight();
void offsetApplied(int n, int m);
void TrafficLightGreenWave(int n, int m);
int GetValueTrafficLight(char type_street, int n, int m);
void SwitchTrafficLight(int n, int m);


void RunCityTrafficSelfOrganizing();
void InializedTrafficSelfOrganizing();
void TrafficSelfOrganizing(int n, int m, char direction_h, char direction_v);
void TrafficSOApproaching(int n, int m, char direction_h, char direction_v);
void SwitchBothRedTrafficLightSO(int n, int m);
void SwitchTrafficLightSO(int n, int m);
int GetValueTrafficLightSO(char type_street, int n, int m);
void RestoreSingleGreen(char type_street, int n, int m);

void celdaOcupada(int n, int m);

void Velocity();
void Flux();

void SaveMeasures();
int mainFunction(int met);


//The city is set to agree to the number of horizontal and vertical streets, and the distance of the blocks
int CityBuilding(int n_hor, int m_ver, int d_blk)
{
    int i, j, n, m, k;

    //Sets values ​​valid in the numbers of horizontal and vertical streets, and the distance of the blocks.
    if (n_hor < 1)
        n_hor_streets = 1;
    else
        n_hor_streets = n_hor;

    if (m_ver < 1)
        m_ver_streets = 1;
    else
        m_ver_streets = m_ver;

    if (d_blk < 2)
        d_side_block = 2;
    else
        d_side_block = d_blk;

    total_cell = 2 * d_side_block * n_hor_streets * m_ver_streets + n_hor_streets * m_ver_streets;

    //The required memory is reserved dynamically.
    //The horizontal distance is greater when more vertical streets.
    //The vertical distance is greater when there is more horizontal streets.


    d_hor_street = d_side_block * m_ver_streets + m_ver_streets;
    d_ver_street = d_side_block * n_hor_streets + n_hor_streets;

    //Pointer of read an write to Matrices A and Z
    pr_intersections = new unsigned int*[n_hor_streets];
    pr_horizontal_streets = new unsigned int*[n_hor_streets];
    pr_vertical_streets = new unsigned int*[m_ver_streets];

    pw_intersections = new unsigned int*[n_hor_streets];
    pw_horizontal_streets = new unsigned int*[n_hor_streets];
    pw_vertical_streets = new unsigned int*[m_ver_streets];


    //Matrices A
    intersections_A = new unsigned int*[n_hor_streets];
    for (i = 0; i < n_hor_streets; i++)
        intersections_A[i] = new unsigned int[m_ver_streets];

    //reserved by n horizontal streets
    horizontal_streets_A = new unsigned int*[n_hor_streets];
    for (i = 0; i < n_hor_streets; i++)
        horizontal_streets_A[i] = new unsigned int[d_hor_street];

    //reserved by vertical distance streets
    vertical_streets_A = new unsigned int*[m_ver_streets];
    for (i = 0; i < m_ver_streets; i++)
        vertical_streets_A[i] = new unsigned int[d_ver_street];


    //Matrices Z
    intersections_Z = new unsigned int*[n_hor_streets];
    for (i = 0; i < n_hor_streets; i++)
        intersections_Z[i] = new unsigned int[m_ver_streets];

    //reserved by n horizontal streets
    horizontal_streets_Z = new unsigned int*[n_hor_streets];
    for (i = 0; i < n_hor_streets; i++)
        horizontal_streets_Z[i] = new unsigned int[d_hor_street];

    //reserved by vertical distance streets
    vertical_streets_Z = new unsigned int*[m_ver_streets];
    for (i = 0; i < m_ver_streets; i++)
        vertical_streets_Z[i] = new unsigned int[d_ver_street];


    SwitchMatricesRW();

    //reserved by horizontal
    h_traffic_light = new STrafficLight*[n_hor_streets];
    for (i = 0; i < n_hor_streets; i++)
        h_traffic_light[i] = new STrafficLight[m_ver_streets];


    //reserved by vertical
    v_traffic_light = new STrafficLight*[m_ver_streets];
    for (i = 0; i < m_ver_streets; i++)
        v_traffic_light[i] = new STrafficLight[n_hor_streets];


    //reserved by vertical
    v_traffic_light_so = new STrafficSelfOrganizing*[m_ver_streets];
    for (i = 0; i < m_ver_streets; i++)
        v_traffic_light_so[i] = new STrafficSelfOrganizing[n_hor_streets];

    //reserved by horizontal
    h_traffic_light_so = new STrafficSelfOrganizing*[n_hor_streets];
    for (i = 0; i < n_hor_streets; i++)
        h_traffic_light_so[i] = new STrafficSelfOrganizing[m_ver_streets];


    //The streets are divided into cells. The state of the cells are classified as follows:
    //0 - The cell is empty
    //1 - The cell contains a vehicle
    //2 - The cell is an intersection (for what needs to redirected to the array of intersections).

    int reposition_cell_h;
    int reposition_cell_v;

    k = 0;
    for (n = 0; n < n_hor_streets; n++){
        for (m = 0; m < m_ver_streets; m++){

            reposition_cell_h = m * d_side_block + m;
            reposition_cell_v = n * d_side_block + n;

            pr_intersections[n][m] = 0;
            pr_horizontal_streets[n][reposition_cell_h] = 2;
            pr_vertical_streets[m][reposition_cell_v] = 2;

            pw_intersections[n][m] = 0;
            pw_horizontal_streets[n][reposition_cell_h] = 2;
            pw_vertical_streets[m][reposition_cell_v] = 2;

            for (i = 1; i <= d_side_block; i++){

                reposition_cell_h = m * d_side_block + m + i;

                pr_horizontal_streets[n][reposition_cell_h] = 0;
                pw_horizontal_streets[n][reposition_cell_h] = 0;

            }

            for (j = 1; j <= d_side_block; j++){

                reposition_cell_v = n * d_side_block + n + j;

                pr_vertical_streets[m][reposition_cell_v] = 0;
                pw_vertical_streets[m][reposition_cell_v] = 0;
            }
        }
    }

    //printCity();

    return 0;
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

void CityDestroying()
{

    int i;

    delete [] pw_intersections;
    delete [] pw_horizontal_streets;
    delete [] pw_vertical_streets;

    delete [] pr_intersections;
    delete [] pr_horizontal_streets;
    delete [] pr_vertical_streets;

    //Matrices A
    for (i = 0; i < n_hor_streets; i++)
        delete intersections_A[i];
    delete [] intersections_A;

    for (i = 0; i < n_hor_streets; i++)
        delete horizontal_streets_A[i];
    delete [] horizontal_streets_A;

    for (i = 0; i < m_ver_streets; i++)
        delete vertical_streets_A[i];
    delete [] vertical_streets_A;

    //Matrices Z
    for (i = 0; i < n_hor_streets; i++)
        delete intersections_Z[i];
    delete [] intersections_Z;

    for (i = 0; i < n_hor_streets; i++)
        delete horizontal_streets_Z[i];
    delete [] horizontal_streets_Z;

    for (i = 0; i < m_ver_streets; i++)
        delete vertical_streets_Z[i];
    delete [] vertical_streets_Z;

    for (i = 0; i < n_hor_streets; i++)
        delete h_traffic_light[i];
    delete [] h_traffic_light;


    for (i = 0; i < m_ver_streets; i++)
        delete v_traffic_light[i];
    delete [] v_traffic_light;


    for (i = 0; i < m_ver_streets; i++)
        delete v_traffic_light_so[i];
    delete [] v_traffic_light_so;

    for (i = 0; i < n_hor_streets; i++)
        delete h_traffic_light_so[i];
    delete [] h_traffic_light_so;

}

int GetValueCellStreet(char type_street, int n, int m, int pos_cell)
{

    int reposition_cell;

    if (n < 0 || m < 0)
        return -1;

    if (n >= n_hor_streets)
        n = 0;

    if (m >= m_ver_streets)
        m = 0;

    if (!(-d_side_block <= pos_cell && pos_cell <= d_side_block))
        return -1;

    if (type_street == 'H'){

        if (m == 0 && pos_cell < 0)
            reposition_cell = d_hor_street + pos_cell;
        else
            reposition_cell = m * d_side_block + m + pos_cell;


        if (pr_horizontal_streets[n][reposition_cell] == 2){

            if (pr_intersections[n][m] == 1 || pr_intersections[n][m] == -1)
                return 1;
            else
                return 0;
        }
        else {

            if (pr_horizontal_streets[n][reposition_cell] == 1 || pr_horizontal_streets[n][reposition_cell] == -1)
                return 1;
            else
                return 0;
        }

    }
    else if(type_street == 'V'){

        if (n == 0 && pos_cell < 0)
            reposition_cell = d_ver_street + pos_cell;
        else
            reposition_cell = n * d_side_block + n + pos_cell;

        if (pr_vertical_streets[m][reposition_cell] == 2){

            if (pr_intersections[n][m] == 1 || pr_intersections[n][m] == -1)
                return 1;
            else
                return 0;
        }
        else{

            if (pr_vertical_streets[m][reposition_cell] == 1 || pr_vertical_streets[m][reposition_cell] == -1)
                return 1;
            else
                return 0;

        }
    }

    return -1;
}


int SetValueCellStreet(char type_street, int n, int m, int pos_cell, int value)
{
    int reposition_cell;


    if (n < 0 || m < 0)
        return -1;

    if (n >= n_hor_streets)
        n = 0;

    if (m >= m_ver_streets)
        m = 0;


    if (!(-d_side_block <= pos_cell && pos_cell <= d_side_block))
        return -1;

    if (type_street == 'H'){

        if (m == 0 && pos_cell < 0)
            reposition_cell = d_hor_street + pos_cell;
        else
            reposition_cell = m * d_side_block + m + pos_cell;


        if (pw_horizontal_streets[n][reposition_cell] == 2)
            pw_intersections[n][m] = value;
        else
            pw_horizontal_streets[n][reposition_cell] = value;

        return 0;
    }
    else if(type_street == 'V'){

        if (n == 0 && pos_cell < 0)
            reposition_cell = d_ver_street + pos_cell;
        else
            reposition_cell = n * d_side_block + n + pos_cell;


        if (pw_vertical_streets[m][reposition_cell] == 2)
            pw_intersections[n][m] = value;
        else
            pw_vertical_streets[m][reposition_cell] = value;

        return 0;
    }

    return -1;
}

void VehiclesStopped(char type_street, int n, int m, int pos_cell)
{

    int reposition_cell;

    if (n < 0 || m < 0)
        return;

    if (n >= n_hor_streets)
        n = 0;

    if (m >= m_ver_streets)
        m = 0;

    if (!(-d_side_block <= pos_cell && pos_cell <= d_side_block))
        return;

    if (type_street == 'H'){

        if (m == 0 && pos_cell < 0)
            reposition_cell = d_hor_street + pos_cell;
        else
            reposition_cell = m * d_side_block + m + pos_cell;


        if (pr_horizontal_streets[n][reposition_cell] == 2){

            if ((pr_intersections[n][m] == 1 || pr_intersections[n][m] == -1) &&
                (pw_intersections[n][m] == 1 || pw_intersections[n][m] == -1)){

                pw_intersections[n][m] = -1; //Indicas que hay un vehiculo parado
            }
        }
        else {

            if ((pr_horizontal_streets[n][reposition_cell] == 1 || pr_horizontal_streets[n][reposition_cell] == -1) &&
                (pw_horizontal_streets[n][reposition_cell] == 1 || pw_horizontal_streets[n][reposition_cell] == -1))

                pw_horizontal_streets[n][reposition_cell] = -1;//Indicas que hay un vehiculo parado
        }

    }
    else if(type_street == 'V'){

        if (n == 0 && pos_cell < 0)
            reposition_cell = d_ver_street + pos_cell;
        else
            reposition_cell = n * d_side_block + n + pos_cell;


        if (pr_vertical_streets[m][reposition_cell] == 2){

            if ((pr_intersections[n][m] == 1 || pr_intersections[n][m] == -1) &&
                (pw_intersections[n][m] == 1 || pw_intersections[n][m] == -1)){

                pw_intersections[n][m] = -1; //Indicas que hay un vehiculo parado
            }
        }
        else {

            if ((pr_vertical_streets[m][reposition_cell] == 1 || pr_vertical_streets[m][reposition_cell] == -1) &&
                (pw_vertical_streets[m][reposition_cell] == 1 || pw_vertical_streets[m][reposition_cell] == -1))

                pw_vertical_streets[m][reposition_cell] = -1;//Indicas que hay un vehiculo parado
        }
    }

}


bool GetVehiclesStopped(char type_street, int n, int m, int pos_cell)
{

    int reposition_cell;

    if (n < 0 || m < 0)
        return -1;

    if (n >= n_hor_streets)
        n = 0;

    if (m >= m_ver_streets)
        m = 0;


    if (!(-d_side_block <= pos_cell && pos_cell <= d_side_block))
        return -1;


    if (type_street == 'H'){

        if (m == 0 && pos_cell < 0)
            reposition_cell = d_hor_street + pos_cell;
        else
            reposition_cell = m * d_side_block + m + pos_cell;


        if (pr_horizontal_streets[n][reposition_cell] == 2){
            if (pr_intersections[n][m] == -1)
                return true;
            else
                return false;
        }
        else {

            if (pr_horizontal_streets[n][reposition_cell] == -1)
                return true;
            else
                return false;

        }

    }
    else if(type_street == 'V'){

        if (n == 0 && pos_cell < 0)
            reposition_cell = d_ver_street + pos_cell;
        else
            reposition_cell = n * d_side_block + n + pos_cell;

        if (pr_vertical_streets[m][reposition_cell] == 2){
            if (pr_intersections[n][m] == -1)
                return true;
            else
                return false;
        }
        else {

            if (pr_vertical_streets[m][reposition_cell] == -1)
                return true;
            else
                return false;
        }
    }
}


int RandomizeVehicles(float density)
{


    int i, j, n, m, k;

    SPosCell ran_pos_tmp;

    for (n = 0; n < n_hor_streets; n++){
        for (m = 0; m < m_ver_streets; m++){

            ran_pos_tmp.type_street = 'H';
            ran_pos_tmp.n_inter = n;
            ran_pos_tmp.m_inter = m;
            ran_pos_tmp.pos_cell = 0;

            random_pos.push_back(ran_pos_tmp);

            for (i = 1; i <= d_side_block; i++){

                ran_pos_tmp.type_street = 'H';
                ran_pos_tmp.pos_cell = i;
                random_pos.push_back(ran_pos_tmp);
            }

            for (j = 1; j <= d_side_block; j++){

                ran_pos_tmp.type_street = 'V';
                ran_pos_tmp.pos_cell = j;
                random_pos.push_back(ran_pos_tmp);
            }
        }
    }

    random_shuffle(random_pos.begin(), random_pos.end());

    if (density < 0)
        density = 0;
    else if (density > 1)
        density = 1;

    limit = density * random_pos.size();

    i = 0;
    while (i < limit){

        SetValueCellStreet(random_pos[i].type_street, random_pos[i].n_inter, random_pos[i].m_inter, random_pos[i].pos_cell, 1);
        i++;
    }

    random_pos.clear();

    SwitchMatricesRW();

    return 0;
}

int LeftNeighbor(char type_street, int n, int m, int pos_cell, SPosCell *neighbor_left)
{

    if (type_street == 'H') {


        //Verifica si la posicion del vecino es menor a la frontera inferior
        if ((m == 0) && pos_cell == -d_side_block){

            neighbor_left->type_street = 'H';
            neighbor_left->m_inter = m_ver_streets - 1;
            neighbor_left->n_inter = n;
            neighbor_left->pos_cell = 0;

            return 0;
        }
        else if (pos_cell == -d_side_block){

            neighbor_left->type_street = 'H';
            neighbor_left->m_inter = m - 1;
            neighbor_left->n_inter = n;
            neighbor_left->pos_cell = 0;

            return 0;
        }

        neighbor_left->type_street = 'H';
        neighbor_left->m_inter = m;
        neighbor_left->n_inter = n;
        neighbor_left->pos_cell = pos_cell - 1;

        return 0;
    }
    else {


        if ((n == 0) && pos_cell == -d_side_block){

            neighbor_left->type_street = 'V';
            neighbor_left->m_inter = m;
            neighbor_left->n_inter = n_hor_streets - 1;
            neighbor_left->pos_cell = 0;

            return 0;
        }
        else if (pos_cell == -d_side_block){

            neighbor_left->type_street = 'V';
            neighbor_left->m_inter = m;
            neighbor_left->n_inter = n - 1;
            neighbor_left->pos_cell = 0;

            return 0;
        }

        neighbor_left->type_street = 'V';
        neighbor_left->m_inter = m;
        neighbor_left->n_inter = n;
        neighbor_left->pos_cell = pos_cell - 1;

        return 0;

    }

    return 0;
}

int RightNeighbor(char type_street, int n, int m, int pos_cell, SPosCell *neighbor_right)
{

    if (type_street == 'H') {

        //Verifica si la posicion del vecino es mayor a la frontera superior
        if ((m == m_ver_streets - 1) && pos_cell == d_side_block){
            neighbor_right->type_street = 'H';
            neighbor_right->m_inter = 0;
            neighbor_right->n_inter = n;
            neighbor_right->pos_cell = 0;

            return 0;
        }
        else if (pos_cell == d_side_block){
            neighbor_right->type_street = 'H';
            neighbor_right->m_inter = m + 1;
            neighbor_right->n_inter = n;
            neighbor_right->pos_cell = 0;

            return 0;
        }

        neighbor_right->type_street = 'H';
        neighbor_right->m_inter = m;
        neighbor_right->n_inter = n;
        neighbor_right->pos_cell = pos_cell + 1;

        return 0;
    }
    else {

        if ((n == n_hor_streets - 1) && pos_cell == d_side_block){
            neighbor_right->type_street = 'V';
            neighbor_right->m_inter = m;
            neighbor_right->n_inter = 0;
            neighbor_right->pos_cell = 0;

            return 0;
        }
        else if (pos_cell == d_side_block){
            neighbor_right->type_street = 'V';
            neighbor_right->m_inter = m;
            neighbor_right->n_inter = n + 1;
            neighbor_right->pos_cell = 0;

            return 0;
        }

        neighbor_right->type_street = 'V';
        neighbor_right->m_inter = m;
        neighbor_right->n_inter = n;
        neighbor_right->pos_cell = pos_cell + 1;

        return 0;
    }

    return 0;
}

int Rule_184(char type_street, int n, int m, int pos_cell, char direction)
{
    SPosCell nei_l;
    SPosCell nei_r;


    if (direction == 'R'){
        LeftNeighbor(type_street, n, m, pos_cell, &nei_l);
        RightNeighbor(type_street, n, m, pos_cell, &nei_r);
    }
    else{
        //Invertir vecinos para aplicar la reglas en otro sentido
        LeftNeighbor(type_street, n, m, pos_cell, &nei_r);
        RightNeighbor(type_street, n, m, pos_cell, &nei_l);
    }

    if(GetValueCellStreet(type_street, n, m, pos_cell) == 0){

        if (GetValueCellStreet(nei_l.type_street, nei_l.n_inter, nei_l.m_inter, nei_l.pos_cell) == 0)
            SetValueCellStreet(type_street, n, m, pos_cell, 0);
        else
            SetValueCellStreet(type_street, n, m, pos_cell, 1);
    }
    else{
        if (GetValueCellStreet(nei_r.type_street, nei_r.n_inter, nei_r.m_inter, nei_r.pos_cell) == 1){

            SetValueCellStreet(type_street, n, m, pos_cell, 1);

        }
        else
            SetValueCellStreet(type_street, n, m, pos_cell, 0);
    }

    return 0;
}


int Rule_252(char type_street, int n, int m, int pos_cell, char direction)
{
    SPosCell nei_l;

    if (direction == 'R'){
        LeftNeighbor(type_street, n, m, pos_cell, &nei_l);
    }
    else{
        //Invertir vecinos para aplicar la reglas en otro sentido
        RightNeighbor(type_street, n, m, pos_cell, &nei_l);
    }

    if(GetValueCellStreet(type_street, n, m, pos_cell) == 0){

        if (GetValueCellStreet(nei_l.type_street, nei_l.n_inter, nei_l.m_inter, nei_l.pos_cell) == 0)
            SetValueCellStreet(type_street, n, m, pos_cell, 0);
        else
            SetValueCellStreet(type_street, n, m, pos_cell, 1);
    }
    else{

        SetValueCellStreet(type_street, n, m, pos_cell, 1);
    }

    return 0;
}

int Rule_136(char type_street, int n, int m, int pos_cell, char direction)
{
    SPosCell nei_r;

    if (direction == 'R'){
        RightNeighbor(type_street, n, m, pos_cell, &nei_r);
    }
    else{
        //Invertir vecinos para aplicar la reglas en otro sentido
        LeftNeighbor(type_street, n, m, pos_cell, &nei_r);
    }

    if(GetValueCellStreet(type_street, n, m, pos_cell) == 1){

        if (GetValueCellStreet(nei_r.type_street, nei_r.n_inter, nei_r.m_inter, nei_r.pos_cell) == 1)
            SetValueCellStreet(type_street, n, m, pos_cell, 1);
        else
            SetValueCellStreet(type_street, n, m, pos_cell, 0);
    }
    else{

        SetValueCellStreet(type_street, n, m, pos_cell, 0);
    }

    return 0;
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

//Green wave///////////////////////////////////////////////////////////////

void RunCityTrafficLightGreenWave()
{
    int i, j, n, m;

    int light_h, light_v;
    char direction_Hor; //R-Right (Bottom for vertical) L-Left (up for vertical)
    char direction_Ver;

    for (n = 0; n < n_hor_streets; n++){

        if (n % 2 == 0)
            direction_Hor = 'R';
        else
            direction_Hor = 'L';

        for (m = 0; m < m_ver_streets; m++){

            if (m % 2 == 0)
                direction_Ver = 'R';
            else
                direction_Ver = 'L';

            TrafficLightGreenWave(n, m);

            light_h = GetValueTrafficLight('H', n, m);
            light_v = GetValueTrafficLight('V', n, m);


            if (light_h == 1)
                Rule_184('H', n, m, 0, direction_Hor);//interseccion
            else
                Rule_184('V', n, m, 0, direction_Ver);//interseccion


            VehiclesStopped('H', n, m, 0); //Establece si el vehiculo esta parado

            // HORIZONTAL ////////////////////////////////

            //Elemento anterior a la interseccion
            light_h = GetValueTrafficLight('H', n, m);//Se necesita el valor del semaforo adyacente

            if (light_h == 1) {//luz verde
                Rule_184('H', n, m, -1, direction_Hor);
            }
            else{ //luz roja

                //Sentido del trafico
                if (direction_Hor == 'R')
                    Rule_252('H', n, m, -1, direction_Hor);//izquierda a derecha
                else
                    Rule_136('H', n, m, -1, direction_Hor); //derecha a izquierda
            }

            VehiclesStopped('H', n, m, -1); //Establece si el vehiculo esta parado


            //primer elemento
            light_h = GetValueTrafficLight('H', n, m);

            if (light_h == 1) {//luz verde

                Rule_184('H', n, m, 1, direction_Hor);

            }
            else{//luz roja

                //Sentido del trafico
                if (direction_Hor == 'R')
                    Rule_136('H', n, m, 1, direction_Hor);//izquierda a derecha
                else
                    Rule_252('H', n, m, 1, direction_Hor); //derecha a izquierda
            }

            VehiclesStopped('H', n, m, 1); //Establece si el vehiculo esta parado

            //elementos intermedios
            for (i = 2; i < d_side_block; i++){
                Rule_184('H', n, m, i, direction_Hor);
                VehiclesStopped('H', n, m, i); //Establece si el vehiculo esta parado

            }

            // VERTICAL ////////////////////////////////

            //Elemento anterior a la interseccion
            light_v = GetValueTrafficLight('V', n, m);//Se necesita el valor del semaforo adyacente

            if (light_v == 1 ) {//luz verde

                Rule_184('V', n, m, -1, direction_Ver);
            }
            else{ //luz roja

                //Sentido del trafico
                if (direction_Ver == 'R')
                    Rule_252('V', n, m, -1, direction_Ver);//arriba a abajo
                else
                    Rule_136('V', n, m, -1, direction_Ver); //abajo a arriba

            }

            VehiclesStopped('V', n, m, -1); //Establece si el vehiculo esta parado


            //primer elemento
            light_v = GetValueTrafficLight('V', n, m);

            if (light_v == 1) {//luz verde
                Rule_184('V', n, m, 1, direction_Ver);
            }
            else{//luz roja

                //Sentido del trafico
                if (direction_Ver == 'R')
                    Rule_136('V', n, m, 1, direction_Ver);//arriba a abajo
                else
                    Rule_252('V', n, m, 1, direction_Ver); //abajo a arriba
            }

            VehiclesStopped('V', n, m, 1); //Establece si el vehiculo esta parado

            //elementos intermedios
            for (j = 2; j < d_side_block; j++){
                Rule_184('V', n, m, j, direction_Ver);
                VehiclesStopped('V', n, m, j); //Establece si el vehiculo esta parado
            }
        }
    }

    Velocity();
    Flux();

    SwitchMatricesRW();

}


void InializedTrafficLight()
{
    int n, m;

    int x;
    int y;

    int offset_t;


    for (n = 0; n < n_hor_streets; n++){
        for (m = 0; m < m_ver_streets; m++){

            x = m * d_side_block + m;
            y = n * d_side_block + n;

            offset_t = floor(fmod(x - y, T / 2.0) + 0.5);


            h_traffic_light[n][m].offset_t = offset_t;
            v_traffic_light[n][m].offset_t = offset_t;

            h_traffic_light[n][m].offset_applied = false;
            v_traffic_light[n][m].offset_applied = false;


            if (floor(fmod(x - y, T) + 0.5) >= T / 2.0){

                h_traffic_light[n][m].light = 1;//Green
                v_traffic_light[n][m].light = 0;//Red

                h_traffic_light[n][m].time = 0;
                v_traffic_light[n][m].time = T;
            }
            else {

                h_traffic_light[n][m].light = 0;//Red
                v_traffic_light[n][m].light = 1;//Green

                v_traffic_light[n][m].time = 0;
                h_traffic_light[n][m].time = T;

            }

        }
    }
}

void offsetApplied(int n, int m){

    if (GetValueTrafficLight('H', n, m) == 1){

        if (h_traffic_light[n][m].time < h_traffic_light[n][m].offset_t)
            h_traffic_light[n][m].time++;

        else{

            SwitchTrafficLight(n, m);
            h_traffic_light[n][m].offset_applied = true;
            v_traffic_light[n][m].offset_applied = true;

        }
    }
    else {

        if (v_traffic_light[n][m].time < v_traffic_light[n][m].offset_t)
            v_traffic_light[n][m].time++;

        else{
            SwitchTrafficLight(n, m);
            h_traffic_light[n][m].offset_applied = true;
            v_traffic_light[n][m].offset_applied = true;
        }
    }

}

void TrafficLightGreenWave(int n, int m)
{

    if ( h_traffic_light[n][m].offset_applied == false && v_traffic_light[n][m].offset_applied == false)
        offsetApplied(n, m);
    else {

        if (GetValueTrafficLight('H', n, m) == 1){

            if (h_traffic_light[n][m].time < T)
                h_traffic_light[n][m].time++;
            else{

                SwitchTrafficLight(n, m);
            }
        }
        else {

            if (v_traffic_light[n][m].time < T)
                v_traffic_light[n][m].time++;
            else{
                SwitchTrafficLight(n, m);
            }
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


    if (GetValueCellStreet('H', n, m, 0) != 1) { // evitar que los vehiculos "turn"

        if (h_traffic_light[n][m].light == 0){

            h_traffic_light[n][m].light = 1;//Green
            v_traffic_light[n][m].light = 0;//Red

            h_traffic_light[n][m].time = 0;
            v_traffic_light[n][m].time = T;
        }
        else {

            h_traffic_light[n][m].light = 0;//Red
            v_traffic_light[n][m].light = 1;//Green

            v_traffic_light[n][m].time = 0;
            h_traffic_light[n][m].time = T;
        }
    }
}

int GetValueTrafficLight(char type_street, int n, int m)
{

    if (n < 0 || m < 0)
        return -1;

    if (n >= n_hor_streets)
        n = 0;

    if (m >= m_ver_streets)
        m = 0;

    if (type_street == 'H')
        return h_traffic_light[n][m].light;
    else if(type_street == 'V')
        return v_traffic_light[n][m].light;

    return -1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Self Organizing Method //

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void RunCityTrafficSelfOrganizing()
{
    int i, j, n, m;

    int light_h, light_v;
    char direction_Hor; //R-Right (Bottom for vertical) L-Left (up for vertical)
    char direction_Ver;


    for (n = 0; n < n_hor_streets; n++){

        if (n % 2 == 0)
            direction_Hor = 'R';
        else
            direction_Hor = 'L';

        for (m = 0; m < m_ver_streets; m++){

            if (m % 2 == 0)
                direction_Ver = 'R';
            else
                direction_Ver = 'L';

            TrafficSelfOrganizing(n, m, direction_Hor, direction_Ver);

            light_h = GetValueTrafficLightSO('H', n, m);
            light_v = GetValueTrafficLightSO('V', n, m);

            if (light_h == 1)
                Rule_184('H', n, m, 0, direction_Hor);//interseccion
            else if (light_v == 1) //Es probable que los dos esten en rojo por lo cual hay que verificar tambien

                Rule_184('V', n, m, 0, direction_Ver);//interseccion
            else
                SetValueCellStreet('H', n, m, 0, 0);

            VehiclesStopped('H', n, m, 0); //Establece si el vehiculo esta parado

            // HORIZONTAL ////////////////////////////////

            //Elemento anterior a la interseccion
            light_h = GetValueTrafficLightSO('H', n, m);//Se necesita el valor del semaforo adyacente

            if (light_h == 1) {//luz verde
                Rule_184('H', n, m, -1, direction_Hor);
            }
            else{ //luz roja

                //Sentido del trafico
                if (direction_Hor == 'R')
                    Rule_252('H', n, m, -1, direction_Hor);//izquierda a derecha
                else
                    Rule_136('H', n, m, -1, direction_Hor); //derecha a izquierda
            }

            VehiclesStopped('H', n, m, -1); //Establece si el vehiculo esta parado


            //primer elemento
            light_h = GetValueTrafficLightSO('H', n, m);

            if (light_h == 1) {//luz verde

                Rule_184('H', n, m, 1, direction_Hor);

            }
            else{//luz roja

                //Sentido del trafico
                if (direction_Hor == 'R')
                    Rule_136('H', n, m, 1, direction_Hor);//izquierda a derecha
                else
                    Rule_252('H', n, m, 1, direction_Hor); //derecha a izquierda
            }

            VehiclesStopped('H', n, m, 1); //Establece si el vehiculo esta parado

            //elementos intermedios
            for (i = 2; i < d_side_block; i++){
                Rule_184('H', n, m, i, direction_Hor);
                VehiclesStopped('H', n, m, i); //Establece si el vehiculo esta parado

            }

            // VERTICAL ////////////////////////////////

            //Elemento anterior a la interseccion
            light_v = GetValueTrafficLightSO('V', n, m);//Se necesita el valor del semaforo adyacente

            if (light_v == 1 ) {//luz verde

                Rule_184('V', n, m, -1, direction_Ver);
            }
            else{ //luz roja

                //Sentido del trafico
                if (direction_Ver == 'R')
                    Rule_252('V', n, m, -1, direction_Ver);//arriba a abajo
                else
                    Rule_136('V', n, m, -1, direction_Ver); //abajo a arriba

            }

            VehiclesStopped('V', n, m, -1); //Establece si el vehiculo esta parado

            //primer elemento
            light_v = GetValueTrafficLightSO('V', n, m);

            if (light_v == 1) {//luz verde
                Rule_184('V', n, m, 1, direction_Ver);
            }
            else{//luz roja

                //Sentido del trafico
                if (direction_Ver == 'R')
                    Rule_136('V', n, m, 1, direction_Ver);//arriba a abajo
                else
                    Rule_252('V', n, m, 1, direction_Ver); //abajo a arriba
            }

            VehiclesStopped('V', n, m, 1); //Establece si el vehiculo esta parado

            //elementos intermedios
            for (j = 2; j < d_side_block; j++){
                Rule_184('V', n, m, j, direction_Ver);
                VehiclesStopped('V', n, m, j); //Establece si el vehiculo esta parado
            }
        }
    }

    Velocity();
    Flux();

    SwitchMatricesRW();
}


void InializedTrafficSelfOrganizing()
{

    int n, m;
    //light;//1.- Green 0.- Red

    if (max_time_u < 0)
        max_time_u = 0;

    if (!(distance_d >= 0 && distance_d <= d_side_block))
        distance_d = d_side_block;

    if (!(distance_r >= 0 && distance_r <= distance_d))
        distance_r = distance_d;

    if (!(distance_e >= 0 && distance_e <= d_side_block))
        distance_e = d_side_block;


    for (n = 0; n < n_hor_streets; n++){
        for (m = 0; m < m_ver_streets; m++){

            h_traffic_light_so[n][m].light = 0;
            v_traffic_light_so[n][m].light = 1;

            h_traffic_light_so[n][m].time_u = 0;
            v_traffic_light_so[n][m].time_u = 0;

            h_traffic_light_so[n][m].n_vehicles = 0;
            v_traffic_light_so[n][m].n_vehicles = 0;

            h_traffic_light_so[n][m].m_vehicles = 0;
            v_traffic_light_so[n][m].m_vehicles = 0;

            h_traffic_light_so[n][m].vehicle_stop = false;
            v_traffic_light_so[n][m].vehicle_stop = false;

            h_traffic_light_so[n][m].inter_ocup = false;
            v_traffic_light_so[n][m].inter_ocup = false;

        }
    }
}


void TrafficSelfOrganizing(int n, int m, char direction_h, char direction_v)
{

    //for rules 1 and 4

    if (h_traffic_light_so[n][m].light == 1)
        h_traffic_light_so[n][m].time_u++;
    else if (v_traffic_light_so[n][m].light == 1)
        v_traffic_light_so[n][m].time_u++;

    TrafficSOApproaching(n, m, direction_h, direction_v);

    if (direction_h == 'R')
        h_traffic_light_so[n][m].vehicle_stop = GetVehiclesStopped('H', n, m, distance_e);
    else
        h_traffic_light_so[n][m].vehicle_stop = GetVehiclesStopped('H', n, m, -distance_e);


    if (direction_v == 'R')
        v_traffic_light_so[n][m].vehicle_stop = GetVehiclesStopped('V', n, m, distance_e);
    else
        v_traffic_light_so[n][m].vehicle_stop = GetVehiclesStopped('V', n, m, -distance_e);


    if (h_traffic_light_so[n][m].light == 1){

        if (h_traffic_light_so[n][m].vehicle_stop == true){

            if (v_traffic_light_so[n][m].vehicle_stop == true)
                SwitchBothRedTrafficLightSO(n, m);  //regla 6
            else
                SwitchTrafficLightSO(n, m);  // regla 5
        }
        else {
            if (h_traffic_light_so[n][m].m_vehicles < maxim_m) {

                if (h_traffic_light_so[n][m].time_u >= min_time)
                    if (v_traffic_light_so[n][m].n_vehicles >= maxim_n){

                    // qDebug () << "Cambio por cantidad de automoviles H";
                    SwitchTrafficLightSO(n, m);
                }
            }
        }
    }
    else if (v_traffic_light_so[n][m].light == 1){

        if (v_traffic_light_so[n][m].vehicle_stop == true){

            if (h_traffic_light_so[n][m].vehicle_stop == true)
                SwitchBothRedTrafficLightSO(n, m);  //regla 6
            else
                SwitchTrafficLightSO(n, m);  // regla 5
        }
        else {

            if (v_traffic_light_so[n][m].m_vehicles < maxim_m) {

                if (v_traffic_light_so[n][m].time_u >= min_time)
                    if (h_traffic_light_so[n][m].n_vehicles >= maxim_n){

                    // qDebug () << "Cambio por cantidad de automoviles V";
                    SwitchTrafficLightSO(n, m);

                }
            }
        }
    }
    else{ //implicito que ambos estan en rojo

        if (h_traffic_light_so[n][m].vehicle_stop == false)
            RestoreSingleGreen('H', n, m);
        else if (v_traffic_light_so[n][m].vehicle_stop == false)
            RestoreSingleGreen('V', n, m);

    }
}

void TrafficSOApproaching(int n, int m, char direction_h, char direction_v)
{

    int pos_cell;
    int tmp_vehicle;

    //Horizontales ///////////////////////////////////////////////////////////////////////////
    h_traffic_light_so[n][m].n_vehicles = 0;
    h_traffic_light_so[n][m].m_vehicles = 0;

    if (direction_h == 'R'){

        for (pos_cell = -distance_d; pos_cell < 0; pos_cell++){

            //Regla 1 y 3
            tmp_vehicle = GetValueCellStreet('H', n, m, pos_cell);
            h_traffic_light_so[n][m].n_vehicles+= tmp_vehicle;

            if (pos_cell >= -distance_r)
                h_traffic_light_so[n][m].m_vehicles+= tmp_vehicle;
        }
    }
    else{

        for (pos_cell = distance_d; pos_cell > 0; pos_cell--){

            //Regla 1 y 3
            tmp_vehicle = GetValueCellStreet('H', n, m, pos_cell);
            h_traffic_light_so[n][m].n_vehicles+= tmp_vehicle;

            if (pos_cell <= distance_r)
                h_traffic_light_so[n][m].m_vehicles+= tmp_vehicle;

        }
    }

    //Verticales ///////////////////////////////////////////////////////////////
    v_traffic_light_so[n][m].n_vehicles = 0;
    v_traffic_light_so[n][m].m_vehicles = 0;

    if (direction_v == 'R'){

        for (pos_cell = -distance_d; pos_cell < 0; pos_cell++){

            //Regla 1 y 3
            tmp_vehicle = GetValueCellStreet('V', n, m, pos_cell);

            v_traffic_light_so[n][m].n_vehicles+= tmp_vehicle;
            if (pos_cell >= -distance_r)
                v_traffic_light_so[n][m].m_vehicles+= tmp_vehicle;
        }
    }
    else{

        for (pos_cell = distance_d; pos_cell > 0; pos_cell--){

            //Regla 1 y 3
            tmp_vehicle = GetValueCellStreet('V', n, m, pos_cell);

            v_traffic_light_so[n][m].n_vehicles+= tmp_vehicle;
            if (pos_cell <= distance_r)
                v_traffic_light_so[n][m].m_vehicles+= tmp_vehicle;
        }
    }

}

void SwitchBothRedTrafficLightSO(int n, int m)
{

    if (GetValueCellStreet('H', n, m, 0) == 0) { // evitar que los vehiculos "turn"

        h_traffic_light_so[n][m].time_u = 0;
        v_traffic_light_so[n][m].time_u = 0;

        if (h_traffic_light_so[n][m].light == 1){
            h_traffic_light_so[n][m].light = 0;//Red
        }

        if (v_traffic_light_so[n][m].light == 1){
            v_traffic_light_so[n][m].light = 0;//Red
        }
    }

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


    if (GetValueCellStreet('H', n, m, 0) == 0) { // evitar que los vehiculos "turn"

        h_traffic_light_so[n][m].time_u = 0;
        v_traffic_light_so[n][m].time_u = 0;

        if (h_traffic_light_so[n][m].light == 0){

            h_traffic_light_so[n][m].light = 1;//Green
            v_traffic_light_so[n][m].light = 0;//Red
        }
        else {

            h_traffic_light_so[n][m].light = 0;//Red
            v_traffic_light_so[n][m].light = 1;//Green
        }
    }
}


int GetValueTrafficLightSO(char type_street, int n, int m)
{

    if (n < 0 || m < 0)
        return -1;

    if (n >= n_hor_streets)
        n = 0;

    if (m >= m_ver_streets)
        m = 0;

    if (type_street == 'H')
        return h_traffic_light_so[n][m].light;
    else if(type_street == 'V')
        return v_traffic_light_so[n][m].light;

    return -1;
}


void RestoreSingleGreen(char type_street, int n, int m)
{

    h_traffic_light_so[n][m].time_u = 0;
    v_traffic_light_so[n][m].time_u = 0;

    if (type_street == 'H'){

        h_traffic_light_so[n][m].light = 1;//Green
        v_traffic_light_so[n][m].light = 0;//Red

    }
    else{

        h_traffic_light_so[n][m].light = 0;//Red
        v_traffic_light_so[n][m].light = 1;//Green
    }
}

//Measures////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Velocity()
{

    int reposition_cell_h;
    int reposition_cell_v;
    int n, m;
    int i, j;

    int cell_changed = 0;

    for (n = 0; n < n_hor_streets; n++){
        for (m = 0; m < m_ver_streets; m++){

            reposition_cell_h = m * d_side_block + m;
            reposition_cell_v = n * d_side_block + n;

            if ((pr_intersections[n][m] == 0 && pw_intersections[n][m] == 1) || (pr_intersections[n][m] == 0 && pw_intersections[n][m] == -1))
                cell_changed++;

            for (i = 1; i <= d_side_block; i++){

                reposition_cell_h = m * d_side_block + m + i;

                if ((pr_horizontal_streets[n][reposition_cell_h] == 0 && pw_horizontal_streets[n][reposition_cell_h] == 1) || (pr_horizontal_streets[n][reposition_cell_h] == 0 && pw_horizontal_streets[n][reposition_cell_h] == -1))
                    cell_changed++;

            }

            for (j = 1; j <= d_side_block; j++){

                reposition_cell_v = n * d_side_block + n + j;

                if ((pr_vertical_streets[m][reposition_cell_v] == 0 && pw_vertical_streets[m][reposition_cell_v] == 1) || (pr_vertical_streets[m][reposition_cell_v] == 0 && pw_vertical_streets[m][reposition_cell_v] == -1))
                    cell_changed++;
            }
        }
    }

    // qDebug() << "Velocidad" << ((float) cell_changed / limit);
    velocity = ((float) cell_changed / limit);
    velocity_total+= velocity;
}

void Flux()
{
    //qDebug() << "Flujo" << ((float) densitiy * velocity);
    flux = ((float) densitiy * velocity);
    flux_total+= flux;
}

void SaveMeasures()
{
    velocity_total = velocity_total / n_ticks;
    flux_total = flux_total / n_ticks;

    fprintf(fp, "%f\t%f\t%f\n", densitiy, velocity_total, flux_total);
}

int mainFunction(int met)
{

    qDebug() << "Execute...\n ";
    //printf ("Execute...\n ");

    fp = fopen("measures.txt", "w");   // Abrir archivo para escritura

    for (densitiy = 0.02; densitiy <= 1.0; densitiy+=0.02){

        switch_matriz = true;

        total_cell = 0;
        velocity = 0;

        flux = 0;
        velocity_total = 0;
        flux_total = 0;
        n_ticks = 5400;


        n_hor_streets = 5; //An integer that represents the amount of horizontal streets
        m_ver_streets = 5; //An integer that represents the amount of vertical streets
        d_side_block = 10; //distance of the smallest area that is surrounded by streets
        T = 50;

        CityBuilding(n_hor_streets, m_ver_streets, d_side_block);
        RandomizeVehicles(densitiy);

        //Inicializa semaforos ////////////////////////////////////////////////////
        InializedTrafficLight(); //Onda verde

        maxim_n = 3;
        maxim_m = 2;
        min_time = 20;

        max_time_u = 1;
        distance_d = 7;
        distance_r = 2;
        distance_e = 2;//no contar interseccion en la distancia, importante punto!!
        InializedTrafficSelfOrganizing(); //Auto Organizado

        int tick;

        if (met == 1){
            for (tick = 0; tick < n_ticks; tick++)
                RunCityTrafficLightGreenWave();
        }
        else {
            for (tick = 0; tick < n_ticks; tick++)
                RunCityTrafficSelfOrganizing();
        }

        SaveMeasures();

        //printf("Density: %f", densitiy);
        qDebug() << "Density: " << densitiy;
        CityDestroying();
    }

    qDebug() << "End";
    //printf ("End...\n ");

    fclose(fp);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//MainWindow

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // this->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);


    QString green_wave = "green-wave";
    QString self_organizing = "self-organizing";

    ui->NameMethod->addItem(green_wave);
    ui->NameMethod->addItem(self_organizing);

    resolution_w = 700;
    resolution_h = 700;

    labelCity = new QLabel(this);
    labelCity->setGeometry(10,100,resolution_w,resolution_h);
    labelCity->setStyleSheet("background-color: rgb(0, 0, 0);");

    t_timer = new QTimer();
    connect(t_timer, SIGNAL(timeout()), this, SLOT(PaintCity()));
    t_timer->start(30);//60


    //PARAMETROS //////////////////////

    switch_matriz = true;

    total_cell = 0;
    velocity = 0;
    flux = 0;
    velocity_total = 0;
    flux_total = 0;
    n_ticks = 5400;

    n_hor_streets = 10; //An integer that represents the amount of horizontal streets
    m_ver_streets = 10; //An integer that represents the amount of vertical streets
    d_side_block = 10; //distance of the smallest area that is surrounded by streets
    T = 50;
    densitiy = 0.3;

    CityBuilding(n_hor_streets, m_ver_streets, d_side_block);
    RandomizeVehicles(densitiy);

    //Inicializa semaforos ////////////////////////////////////////////////////
    InializedTrafficLight(); //Onda verde

    maxim_n = 3;
    maxim_m = 2;
    min_time = 20;

    max_time_u = 1;
    distance_d = 7;
    distance_r = 2;
    distance_e = 2;//no contar interseccion en la distancia, importante punto!!
    InializedTrafficSelfOrganizing(); //Auto Organizado

    qi_city =  new QImage (n_hor_streets * d_side_block + n_hor_streets, m_ver_streets * d_side_block + m_ver_streets, QImage::Format_RGB888);

    metodo = 1; //Green wave

}

MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::on_NameMethod_itemSelectionChanged()
{

    QString green_wave = "green-wave";
    QString self_organizing = "self-organizing";
    QString str_item;

    QListWidgetItem *ptrItem;

    ptrItem = ui->NameMethod->currentItem();
    str_item = ptrItem->data(0).toString();

    if (str_item == green_wave){
        metodo = 1;
    }
    else if (str_item == self_organizing){
        metodo = 2;
    }
}

void MainWindow::on_pBInicia_clicked()
{

    inicio = 1;
    ui->pBInicia->setEnabled(false);
    ui->Experiment->setEnabled(false);
}

void MainWindow::on_Experiment_clicked()
{
    inicio = 0;
    ui->pBInicia->setEnabled(false);
    ui->NameMethod->setEnabled(false);
    ui->Experiment->setEnabled(false);

    mainFunction(metodo);

}

void MainWindow::PaintCity()
{

    if (inicio == 1){


        qi_city->fill(Qt::red);

        imgpaint = QPixmap::fromImage(*qi_city);
        QPainter paint(&imgpaint);

        size_s.setHeight(resolution_h);
        size_s.setWidth(resolution_w);

        if (metodo == 1){
            RunCityTrafficLightGreenWave();
            //qDebug() << "GreenWave";
        }
        else if (metodo == 2){
            RunCityTrafficSelfOrganizing();
            //qDebug() << "SelfOrganizing";
        }

        PrintStreetCity(&paint, p);
        img_scaled = imgpaint.scaled(size_s, Qt::KeepAspectRatio);
        labelCity->setPixmap(img_scaled);

    }
}

void MainWindow::PrintStreetCity(QPainter *paint, QPen &p)
{
    int i, j, n, m;

   
    unsigned int **screen;

    screen = new unsigned int*[d_hor_street];
    for (i = 0; i < d_hor_street; i++)
        screen[i] = new unsigned int[d_ver_street];

    for (i = 0; i < d_hor_street; i++)
        for (j = 0; j < d_ver_street; j++)
            screen[i][j] = 8;

    int reposition_cell_h;
    int reposition_cell_v;

    int reposition_light_h;
    int reposition_light_v;

    char direction_Hor; //R-Right (Bottom for vertical) L-Left (up for vertical)
    char direction_Ver;


    for (n = 0; n < n_hor_streets; n++){

        if (n % 2 == 0)
            direction_Hor = 'R';
        else
            direction_Hor = 'L';

        for (m = 0; m < m_ver_streets; m++){

            if (m % 2 == 0)
                direction_Ver = 'R';
            else
                direction_Ver = 'L';

            reposition_light_h = reposition_cell_h = m * d_side_block + m;
            reposition_light_v = reposition_cell_v = n * d_side_block + n;

            if (GetVehiclesStopped('H', n, m, 0) == true)
                screen[reposition_cell_v][reposition_cell_h] = -1;
            else
                screen[reposition_cell_v][reposition_cell_h] = GetValueCellStreet('H', n, m, 0);

            // HORIZONTAL ////////////////////////////////

            for (i = 1; i <= d_side_block; i++){
                reposition_cell_h = m * d_side_block + m + i;

                if (GetVehiclesStopped('H', n, m, i) == true)
                    screen[reposition_cell_v][reposition_cell_h] = -1;
                else
                    screen[reposition_cell_v][reposition_cell_h] = GetValueCellStreet('H', n, m, i);
            }

            if (metodo == 1) {

                //Pintar semaforos con green wave
                if (direction_Hor == 'R'){
                    if (m > 0){
                        screen[reposition_light_v][reposition_light_h - 1] = GetValueTrafficLight('H', n, m) == 1 ? 3 : 4;

                        if (m == (m_ver_streets - 1))
                            screen[reposition_light_v][d_hor_street - 1] = GetValueTrafficLight('H', n, 0) == 1 ? 3 : 4;
                    }
                }
                else
                    screen[reposition_light_v][reposition_light_h + 1] = GetValueTrafficLight('H', n, m) == 1 ? 3 : 4;
            }
            else {

                //Pintar semaforos con self-organizated
                if (direction_Hor == 'R'){

                    if (m > 0){
                        screen[reposition_light_v][reposition_light_h - 1] = GetValueTrafficLightSO('H', n, m) == 1 ? 3 : 4;

                        if (m == (m_ver_streets - 1))
                            screen[reposition_light_v][d_hor_street - 1] = GetValueTrafficLightSO('H', n, 0) == 1 ? 3 : 4;
                    }
                }
                else
                    screen[reposition_light_v][reposition_light_h + 1] = GetValueTrafficLightSO('H', n, m) == 1 ? 3 : 4;

            }

            // VERTICAL ////////////////////////////////

            reposition_cell_h = m * d_side_block + m;//la reinica porque fue movida anteriormente

            for (j = 1; j <= d_side_block; j++){
                reposition_cell_v = n * d_side_block + n + j;

                if (GetVehiclesStopped('V', n, m, j) == true)
                    screen[reposition_cell_v][reposition_cell_h] = -1;
                else
                    screen[reposition_cell_v][reposition_cell_h] = GetValueCellStreet('V', n, m, j);
            }

            if (metodo == 1) {
                //Pintar semaforos con green wave
                if (direction_Ver == 'R'){

                    if (n > 0){
                        screen[reposition_light_v - 1][reposition_light_h] = GetValueTrafficLight('V', n, m) == 1 ? 3 : 4;

                        if (n == (n_hor_streets - 1))
                            screen[d_ver_street - 1][reposition_light_h] = GetValueTrafficLight('V', n, 0) == 1 ? 3 : 4;
                    }
                }
                else
                    screen[reposition_light_v + 1][reposition_light_h] = GetValueTrafficLight('V', n, m) == 1 ? 3 : 4;

            }
            else {


                //Pintar semaforos con self-organizated
                if (direction_Ver == 'R'){

                    if (n > 0){
                        screen[reposition_light_v - 1][reposition_light_h] = GetValueTrafficLightSO('V', n, m) == 1 ? 3 : 4;

                        if (n == (n_hor_streets - 1))
                            screen[d_ver_street - 1][reposition_light_h] = GetValueTrafficLightSO('V', 0, m) == 1 ? 3 : 4;
                    }
                }
                else
                    screen[reposition_light_v + 1][reposition_light_h] = GetValueTrafficLightSO('V', n, m) == 1 ? 3 : 4;

            }
        }
    }


    // printf("\n");
    for (i = 0; i < d_hor_street; i++){
        for (j = 0; j < d_ver_street; j++){

            if (screen[i][j] == 0){

                p.setColor(QColor(192,192,192));
                paint->setPen(p);

                paint->drawPoint(j, i);
            }
            else if (screen[i][j] == 1){

                p.setColor(QColor(0,0,255));
                paint->setPen(p);

                paint->drawPoint(j, i);
            }
            else if (screen[i][j] == -1){

                p.setColor(QColor(255,255,0));
                paint->setPen(p);

                paint->drawPoint(j, i);
            }
            else if (screen[i][j] == 3){

                p.setColor(QColor(0,255,0));
                paint->setPen(p);

                paint->drawPoint(j, i);
                //paint->drawEllipse ( j, i, 2, 2);
            }
            else if (screen[i][j] == 4){

                p.setColor(QColor(255,0,0));
                paint->setPen(p);

                paint->drawPoint(j, i);
                // paint->drawEllipse ( j, i, 10, 10);
            }

            // qDebug() << screen[i][j] << " ";

            //p.setColor(QColor(255,0,255));
            //paint->setPen(p);
            //paint->drawPoint(j, i);
        }
        //    printf ("\n");

    }

    for (i = 0; i < d_hor_street; i++)
        delete screen[i];

    delete [] screen;
}

MyLabel::MyLabel(QWidget *parent):QLabel(parent)
{
}

MyLabel::~MyLabel()
{
}


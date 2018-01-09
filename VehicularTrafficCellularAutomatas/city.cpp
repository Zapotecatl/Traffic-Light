#include "city.h"
#include "vehicle.h"
#include "measures.h"

//Pointers for read
SCell **pr_horizontal_streets;
SCell **pr_vertical_streets;
SCell **pr_intersections;

//Pointers for write
SCell **pw_horizontal_streets;
SCell **pw_vertical_streets;
SCell **pw_intersections;

SCell **horizontal_streets_A;//Two-dimensional array representing the streets in horizontal direction
SCell **vertical_streets_A;//Two-dimensional array representing the streets in vertical direction
SCell **intersections_A;//Two dimensional array that represents the intersections between horizontal and vertical streets

SCell **horizontal_streets_Z;//Two-dimensional array representing the streets in horizontal direction
SCell **vertical_streets_Z;//Two-dimensional array representing the streets in vertical direction
SCell **intersections_Z;//Two dimensional array that represents the intersections between horizontal and vertical streets

SVehicle *pr_vehicles;
SVehicle *pw_vehicles;

SVehicle *vehicles_A;
SVehicle *vehicles_Z;

bool switch_matriz;
bool switch_vehicles;

int intersection_control;

int n_hor_streets; //An integer that represents the amount of horizontal streets
int m_ver_streets; //An integer that represents the amount of vertical streets
int d_hor_street; //horizontal distance of the streets
int d_ver_street; //vertical distance of the streets

int d_side_block; //distance of the smallest area that is surrounded by streets
int n_blocks; //An integer that represents the amount of blocks

float density;
float density_h;
float density_v;

int size_vehicles;

int total_cell;
int n_cell_h;
int n_cell_v;

int n_exp;
int n_ticks;
float size_step;

int *h_regionIntersection[2];
int *v_regionIntersection[2];

bool combination_rules[6];

//int metodo;
int inicio;

int InializedCity(int inter_control, int n_h_streets, int m_v_streets, int d_s_block, float dens_h, float dens_v)
{

    //PARAMETROS //////////////////////

    switch_matriz = true;

    if (inter_control < 1) {
        qDebug() << "ERROR: inter_control" << __PRETTY_FUNCTION__;
        inter_control = 1;
    }

    intersection_control = inter_control;

    total_cell = 0;
    _velocity = 0;
    flux = 0;
    velocity_total = 0;
    flux_total = 0;
    //n_ticks = 5400;

    n_hor_streets = n_h_streets; //An integer that represents the amount of horizontal streets
    m_ver_streets = m_v_streets; //An integer that represents the amount of vertical streets
    d_side_block = d_s_block; //distance of the smallest area that is surrounded by streets

    CityBuilding(n_hor_streets, m_ver_streets, d_side_block);

    int result = DistribuiteVehicles(dens_h, dens_v);

    return result;
}

//The city is set to agree to the number of horizontal and vertical streets, and the distance of the blocks
int CityBuilding(int n_hor, int m_ver, int d_blk)
{

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

    //Las celdas solo consideran las calles (no todo el terreno)
    n_cell_h = n_hor_streets * (d_side_block * m_ver_streets) + m_ver_streets;
    n_cell_v = m_ver_streets * (d_side_block * n_hor_streets) + n_hor_streets;
    total_cell = n_cell_h + n_cell_v; //2 * d_side_block * n_hor_streets * m_ver_streets + n_hor_streets * m_ver_streets;

    //The required memory is reserved dynamically.
    //The horizontal distance is greater when more vertical streets.
    //The vertical distance is greater when there is more horizontal streets.
    d_hor_street = d_side_block * m_ver_streets + m_ver_streets;
    d_ver_street = d_side_block * n_hor_streets + n_hor_streets;

    allocateMemoryCity();
    allocateMemoryRegionIntersection();
    regionIntersection();

    //The streets are divided into cells. The state of the cells are classified as follows:
    //0 - The cell is empty
    //1 - The cell contains a vehicle
    //2 - The cell is an intersection (for what needs to redirected to the array of intersections).

    SwitchMatricesRW();
    resetCityWrite();
    SwitchMatricesRW();
    resetCityWrite();

    //printCity();

    return 0;
}

int DistribuiteVehicles(float d_h, float d_v)
{
    int i;
    int m, n;

    if (d_h < 0.0){
        qDebug() << "Warning DH:" << d_h;
        density_h = 0.0;
    }
    else if (d_h > 1.0){
        qDebug() << "Warning DH:" << d_h;
        density_h = 1.0;
    }
    else
        density_h = d_h;

    if (d_v < 0.0){
        qDebug() << "Warning DV:" << d_v;
        density_v = 0.0;
    }
    else if (d_v > 1.0){
        qDebug() << "Warning DV:" << d_v;
        density_v = 1.0;
    }
    else
        density_v = d_v;

    density = (density_h + density_v) / 2.0;

    if (ls == 0) {
        qDebug() << "ERROR: division entre 0, Invalid size of vehicle in ls";
        return 0;
    }

    int n_veh_h = n_cell_h / ls;
    int size_vehicles_h = ceil((float) density_h * n_veh_h);

    int n_veh_v = n_cell_v / ls;
    int size_vehicles_v = ceil((float) density_v * n_veh_v);

    vector<SVehicle> random_pos;
    vector<SVehicle> random_pos_h;
    vector<SVehicle> random_pos_v;

    random_pos_h.clear();
    random_pos_v.clear();
    random_pos.clear();

    SVehicle tmp_vehicle;

    int id = 0;
    for (n = 0; n < n_hor_streets; n++) {

        int p1 = d_hor_street - 1;
        int p2 = p1 - (ls - 1);
        int direction = (n % 2 == 0) ? 'R' : 'L';

        m = m_ver_streets - 1;
        while (m >= 0) {

            int pos_intersection = getIntersectionPosition('H', n, m).x;

            while (!(pos_intersection >= p2 && pos_intersection <= p1)) {

                tmp_vehicle.id = id;
                tmp_vehicle.type_street = 'H';
                tmp_vehicle.speed = 0;
                tmp_vehicle.ls = ls;
                tmp_vehicle.visible = true;
                tmp_vehicle.autonomous = false;
                tmp_vehicle.color.r = 0;
                tmp_vehicle.color.g = 0;
                tmp_vehicle.color.b = 255;
                tmp_vehicle.direction = direction;

                if (direction == 'R') {
                    tmp_vehicle.position.x = p1;
                    tmp_vehicle.front_position = p1;
                    tmp_vehicle.rear_position = p2;
                    tmp_vehicle.previous_position = tmp_vehicle.position.x;
                }
                else {
                    tmp_vehicle.position.x = p2;
                    tmp_vehicle.front_position = p2;
                    tmp_vehicle.rear_position = p1;
                    tmp_vehicle.previous_position = tmp_vehicle.position.x;
                }

                tmp_vehicle.position.y = n;
                random_pos_h.push_back(tmp_vehicle);
                id++;

                p1 = p2 - 1;
                p2 = p1 - (ls - 1);
            }

            p1 = pos_intersection - 1;
            p2 = p1 - (ls - 1);
            m--;
        }
    }

    for (m = 0; m < m_ver_streets; m++) {

        int p1 = d_ver_street - 1;
        int p2 = p1 - (ls - 1);
        int direction = (m % 2 == 0) ? 'R' : 'L';

        n = n_hor_streets - 1;
        while (n >= 0) {

            int pos_intersection = getIntersectionPosition('V', n, m).x;

            while (!(pos_intersection >= p2 && pos_intersection <= p1)) {

                tmp_vehicle.id = id;
                tmp_vehicle.type_street = 'V';
                tmp_vehicle.speed = 0;
                tmp_vehicle.ls = ls;
                tmp_vehicle.visible = true;
                tmp_vehicle.autonomous = false;
                tmp_vehicle.color.r = 0;
                tmp_vehicle.color.g = 0;
                tmp_vehicle.color.b = 255;
                tmp_vehicle.direction = direction;

                if (direction == 'R') {
                    tmp_vehicle.position.x = p1;
                    tmp_vehicle.front_position = p1;
                    tmp_vehicle.rear_position = p2;
                    tmp_vehicle.previous_position = tmp_vehicle.position.x;
                }
                else {
                    tmp_vehicle.position.x = p2;
                    tmp_vehicle.front_position = p2;
                    tmp_vehicle.rear_position = p1;
                    tmp_vehicle.previous_position = tmp_vehicle.position.x;
                }

                tmp_vehicle.position.y = m;
                random_pos_v.push_back(tmp_vehicle);
                id++;

                p1 = p2 - 1;
                p2 = p1 - (ls - 1);
            }

            p1 = pos_intersection - 1;
            p2 = p1 - (ls - 1);
            n--;
        }
    }


    //    random_shuffle(random_pos_h.begin(), random_pos_h.end(), myrandom);
    //    random_pos_h.resize(size_vehicles_h);

    //    random_shuffle(random_pos_v.begin(), random_pos_v.end(), myrandom);
    //    random_pos_v.resize(size_vehicles_v);

    //Version C++11 para generar los aleatorios
    // obtain a time-based seed:
    unsigned seed;

    seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle (random_pos_h.begin(), random_pos_h.end(), std::default_random_engine(seed));

    if (size_vehicles_h > random_pos_h.size())
        size_vehicles_h = random_pos_h.size();
    random_pos_h.resize(size_vehicles_h);

    usleep(2000);
    seed = std::chrono::system_clock::now().time_since_epoch().count();

    shuffle (random_pos_v.begin(), random_pos_v.end(), std::default_random_engine(seed));

    if (size_vehicles_v > random_pos_v.size())
        size_vehicles_v = random_pos_v.size();
    random_pos_v.resize(size_vehicles_v);

    random_pos_h.insert(random_pos_h.end(), random_pos_v.begin(), random_pos_v.end());
    random_pos.insert(random_pos.end(), random_pos_h.begin(), random_pos_h.end());

    random_shuffle(random_pos.begin(), random_pos.end(), myrandom);//Coloca en ordena aleatorio las posiciones iniciales

    //qDebug() << "Numero h y s:" << size_vehicles_h << size_vehicles_v;
    //qDebug() << "Numero h y s:" << random_pos_h.size() << random_pos_v.size();
    //qDebug() << "Random size vehicles: " << random_pos.size();

    size_vehicles = size_vehicles_h + size_vehicles_v;

    // qDebug() << "Numero h y s:" << h_size_vehices << v_size_vehices;
    // qDebug() << "Numero h y s:" << random_pos_hor.size() << random_pos_ver.size();
    allocateMemoryVehicles(size_vehicles);
    SwitchVehiclesRW();

    if (random_pos.size() < size_vehicles) {
        qDebug() << "ERROR: invalid number of random vehicles";
        return 0;
    }

    int size_autonomous = per_autonomous * size_vehicles;
    //qDebug () << size_autonomous << per_autonomous << size_vehicles;

    i = 0;
    while (i < size_vehicles) {
        vehicles_A[i] = random_pos[i];
        vehicles_Z[i] = random_pos[i];
        vehicles_A[i].id = i;
        vehicles_Z[i].id = i;

        if (i < size_autonomous){
            vehicles_A[i].autonomous = true;
            vehicles_Z[i].autonomous = true;
        }

        SetVehicleCell(vehicles_A[i].position.y, vehicles_A[i].position.x, true, i);
        i++;
    }

    //qDebug() << "Size:" << size_vehicles;
    //qDebug() << "Random size:" << random_pos.size();

    random_pos_h.clear();
    random_pos_v.clear();
    random_pos.clear();

    SwitchMatricesRW();
    SwitchVehiclesRW();

    return 1;
}

void SetVehicleCell(int y, int x, bool visible, int id)
{

    if (id < 0 || id >= size_vehicles) {
        qDebug() << "ERROR: " << __PRETTY_FUNCTION__;
        id = 0;
    }

    int d_street;
    if (pr_vehicles[id].type_street == 'H')
        d_street = d_hor_street;
    else
        d_street = d_ver_street;

    //Body val = 1 (front), val = 2 (body), ..., val = ls (rear)
    int body;
    if (pr_vehicles[id].direction == 'R') {//direccion derecha
        int x_new;
        for (body = 1; body <= ls; body++){
            x_new = (x - (body - 1)) >= 0 ? x - (body - 1) : d_street + (x - (body - 1));
            SetValueCellStreet(y, x_new, body, visible, id);
        }
    }
    else {//direccion izquierda
        int x_new;
        for (body = 1; body <= ls; body++){
            x_new = (x + (body - 1)) <= (d_street - 1) ? x + (body - 1) : (x + (body - 1)) - d_street;
            SetValueCellStreet(y, x_new, body, visible, id);
        }
    }

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

void resetCityWrite()
{
    int i;
    int m, n;

    for (n = 0; n < n_hor_streets; n++){
        for (i = 0; i < d_hor_street; i++){
            pw_horizontal_streets[n][i].value = 0;
            pw_horizontal_streets[n][i].id = INVALID;
            pw_horizontal_streets[n][i].visible = true;
        }
    }

    for (m = 0; m < m_ver_streets; m++){
        for (i = 0; i < d_ver_street; i++){
            pw_vertical_streets[m][i].value = 0;
            pw_vertical_streets[m][i].id = INVALID;
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
            pw_intersections[n][m].id = INVALID;
            pw_intersections[n][m].visible = true;

            pw_horizontal_streets[n][x_h].value = POINTER_INTERSECTION;
            pw_vertical_streets[m][x_v].value = POINTER_INTERSECTION;

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


int allocateMemoryCity()
{
    int i;

    pr_intersections = NULL;
    pr_horizontal_streets = NULL;
    pr_vertical_streets = NULL;

    pw_intersections = NULL;
    pw_horizontal_streets = NULL;
    pw_vertical_streets = NULL;

    intersections_A = NULL;
    horizontal_streets_A = NULL;
    vertical_streets_A = NULL;
    intersections_Z = NULL;
    horizontal_streets_Z = NULL;
    vertical_streets_Z = NULL;

    //Pointer of read an write to Matrices A and Z
    pr_intersections = new SCell*[n_hor_streets];
    pr_horizontal_streets = new SCell*[n_hor_streets];
    pr_vertical_streets = new SCell*[m_ver_streets];

    pw_intersections = new SCell*[n_hor_streets];
    pw_horizontal_streets = new SCell*[n_hor_streets];
    pw_vertical_streets = new SCell*[m_ver_streets];

    //Matrices A
    intersections_A = new SCell*[n_hor_streets];
    for (i = 0; i < n_hor_streets; i++)
        intersections_A[i] = new SCell[m_ver_streets];

    //reserved by n horizontal streets
    horizontal_streets_A = new SCell*[n_hor_streets];
    for (i = 0; i < n_hor_streets; i++)
        horizontal_streets_A[i] = new SCell[d_hor_street];

    //reserved by vertical distance streets
    vertical_streets_A = new SCell*[m_ver_streets];
    for (i = 0; i < m_ver_streets; i++)
        vertical_streets_A[i] = new SCell[d_ver_street];

    //Matrices Z
    intersections_Z = new SCell*[n_hor_streets];
    for (i = 0; i < n_hor_streets; i++)
        intersections_Z[i] = new SCell[m_ver_streets];

    //reserved by n horizontal streets
    horizontal_streets_Z = new SCell*[n_hor_streets];
    for (i = 0; i < n_hor_streets; i++)
        horizontal_streets_Z[i] = new SCell[d_hor_street];

    //reserved by vertical distance streets
    vertical_streets_Z = new SCell*[m_ver_streets];
    for (i = 0; i < m_ver_streets; i++)
        vertical_streets_Z[i] = new SCell[d_ver_street];

    return 0;
}


void FreeCity()
{
    int i;

    FreeVehicles();
    freeRegionIntersection();

    if (vertical_streets_Z != NULL){
        for (i = 0; i < m_ver_streets; i++)
            delete [] vertical_streets_Z[i];
        delete [] vertical_streets_Z;
        vertical_streets_Z = NULL;
    }

    if (horizontal_streets_Z != NULL){
        for (i = 0; i < n_hor_streets; i++)
            delete [] horizontal_streets_Z[i];
        delete [] horizontal_streets_Z;
        horizontal_streets_Z = NULL;
    }

    if (intersections_Z != NULL){
        for (i = 0; i < n_hor_streets; i++)
            delete [] intersections_Z[i];
        delete [] intersections_Z;
        intersections_Z = NULL;
    }

    if (vertical_streets_A != NULL){
        for (i = 0; i < m_ver_streets; i++)
            delete [] vertical_streets_A[i];
        delete [] vertical_streets_A;
        vertical_streets_A = NULL;
    }

    if (horizontal_streets_A != NULL){
        for (i = 0; i < n_hor_streets; i++)
            delete [] horizontal_streets_A[i];
        delete [] horizontal_streets_A;
        horizontal_streets_A = NULL;
    }

    if (intersections_A != NULL){
        for (i = 0; i < n_hor_streets; i++)
            delete [] intersections_A[i];
        delete [] intersections_A;
        intersections_A = NULL;
    }

    if (pw_intersections != NULL){
        delete [] pw_intersections;
        pw_intersections = NULL;
    }

    if (pw_horizontal_streets != NULL){
        delete [] pw_horizontal_streets;
        pw_horizontal_streets = NULL;
    }

    if (pw_vertical_streets != NULL){
        delete [] pw_vertical_streets;
        pw_vertical_streets = NULL;
    }

    if (pr_intersections != NULL){
        delete [] pr_intersections;
        pr_intersections = NULL;
    }

    if (pr_horizontal_streets != NULL){
        delete [] pr_horizontal_streets;
        pr_horizontal_streets = NULL;
    }

    if (pr_vertical_streets != NULL){
        delete [] pr_vertical_streets;
        pr_vertical_streets = NULL;
    }

}

void SetValueCellStreet(int y, int x, int value, bool visible, int id)
{

    if (id < 0 || id >= size_vehicles) {
        qDebug() << "ERROR: " << __PRETTY_FUNCTION__;
        id = 0;
    }

    if (pr_vehicles[id].type_street == 'H'){

        if (y < 0 || y >= n_hor_streets){
            qDebug() << "ERROR: n_hor_streets" << __PRETTY_FUNCTION__;
            y = 0;
        }

        if (x < 0 || x >= d_hor_street){
            qDebug() << "ERROR: d_hor_street, x = " << x << __PRETTY_FUNCTION__;
            x = 0;
        }

        if (pw_horizontal_streets[y][x].value == POINTER_INTERSECTION) {
            int m = x / (1 + d_side_block);
            pw_intersections[y][m].value = value;
            pw_intersections[y][m].id = id;
            pw_intersections[y][m].visible = visible;

            if (pw_intersections[y][m].value >= 1)
                collisions++;
        }
        else {
            pw_horizontal_streets[y][x].value = value;
            pw_horizontal_streets[y][x].id = id;
            pw_horizontal_streets[y][x].visible = visible;
        }
    }
    else {

        if (y < 0 || y >= m_ver_streets){
            qDebug() << "ERROR: m_ver_streets" << __PRETTY_FUNCTION__;
            y = 0;
        }

        if (x < 0 || x >= d_ver_street){
            qDebug() << "ERROR: d_ver_street, x = " << x << __PRETTY_FUNCTION__;
            x = 0;
        }

        if (pw_vertical_streets[y][x].value == POINTER_INTERSECTION){
            int n = x / (1 + d_side_block);
            pw_intersections[n][y].value = value;
            pw_intersections[n][y].id = id;
            pw_intersections[n][y].visible = visible;

            if (pw_intersections[n][y].value >= 1)
                collisions++;
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
            qDebug() << "ERROR: n_hor_streets" << __PRETTY_FUNCTION__;
            return 0;
        }
        if (x < 0 || x >= d_hor_street) {
            qDebug() << "ERROR: d_hor_street" << __PRETTY_FUNCTION__;
            return 0;
        }
        if (pr_horizontal_streets[y][x].value == POINTER_INTERSECTION) {
            int m = x / (1 + d_side_block);
            value = pr_intersections[y][m].value;
        }
        else
            value = pr_horizontal_streets[y][x].value;
    }
    else {
        if (y < 0 || y >= m_ver_streets) {
            qDebug() << "ERROR: m_ver_streets" << __PRETTY_FUNCTION__;
            return 0;
        }
        if (x < 0 || x >= d_ver_street) {
            qDebug() << "ERROR: d_ver_street" << __PRETTY_FUNCTION__;
            return 0;
        }
        if (pr_vertical_streets[y][x].value == POINTER_INTERSECTION) {
            int n = x / (1 + d_side_block);
            value = pr_intersections[n][y].value;
        }
        else
            value = pr_vertical_streets[y][x].value;
    }

    return value;
}



int GetIDCellStreet(char type_street, int y, int x)
{
    int id;

    id = INVALID;
    if (type_street == 'H'){
        if (y < 0 || y >= n_hor_streets) {
            qDebug() << "ERROR: n_hor_streets" << __PRETTY_FUNCTION__;
            return INVALID;
        }

        if (x < 0 || x >= d_hor_street) {
            qDebug() << "ERROR: d_hor_street" << __PRETTY_FUNCTION__;
            return INVALID;
        }

        if (pr_horizontal_streets[y][x].value == POINTER_INTERSECTION) {
            int m = x / (1 + d_side_block);
            id = pr_intersections[y][m].id;
        }
        else
            id = pr_horizontal_streets[y][x].id;
    }
    else if (type_street == 'V'){

        if (y < 0 || y >= m_ver_streets) {
            qDebug() << "ERROR: m_ver_streets" << __PRETTY_FUNCTION__;
            return INVALID;
        }

        if (x < 0 || x >= d_ver_street) {
            qDebug() << "ERROR: d_ver_street" << __PRETTY_FUNCTION__;
            return INVALID;
        }

        if (pr_vertical_streets[y][x].value == POINTER_INTERSECTION) {
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
            qDebug() << "ERROR: n_hor_streets" << __PRETTY_FUNCTION__;
            return false;
        }

        if (x < 0 || x >= d_hor_street) {
            qDebug() << "ERROR: d_hor_street" << __PRETTY_FUNCTION__;
            return false;
        }

        if (pr_horizontal_streets[y][x].value == POINTER_INTERSECTION) {
            int m = x / (1 + d_side_block);
            visible = pr_intersections[y][m].visible;
        }
        else
            visible = pr_horizontal_streets[y][x].visible;
    }
    else if (type_street == 'V'){
        if (y < 0 || y >= m_ver_streets){
            qDebug() << "ERROR: m_ver_streets" << __PRETTY_FUNCTION__;
            return false;
        }

        if (x < 0 || x >= d_ver_street){
            qDebug() << "ERROR: d_ver_street" << __PRETTY_FUNCTION__;
            return false;
        }

        if (pr_vertical_streets[y][x].value == POINTER_INTERSECTION) {
            int n = x / (1 + d_side_block);
            visible = pr_intersections[n][y].visible;
        }
        else
            visible = pr_vertical_streets[y][x].visible;
    }

    return visible;
}


bool turn(int &y, int &x, int &v, bool visible, int id)
{

    int is_intersection;
    char new_type_street;
    char direction;

    if (id < 0 || id >= size_vehicles) {
        qDebug() << "ERROR: " << __PRETTY_FUNCTION__;
        id = 0;
    }

    if (pr_vehicles[id].type_street == 'H'){

        if (y < 0 || y >= n_hor_streets) {
            qDebug() << "ERROR: n_hor_streets" << __PRETTY_FUNCTION__;
            return false;
        }

        if (x < 0 || x >= d_hor_street) {
            qDebug() << "ERROR: d_hor_street" << __PRETTY_FUNCTION__;
            return false;
        }

        is_intersection = pr_horizontal_streets[y][x].value;
        new_type_street = 'V';

    }else {

        if (y < 0 || y >= m_ver_streets) {
            qDebug() << "ERROR: m_ver_streets" << __PRETTY_FUNCTION__;
            return false;
        }

        if (x < 0 || x >= d_ver_street) {
            qDebug() << "ERROR: d_ver_street" << __PRETTY_FUNCTION__;
            return false;
        }

        is_intersection = pr_vertical_streets[y][x].value;
        new_type_street = 'H';

    }


    if (is_intersection == POINTER_INTERSECTION) {

        int y_new = x / (1 + d_side_block);
        int x_new = y * d_side_block + y;

        y = y_new;
        x = x_new;

        SetTypeStreetVehicle(new_type_street, id);
        SetPositionVehicle(y, x, id);

        if ((y % 2) == 0)
            direction = 'R';
        else
            direction = 'L';

        SetDirectionVehicle(direction, id);
        SetVelocityVehicle(v, id);
        SetVisibleVehicle(visible, id);

        return true;
    }

    return false;
}

bool isIntersection(char type_street, int y, int x)
{
    if (type_street == 'H'){
        if (pr_horizontal_streets[y][x].value == POINTER_INTERSECTION)
            return true;
    }
    else {
        if (pr_vertical_streets[y][x].value == POINTER_INTERSECTION)
            return true;
    }

    return false;
}

SPosition getIntersectionPosition(char type_street, int n, int m)
{
    SPosition position;

    if (type_street == 'H') {
        position.x = m * d_side_block + m;
        position.y = n;
    }
    else {
        position.x = n * d_side_block + n;
        position.y = m;
    }

    return  position;

}


int searchVehicleFrontID(int id)
{
    char type_street;
    char direction;
    int y, x;
    int rear;
    int id_f;
    int k;
    int upper_limit;
    int lower_limit;

    SVehicle vehicle = GetVehicle(id);

    type_street = vehicle.type_street;
    direction = vehicle.direction;
    y = vehicle.position.y;
    x = vehicle.position.x;
    rear = vehicle.rear_position;

    id_f = INVALID;

    if (type_street == 'H')
        upper_limit = d_hor_street;
    else
        upper_limit = d_ver_street;

    lower_limit = rear;

    if (direction == 'R'){

        for (k = x + 1; k < upper_limit; k++)
            if (GetValueCellStreet(type_street, y, k) >= 1) {

                id_f = GetIDCellStreet(type_street, y, k);

                if (id_f != id) {
                    return id_f;
                }
                else {
                    return INVALID;
                }
            }

        for (k = 0; k <= lower_limit; k++)
            if (GetValueCellStreet(type_street, y, k) >= 1) {
                id_f = GetIDCellStreet(type_street, y, k);
                if (id_f != id)
                    return id_f;
                else {
                    return INVALID;
                }
            }
    }
    else {

        for (k = x - 1; k >= 0; k--)
            if (GetValueCellStreet(type_street, y, k) >= 1) {
                id_f = GetIDCellStreet(type_street, y, k);

                if (id_f != id)
                    return id_f;
                else {
                    return INVALID;
                }
            }

        for (k = upper_limit - 1; k >= lower_limit; k--)
            if (GetValueCellStreet(type_street, y, k) >= 1) {
                id_f = GetIDCellStreet(type_street, y, k);
                if (id_f != id)
                    return id_f;
                else{
                    return INVALID;
                }
            }
    }


    return id_f;
}


int GetPositionIntersection(char type_street, int n, int m)
{
    int pos_int;

    pos_int = GetPositionIntersectionTrafficLightSO(type_street, n, m);

}

void allocateMemoryRegionIntersection()
{
    h_regionIntersection[0] = NULL;
    h_regionIntersection[1] = NULL;

    v_regionIntersection[0] = NULL;
    v_regionIntersection[1] = NULL;

    h_regionIntersection[0] = new int[d_hor_street];
    h_regionIntersection[1] = new int[d_hor_street];

    v_regionIntersection[0] = new int[d_ver_street];
    v_regionIntersection[1] = new int[d_ver_street];
}

void freeRegionIntersection()
{
    if (h_regionIntersection[0] != NULL){
        delete [] h_regionIntersection[0];
        h_regionIntersection[0] = NULL;
    }

    if (h_regionIntersection[1] != NULL){
        delete [] h_regionIntersection[1];
        h_regionIntersection[1] = NULL;
    }

    if (v_regionIntersection[0] != NULL){
        delete [] v_regionIntersection[0];
        v_regionIntersection[0] = NULL;
    }

    if (v_regionIntersection[1] != NULL){
        delete [] v_regionIntersection[1];
        v_regionIntersection[1] = NULL;
    }
}

void regionIntersection() {

    //Create region for each traffic light
    int k, i, m, n;
    int reposition_cell_h;
    int reposition_cell_v;


    for (k = 0; k < 2; k++){
        for (m = 0; m < m_ver_streets; m++){

            reposition_cell_h = m * d_side_block + m;

            for (i = reposition_cell_h; i <= reposition_cell_h + d_side_block; i++){

                if (k == 0){
                    if ((m + 1) == m_ver_streets){
                        h_regionIntersection[k][i] = 0;
                    }
                    else{
                        h_regionIntersection[k][i] = m + 1;
                    }
                }
                else{
                    if (i == 0){
                        h_regionIntersection[k][i] = m_ver_streets - 1;
                    }
                    else {

                        if (i == reposition_cell_h)
                            h_regionIntersection[k][i] = m - 1;
                        else
                            h_regionIntersection[k][i] = m;
                    }
                }
                //printf("%d : %d - %d\n", k, i, h_regionTraffic_light[k][i]);
            }
        }
    }

    for (k = 0; k < 2; k++){
        for (n = 0; n < n_hor_streets; n++){

            reposition_cell_v = n * d_side_block + n;

            for (i = reposition_cell_v; i <= reposition_cell_v + d_side_block; i++){

                if (k == 0){
                    if ((n + 1) == n_hor_streets){
                        v_regionIntersection[k][i] = 0;
                    }
                    else{
                        v_regionIntersection[k][i] = n + 1;
                    }
                }
                else{
                    if (i == 0){
                        v_regionIntersection[k][i] = n_hor_streets - 1;
                    }
                    else {

                        if (i == reposition_cell_v)
                            v_regionIntersection[k][i] = n - 1;
                        else
                            v_regionIntersection[k][i] = n;
                    }
                }
                // printf("%d : %d - %d\n", k, i, v_regionTraffic_light[k][i]);
            }
        }
    }
}

int GetIndexIntersection(char type_street, int x, int direction)
{

    int value;

    if (type_street == 'H') {

        if (direction == 'R')
            value = h_regionIntersection[0][x];
        else
            value = h_regionIntersection[1][x];
    }
    else {

        if (direction == 'R')
            value = v_regionIntersection[0][x];
        else
            value = v_regionIntersection[1][x];
    }

    return value;

}

/*
void SetIDCellStreet(int y, int x, int id)
{

    if (id < 0 || id >= size_vehicles) {
        qDebug() << "ERROR: " << __PRETTY_FUNCTION__;
        id = 0;
    }

    if (pr_vehicles[id].type_street == 'H'){
        if (y < 0 || y >= n_hor_streets) {
            qDebug() << "ERROR: n_hor_streets" << __PRETTY_FUNCTION__;
            y = 0;
        }

        if (x < 0 || x >= d_hor_street) {
            qDebug() << "ERROR: d_hor_street" << __PRETTY_FUNCTION__;
            x = 0;
        }

        if (pr_horizontal_streets[y][x].value == 2) {
            int m = x / (1 + d_side_block);
            pw_intersections[y][m].id = id;
        }
        else
            pw_horizontal_streets[y][x].id = id;
    }
    else {
        if (y < 0 || y >= m_ver_streets) {
            qDebug() << "ERROR: m_ver_streets" << __PRETTY_FUNCTION__;
            y = 0;
        }

        if (x < 0 || x >= d_ver_street) {
            qDebug() << "ERROR: d_ver_street" << __PRETTY_FUNCTION__;
            x = 0;
        }

        if (pr_vertical_streets[y][x].value == 2) {
            int n = x / (1 + d_side_block);
            pw_intersections[n][y].id = id;
        }
        else
            pw_vertical_streets[y][x].id = id;
    }

}*/

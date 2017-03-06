#include "city.h"
#include "vehicle.h"
#include "measures.h"

bool switch_matriz;
bool switch_vehicles;

int n_hor_streets; //An integer that represents the amount of horizontal streets
int m_ver_streets; //An integer that represents the amount of vertical streets
int d_hor_street; //horizontal distance of the streets
int d_ver_street; //vertical distance of the streets

int d_side_block; //distance of the smallest area that is surrounded by streets
int n_blocks; //An integer that represents the amount of blocks

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

int size_vehicles;

SVehicle *pr_vehicles;
SVehicle *pw_vehicles;

SVehicle *vehicles_A;
SVehicle *vehicles_Z;

vector<SVehicle> random_pos;
vector<SVehicle> random_pos_h;
vector<SVehicle> random_pos_v;

float density;
float density_h;
float density_v;

int total_cell;
int n_cell_h;
int n_cell_v;
float p_turn;

int collisions;

float save_velocity;
float save_flux;
float save_value_intersections;

int n_exp;
int n_ticks;
float size_step;

int error_fp = 0;
int error_fn = 0;

int test_error_fp = 0;
int test_error_fn = 0;

FILE *fp_f;
FILE *fp_v;
FILE *fp_i;

FILE *fp_fopt;
FILE *fp_vopt;

bool combination_rules[6];

//int metodo;
int inicio;

// random generator function:
int myrandom (int i)
{
    return rand() % i;
}

double frand()
{
    return (double)rand() / RAND_MAX;
}


void convertBinary(int num, bool *numBin, int size)
{

    if (num < 0 || size <= 0) {
        cout << "W: Entrada invalida para convertir a binario" << endl;
        return;
    }
    else if(num == 0){
        for (int i = 0; i < size; i++)
            numBin[i] = false;
        return;
    }

    for (int i = 0; i < size; i++) {
        numBin[i] = ((num % 2) == 0) ? false : true;
        num = num / 2;
    }

}

int allocateMemoryCity()
{
    int i, m, n;

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

void FreeVehicles()
{
    delete [] vehicles_Z;
    delete [] vehicles_A;
}

void FreeCity()
{
    int i;



    for (i = 0; i < m_ver_streets; i++)
        delete [] vertical_streets_Z[i];
    delete [] vertical_streets_Z;

    for (i = 0; i < n_hor_streets; i++)
        delete [] horizontal_streets_Z[i];
    delete [] horizontal_streets_Z;

    //Matrices Z
    for (i = 0; i < n_hor_streets; i++)
        delete [] intersections_Z[i];
    delete [] intersections_Z;

    for (i = 0; i < m_ver_streets; i++)
        delete [] vertical_streets_A[i];
    delete [] vertical_streets_A;

    for (i = 0; i < n_hor_streets; i++)
        delete [] horizontal_streets_A[i];
    delete [] horizontal_streets_A;

    //Matrices A
    for (i = 0; i < n_hor_streets; i++)
        delete [] intersections_A[i];
    delete [] intersections_A;

    delete [] pw_intersections;
    delete [] pw_horizontal_streets;
    delete [] pw_vertical_streets;

    delete [] pr_intersections;
    delete [] pr_horizontal_streets;
    delete [] pr_vertical_streets;

}

//The city is set to agree to the number of horizontal and vertical streets, and the distance of the blocks
int CityBuilding(int n_hor, int m_ver, int d_blk)
{
    int i, n, m;

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

int InializedCity(int n_h_streets, int m_v_streets, int d_s_block, float dens_h, float dens_v, float p_t)
{
    //PARAMETROS //////////////////////

    switch_matriz = true;

    total_cell = 0;
    velocity = 0;
    flux = 0;
    velocity_total = 0;
    flux_total = 0;
    //n_ticks = 5400;

    n_hor_streets = n_h_streets; //An integer that represents the amount of horizontal streets
    m_ver_streets = m_v_streets; //An integer that represents the amount of vertical streets
    d_side_block = d_s_block; //distance of the smallest area that is surrounded by streets

    p_turn = p_t >= 0.0 && p_t <= 1.0 ? p_t : 0.0; //probabilidad de girar en las intersecciones

    CityBuilding(n_hor_streets, m_ver_streets, d_side_block);

    int result = DistribuiteVehicles(dens_h, dens_v);

    return result;
}


bool isIntersection(char type_street, int y, int x)
{
    if (type_street == 'H'){
        if (pr_horizontal_streets[y][x].value == 2)
            return true;
    }
    else {
        if (pr_vertical_streets[y][x].value == 2)
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

int RunSimulation(int tick)
{

    if (metodo_light == 1)
        RunSimulationGreenWave(tick);
    else
        RunSimulationSelfOrganization(tick);

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


    int n_veh_h = n_cell_h;
    int size_vehicles_h = ceil(density_h * n_veh_h);

    int n_veh_v = n_cell_v;
    int size_vehicles_v = ceil(density_v * n_veh_v);


    size_vehicles = size_vehicles_h + size_vehicles_v;


    // qDebug() << "Numero h y s:" << h_size_vehices << v_size_vehices;
    // qDebug() << "Numero h y s:" << random_pos_hor.size() << random_pos_ver.size();

    pr_vehicles = NULL;
    pw_vehicles = NULL;

    if (size_vehicles <= 0) {
        qDebug() << "ERROR: invalid number of vehicles: " << size_vehicles;
        return -1;
    }

    vehicles_A = new SVehicle[size_vehicles];
    vehicles_Z = new SVehicle[size_vehicles];

    SwitchVehiclesRW();

    random_pos_h.clear();
    random_pos_v.clear();
    random_pos.clear();

    int id = 0;
    SVehicle tmp_vehicle;

    for (n = 0; n < n_hor_streets; n++) {

        int p1 = d_hor_street - 1;
        int direction = (n % 2 == 0) ? 'R' : 'L';

        m = m_ver_streets - 1;
        while (m >= 0) {

            int pos_intersection = getIntersectionPosition('H', n, m).x;

            while (pos_intersection != p1) {

                tmp_vehicle.id = id;
                tmp_vehicle.type_street = 'H';

                tmp_vehicle.speed = 0;
                tmp_vehicle.visible = true;
                tmp_vehicle.color.r = 0;
                tmp_vehicle.color.g = 0,
                        tmp_vehicle.color.b = 255;
                tmp_vehicle.direction = direction;

                tmp_vehicle.position.x = p1;
                tmp_vehicle.position.y = n;

                random_pos_h.push_back(tmp_vehicle);
                id++;

                p1--;

            }

            p1 = pos_intersection - 1;
            m--;
        }
    }


    for (m = 0; m < m_ver_streets; m++) {

        int p1 = d_ver_street - 1;
        int direction = (m % 2 == 0) ? 'R' : 'L';

        n = n_hor_streets - 1;
        while (n >= 0) {

            int pos_intersection = getIntersectionPosition('V', n, m).x;

            while (pos_intersection != p1) {

                tmp_vehicle.id = id;
                tmp_vehicle.type_street = 'V';

                tmp_vehicle.speed = 0;
                tmp_vehicle.visible = true;
                tmp_vehicle.color.r = 0;
                tmp_vehicle.color.g = 0,
                        tmp_vehicle.color.b = 255;
                tmp_vehicle.direction = direction;

                tmp_vehicle.position.x = p1;
                tmp_vehicle.position.y = m;

                random_pos_v.push_back(tmp_vehicle);
                id++;

                p1--;

            }

            p1 = pos_intersection - 1;
            n--;
        }
    }



    //    random_shuffle(random_pos_h.begin(), random_pos_h.end(), myrandom);
    //    random_pos_h.resize(size_vehicles_h);

    //    random_shuffle(random_pos_v.begin(), random_pos_v.end(), myrandom);
    //    random_pos_v.resize(size_vehicles_v);


    //Version C++11 para generar los aleatorios
    // obtain a time-based seed:
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    shuffle (random_pos_h.begin(), random_pos_h.end(), std::default_random_engine(seed));
    random_pos_h.resize(size_vehicles_h);

    shuffle (random_pos_v.begin(), random_pos_v.end(), std::default_random_engine(seed));
    random_pos_v.resize(size_vehicles_v);

    random_pos_h.insert(random_pos_h.end(), random_pos_v.begin(), random_pos_v.end());
    random_pos.insert(random_pos.end(), random_pos_h.begin(), random_pos_h.end());

    random_shuffle(random_pos.begin(), random_pos.end(), myrandom);//Coloca en ordena aleatorio las posiciones iniciales

    //qDebug() << "Numero h y s:" << size_vehicles_h << size_vehicles_v;
    //qDebug() << "Numero h y s:" << random_pos_h.size() << random_pos_v.size();


    //qDebug() << "Random size vehicles: " << random_pos.size();

    if (random_pos.size() < size_vehicles) {
        qDebug() << "ERROR: invalid number of random vehicles";
        return -1;
    }


    i = 0;
    while (i < size_vehicles) {

        vehicles_A[i] = random_pos[i];
        vehicles_Z[i] = random_pos[i];

        SetValueCellStreet(vehicles_A[i].type_street, vehicles_A[i].position.y, vehicles_A[i].position.x, 1, true, i);

        i++;
    }

    //qDebug() << "Size:" << size_vehicles;
    //qDebug() << "Random size:" << random_pos.size();

    random_pos.clear();

    SwitchMatricesRW();
    SwitchVehiclesRW();

    return 1;
}



int mainFunction(int n_h, int n_v, int n_b, float p_t, int met, float _P, int max_n, int max_m, int min_t, int max_t, int met_s, float pre, int dis_d, int dis_r, int dis_e)
{

    float density;

    fp_f = fopen("measuresflu.csv", "w");   // Abrir archivo para escritura
    fp_v = fopen("measuresvel.csv", "w");   // Abrir archivo para escritura
    fp_i = fopen("measuresint.csv", "w");   // Abrir archivo para escritura

    //density = 0.9;
    int tmp_ticks = 5400;
    //int tmp_ticks = 0;

    qDebug() << "Ejecuntando, espere por favor...";
    clock_t start = clock();

    SaveMeasuresOpt();


    fprintf(fp_f, "%s,%s\n", "Densidad", "Flujo");
    fprintf(fp_v, "%s,%s\n", "Densidad", "Velocidad");
    fprintf(fp_i, "%s,%s\n", "Densidad", "Intersecciones");

    fprintf(fp_v, "%f,%f\n", 0.0, 0.0);
    fprintf(fp_f, "%f,%f\n", 0.0, 0.0);//con estos datos se grafica el diagrama fundamental del trafico
    fprintf(fp_i, "%f,%f\n", 0.0, 0.0);


    int number_cell_h = n_h * (n_b * n_v);
    int number_cell_v = n_v * (n_b * n_h);
    int tot_cell = number_cell_h + number_cell_v;

    int n_veh_h = number_cell_h;
    int n_veh_v = number_cell_v;
    int n_tot_veh = tot_cell;

    //float dens = 1.0 / (float) (n_h * (n_b * n_h) + n_h); //Se asume que la ciudad tiene el mismo tamanio horizontal y vertical para que la densidad sea la misma

    float dens = 0;
    if (DIRECTION_METRIC == 1)
       dens = 1.0 / n_veh_h;//Minima densidad que al menos genera un vehiculo en cada direccion
    else if (DIRECTION_METRIC == 2)
       dens = 1.0 / n_veh_h;
    else
       dens = 1.0 / n_veh_v;

    qDebug() << dens << n_veh_h << n_veh_v;


   for (density = dens; density <= 1.0; density+=size_step){
   //for (density = 0.161; density <= 0.161; density+=size_step){
        //printf("%f\n", density);
        save_velocity = 0;
        save_flux = 0;
        save_value_intersections = 0;

        for (int i = 0; i < n_exp; i++) {

            switch_matriz = true;
            switch_vehicles = true;

            velocity = 0;
            flux = 0;
            velocity_total = 0;
            flux_total = 0;
            value_intersections_total = 0;
            collisions = 0;

            if (DIRECTION_METRIC == 1)
                InializedCity(n_h, n_v, n_b, density, density, p_t);
            else if (DIRECTION_METRIC == 2)
                InializedCity(n_h, n_v, n_b, density, 0.0, p_t);
            else
                InializedCity(n_h, n_v, n_b, 0.0, density, p_t);


            InializedTrafficLights(met, _P, max_n, max_m, min_t, max_t);
            InializedSensores(met_s, pre, dis_d, dis_r, dis_e);

            for (int tick = 0; tick < tmp_ticks; tick++)//Establizar sistema (superar trasciendes)
              RunSimulation(tick);

            velocity = 0;
            flux = 0;
            velocity_total = 0;
            flux_total = 0;
            value_intersections_total = 0;
            collisions = 0;

            for (int tick = 0; tick < n_ticks; tick++)
                RunSimulation(tick);

            //qDebug() << "No. Colisiones: " << collisions << "- No. Vehiculos" << total_vehicles;
            CalculateSaveMeasures();

             qDebug() << "Density:" << density;

             freeSensors();
             freeSensorsTraditional();
             freeTrafficLightSO();
             freeTrafficLight();
             FreeVehicles();
             FreeCity();

        }

        if (DIRECTION_METRIC == 1)
           SaveMeasures(density);
        else if (DIRECTION_METRIC == 2)
           SaveMeasures(density_h);
        else
           SaveMeasures(density_v);

        //printf("Density: %f", densitiy);

        //printf("\a\a");
    }

    qDebug() << "Fin:";

    //printf ("End...\n ");
     printf("Tiempo transcurrido: %f", ((double)clock() - start) / CLOCKS_PER_SEC);


    fclose(fp_f);
    fclose(fp_v);
    fclose(fp_i);


    return 0;
}


int mainFunctionRules(int n_h, int n_v, int n_b, float p_t, int met, float _P, int max_n, int max_m, int min_t, int max_t, int met_s, float pre, int dis_d, int dis_r, int dis_e)
{

    float density_h;
    float density_v;


    //density = 0.9;
    int tmp_ticks = 5400;
    //int tmp_ticks = 0;

    qDebug() << "Ejecuntando, espere por favor...";
    clock_t start = clock();

    SaveMeasuresOpt();


     int number_cell_h = n_h * (n_b * n_v);
    int number_cell_v = n_v * (n_b * n_h);
    int tot_cell = number_cell_h + number_cell_v;

    int n_veh_h = number_cell_h;
    int n_veh_v = number_cell_v;
    int n_tot_veh = tot_cell;

    //float dens = 1.0 / (float) (n_h * (n_b * n_h) + n_h); //Se asume que la ciudad tiene el mismo tamanio horizontal y vertical para que la densidad sea la misma

    float dens_h = 0;
    float dens_v = 0;

    dens_h = 1.0 / n_veh_h;
    dens_v = 1.0 / n_veh_v;

    qDebug() << dens_h << dens_v << n_veh_h << n_veh_v;

    combination_rules[5] = true;
    combination_rules[4] = true;
    combination_rules[3] = true;
    combination_rules[2] = true;
    combination_rules[1] = true;
    combination_rules[0] = true;


    for (int rules = 0; rules < 64; rules++){

        qDebug() << rules;

        convertBinary(rules, combination_rules, 6);

        FILE *fv = NULL;
        FILE *ff = NULL;

        char str_num[15];
        char str[100];
        char path_v[100];
        char path_f[100];


        strcpy(str, "Rules/");
        sprintf(str_num, "%d", rules);
        strcat (str, str_num);
        strcpy(path_v, str);
        strcpy(path_f, str);
        strcat (path_v,"rulev.csv ");
        strcat (path_f,"rulef.csv ");


        fv = fopen(path_v, "w");   // Abrir archivo para escritura
        ff = fopen(path_f, "w");   // Abrir archivo para escritura


        fprintf(fv, "%s,%s,%s,%s\n", "Densidad_H", "Densidad_V", "Velocidad", "Reglas");
        fprintf(ff, "%s,%s,%s,%s\n", "Densidad_H", "Densidad_V", "Flujo", "Reglas");


        int i, j;

        int steps = float ((1.0 / size_step) + 1.0);

        //qDebug() << "Steps: " << steps << size_step << size_step;

        density_h = 0.0;
        for (i = 0; i < steps; i++){
            density_v = 0.0;
            for (j = 0; j < steps; j++){
                //printf("%f\n", density);
                save_velocity = 0;
                save_flux = 0;

                for (int i = 0; i < n_exp; i++) {

                    switch_matriz = true;
                    switch_vehicles = true;

                    velocity = 0;
                    flux = 0;
                    velocity_total = 0;
                    flux_total = 0;
                    collisions = 0;


                    int r = InializedCity(n_h, n_v, n_b, density_h, density_v, p_t);
                    //int r = InializedCity(n_h, n_v, n_b, density_v, density_v, p_t);
                    InializedTrafficLights(met, _P, max_n, max_m, min_t, max_t);
                    InializedSensores(met_s, pre, dis_d, dis_r, dis_e);


                    if (r > 0) {


                        for (int tick = 0; tick < tmp_ticks; tick++)//Establizar sistema (superar trasciendes)
                            RunSimulation(tick);

                        velocity = 0;
                        flux = 0;
                        velocity_total = 0;
                        flux_total = 0;
                        collisions = 0;

                        for (int tick = 0; tick < n_ticks; tick++)
                            RunSimulation(tick);

                    }

                    //qDebug() << "No. Colisiones: " << collisions << "- No. Vehiculos" << total_vehicles;
                    CalculateSaveMeasures();


                    //qDebug() << "Density_H:" << density_h << "Density_V:" << density_v;

                    freeSensors();
                    freeSensorsTraditional();
                    freeTrafficLightSO();
                    freeTrafficLight();
                    FreeCity();

                    if (r > 0)
                     FreeVehicles();


                }


                SaveMeasuresRules(density_h, density_v, rules, fv, ff);
                //SaveMeasuresRules(density_v, density_v, rules, fv, ff);
                //printf("Density: %f", densitiy);
                //printf("\a\a");
                density_v+=size_step;

            }

             density_h+=size_step;
        }




        fclose(fv);   // Abrir archivo para escritura
        fclose(ff);
    }



    qDebug() << "Fin:";

    //printf ("End...\n ");
     printf("Tiempo transcurrido: %f", ((double)clock() - start) / CLOCKS_PER_SEC);


     calculateAverage();


    return 0;
}


void calculateAverage()
{

    int size = (1 / size_step + 1) * (1 / size_step + 1);
    //int size = (1 / size_step + 1);

    qDebug() << size;

    float dataV[size][4];
    float dataF[size][4];
    float average[64];

    FILE *frul = NULL;

    frul = fopen("Rules/rules.csv", "w");   // Abrir archivo para escritura

    fprintf(frul, "%s,%s\n", "Regla", "Promedio");

    for (int rules = 0; rules < 64; rules++){

        average[rules] = 0;
      //  qDebug() << rules;

        FILE *fv = NULL;
        FILE *ff = NULL;

        char str_num[15];
        char str[100];
        char path_v[100];
        char path_f[100];


        strcpy(str, "Rules/");
        sprintf(str_num, "%d", rules);
        strcat (str, str_num);
        strcpy(path_v, str);
        strcpy(path_f, str);
        strcat (path_v,"rulev.csv ");
        strcat (path_f,"rulef.csv ");


        fv = fopen(path_v, "r");   // Abrir archivo para escritura
        ff = fopen(path_f, "r");   // Abrir archivo para escritura



        /*
        if (fv != NULL)
            qDebug() << "Archivo velocidad: " << path_v;
        else
            qDebug() << "Error";


        if (ff != NULL)
            qDebug() << "Archivo flujo: " << path_v;
        else
            qDebug() << "Error";*/


        fscanf(fv, "%s,%s,%s,%s\n", str, str, str, str);
        fscanf(ff, "%s,%s,%s,%s\n", str, str, str, str);



        float sum = 0;
        for (int i = 0; i < size; i++){
            //fscanf (fv, "%f,%f,%f,%f", dataV[i][0], dataV[i][1], dataV[i][2], dataV[i][3]);
            fscanf (ff, "%f,%f,%f,%f", &dataF[i][0], &dataF[i][1], &dataF[i][2], &dataF[i][3]);
            sum+= dataF[i][2];
        }

        average[rules] = sum / size;


        fprintf(frul, "%d,%f\n", rules, average[rules]);

      /* for (int i = 0; i < size; i++){
            qDebug() << dataF[i][0] << dataF[i][1] << dataF[i][2] << dataF[i][3];
        }*/


        fclose(fv);   // Abrir archivo para escritura
        fclose(ff);

    }


    fclose(frul);

      qDebug() << "Fin promedios";

}



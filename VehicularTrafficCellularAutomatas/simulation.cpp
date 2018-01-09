#include "simulation.h"

void RunSimulation(int tick)
{
    int x, y;
    char direction;
    char type_street;
    int xp;
    int vp;
    int n, m;
    int d_street;
    int dn;
    int v;
    int vn_new;
    int k_intersection = 0;
    bool prev_visible;
    bool visible;

    resetCityWrite();
    resetDetectVehicle();

    SVehicle vehicle, front_vehicle;

    for (int i = 0; i < size_vehicles; i++) {
        vehicle = GetVehicle(i);
        x = vehicle.position.x;
        y = vehicle.position.y;
        v = vehicle.speed;
        direction = vehicle.direction;
        type_street = vehicle.type_street;
        prev_visible = vehicle.visible;

        k_intersection = GetIndexIntersection(type_street, x, direction);

        if (type_street == 'H') {
            n = y;
            m = k_intersection;
            d_street = d_hor_street;
        }
        else {
            n = k_intersection;
            m = y;
            d_street = d_ver_street;
        }

        int id_f = searchVehicleFrontID(i);

        if (id_f != INVALID){
            front_vehicle = GetVehicle(id_f);

            if (type_street == front_vehicle.type_street){
                xp = front_vehicle.rear_position;
                vp = front_vehicle.speed;
                //qDebug() << id_f << direction << x << "-" << xp;
            }
            else {
                int pos_intersection = GetPositionIntersection(type_street, n, m);
                xp = pos_intersection;
                vp = 0;
            }
        }
        else {
            vp = vmax;
            if (direction == 'R')
                xp = (x == 0) ? d_street - 1 : x - 1;
            else
                xp = (x == d_street - 1) ? 0 : x + 1;
            //qDebug() << "WARNING: No hay frontal" << __PRETTY_FUNCTION__;
        }

        //Distancia respecto al vehiculo
        dn = calculateDistance(x, xp, direction, d_street);
        vn_new = CalculateVelocity(dn, vp, n, m, d_street, i);

        if (direction == 'R') {//direccion derecha
            if ((x + vn_new) >= d_street)
                x = (x + vn_new) % d_street;
            else
                x = x + vn_new;
        }
        else {//direccion izquierda
            if ((x - vn_new) < 0)
                x = d_street + ((x - vn_new) % d_street);
            else
                x = x - vn_new;
        }

        SetVehicle(&vehicle, i);
        SetPositionVehicle(y, x, i);
        SetVelocityVehicle(vn_new, i);

        visible = determineVisible(type_street, direction, x, prev_visible);//Visibilidad
        /*
         * SetDirectionVehicle(direction, i); //Cambiar en turn?
        if (frand() < p_turn) {
            turn(type_street, y, x, vn_new, direction, visible, i);
        }
        */
        SetVehicleCell(y, x, visible, i);
    }

    for (n = 0; n < n_hor_streets; n++)
        for (m = 0; m < m_ver_streets; m++)
            RunIntersectionControl(n, m);

    //printf("Colision: %d \n", collisions);
    // qDebug() << "Colision: " << collisions;

    if (tick < n_ticks) {
        Velocity();


        if (DIRECTION_METRIC == 1)
            Flux(density);
        else if (DIRECTION_METRIC == 2)
            Flux(density_h);
        else
            Flux(density_v);
    }

    SwitchMatricesRW();
    SwitchVehiclesRW();

    //qDebug() << "Run simulation";

}

void RunIntersectionControl(int n, int m)
{

    if (intersection_control == 1) { //GREEN WAVE
        TrafficLightGreenWave(n, m);
    }
    else if (intersection_control == 2){//SELF-ORGANIZING

        if (metodo_sensado == 1)
            traditionalSensing(n, m);//REACTIVE
        else
            deliberativeSensing(n, m);//DELIBERATIVE

       // qDebug() << "Here";

        TrafficLightSelfOrganizing(n, m);
    }
    else if (intersection_control == 3){//PRESSURE SELF-ORGANIZING

        if (metodo_sensado == 1)
            traditionalSensing(n, m);//REACTIVE
        else
            deliberativeSensing(n, m);//DELIBERATIVE

        TrafficLightPressureSelfOrganizing(n, m);
    }
    else if (intersection_control == 4){// SLOT FAIR  SLOT BATCH
        slotBasedSystem(n, m);
    }

}

int RunExperiments(int _model, int _ls, int _vmax, int n_h, int n_v, int n_b, float p_t, int met, float _P, int max_n, int max_m, int min_t, int max_t, int met_s, float pre, int dis_d, int dis_r, int dis_e, int dis_z, float per_auto, int _N)
{
    float density;

    fp_f = fopen("measuresflu.csv", "w");   // Abrir archivo para escritura
    fp_v = fopen("measuresvel.csv", "w");   // Abrir archivo para escritura

    fp_fopt = fopen("fluopt.csv", "w");   // Abrir archivo para escritura
    fp_vopt = fopen("velopt.csv", "w");   // Abrir archivo para escritura

    int tmp_ticks = 3600;//para estabilizar el sistema

    fprintf(fp_v, "%f,%f\n", 0.0, 0.0);//con estos datos se grafica el diagrama fundamental del trafico
    fprintf(fp_f, "%f,%f\n", 0.0, 0.0);//con estos datos se grafica el diagrama fundamental del trafico

    fprintf(fp_vopt, "%f,%f\n", 0.0, 0.0);
    fprintf(fp_fopt, "%f,%f\n", 0.0, 0.0);

    qDebug() << "Ejecuntando, espere por favor...";
    clock_t start = clock();

    InitializeVehicularModel(_model, _ls, _vmax, p_t, per_auto);

    int number_cell_h = n_h * (n_b * n_v);
    int number_cell_v = n_v * (n_b * n_h);
    int tot_cell = number_cell_h + number_cell_v;

    int n_veh_h = number_cell_h / ls;
    int n_veh_v = number_cell_v / ls;
    int n_tot_veh = tot_cell / ls;

    float dens = 0;

    dens = 1.0 / n_veh_h;//Minima densidad que al menos genera un vehiculo en cada direccion
    //qDebug() << dens << n_veh_h << n_veh_v;
    dens = 0.02;

    for (density = dens; density <= 1.0; density+=size_step){
        //printf("%f\n", density);
        save_velocity = 0;
        save_flux = 0;

        for (int i = 0; i < n_exp; i++) {
            switch_matriz = true;
            switch_vehicles = true;

            _velocity = 0;
            flux = 0;
            velocity_total = 0;
            flux_total = 0;
            collisions = 0;

            InializedCity(met, n_h, n_v, n_b, density, density);
            InializedTrafficLights(_P, max_n, max_m, min_t, max_t);
            InializedSlotSystem(_N);
            InializedSensors(met_s, pre, dis_d, dis_r, dis_e, dis_z);

            for (int tick = 0; tick < tmp_ticks; tick++)//Establizar sistema (superar trasciendes)
                RunSimulation(tick);

            _velocity = 0;
            flux = 0;
            velocity_total = 0;
            flux_total = 0;
            collisions = 0;

            for (int tick = 0; tick < n_ticks; tick++)
                RunSimulation(tick);

            //qDebug() << "No. Colisiones: " << collisions << "- No. Vehiculos" << total_vehicles;
            CalculateSaveMeasures();
            qDebug() << "Density:" << density;

            FreeSensors();
            freeSlotSystem();
            FreeTrafficLights();
            FreeCity();
        }

        SaveMeasures(density);
        //printf("Density: %f", densitiy);
        //printf("\a\a");
    }

    freeLAI();
    qDebug() << "Fin:";

    //printf ("End...\n ");
    //  printf("Tiempo transcurrido: %f", ((double)clock() - start) / CLOCKS_PER_SEC);

    fclose(fp_f);
    fclose(fp_v);
    fclose(fp_fopt);
    fclose(fp_vopt);

    return 0;
}

/*
int RunExperiments(int n_h, int n_v, int n_b, float p_t, int met, float _P, int max_n, int max_m, int min_t, int max_t, int met_s, float pre, int dis_d, int dis_r, int dis_e, int dis_z)
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

    int n_veh_h = number_cell_h;
    int n_veh_v = number_cell_v;

    //int tot_cell = number_cell_h + number_cell_v;
    //int n_tot_veh = tot_cell;

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

            _velocity = 0;
            flux = 0;
            velocity_total = 0;
            flux_total = 0;
            value_intersections_total = 0;
            collisions = 0;

       //     InitializeVehicularModel(_model, _ls, _vmax, p_t, _per_autonomous);

            if (DIRECTION_METRIC == 1)
                InializedCity(met, n_h, n_v, n_b, density, density);
            else if (DIRECTION_METRIC == 2)
                InializedCity(met, n_h, n_v, n_b, density, 0.0);
            else
                InializedCity(met, n_h, n_v, n_b, 0.0, density);


            InializedTrafficLights(_P, max_n, max_m, min_t, max_t);
            InializedSensors(met_s, pre, dis_d, dis_r, dis_e, dis_z);

            for (int tick = 0; tick < tmp_ticks; tick++)//Establizar sistema (superar trasciendes)
                RunSimulation(tick);

            _velocity = 0;
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

            freeDeliberativeSensors();
            freeTraditionalSensors();
            freeTrafficLightSO();
            freeTrafficLight();
            freeRegionIntersection();
            FreeVehicles();
            freeLAI();
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
*/

int mainFunctionRules(int n_h, int n_v, int n_b, float p_t, int met, float _P, int max_n, int max_m, int min_t, int max_t, int met_s, float pre, int dis_d, int dis_r, int dis_e, int dis_z)
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

                    _velocity = 0;
                    flux = 0;
                    velocity_total = 0;
                    flux_total = 0;
                    collisions = 0;

                    //InitializeVehicularModel(_model, _ls, _vmax, p_t, _per_autonomous);
                    int r = InializedCity(met, n_h, n_v, n_b, density_h, density_v);
                    //int r = InializedCity(n_h, n_v, n_b, density_v, density_v, p_t);
                    InializedTrafficLights(_P, max_n, max_m, min_t, max_t);
                    InializedSensors(met_s, pre, dis_d, dis_r, dis_e, dis_z);

                    if (r > 0) {

                        for (int tick = 0; tick < tmp_ticks; tick++)//Establizar sistema (superar trasciendes)
                            RunSimulation(tick);

                        _velocity = 0;
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

                    freeDeliberativeSensors();
                    freeTraditionalSensors();
                    freeSlotSystem();
                    freeTrafficLightSO();
                    freeTrafficLight();
                    freeRegionIntersection();
                    freeLAI();
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


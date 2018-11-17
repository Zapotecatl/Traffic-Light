#include "measures.h"

float _velocity;
float velocity_total;
float save_velocity;
FILE *fp_v;

float flux;
float flux_total;
float save_flux;
FILE *fp_f;

float waiting_time;
float waiting_time_total;
float save_waiting_time;
FILE *fp_wt;

float *SD_velocity_total;
float save_SD_velocity;
FILE *fp_sdv;

float *SD_flux_total;
float save_SD_flux;
FILE *fp_sdf;

float *SD_wait_total;
float save_SD_wait;
FILE *fp_sdw;

int value_intersections;
float value_intersections_total;
float save_value_intersections;
FILE *fp_i;

int collisions;

FILE *fp_fopt;
FILE *fp_vopt;

float v_optim;
float j_optim;
float j_max = 1.0 / 4.0;

//Measures////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Velocity()
{
    int i;
    int v_total;

    v_total = 0;
    for (i = 0; i < size_vehicles; i++) {
        v_total+= GetVelocityVehicle(i);

       // qDebug() << "Velocity: " << GetVelocityVehicle(i) << size_vehicles;
    }

    //qDebug() << "Measure Size vehicles: " << size_vehicles;
    if (size_vehicles > 0){
        _velocity = ((float) v_total / size_vehicles);
        //  qDebug() << "Velocity: " << _velocity;
    }
    else {
        _velocity = 0;
    }

    //qDebug() <<  _velocity << size_vehicles;
    velocity_total+= _velocity;

}

void Flux(float density)
{
   // qDebug() << "Flujo" << ((float) density * velocity);
    flux = ((float) density * _velocity);
    //qDebug() <<  flux << density;

    flux_total+= flux;
}

void WaitingTime()
{
    int i;
    int wait_total;

    wait_total = 0;
    for (i = 0; i < size_vehicles; i++) {
        if (GetVelocityVehicle(i) == 0)
           wait_total++;
    }

    //qDebug() << "Measure Size vehicles: " << size_vehicles;
    if (size_vehicles > 0){
      waiting_time = ((float) wait_total / size_vehicles);
    //  qDebug() << "Velocity: " << velocity;
    }
    else {
      waiting_time = 0;
    }

    //qDebug() <<  _velocity << size_vehicles;
    waiting_time_total+= waiting_time;
}


void averageValueIntersections()//Probando por que hay un mayor desempenio en el diagrama fundamental respecto al optimo
{
    value_intersections = 0;
    for (int n = 0; n < n_hor_streets; n++)
        for (int m = 0; m < m_ver_streets; m++)
            value_intersections+= pr_intersections[n][m].value;
    value_intersections_total+= value_intersections;
}

void CalculateSaveMeasures(int ind)
{
    save_velocity+= (velocity_total / n_ticks);
    save_flux+= (flux_total / n_ticks);
    save_waiting_time+= waiting_time_total;//(waiting_time_total / n_ticks);
    save_value_intersections+= (value_intersections_total / n_ticks);

    SD_velocity_total[ind] = (velocity_total / n_ticks);
    SD_flux_total[ind] = (flux_total / n_ticks);
    SD_wait_total[ind] = waiting_time_total;

}

void SaveMeasures(float density)
{
    save_velocity = save_velocity / n_exp;
    save_flux = save_flux / n_exp;
    save_waiting_time = save_waiting_time / n_exp;
    save_SD_velocity = save_SD_velocity / n_exp;
    save_SD_flux = save_SD_flux / n_exp;
    save_value_intersections = save_value_intersections / n_exp;

    float sum_SD_velocity = 0;
    float difference_v;
    for (int i = 0; i < n_exp; i++) {
        difference_v = SD_velocity_total[i] - save_velocity;
        sum_SD_velocity+= (difference_v * difference_v);
    }
    sum_SD_velocity = sum_SD_velocity / (float) n_exp;
    save_SD_velocity = sqrt(sum_SD_velocity);

    float sum_SD_flux = 0;
    float difference_f;
    for (int i = 0; i < n_exp; i++) {
        difference_f = SD_flux_total[i] - save_flux;
        sum_SD_flux+= (difference_f * difference_f);
    }
    sum_SD_flux = sum_SD_flux / (float) n_exp;
    save_SD_flux = sqrt(sum_SD_flux);


    float sum_SD_wait = 0;
    float difference_w;
    for (int i = 0; i < n_exp; i++) {
        difference_w = SD_wait_total[i] - save_waiting_time;
        sum_SD_wait+= (difference_w * difference_w);
    }
    sum_SD_wait = sum_SD_wait / (float) n_exp;
    save_SD_wait = sqrt(sum_SD_wait);


    //qDebug() << n_exp << density;
    v_optim = vOptima(density);
    j_optim = jOptima(density);

    fprintf(fp_v, "%f,%f\n", density, save_velocity);
    fprintf(fp_f, "%f,%f\n", density, save_flux);//con estos datos se grafica el diagrama fundamental del trafico
    fprintf(fp_wt, "%f,%f\n", density, save_waiting_time);
    fprintf(fp_sdv, "%f,%f\n", density, save_SD_velocity);
    fprintf(fp_sdf, "%f,%f\n", density, save_SD_flux);
    fprintf(fp_sdw, "%f,%f\n", density, save_SD_wait);

}

void SaveMeasures3D(float density_h, float density_v)
{
    save_velocity = save_velocity / n_exp;
    save_flux = save_flux / n_exp;
    save_waiting_time = save_waiting_time / n_exp;
    save_SD_velocity = save_SD_velocity / n_exp;
    save_SD_flux = save_SD_flux / n_exp;
    save_value_intersections = save_value_intersections / n_exp;

    float sum_SD_velocity = 0;
    float difference_v;
    for (int i = 0; i < n_exp; i++) {
        difference_v = SD_velocity_total[i] - save_velocity;
        sum_SD_velocity+= (difference_v * difference_v);
    }
    sum_SD_velocity = sum_SD_velocity / (float) n_exp;
    save_SD_velocity = sqrt(sum_SD_velocity);

    float sum_SD_flux = 0;
    float difference_f;
    for (int i = 0; i < n_exp; i++) {
        difference_f = SD_flux_total[i] - save_flux;
        sum_SD_flux+= (difference_f * difference_f);
    }
    sum_SD_flux = sum_SD_flux / (float) n_exp;
    save_SD_flux = sqrt(sum_SD_flux);


    float sum_SD_wait = 0;
    float difference_w;
    for (int i = 0; i < n_exp; i++) {
        difference_w = SD_wait_total[i] - save_waiting_time;
        sum_SD_wait+= (difference_w * difference_w);
    }
    sum_SD_wait = sum_SD_wait / (float) n_exp;
    save_SD_wait = sqrt(sum_SD_wait);

    float total_density = (density_h + density_v) / 2.0;

    fprintf(fp_v, "%f,%f,%f,%f\n", density_h, density_v, save_velocity, total_density);
    fprintf(fp_f, "%f,%f,%f,%f\n", density_h, density_v, save_flux, total_density);//con estos datos se grafica el diagrama fundamental del trafico
    fprintf(fp_wt, "%f,%f,%f,%f\n", density_h, density_v, save_waiting_time, total_density);
    fprintf(fp_sdv, "%f,%f,%,%ff\n", density_h, density_v, save_SD_velocity, total_density);
    fprintf(fp_sdf, "%f,%f,%f,%f\n", density_h, density_v, save_SD_flux, total_density);
    fprintf(fp_sdw, "%f,%f,%f,%f\n", density_h, density_v, save_SD_wait, total_density);

}


void SaveMeasuresRules(float density_h, float density_v, int rules, FILE *fv, FILE *ff)
{
    save_velocity = save_velocity / n_exp;
    save_flux = save_flux / n_exp;

    //qDebug() << n_exp;
    fprintf(fv, "%f,%f,%f,%d\n", density_h, density_v, save_velocity, rules);//con estos datos se grafica el diagrama fundamental del trafico
    fprintf(ff, "%f,%f,%f,%d\n", density_h, density_v, save_flux, rules);//con estos datos se grafica el diagrama fundamental del trafico


}

float vOptima(float density)
{

    //Es mejor graficar desde R graphics porque se pierden puntos criticos
    if (density <= j_max)
        return 1.0;
    else if (density >= (1.0 - j_max))
        return (1.0 - density) / density;
    else if (density > j_max && density < (1.0 - j_max))
        return j_max / density;

    return 0.0;
}

float jOptima(float density)
{
    //Es mejor graficar desde R graphics porque se pierden puntos criticos
    if (density <= j_max)
        return density;
    else if (density >= (1.0 - j_max))
        return (1.0 - density);
    else if (density > j_max && density < (1.0 - j_max))
        return j_max;

    return 0.0;
}

void SaveMeasuresOpt()
{
    float v_optim, j_optim;

    fp_fopt = fopen("fluopt.csv", "w");   // Abrir archivo para escritura
    fp_vopt = fopen("velopt.csv", "w");   // Abrir archivo para escritura

    fprintf(fp_fopt, "%s,%s\n", "Densidad", "Flujo");
    fprintf(fp_vopt, "%s,%s\n", "Densidad", "Velocidad");

    for (float density = 0.0; density <= 1.0; density+=0.01){

        v_optim = vOptima(density);
        j_optim = jOptima(density);

        fprintf(fp_vopt, "%f,%f\n", density, v_optim);
        fprintf(fp_fopt, "%f,%f\n", density, j_optim);
    }


    fclose(fp_fopt);
    fclose(fp_vopt);

}


/*
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

            if ((pr_intersections[n][m].value == 0 && pw_intersections[n][m].value == 1))
                cell_changed++;

            for (i = 1; i <= d_side_block; i++){
                reposition_cell_h = m * d_side_block + m + i;
                if (pr_horizontal_streets[n][reposition_cell_h].value == 0 && pw_horizontal_streets[n][reposition_cell_h].value == 1)
                    cell_changed++;
            }

            for (j = 1; j <= d_side_block; j++){
                reposition_cell_v = n * d_side_block + n + j;
                if (pr_vertical_streets[m][reposition_cell_v].value == 0 && pw_vertical_streets[m][reposition_cell_v].value == 1)
                    cell_changed++;
            }
        }
    }

    if (size_vehicles > 0)
      _velocity = ((float) cell_changed / size_vehicles);
    else
      _velocity = 0;


    qDebug() << "------------------------------";

    qDebug() << "HT:" << _velocity << size_vehicles << cell_changed;
    velocity_total+= _velocity;



    //int i;
    int v_total;

    v_total = 0;
    for (i = 0; i < size_vehicles; i++) {
        v_total+= pw_vehicles[i].speed;// GetVelocityVehicle(i);
    }

    //qDebug() << "Measure Size vehicles: " << size_vehicles;
    if (size_vehicles > 0){
      _velocity = ((float) v_total / size_vehicles);
    //  qDebug() << "Velocity: " << velocity;
    }
    else
      _velocity = 0;

    qDebug() << "HV:" <<  _velocity << size_vehicles << v_total;
    qDebug() << "------------------------------";


}
*/





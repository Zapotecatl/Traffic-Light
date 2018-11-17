#include "vehicular_model.h"

int vmax; //velocidad maxima
int ls; //tamanio del vehiculo
float p_turn;
float per_autonomous;

int M;// 2 Capacidad de frenado (5.00m / Dx = 2.5m)
float *Ra; //Probabilidad de acelerar
float Rs;//probabilidad de sobre frenar
float R0;
float Rd;//1.0

float delta_x;
float delta_v;

float vs; //Velocidad baja

int model;

void InitializeVehicularModel(int _model, int _ls, int _vmax, float p_t, float _per_autonomous)
{

    if (_model <= 0) {
        qDebug() << "ERROR: _model" << __PRETTY_FUNCTION__;
        model = 1;
    }
    else
        model = _model;

    if (_ls <= 0) {
        qDebug() << "ERROR: ls" << __PRETTY_FUNCTION__;
        ls = 1;
    }
    else
        ls = _ls;

    if (_vmax < 0) {
        qDebug() << "ERROR: _vmax" << __PRETTY_FUNCTION__;
        vmax = 1;
    }
    else {
        if (model == 1)
            vmax = 1;
        else
            vmax = _vmax;
    }

    if (p_t >= 0.0 && p_t <= 1.0)//probabilidad de girar en las intersecciones
        p_turn = p_t;
    else {
        qDebug() << "ERROR: p_turn" << __PRETTY_FUNCTION__;
        p_turn = 0.0;
    }

    if (_per_autonomous >= 0.0 && _per_autonomous <= 1.0)//Porcentage de vehiculos autonomos
        per_autonomous = _per_autonomous;
    else {
        qDebug() << "ERROR: p_turn" << __PRETTY_FUNCTION__;
        per_autonomous = 0.0;
    }

    Rs = 0.01;//probabilidad de sobre frenar
    R0 = 0.8;
    Rd = 1.0;//1.0
    delta_x = 2.5;
    delta_v = 2.5 / delta_x;
    vs = 3;//(5.0 / delta_x) + 1; //Velocidad baja
    M = 2;

    CalculateRa();
    CalculateTables();
}

//RULE 184/////////////////////////////////////////////////////////////////////

int Rule_184(int distance)
{
    if (distance == 0)
        return 0;
    else
        return 1;

    return INVALID;
}

int Rule_252()
{
    return 0;
}

//LAI/////////////////////////////////////////////////////////////////////



unsigned int **acc;
unsigned int **keep;
unsigned int **dec0;
unsigned int *dbreak;

void CalculateRa()
{
    int i;

    Ra = NULL;

    Ra = new float[vmax + 1];

    for (i = 0; i < vmax + 1; i++)
        Ra[i] = minimum(Rd, R0 + (float)i * ((Rd - R0) / vs));

    /*    printf ("Ra: ");
    for (i = 0; i < vmax + 1; i++)
        printf ("%f ", Ra[i]);*/

}

int CalculateTables()
{

    int i;
    int dnacc, dnkeep, dndec, dnbreak;
    int dp = 0;

    dnacc = dnkeep = dndec = dnbreak = 0;

    acc = NULL;
    acc = new unsigned int*[vmax + 1];
    for (i = 0; i < vmax + delta_v; i++)
        acc[i] = new unsigned int[vmax + 1];

    keep = NULL;
    keep = new unsigned int*[vmax + 1];
    for (i = 0; i < vmax + delta_v; i++)
        keep[i] = new unsigned int[vmax + 1];

    dec0 = NULL;
    dec0 = new unsigned int*[vmax + 1];
    for (i = 0; i < vmax + 1; i++)
        dec0[i] = new unsigned int[vmax + 1];

    dbreak = NULL;
    dbreak = new unsigned int[vmax + 1];

    int vn = 0, vp = 0;

    for (vn = 0; vn <= vmax; vn++) {

        for (i = 0; i <= (vn + delta_v) / M; i++)
            dnacc += (vn + delta_v) - (i * M);

        for (i = 0; i <= (vn) / M; i++)
            dnkeep += (vn - i * M);

        for (i = 0; i <= ((vn - delta_v) / M); i++)
            dndec += (vn - delta_v) - (i * M);

        for (i = 0; i <= ((vn - M) / M); i++)
            dnbreak += (vn - M) - (i * M);

        dndec = (dndec < 0) ? 0 : dndec;
        dnkeep = (dnkeep < 0) ? 0 : dnkeep;
        dnacc = (dnacc < 0) ? 0 : dnacc;
        dnbreak = (dnbreak < 0) ? 0 : dnbreak;

        for (vp = 0; vp <= vmax; vp++) {

            for (i = 0; i <= (vp - M) / M; i++)
                dp += (vp - M) - (i * M);

            dp = (dp < 0) ? 0 : dp;

            acc[vn][vp] = ((dnacc - dp) < 0) ? 0 : dnacc - dp;
            keep[vn][vp] = ((dnkeep - dp) < 0) ? 0 : dnkeep - dp;
            dec0[vn][vp] = ((dndec - dp) < 0) ? 0 : dndec - dp;

            dp = 0;
        }


        dbreak[vn] = (dnbreak < 0) ? 0 : dnbreak;

        dnacc = dnkeep = dndec = dnbreak = 0;
    }


    /*
    for (vn = 0; vn <= vmax; vn++) {
        for (vp = 0; vp <= vmax; vp++) {
            qDebug () << "Acc: " << keep[vn][vp];
        }
    }*/


    return 0;

}

void freeLAI()
{
    int i;

    if (acc != NULL) {
        for (i = 0; i < vmax + delta_v; i++)
            delete [] acc[i];
        delete [] acc;
        acc = NULL;
    }

    if (keep != NULL) {
        for (i = 0; i < vmax + delta_v; i++)
            delete [] keep[i];
        delete [] keep;
        keep = NULL;
    }

    if (dec0 != NULL) {
        for (i = 0; i < vmax + 1; i++)
            delete [] dec0[i];
        delete [] dec0;
        dec0 = NULL;
    }

    if (dbreak != NULL) {
        delete [] dbreak;
        dbreak = NULL;
    }

    if (Ra != NULL) {
        delete [] Ra;
        Ra = NULL;
    }

}

int CalculateVelocityLAI(unsigned int dn, int vn, int vp)
{
    int vn_new = 0;

    //Acceleration
    if (dn >= acc[vn][vp]){

        if (frand() <= Ra[vn]){ //Acelera
            int tmp_vn = vn + delta_v;
            vn_new = minimum(tmp_vn, vmax);
        }
        else
            vn_new = vn;

    }
    else if (dn < acc[vn][vp] && dn >= keep[vn][vp]){//Random slowing //|| vn == vmax
        if (frand() <= Rs){
            int tmp_vn = vn - delta_v;
            vn_new = maximum(tmp_vn, 0);
        }
        else
            vn_new = vn;

    }
    else if (dn < keep[vn][vp] && dn >= dec0[vn][vp] && vn > 0){//breaking
        int tmp_vn = vn - delta_v;
        vn_new = maximum(tmp_vn, 0);
    }
    else if (dn < dec0[vn][vp] && vn > 0){//emergency breaking
        int tmp_vn = vn - M;
        vn_new = maximum(tmp_vn, 0);
    }

    return vn_new;
}

int CalculateVelocityLightLAI(unsigned int dn, int vn)
{
    int vn_new = 0;

    //Acceleration
    if (dn >= acc[vn][0]){
        if (frand() <= Ra[vn]){ //Acelera
            int tmp_vn = vn + delta_v;
            vn_new = minimum(tmp_vn, vmax);
        }
        else
            vn_new = vn;

    }
    else if ((dn < acc[vn][0] && dn >= keep[vn][0])){//Random slowing //|| vn == vmax
        if (frand() <= Rs){
            int tmp_vn = vn - delta_v;
            vn_new = maximum(tmp_vn, 0);
        }
        else
            vn_new = vn;

    }
    else if (dn < keep[vn][0] && dn >= dec0[vn][0] && vn > 0){//breaking
        int tmp_vn = vn - delta_v;
        vn_new = maximum(tmp_vn, 0);
    }
    else if (dn < dec0[vn][0] && dn >= dbreak[vn] && vn > 0){//emergency breaking

        int tmp_vn = vn - M;
        vn_new = maximum(tmp_vn, 0);

    }
    else if (dn < dbreak[vn]){ //Predice que no puede detenerse antes del semaforo
        vn_new = vn; // continua con la misma velocidad para pasarse el amarillo o rojo

        //qDebug() << "Aqui se lo pasa" << vn << endl;
    }


    return vn_new;
}

/////////////////////////////////////////////
//Autonomous vehicle

int CalculateVelocityAutonomousLAI(unsigned int dn, int vn, int vp)
{
    int vn_new = 0;

    //Acceleration
    if (dn >= acc[vn][vp]){
        //Siempre acelera
        int tmp_vn = vn + delta_v;
        vn_new = minimum(tmp_vn, vmax);
    }
    else if (dn < acc[vn][vp] && dn >= keep[vn][vp] ){//Random slowing
        //Mantiene velocidad y nunca sobre frena
        vn_new = vn;
    }
    else if (dn < keep[vn][vp] && dn >= dec0[vn][vp] && vn > 0){//breaking
        int tmp_vn = vn - delta_v;
        vn_new = maximum(tmp_vn, 0);
    }
    else if (dn < dec0[vn][vp] && vn > 0){//emergency breaking
        int tmp_vn = vn - M;
        vn_new = maximum(tmp_vn, 0);
    }

    return vn_new;
}


int CalculateVelocityAutonomousLightLAI(unsigned int dn, int vn)
{
    int vn_new = 0;

    //Acceleration
    if (dn >= acc[vn][0]){
        //siempre Acelera
        int tmp_vn = vn + delta_v;
        vn_new = minimum(tmp_vn, vmax);

    }
    else if (dn < acc[vn][0] && dn >= keep[vn][0] ){//Random slowing
        //nunca sobre frena
        vn_new = vn;
    }
    else if (dn < keep[vn][0] && dn >= dec0[vn][0] && vn > 0){//breaking
        int tmp_vn = vn - delta_v;
        vn_new = maximum(tmp_vn, 0);
    }
    else if (dn < dec0[vn][0] && dn >= dbreak[vn] && vn > 0){//emergency breaking

        int tmp_vn = vn - M;
        vn_new = maximum(tmp_vn, 0);

    }
    else if (dn < dbreak[vn]){ //Predice que no puede detenerse antes del semaforo
        vn_new = vn; // continua con la misma velocidad para pasarse el amarillo o rojo
    }

    return vn_new;
}




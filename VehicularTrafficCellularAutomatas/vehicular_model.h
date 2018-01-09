#ifndef VEHICULAR_MODEL_H
#define VEHICULAR_MODEL_H

#include <math.h>
#include <algorithm>
#include <QDebug>

#include "utilities.h"

extern int vmax; //velocidad maxima
extern int ls; //tamanio del vehiculo
extern int model;
extern float p_turn;
extern float per_autonomous;
extern int M;

using std::min;
using std::max;

void InitializeVehicularModel(int _model, int _ls, int _vmax, float p_t, float _per_autonomous);

//184 Model (ACE)//
int Rule_184(int distance);
int Rule_252();

//LAI Model (AC)//
void CalculateRa();
int CalculateTables();
int CalculateVelocityLAI(unsigned int dn, int vn, int vp);
int CalculateVelocityLightLAI(unsigned int dn, int vn);
int CalculateVelocityAutonomousLAI(unsigned int dn, int vn, int vp);
int CalculateVelocityAutonomousLightLAI(unsigned int dn, int vn);
void freeLAI();

#endif // VEHICULAR_MODEL_H

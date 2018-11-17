

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//MainWindow

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "utilities.h"
#include "city.h"
#include "vehicle.h"
#include "traffic_light.h"
#include "sensor.h"
#include "slot_system.h"

#include "simulation.h"

#define STEPS 0
#define TRAFFIC_LIGHTS 1
#define PRINT 0
#define OFFSET 1

int ini = 0;

int n_h_streets;
int m_v_streets;
int d_s_block;
float dens_h;
float dens_v;
int _model;
int _ls;
int _vmax;
float p_t;
float _per_autonomous;

int inter_control;

int maxim_n;
int maxim_m;
int min_time;
int max_time;

float _P;

int _N;

int metodo_s;
int distance_d;
int distance_r;
int distance_e;
int distance_z;
float precision;

int d_h_street;
int d_v_street;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // this->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    key = 0;

    resolution_w = 600;
    resolution_h = 600;

    labelCity = new QLabel(this);
    labelCity->setGeometry(10,20,resolution_w,resolution_h);
    labelCity->setStyleSheet("background-color: rgb(0, 0, 0);");

    t_timer = new QTimer();
    connect(t_timer, SIGNAL(timeout()), this, SLOT(PaintCity()));
    t_timer->start(30);//60

    QDir dir;
    QString dir_string = dir.absolutePath();

    QStringList list_path = dir_string.split("/");
    QString aux = list_path[list_path.size() - 1];//Obtener unicamnete el ultimo
    int length = aux.length();

    dir_string.remove(dir_string.length() - length, dir_string.length());
    dir_string+= "VehicularTrafficCellularAutomatas/";

    //qDebug() << dir_string;

    QString scenefile = dir_string + "config.xml";
    readScene(scenefile);

    d_h_street = d_s_block * m_v_streets + m_v_streets;
    d_v_street = d_s_block * n_h_streets + n_h_streets;

    //_T = d_h_street >= d_v_street ? (d_h_street / 2.0) / m_v_streets : (d_v_street / 2.0) / n_h_streets;
    str_info+= "Distancia H: " + QString::number(d_h_street) + "\n" +
            "Distancia V: " + QString::number(d_h_street) + "\n";
    // "Tiempo P: " + QString::number(_P) + "\n";


    ui->txt_info->setText(str_info);

    qi_city =  new QImage (d_h_street, d_v_street, QImage::Format_RGB888);

    tmp_screen = new int*[d_v_street];
    for (int i = 0; i < d_v_street; i++)
        tmp_screen[i] = new int[d_h_street];

    screen = new int*[d_v_street];
    for (int i = 0; i < d_v_street; i++)
        screen[i] = new int[d_h_street];


}

MainWindow::~MainWindow()
{

    FreeSensors();
    freeSlotSystem();
    freeDistributedControl();
    FreeTrafficLights();
    FreeCity();
    freeLAI();

    for (int i = 0; i < d_v_street; i++)
        delete screen[i];
    delete [] screen;

    for (int i = 0; i < d_v_street; i++)
        delete tmp_screen[i];
    delete [] tmp_screen;

    delete qi_city;
    delete t_timer;
    delete labelCity;

    delete ui;
}

void MainWindow::on_pBInicia_clicked()
{

    InitializeVehicularModel(_model, _ls, _vmax, p_t, _per_autonomous);
    InializedCity(inter_control, n_h_streets, m_v_streets, d_s_block, dens_h, dens_v);
    InializedTrafficLights(_P, maxim_n, maxim_m, min_time, max_time);
    InializedSlotSystem(_N);
    InializedDistributedControl(maxim_n, maxim_m, min_time, max_time);
    InializedSensors(metodo_s, precision, distance_d, distance_r, distance_e, distance_z);

    ini = 1;

    //ui->pBInicia->setEnabled(false);
    // ui->Experiment->setEnabled(false);

}

void MainWindow::readScene(QString filename)
{

    str_info = "";

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    m_xmlReader.clear();
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        processReadXML(line);
    }

}


void MainWindow::processReadXML(QByteArray data)
{

    m_xmlReader.addData(data);

    while (!m_xmlReader.atEnd()) {
        m_xmlReader.readNext();
        if (m_xmlReader.isStartElement()) {

            if (m_xmlReader.name() == "welcome") {
                ;//Unicamente leyo la etiqueta welcome
            } else if (m_xmlReader.name() == "city") {

                inter_control = m_xmlReader.attributes().value("intersection_control").toString().toInt();
                n_h_streets = m_xmlReader.attributes().value("n_h_streets").toString().toInt();
                m_v_streets = m_xmlReader.attributes().value("m_v_streets").toString().toInt();
                dens_h = m_xmlReader.attributes().value("density_h").toString().toDouble();
                dens_v = m_xmlReader.attributes().value("density_v").toString().toDouble();
                d_s_block = m_xmlReader.attributes().value("d_s_block").toString().toInt();

                QString tmp_met;

                if (inter_control == 1)
                    tmp_met = "GREEN WAVE";
                else if (inter_control == 2)
                    tmp_met = "SELF-ORGANIZING";
                else if (inter_control == 3)
                    tmp_met = "IMPULSE SELF-ORGANIZING";
                else if (inter_control == 4)
                    tmp_met = "SLOT FAIR (N = 1) or BATCH (N > 1)";
                else if (inter_control == 5)
                    tmp_met = "DISTRIBUTED SYSTEM";
                else {
                    inter_control = 1;
                    tmp_met = "GREEN WAVE";
                }

                str_info+= "Control de la interseccion: " + tmp_met + "\n\n" +
                        "Calles horizontales: " + QString::number(n_h_streets) + "\n" +
                        "Calles verticales: " + QString::number(m_v_streets) + "\n" +
                        "Densidad H: " + QString::number(dens_h) + "\n" +
                        "Densidad V: " + QString::number(dens_v) + "\n" +
                        "Densidad Tot: " + QString::number((dens_h + dens_v) / 2.0) + "\n" +
                        "Dist. cuadra: " + QString::number(d_s_block) + "\n\n";

            }
            else if (m_xmlReader.name() == "vehicles") {
                _model = m_xmlReader.attributes().value("model").toString().toDouble();
                _ls = m_xmlReader.attributes().value("ls").toString().toDouble();
                _vmax = m_xmlReader.attributes().value("vmax").toString().toDouble();

                if (_model == 1)
                    _vmax = 1;

                p_t = m_xmlReader.attributes().value("p_turn").toString().toDouble();
                _per_autonomous = m_xmlReader.attributes().value("per_autonomous").toString().toDouble();

                str_info+= "Model: " + QString::number(_model) + "\n" +
                        "Ls: " + QString::number(_ls) + "\n" +
                        "Vmax: " + QString::number(_vmax) + "\n" +
                        "P. Turn: " + QString::number(p_t) + "\n" +
                        "% autonomous: " + QString::number(_per_autonomous) + "\n\n";
            }
            else if (m_xmlReader.name() == "green_wave") {

                _P = m_xmlReader.attributes().value("P").toString().toDouble();
                str_info+= "Green wave, P: " + QString::number(_P) + "\n\n";
            }
            else if (m_xmlReader.name() == "self_organizing") {

                maxim_n = m_xmlReader.attributes().value("maxim_n").toString().toInt();
                maxim_m = m_xmlReader.attributes().value("maxim_m").toString().toInt();
                min_time = m_xmlReader.attributes().value("min_time").toString().toInt();
                max_time = m_xmlReader.attributes().value("max_time").toString().toInt();

                str_info+= "Auto organizante:\n";
                str_info+= "Máximo n:" + QString::number(maxim_n) + "\n" +
                        "Máximo m: " + QString::number(maxim_m) + "\n" +
                        "Tiempo mínimo: " + QString::number(min_time) + "\n" +
                        "Tiempo máximo: " + QString::number(max_time) + "\n\n";
            }
            else if (m_xmlReader.name() == "sensors") {

                metodo_s = m_xmlReader.attributes().value("method").toString().toInt();
                distance_d = m_xmlReader.attributes().value("distance_d").toString().toInt();
                distance_r = m_xmlReader.attributes().value("distance_r").toString().toInt();
                distance_e = m_xmlReader.attributes().value("distance_e").toString().toInt();
                //distance_e++; //Mas la interseccion
                distance_z = m_xmlReader.attributes().value("distance_z").toString().toInt();
                precision = m_xmlReader.attributes().value("precision").toString().toDouble();

                QString tmp_met = metodo_s == 2 ? "DELIBERATIVE" : "REACTIVE";

                str_info+= "Sensores: " + tmp_met + "\n" +
                        "Distancia d: " + QString::number(distance_d) + "\n" +
                        "Distancia r: " + QString::number(distance_r) + "\n" +
                        "Distancia e: " + QString::number(distance_e) + "\n" +
                        "Distancia z: " + QString::number(distance_z) + "\n" +
                        "Precision: " + QString::number(precision) + "\n\n";

            }
            else if (m_xmlReader.name() == "slot_system") {

                _N = m_xmlReader.attributes().value("N").toString().toInt();

                str_info+= "Slot system:\n";
                str_info+= "N: " + QString::number(_N) + "\n\n";
            }
            else if (m_xmlReader.name() == "experiments") {

                type_experiment = m_xmlReader.attributes().value("type_experiment").toString().toInt();
                n_exp = m_xmlReader.attributes().value("n_exp").toString().toInt();
                n_ticks = m_xmlReader.attributes().value("n_ticks").toString().toInt();
                size_step = m_xmlReader.attributes().value("size_step").toString().toDouble();

                QString tmp_exp = type_experiment == 1 ? "2D GLOBAL DENSITY" : "3D H and V DENSITY";

                str_info+= "Experiments: " + tmp_exp + "\n" +
                        "Numero de experimentos: " + QString::number(n_exp) + "\n" +
                        "Numero de pasos: " + QString::number(n_ticks) + "\n" +
                        "Tamanio del paso: " + QString::number(size_step) + "\n\n";

            }
            else if (m_xmlReader.name() == "rules") {

                combination_rules[0] = (m_xmlReader.attributes().value("r1").toString().toInt() == 1) ? true : false;
                combination_rules[1] = (m_xmlReader.attributes().value("r2").toString().toInt() == 1) ? true : false;
                combination_rules[2] = (m_xmlReader.attributes().value("r3").toString().toInt() == 1) ? true : false;
                combination_rules[3] = (m_xmlReader.attributes().value("r4").toString().toInt() == 1) ? true : false;
                combination_rules[4] = (m_xmlReader.attributes().value("r5").toString().toInt() == 1) ? true : false;
                combination_rules[5] = (m_xmlReader.attributes().value("r6").toString().toInt() == 1) ? true : false;

            }
        }

    }//end While

}

void MainWindow::on_Experiment_clicked()
{
    ini = 0;

    ui->pBInicia->setEnabled(false);
    ui->Experiment->setEnabled(false);

    if (type_experiment == 1)
        RunExperiments(_model, _ls, _vmax, n_h_streets, m_v_streets, d_s_block, p_t, inter_control, _P, maxim_n, maxim_m, min_time, max_time, metodo_s, precision, distance_d, distance_r, distance_e, distance_z, _per_autonomous, _N);
    else
        RunExperiments3D(_model, _ls, _vmax, n_h_streets, m_v_streets, d_s_block, p_t, inter_control, _P, maxim_n, maxim_m, min_time, max_time, metodo_s, precision, distance_d, distance_r, distance_e, distance_z, _per_autonomous, _N);

}

void MainWindow::PaintCity()
{

#if 1
    if (STEPS == 1)
        if (key == 80)
            ini = 1;
        else
            ini = 0;

    if (ini == 1){

        qi_city->fill(Qt::black);

        imgpaint = QPixmap::fromImage(*qi_city);
        QPainter paint(&imgpaint);

        size_s.setHeight(resolution_h);
        size_s.setWidth(resolution_w);

        PrintStreetCity(&paint, p);

        //img_scaled = imgpaint.scaled(size_s, Qt::KeepAspectRatio);
        img_scaled = imgpaint.scaled(size_s, Qt::IgnoreAspectRatio);
        labelCity->setPixmap(img_scaled);

        RunSimulation(1);

    }

    key = 0;
#endif

}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    key = event->key();
}


void MainWindow::PrintStreetCity(QPainter *paint, QPen &p)
{

    int i, j;
    int x_h;
    int x_v;
    int m, n;
    int id;

    // 0 Street
    // 1 vehicle H
    // 2 Pointer to intersection
    // 3 stop
    // 4 Green
    // 5 Red
    // 6 Terrain
    // 7 Sensor
    // 8 Virtual vehicle
    // -1 vehicle V

    //Terrain
    for (i = 0; i < d_v_street; i++)
        for (j = 0; j < d_h_street; j++)
            tmp_screen[i][j] = TERRAIN; // terrain


    //Street and vehicles (stop)
    for (n = 0; n < n_h_streets; n++) { //horizontal
        x_v = n * d_s_block + n;
        for (j = 0; j < d_h_street; j++) {

            if (GetValueCellStreet('H', n, j) >= 1) {
                id = GetIDCellStreet('H', n, j);
                if (GetTypeStreetVehicle(id) == 'H') {
                    if (GetVelocityVehicle(id) >= 1) {
                        tmp_screen[x_v][j] = 1; //Vehicle

                        //if (GetValueCellStreet('H', n, j) == 1)
                        //  tmp_screen[x_v][j] = RED;
                    }
                    else {
                        tmp_screen[x_v][j] = STOP; //Stop

                        //if (GetValueCellStreet('H', n, j) == 1)
                        //  tmp_screen[x_v][j] = RED;
                    }
                }
            }
            else
                tmp_screen[x_v][j] = 0; //Street
        }
    }


    for (m = 0; m < m_v_streets; m++) { //vertical
        x_h = m * d_s_block + m;
        for (j = 0; j < d_v_street; j++) {
            if (GetValueCellStreet('V', m, j) >= 1) {
                id = GetIDCellStreet('V', m, j);
                if (GetTypeStreetVehicle(id) == 'V') {
                    if (GetVelocityVehicle(id) >= 1){
                        tmp_screen[j][x_h] = 2; //Vehicle

                        //if (GetValueCellStreet('V', m, j) == 1)
                        // tmp_screen[j][x_h] = RED;
                    }
                    else {
                        tmp_screen[j][x_h] = STOP; //Stop

                        // if (GetValueCellStreet('V', m, j) == 1)
                        //  tmp_screen[j][x_h] = RED;
                    }

                }
            }
            else
                tmp_screen[j][x_h] = 0; //Street
        }
    }

    //Traffic lights
#if TRAFFIC_LIGHTS


    // Traffic Lights
    if (inter_control == 1) {

        for (n = 0; n < n_h_streets; n++){
            x_v = n * d_s_block + n;
            for (m = 0; m < m_v_streets; m++){
                x_h = m * d_s_block + m;

                tmp_screen[x_v][GetPositionTrafficLight('H', n, m)] = (GetValueTrafficLight('H', n, m) == 1) ? GREEN : RED;
                tmp_screen[GetPositionTrafficLight('V', n, m)][x_h] = (GetValueTrafficLight('V', n, m) == 1) ? GREEN : RED;
            }
        }
    }
    else if (inter_control == 2 || inter_control == 3) {

        for (n = 0; n < n_h_streets; n++){
            x_v = n * d_s_block + n;
            for (m = 0; m < m_v_streets; m++){
                x_h = m * d_s_block + m;

                tmp_screen[x_v][GetPositionTrafficLightSO('H', n, m)] = (GetValueTrafficLightSO('H', n, m) == 1) ? GREEN : RED;
                tmp_screen[GetPositionTrafficLightSO('V', n, m)][x_h] = (GetValueTrafficLightSO('V', n, m) == 1) ? GREEN : RED;
            }
        }
    }


#endif

    //Sensores
#if 1

    if (inter_control == 2 || inter_control == 3) {

        if (metodo_s == 2) {
            //Sensores
            for (n = 0; n < n_h_streets; n++){
                x_v = n * d_s_block + n;
                for (m = 0; m < m_v_streets; m++){
                    x_h = m * d_s_block + m;

                    //qDebug() << GetPositionTrafficLightSO('H', n, m);
                    //qDebug() << GetValueTrafficLightSO('H', n, m);

                    tmp_screen[x_v + 1][GetPositionSensor('H', n, m)] = SENSOR; //Sensor H
                    tmp_screen[GetPositionSensor('V', n, m)][x_h + 1] = SENSOR; //sensor V
                }
            }

            //Horizontal
            for (n = 0; n < n_h_streets; n++) {
                x_v = n * d_s_block + n;
                for (m = 0; m < m_v_streets; m++){

                    int tmp_x_v =  x_v == 0 ? d_v_street - 2 : x_v - 2;

                    for (int i = 0; i <= d_s_block; i++){

                        if (GetDirectionSensor('H', n, m) == 'R') {

                            if (m == 0) {
                                if (i < (d_s_block - GetOffsetSensor('H', n, m))) {

                                    if (GetVirtualCellValue('H', n, m, i) >= 1) {
                                        int pos = i + GetPositionSensor('H', n, m);
                                        tmp_screen[tmp_x_v][pos] = GetVirtualCellVelocity('H', n, m, i) > 0 ? VIRTUAL_VEHICLE : STOP;
                                    }
                                }
                                else {
                                    if (GetVirtualCellValue('H', n, m, i) >= 1) {
                                        int pos = i - (d_s_block - (GetOffsetSensor('H', n, m)));
                                        tmp_screen[tmp_x_v][pos] = GetVirtualCellVelocity('H', n, m, i) > 0 ? VIRTUAL_VEHICLE : STOP;
                                    }
                                }
                            }
                            else {
                                if (GetVirtualCellValue('H', n, m, i) >= 1) {

                                    int pos = i + GetPositionSensor('H', n, m);
                                    tmp_screen[tmp_x_v][pos] = GetVirtualCellVelocity('H', n, m, i) > 0 ? VIRTUAL_VEHICLE : STOP;
                                }
                            }
                        }
                        else {

                            if (m == 0) {

                                if (i < GetOffsetSensor('H', n, m)) {
                                    if (GetVirtualCellValue('H', n, m, i) >= 1) {
                                        int pos = i + (d_h_street - GetOffsetSensor('H', n, m));
                                        tmp_screen[tmp_x_v][pos] = GetVirtualCellVelocity('H', n, m, i) > 0 ? VIRTUAL_VEHICLE : STOP;
                                    }
                                }
                                else {
                                    if (GetVirtualCellValue('H', n, m, i) >= 1) {
                                        int pos = GetPositionSensor('H', n, m) - d_s_block + i;
                                        tmp_screen[tmp_x_v][pos] = GetVirtualCellVelocity('H', n, m, i) > 0 ? VIRTUAL_VEHICLE : STOP;
                                    }
                                }
                            }
                            else {
                                if (GetVirtualCellValue('H', n, m, i) >= 1) {
                                    int pos = GetPositionSensor('H', n, m) - d_s_block + i;
                                    tmp_screen[tmp_x_v][pos] = GetVirtualCellVelocity('H', n, m, i) > 0 ? VIRTUAL_VEHICLE : STOP;
                                }
                            }
                        }
                    }
                }
            }

            //Vertical
            for (m = 0; m < m_v_streets; m++) {

                x_h = m * d_s_block + m;

                for (n = 0; n < n_h_streets; n++){

                    int tmp_x_h =  x_h == 0 ? d_h_street - 2 : x_h - 2;

                    for (int i = 0; i <= d_s_block; i++){

                        if (GetDirectionSensor('V', n, m) == 'R') {

                            if (n == 0) {

                                if (i < (d_s_block - GetOffsetSensor('V', n, m))) {

                                    if (GetVirtualCellValue('V', n, m, i) >= 1) {
                                        int pos = i + GetPositionSensor('V', n, m);
                                        tmp_screen[pos][tmp_x_h] = GetVirtualCellVelocity('V', n, m, i) > 0 ? VIRTUAL_VEHICLE : STOP;
                                    }
                                }
                                else {

                                    if (GetVirtualCellValue('V', n, m, i) >= 1) {
                                        int pos = i - (d_s_block - (GetOffsetSensor('V', n, m)));
                                        tmp_screen[pos][tmp_x_h] = GetVirtualCellVelocity('V', n, m, i) > 0 ? VIRTUAL_VEHICLE : STOP;
                                    }
                                }
                            }
                            else {
                                if (GetVirtualCellValue('V', n, m, i) >= 1) {
                                    int pos = i + GetPositionSensor('V', n, m);
                                    tmp_screen[pos][tmp_x_h] = GetVirtualCellVelocity('V', n, m, i) > 0 ? VIRTUAL_VEHICLE : STOP;
                                }
                            }
                        }
                        else {

                            if (n == 0) {

                                if (i < GetOffsetSensor('V', n, m)) {

                                    if (GetVirtualCellValue('V', n, m, i) >= 1) {
                                        int pos = i + (d_v_street - GetOffsetSensor('V', n, m));
                                        tmp_screen[pos][tmp_x_h] = GetVirtualCellVelocity('V', n, m, i) > 0 ? VIRTUAL_VEHICLE : STOP;
                                    }
                                }
                                else {

                                    if (GetVirtualCellValue('V', n, m, i) >= 1) {
                                        int pos = GetPositionSensor('V', n, m) - d_s_block + i;
                                        tmp_screen[pos][tmp_x_h] = GetVirtualCellVelocity('V', n, m, i) > 0 ? VIRTUAL_VEHICLE : STOP;
                                    }
                                }
                            }
                            else {
                                if (GetVirtualCellValue('V', n, m, i) >= 1) {
                                    int pos = GetPositionSensor('V', n, m) - d_s_block + i;
                                    tmp_screen[pos][tmp_x_h] = GetVirtualCellVelocity('V', n, m, i) > 0 ? VIRTUAL_VEHICLE : STOP;
                                }
                            }
                        }
                    }
                }
            }

        }
    }

#endif


#if OFFSET
    int offset = d_s_block / 2;
#else
    int offset = 0;
#endif

    int h = 0;
    int v = 0;

    int count_v = 0;
    for (i = 0; i < d_v_street; i++){
        int count_h = 0;
        v = i < d_v_street - offset ? i + offset: count_v++;
        for (j = 0; j < d_h_street; j++){
            h = j < d_h_street - offset ? j + offset: count_h++;
            screen[v][h] = tmp_screen[i][j];
        }

    }

    // 0 Street
    // 1 -- ls vehicle
    // -1 Pointer to intersection
    // -2 stop
    // -3 Green
    // -4 Red
    // -5 Terrain
    // -6 Sensor
    // -7 Virtual vehicle

    for (i = 0; i < d_v_street; i++)
        for (j = 0; j < d_h_street; j++) {

            if (screen[i][j] == TERRAIN){ //Terrain (more propability)

                p.setColor(QColor(0,0,0));
                paint->setPen(p);
                paint->drawPoint(j, i);
                // paint->drawEllipse ( j, i, 10, 10);
            }
            else if (screen[i][j] == 0){

                p.setColor(QColor(192,192,192));
                paint->setPen(p);
                paint->drawPoint(j, i);
            }
            else if (screen[i][j] == 1){//Vehicle H

                p.setColor(QColor(0,0,255));
                paint->setPen(p);
                paint->drawPoint(j, i);
            }
            else if (screen[i][j] == 2){//Vehicle V

                p.setColor(QColor(74,25,44));
                paint->setPen(p);
                paint->drawPoint(j, i);
            }
            else if (screen[i][j] == STOP){ //stop

                p.setColor(QColor(255,255,0));
                paint->setPen(p);
                paint->drawPoint(j, i);
            }
            else if (screen[i][j] == GREEN) { //Green

                p.setColor(QColor(0,255,0));
                // p.setColor(QColor(0,255,0));
                paint->setPen(p);
                paint->drawPoint(j, i);
                //paint->drawEllipse ( j, i, 2, 2);
            }
            else if (screen[i][j] == RED){ //Red

                p.setColor(QColor(255,0,0));
                paint->setPen(p);
                paint->drawPoint(j, i);
            }
            else if (screen[i][j] == SENSOR){ //sensor

                p.setColor(QColor(255,255,255));
                paint->setPen(p);
                paint->drawPoint(j, i);
            }
            else if (screen[i][j] == VIRTUAL_VEHICLE){//Virtual Vehicle

                p.setColor(QColor(0,255,255));
                paint->setPen(p);
                paint->drawPoint(j, i);
            }
            else { //Ruido

                p.setColor(QColor(110,110,0));
                paint->setPen(p);
                paint->drawPoint(j, i);
            }
        }



}



//#include <QtGui/QApplication>

#include <QApplication>
#include "mainwindow.h"

#include <time.h>
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand

#include <QtGui>

#if QT_VERSION >= 0x050000
    #include <QtWidgets>
#endif

int main(int argc, char *argv[])
{
    //srand(time(NULL));
    //std::srand(unsigned (std::time(0)));

    std::srand(1000);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}


















#if 0
    if (h_sensores[n][m].stop == false) {


       // density_local_h = float (h_sensores[n][m].received_mapa.size() +  h_sensores[n][m].sended_mapa.size() + h_sensores[n][m_tmp].received_mapa.size()) /
        //       float (2 * d_side_block - distance_e);
       //if (density > 0.6) {


        bool stop_received_front = false;
        bool stop_sended = false;

        stop_received_front = h_sensores[n][m_tmp].stop_received;
        stop_sended = h_sensores[n][m].stop_sended;

        if (stop_sended == true) {
           h_sensores[n][m].stop = true;
           h_sensores[n][m].counter_stop = 0;
           h_sensores[n][m].relax = false;

        }
        else if (stop_received_front == true) {

            h_sensores[n][m].stop = true;

            if (h_sensores[n][m].relax == false)
                h_sensores[n][m].counter_stop++;


            if (h_sensores[n][m].counter_stop >= umbral_stop) {
                h_sensores[n][m].stop = false;
                h_sensores[n][m].relax = true;
                h_sensores[n][m].counter_stop = 0;
            }


        }
        else {
            h_sensores[n][m].counter_stop = 0;
            h_sensores[n][m].relax = false;
        }



        /*
        else if (h_sensores[n][m].counter_stop >= umbral_stop) {
           h_sensores[n][m].stop = true;
           h_sensores[n][m].counter_stop = 0;
        }
        else if (stop_received_front == true && stop_sended == false) {
           h_sensores[n][m].counter_stop++;
        }*/




        m_tmp = PositionFrontSensor('H', n, m);
        int x = GetPositionSensor('H', n, m_tmp);
        int y = n;

        /*
        if (GetVehiclesStopped('H', y, x) == true && h_sensores[n][m].stop == false) {
            qDebug() << "Error" << error_fp << error_fn;
            error_fp++;
        }
        else if (GetVehiclesStopped('H', y, x) == false && h_sensores[n][m].stop == true) {
            qDebug() << "Error" << error_fp << error_fn;
            error_fn++;
        }*/


    }
    else {
        h_sensores[n][m].counter_stop = 0;
        h_sensores[n][m].relax = false;
    }

#endif




#if 0


        int m_aux;
        m_aux = m; //m_tmp
        VirtualVehicle veh_h;


        /*
        if (n == 0 && m_aux == 2) {

            // qDebug() << "Pos T: " << h_traffic_light_so[n][m_aux].position;
            //qDebug() << "Pos T: " << h_traffic_light_so[n][m_aux - 1].position;

            if (!h_sensores[n][m_aux].received_mapa.empty()) {

                qDebug() << "Pos Sen: " << h_sensores[n][m_aux].position;
                qDebug() << "Size R: " << h_sensores[n][m_aux].received_mapa.size();
                veh_h = h_sensores[n][m_aux].received_mapa.back();
                qDebug() << "Pos Last: " << veh_h.position;
                //qDebug() << "Last Stop: " << veh_h.vehicle_stop;
            }

            if (!h_sensores[n][m_aux - 1].sended_mapa.empty())
            qDebug() << "Before Size E: " << h_sensores[n][m_aux - 1].sended_mapa.size();

            qDebug() << "Stop en R: " << h_sensores[n][m_aux].stop_received;
            qDebug() << "Stop -1 en E: " << h_sensores[n][m_aux - 1].stop_sended;

            qDebug() << "My Stop: " << h_sensores[n][m_aux].my_stop;
            qDebug() << "Stop atras: " << h_sensores[n][m_aux-1].stop;


            qDebug() << "";

        }*/






        /*
              //  if (GetValueCellStreet('H', y, x) != 0)
                //     test_error_fp++;

              //    satisface = true;

                if (GetValueCellStreet('H', y, x) != 0 && h_satisface == true) {

                   // qDebug() << "TT-Correct: " << error_fp << "Error: " << error_fn << density_local_h;
                    error_fp++;
                }
                else if (GetValueCellStreet('H', y, x) == 0 && h_satisface == false) {

                   // qDebug() << "FF-Correct: " << error_fp << "Error: " << error_fn << density_local_h;
                    error_fp++;
                }
                else if ((GetValueCellStreet('H', y, x) != 0 && h_satisface == false)) {

                    qDebug() << "Correct: " << error_fp << "TF-Error: " << error_fn << density_local_h;
                    error_fn++;

                }
                else if (GetValueCellStreet('H', y, x) == 0 && h_satisface == true) {

                    qDebug() << "Correct: " << error_fp << "FT-Error: " << error_fn << density_local_h;
                    error_fn++;
                }

            */

#endif



#if 0
    if (v_sensores[m][n].value_vehicle == 1) {

        if (v_sensores[m][n].before_id != v_sensores[m][n].id && (v_sensores[m][n].received_mapa.size() < (d_side_block - distance_e))) {

            VirtualVehicle veh;
            // veh.t = 0;
            veh.position = GetPositionSensor('V', n, m);;
            veh.p_anterior = -1;
            veh.vehicle_stop = false;

            v_sensores[m][n].received_mapa.push_back(veh);
            v_sensores[m][n].before_id = v_sensores[m][n].id;

            v_sensores[m][n].received++;

        }
    }
    else {
        v_sensores[m][n].before_id = -1;
    }

#endif


#if 0
    if (v_sensores[m][n].stop == false) {


        // density_local_v = float (v_sensores[m][n].received_mapa.size() +  v_sensores[m][n].sended_mapa.size() + v_sensores[m][n_tmp].received_mapa.size()) /
        //       float (2 * d_side_block - distance_e);

        //if (density > 0.6) {


        bool stop_received_front = false;
        bool stop_sended = false;

        stop_received_front = v_sensores[m][n_tmp].stop_received;
        stop_sended = v_sensores[m][n].stop_sended;

        if (stop_sended == true) {
           v_sensores[m][n].stop = true;
           v_sensores[m][n].counter_stop = 0;
           v_sensores[m][n].relax = false;
        }
        else if (stop_received_front == true) {

            v_sensores[m][n].stop = true;

            if (v_sensores[m][n].relax == false)
                v_sensores[m][n].counter_stop++;


           /* if (v_sensores[m][n].counter_stop >= umbral_stop) {
                v_sensores[m][n].stop = false;
                v_sensores[m][n].relax = true;
                v_sensores[m][n].counter_stop = 0;
            }*/


        }
        else {
            v_sensores[m][n].counter_stop = 0;
            v_sensores[m][n].relax = false;
        }


        /*else if (v_sensores[m][n].counter_stop >= umbral_stop) {
           v_sensores[m][n].stop = true;
           v_sensores[m][n].counter_stop = 0;
        }
        else if (stop_received_front == true && stop_sended == false) {
           v_sensores[m][n].counter_stop++;
        }*/


    }
    else {
        v_sensores[m][n].counter_stop = 0;
        v_sensores[m][n].relax = false;
    }

#endif


#if 0

    int n_t = 1;
    int m_t = 0;


    if (n == n_t && m == m_t) {


        qDebug() << "HORIZONTALES";

        qDebug() << "epsilon: " << h_sensores[n][m].epsilon;
        qDebug() << "Numero de autos R: " << h_sensores[n][m].received_mapa.size();
        qDebug() << "Numero de autos E: " << h_sensores[n][m].sended_mapa.size();

        qDebug() << "Numero de autos Front Rp: " << h_sensores[n][m_tmp].received_mapa.size();
        qDebug() <<  "STOP: " << h_sensores[n][m].stop;

        /*
        qDebug() << "Enviados: " <<    h_sensores[n][m].sended;
        qDebug() << "Recibidos: " << h_sensores[n][m_tmp].received;
        qDebug() <<  "STOP: " << h_sensores[n][m].stop;*/

        /*
        qDebug() << "N: " << h_traffic_light_so[n][m].n_vehicles;
        qDebug() << "M: " << h_traffic_light_so[n][m].m_vehicles;
        qDebug() << "stop: " << h_traffic_light_so[n][m].vehicle_stop;
        qDebug() << "time: " << h_traffic_light_so[n][m].time_u;*/

        //qDebug() << endl;


        /*
        ///Verticales /////////////////////////////////////////////////////

        qDebug() << "VERTICALES";

       qDebug() << "epsilon: " << v_sensores[m][n].epsilon;
        qDebug() << "Numero de autos R: " << v_sensores[m][n].received_mapa.size();
        qDebug() << "Numero de autos E: " << v_sensores[m][n].sended_mapa.size();
        qDebug() << "Enviados: " <<    v_sensores[m][n].sended;
        qDebug() << "Recibidos: " << v_sensores[m][n_tmp].received;
        qDebug() <<  "STOP: " << v_sensores[m][n].stop;


        qDebug() << "N: " << v_traffic_light_so[m][n].n_vehicles;
        qDebug() << "M: " << v_traffic_light_so[m][n].m_vehicles;
        qDebug() << "stop: " << v_traffic_light_so[m][n].vehicle_stop;
        qDebug() << "time: " << v_traffic_light_so[m][n].time_u;*/

        qDebug() << endl;


    }

#endif


    #if PRINT
    if (n == n_t && m == m_t) {

        qDebug() << "Hor n: " << h_traffic_light_so[n][m].n_vehicles;
        qDebug() << "Hor m: " << h_traffic_light_so[n][m].m_vehicles;

       /* qDebug() << "Ver n: " << v_traffic_light_so[m][n].n_vehicles;
        qDebug() << "Ver m: " << v_traffic_light_so[m][n].m_vehicles;
*/
        qDebug() << endl;
    }
#endif


#if 0
    if (h_sensores[n][m].value_vehicle == 1) {

        if (h_sensores[n][m].before_id != h_sensores[n][m].id && (h_sensores[n][m].received_mapa.size() < (d_side_block - distance_e))) {

            VirtualVehicle veh;
            //   veh.t = 0;
            veh.position = GetPositionSensor('H', n, m);
            veh.p_anterior = -1;
            veh.vehicle_stop = false;


            veh.velocity = 0;
            veh.direction = h_sensores[n][m].direction;



           // if (GetReceivedSizeVirtualVehicles('H', h_sensores[n][m].direction, n, m) < (d_side_block)){


                int k = localVirtualVehiclePosition('H', h_sensores[n][m].direction, n, m, veh.position);
                SetVirtualCellValue('H', n, m, k, 1);
                SetVirtualCellStop('H', n, m, k, veh.velocity);
                h_sensores[n][m].vehicles_read.push_back(veh);

            //}


            h_sensores[n][m].received_mapa.push_back(veh);
            h_sensores[n][m].before_id = h_sensores[n][m].id;

            h_sensores[n][m].received++;
        }
    }
    else {
        h_sensores[n][m].before_id = -1;
    }

#endif





















































#if 0

void SimulacioReceived(char type_street, int n, int m)
{

    int i;
    int tam;

    if (type_street == 'H') {


       if (h_sensores[n][m].my_stop == true)
          h_sensores[n][m].stop_received = true;
       else
          h_sensores[n][m].stop_received = false;


        //Simular un paso de tiempo del mapa received
        if (!h_sensores[n][m].received_mapa.empty()) {

            h_sensores[n][m].aux_received_mapa = h_sensores[n][m].received_mapa;

            if (h_sensores[n][m].direction == 'R') {

                int d = h_traffic_light_so[n][m].position - h_sensores[n][m].aux_received_mapa.front().position;

                if (d >= 1) //Se encuentra distante del semaforo
                    h_sensores[n][m].received_mapa.front().position++;
                else if (d == 0 && h_sensores[n][m].state_light == 1) //Se encuentra en el semaforo y comprueba a la luz verde
                    h_sensores[n][m].received_mapa.front().position++;
                else if (d < 0) //Despues del semaforo
                    h_sensores[n][m].received_mapa.front().position++;


                //deterinar si el vehiculo de enfrente esta detenido
                if (h_sensores[n][m].received_mapa.front().p_anterior ==  h_sensores[n][m].received_mapa.front().position)
                    h_sensores[n][m].received_mapa.front().vehicle_stop = true;
                else
                    h_sensores[n][m].received_mapa.front().vehicle_stop = false;

                h_sensores[n][m].received_mapa.front().p_anterior =  h_sensores[n][m].received_mapa.front().position;



                tam = h_sensores[n][m].aux_received_mapa.size() - 1;
                for (i = tam; i > 0; i--) {

                    if ((h_sensores[n][m].aux_received_mapa[i - 1].position - h_sensores[n][m].aux_received_mapa[i].position) > 1)
                        h_sensores[n][m].received_mapa[i].position++;


                    if (h_sensores[n][m].received_mapa[i].p_anterior == h_sensores[n][m].received_mapa[i].position)
                        h_sensores[n][m].received_mapa[i].vehicle_stop = true;
                    else
                        h_sensores[n][m].received_mapa[i].vehicle_stop = false;

                    h_sensores[n][m].received_mapa[i].p_anterior = h_sensores[n][m].received_mapa[i].position;
                }


                VirtualVehicle veh_h = h_sensores[n][m].received_mapa.back();
                int p_f;

                p_f = h_sensores[n][m].position;

                //TODO si solo hay un ellemento y calles chicas, esto puede fallar porque deja el stop_received en true y ese auto es eliminado despues?
                if ((veh_h.position >= p_f && veh_h.position <= p_f + holgura) && veh_h.vehicle_stop == true)
                   h_sensores[n][m].stop_received = true;


                // Enviar elementos
                if (h_sensores[n][m].received_mapa.front().position > h_traffic_light_so[n][m].position) {

                    veh_h =  h_sensores[n][m].received_mapa.front();
                    h_sensores[n][m].received_mapa.erase(h_sensores[n][m].received_mapa.begin());

                    if (h_sensores[n][m].sended_mapa.size() < (distance_e + 2))//Previene de no meter mas elementos que el tamanio e (el 2 por interseccion y e comienza en 0)
                        h_sensores[n][m].sended_mapa.push_back(veh_h);

                }

            }
            else {

                int d = h_sensores[n][m].aux_received_mapa.front().position - h_traffic_light_so[n][m].position;

                if (d >= 1)
                    h_sensores[n][m].received_mapa.front().position--;
                else if (d == 0 && h_sensores[n][m].state_light == 1)
                    h_sensores[n][m].received_mapa.front().position--;
                else if (d < 0)
                    h_sensores[n][m].received_mapa.front().position--;



                //deterinar si el vehiculo de enfrente esta detenido
                if (h_sensores[n][m].received_mapa.front().p_anterior ==  h_sensores[n][m].received_mapa.front().position)
                    h_sensores[n][m].received_mapa.front().vehicle_stop = true;
                else
                    h_sensores[n][m].received_mapa.front().vehicle_stop = false;

                h_sensores[n][m].received_mapa.front().p_anterior =  h_sensores[n][m].received_mapa.front().position;


                tam = h_sensores[n][m].aux_received_mapa.size() - 1;
                for (i = tam; i > 0; i--) {

                    if ((h_sensores[n][m].aux_received_mapa[i].position - h_sensores[n][m].aux_received_mapa[i - 1].position) > 1)
                        h_sensores[n][m].received_mapa[i].position--;

                    if (h_sensores[n][m].received_mapa[i].p_anterior == h_sensores[n][m].received_mapa[i].position)
                        h_sensores[n][m].received_mapa[i].vehicle_stop = true;
                    else
                        h_sensores[n][m].received_mapa[i].vehicle_stop = false;

                    h_sensores[n][m].received_mapa[i].p_anterior = h_sensores[n][m].received_mapa[i].position;

                }


                VirtualVehicle veh_h = h_sensores[n][m].received_mapa.back();
                int p_f;

                p_f = h_sensores[n][m].position;

                //TODO si solo hay un ellemento y calles chicas, esto puede fallar porque deja el stop_received en true y ese auto es eliminado despues?
                if ((veh_h.position <= p_f && veh_h.position >= p_f - holgura) && veh_h.vehicle_stop == true)
                   h_sensores[n][m].stop_received = true;


                // Enviar elementospp
                if (h_sensores[n][m].received_mapa.front().position < h_traffic_light_so[n][m].position) {

                    veh_h = h_sensores[n][m].received_mapa.front();

                    h_sensores[n][m].received_mapa.erase(h_sensores[n][m].received_mapa.begin());

                    if (h_sensores[n][m].sended_mapa.size() < (distance_e + 2)) //(el 2 por interseccion y e comienza en 0)
                       h_sensores[n][m].sended_mapa.push_back(veh_h);
                }

            }
        }
    }
    else {

        int i;
        int tam;

        if (v_sensores[m][n].my_stop == true)
            v_sensores[m][n].stop_received = true;
        else
            v_sensores[m][n].stop_received = false;


        //Simular un paso de tiempo del mapa received
        if (!v_sensores[m][n].received_mapa.empty()) {

            v_sensores[m][n].aux_received_mapa = v_sensores[m][n].received_mapa;

            if (v_sensores[m][n].direction == 'R') {

                int d = v_traffic_light_so[m][n].position - v_sensores[m][n].aux_received_mapa.front().position;

                if (d >= 1)
                    v_sensores[m][n].received_mapa.front().position++;
                else if (d == 0 && v_sensores[m][n].state_light == 1) //Aqui parte que comprueba a la luz verde
                    v_sensores[m][n].received_mapa.front().position++;
                else  if (d < 0)
                    v_sensores[m][n].received_mapa.front().position++;



                if (v_sensores[m][n].received_mapa.front().p_anterior == v_sensores[m][n].received_mapa.front().position)
                     v_sensores[m][n].received_mapa.front().vehicle_stop = true;
                else
                     v_sensores[m][n].received_mapa.front().vehicle_stop = false;

                 v_sensores[m][n].received_mapa.front().p_anterior =  v_sensores[m][n].received_mapa.front().position;



                tam = v_sensores[m][n].aux_received_mapa.size() - 1;
                for (i = tam; i > 0; i--) {

                    if ((v_sensores[m][n].aux_received_mapa[i - 1].position - v_sensores[m][n].aux_received_mapa[i].position) > 1)
                        v_sensores[m][n].received_mapa[i].position++;

                    if (v_sensores[m][n].received_mapa[i].p_anterior == v_sensores[m][n].received_mapa[i].position)
                        v_sensores[m][n].received_mapa[i].vehicle_stop = true;
                    else
                        v_sensores[m][n].received_mapa[i].vehicle_stop = false;

                    v_sensores[m][n].received_mapa[i].p_anterior = v_sensores[m][n].received_mapa[i].position;
                }


                VirtualVehicle veh_v = v_sensores[m][n].received_mapa.back();
                int p_f;

                p_f = v_sensores[m][n].position;

                //TODO si solo hay un ellemento y calles chicas, esto puede fallar porque deja el stop_received en true y ese auto es eliminado despues?
                if ((veh_v.position >= p_f && veh_v.position <= p_f + holgura) && veh_v.vehicle_stop == true)
                   v_sensores[m][n].stop_received = true;


                // Enviar elementos
                if (v_sensores[m][n].received_mapa.front().position > v_traffic_light_so[m][n].position) {

                    veh_v =  v_sensores[m][n].received_mapa.front();

                    v_sensores[m][n].received_mapa.erase(v_sensores[m][n].received_mapa.begin());

                    if (v_sensores[m][n].sended_mapa.size() < (distance_e + 2))
                        v_sensores[m][n].sended_mapa.push_back(veh_v);
                }
            }
            else {

                int d = v_sensores[m][n].aux_received_mapa.front().position - v_traffic_light_so[m][n].position;

                if (d >= 1)
                    v_sensores[m][n].received_mapa.front().position--;
                else if (d == 0 && v_sensores[m][n].state_light == 1)
                    v_sensores[m][n].received_mapa.front().position--;
                else if (d < 0)
                    v_sensores[m][n].received_mapa.front().position--;


                if (v_sensores[m][n].received_mapa.front().p_anterior == v_sensores[m][n].received_mapa.front().position)
                    v_sensores[m][n].received_mapa.front().vehicle_stop = true;
                else
                    v_sensores[m][n].received_mapa.front().vehicle_stop = false;

                v_sensores[m][n].received_mapa.front().p_anterior =  v_sensores[m][n].received_mapa.front().position;


                tam = v_sensores[m][n].aux_received_mapa.size() - 1;
                for (i = tam; i > 0; i--) {

                    if ((v_sensores[m][n].aux_received_mapa[i].position - v_sensores[m][n].aux_received_mapa[i - 1].position) > 1)
                        v_sensores[m][n].received_mapa[i].position--;


                    if (v_sensores[m][n].received_mapa[i].p_anterior == v_sensores[m][n].received_mapa[i].position)
                        v_sensores[m][n].received_mapa[i].vehicle_stop = true;
                    else
                        v_sensores[m][n].received_mapa[i].vehicle_stop = false;

                    v_sensores[m][n].received_mapa[i].p_anterior = v_sensores[m][n].received_mapa[i].position;

                }

                VirtualVehicle veh_v = v_sensores[m][n].received_mapa.back();
                int p_f;

                p_f = v_sensores[m][n].position;

                //TODO si solo hay un ellemento y calles chicas, esto puede fallar porque deja el stop_received en true y ese auto es eliminado despues?
                if ((veh_v.position <= p_f && veh_v.position >= p_f - holgura) && veh_v.vehicle_stop == true)
                   v_sensores[m][n].stop_received = true;


                // Enviar elementospp
                if (v_sensores[m][n].received_mapa.front().position < v_traffic_light_so[m][n].position) {

                    veh_v =  v_sensores[m][n].received_mapa.front();

                    v_sensores[m][n].received_mapa.erase(v_sensores[m][n].received_mapa.begin());

                    if (v_sensores[m][n].sended_mapa.size() < (distance_e + 2))
                        v_sensores[m][n].sended_mapa.push_back(veh_v);

                }
            }
        }
    }
}


void SimulacioSended(char type_street, int n, int m)
{

    int i;
    int x;
    int tam;
    int m_tmp;
    int n_tmp;
    bool stop_received = false;


    if (type_street == 'H') {

        h_sensores[n][m].stop_sended = false; //Muy importante para no trabar en falso
        m_tmp = PositionFrontSensor('H', n, m);
        stop_received = h_sensores[n][m_tmp].stop_received;

        //Simular un paso de tiempo del mapa sended
        if (!h_sensores[n][m].sended_mapa.empty()) {

            h_sensores[n][m].aux_sended_mapa = h_sensores[n][m].sended_mapa;

            if (h_sensores[n][m].direction == 'R') {

                int aux_x = h_traffic_light_so[n][m].position + distance_e + 2; //Es mas facil en las fronteras hacer el desface y suponer que la calle continua...
                x = aux_x;

                int d = x - h_sensores[n][m].aux_sended_mapa.front().position;

                if (d > 1) //hay espacio para moverse antes del sensor
                    h_sensores[n][m].sended_mapa.front().position++;
                else if (d == 0 || d == 1) {//El vehiculo esta una posiicon antes o en el sensor

                    if (stop_received == false) //Si esta en falso puede avanzar; no podra avanzar si hay inidicacion de vehiculo detenido
                        h_sensores[n][m].sended_mapa.front().position++;
                }
                else
                   h_sensores[n][m].sended_mapa.front().position++;//El auto se encuentra mas alla del sensor...


                if (h_sensores[n][m].sended_mapa.front().p_anterior ==  h_sensores[n][m].sended_mapa.front().position) {
                     h_sensores[n][m].sended_mapa.front().vehicle_stop = true;
                     h_sensores[n][m].stop_sended = true;

                }
                else
                     h_sensores[n][m].sended_mapa.front().vehicle_stop = false;

                 h_sensores[n][m].sended_mapa.front().p_anterior =  h_sensores[n][m].sended_mapa.front().position;


                tam = h_sensores[n][m].aux_sended_mapa.size() - 1;
                for (i = tam; i > 0; i--) {

                    if ((h_sensores[n][m].aux_sended_mapa[i - 1].position - h_sensores[n][m].aux_sended_mapa[i].position) > 1)
                        h_sensores[n][m].sended_mapa[i].position++;

                    if (h_sensores[n][m].sended_mapa[i].p_anterior == h_sensores[n][m].sended_mapa[i].position) {
                        h_sensores[n][m].sended_mapa[i].vehicle_stop = true;
                        h_sensores[n][m].stop_sended = true;
                    }
                    else
                        h_sensores[n][m].sended_mapa[i].vehicle_stop = false;

                    h_sensores[n][m].sended_mapa[i].p_anterior = h_sensores[n][m].sended_mapa[i].position;

                }



                //int m_tmp = PositionFrontSensor('H', n, m);
                //int aux_x = h_sensores[n][m_tmp].position;//Interseccion y distanica e comienza des de 0

                /* if (n == 0 && m == 2) {


                     qDebug() << "Size Recieve Front: " << h_sensores[n][m+1].received_mapa.size() << endl;
                    // qDebug() << "State sotpe Front: " << h_sensores[n][m+1].stop << endl;

                     qDebug() << "Pos traffic L: " << h_traffic_light_so[n][m].position;
                     qDebug() << "Pos Sen: " << h_sensores[n][m].position;
                     qDebug() << "Pos Front: " << h_sensores[n][m+1].position ;
                     qDebug() << "Pos sensor Aux: " << aux_x;
                     qDebug() << "Pos Veh Front: " << h_sensores[n][m].sended_mapa.front().position;
                     qDebug() << "size Sen: " << h_sensores[n][m].sended_mapa.size();
                     qDebug() << "State stop: " << h_sensores[n][m].stop_sended << endl;

                 }*/


                if (h_sensores[n][m].sended_mapa.front().position > aux_x) {//aux_x es la posicion del sensor de en frente

                    h_sensores[n][m].sended_mapa.erase(h_sensores[n][m].sended_mapa.begin());
                    h_sensores[n][m].sended++;
                }

            }
            else {

                int aux_x = h_traffic_light_so[n][m].position - distance_e - 2; //Es mas facil en las fronteras hacer el desface y suponer que la calle continua...
                x = aux_x;



                int d = h_sensores[n][m].aux_sended_mapa.front().position - x;

                if (d > 1) //hay espacio para moverse antes del sensor
                    h_sensores[n][m].sended_mapa.front().position--;
                else if (d == 0 || d == 1) {//El vehiculo esta una posiicon antes o en el sensor

                    if (stop_received == false) //Si esta en falso puede avanzar; no podra avanzar si hay inidicacion de vehiculo detenido
                        h_sensores[n][m].sended_mapa.front().position--;
                }
                else
                   h_sensores[n][m].sended_mapa.front().position--;//El auto se encuentra mas alla del sensor...



                if (h_sensores[n][m].sended_mapa.front().p_anterior ==  h_sensores[n][m].sended_mapa.front().position) {
                     h_sensores[n][m].sended_mapa.front().vehicle_stop = true;
                     h_sensores[n][m].stop_sended = true;
                }
                else
                     h_sensores[n][m].sended_mapa.front().vehicle_stop = false;

                 h_sensores[n][m].sended_mapa.front().p_anterior =  h_sensores[n][m].sended_mapa.front().position;


                 tam = h_sensores[n][m].aux_sended_mapa.size() - 1;


                 for (i = tam; i > 0; i--) {

                     if ((h_sensores[n][m].aux_sended_mapa[i].position - h_sensores[n][m].aux_sended_mapa[i - 1].position) > 1)
                         h_sensores[n][m].sended_mapa[i].position--;


                     if (h_sensores[n][m].sended_mapa[i].p_anterior == h_sensores[n][m].sended_mapa[i].position) {
                         h_sensores[n][m].sended_mapa[i].vehicle_stop = true;
                         h_sensores[n][m].stop_sended = true;
                     }
                     else
                         h_sensores[n][m].sended_mapa[i].vehicle_stop = false;

                     h_sensores[n][m].sended_mapa[i].p_anterior = h_sensores[n][m].sended_mapa[i].position;

                 }

                 // qDebug() << "";


                // int m_tmp = PositionFrontSensor('H', n, m);
                // int aux_x = h_sensores[n][m_tmp].position;//Interseccion y distanica e comienza des de 0


                 /*if (n == 1 && m == 0) {

                     qDebug() << "Dis: " << aux_x;
                     qDebug() << "Pos traffic L: " << h_traffic_light_so[n][m].position;
                     qDebug() << "Pos Aux: " << aux_x;
                     qDebug() << "Pos Sen F: " << h_sensores[n][m_ver_streets - 1].position ;
                 }*/


                 if (h_sensores[n][m].sended_mapa.front().position < aux_x) {//aux_x es la posicion del sensor de en frente

                     h_sensores[n][m].sended_mapa.erase(h_sensores[n][m].sended_mapa.begin());
                     h_sensores[n][m].sended++;
                 }
            }
        }
    }
    else {

        int i;
        int tam;

        v_sensores[m][n].stop_sended = false; //Muy importante para no trabar en falso
        n_tmp = PositionFrontSensor('V', n, m);
        stop_received = v_sensores[m][n_tmp].stop_received;

        if (!v_sensores[m][n].sended_mapa.empty()) {

            v_sensores[m][n].aux_sended_mapa = v_sensores[m][n].sended_mapa;

            if (v_sensores[m][n].direction == 'R') {

                int aux_x = v_traffic_light_so[m][n].position + distance_e + 2; //Es mas facil en las fronteras hacer el desface y suponer que la calle continua...

                x = aux_x;

                int d = x - v_sensores[m][n].aux_sended_mapa.front().position;


                if (d > 1) //hay espacio para moverse antes del sensor
                    v_sensores[m][n].sended_mapa.front().position++;
                else if (d == 0 || d == 1) {//El vehiculo esta una posiicon antes o en el sensor

                    if (stop_received == false) //Si esta en falso puede avanzar; no podra avanzar si hay inidicacion de vehiculo detenido
                        v_sensores[m][n].sended_mapa.front().position++;
                }
                else
                   v_sensores[m][n].sended_mapa.front().position++;//El auto se encuentra mas alla del sensor...


                if (v_sensores[m][n].sended_mapa.front().p_anterior == v_sensores[m][n].sended_mapa.front().position) {
                     v_sensores[m][n].sended_mapa.front().vehicle_stop = true;
                     v_sensores[m][n].stop_sended = true;
                }
                else
                     v_sensores[m][n].sended_mapa.front().vehicle_stop = false;

                 v_sensores[m][n].sended_mapa.front().p_anterior =  v_sensores[m][n].sended_mapa.front().position;


                tam = v_sensores[m][n].aux_sended_mapa.size() - 1;
                for (i = tam; i > 0; i--) {

                    if ((v_sensores[m][n].aux_sended_mapa[i - 1].position - v_sensores[m][n].aux_sended_mapa[i].position) > 1)
                        v_sensores[m][n].sended_mapa[i].position++;

                    //   if (n == 0 && m == 1)
                    //    qDebug() << "PosV: " << i << v_sensores[m][n].sended_mapa[i].position;


                    if (v_sensores[m][n].sended_mapa[i].p_anterior ==  v_sensores[m][n].sended_mapa[i].position) {
                         v_sensores[m][n].sended_mapa[i].vehicle_stop = true;
                         v_sensores[m][n].stop_sended = true;
                    }
                    else
                         v_sensores[m][n].sended_mapa[i].vehicle_stop = false;

                     v_sensores[m][n].sended_mapa[i].p_anterior = v_sensores[m][n].sended_mapa[i].position;

                }

                //if (n == 0 && m == 1)
                // qDebug() << "PosV: " << 0 << v_sensores[m][n].sended_mapa[0].position;


                //int n_tmp = PositionFrontSensor('V', n, m);
                //int aux_x = v_sensores[m][n_tmp].position;//Interseccion y distanica e comienza des de 0


                if (v_sensores[m][n].sended_mapa.front().position > aux_x) {//aux_x es la posicion del sensor de enfrente

                    v_sensores[m][n].sended_mapa.erase(v_sensores[m][n].sended_mapa.begin());
                    v_sensores[m][n].sended++;
                }
            }
            else {

                int aux_x = v_traffic_light_so[m][n].position - distance_e - 2; //Es mas facil en las fronteras hacer el desface y suponer que la calle continua...

                x = aux_x;

                int d = v_sensores[m][n].aux_sended_mapa.front().position - x;


                if (d > 1) //hay espacio para moverse antes del sensor
                    v_sensores[m][n].sended_mapa.front().position--;
                else if (d == 0 || d == 1) {//El vehiculo esta una posiicon antes o en el sensor

                    if (stop_received == false) //Si esta en falso puede avanzar; no podra avanzar si hay inidicacion de vehiculo detenido
                        v_sensores[m][n].sended_mapa.front().position--;
                }
                else
                   v_sensores[m][n].sended_mapa.front().position--;//El auto se encuentra mas alla del sensor...


                if (v_sensores[m][n].sended_mapa.front().p_anterior ==  v_sensores[m][n].sended_mapa.front().position) {
                     v_sensores[m][n].sended_mapa.front().vehicle_stop = true;
                     v_sensores[m][n].stop_sended = true;
                }
                else
                     v_sensores[m][n].sended_mapa.front().vehicle_stop = false;

                 v_sensores[m][n].sended_mapa.front().p_anterior =  v_sensores[m][n].sended_mapa.front().position;


                // if (n == 0 && m == 1)
                // qDebug() << "tos tamanio: " << v_sensores[m][n].sended_mapa.size();


                tam = v_sensores[m][n].aux_sended_mapa.size() - 1;
                for (i = tam; i > 0; i--) {

                    if ((v_sensores[m][n].aux_sended_mapa[i].position - v_sensores[m][n].aux_sended_mapa[i - 1].position) > 1)
                        v_sensores[m][n].sended_mapa[i].position--;

                    //  if (n == 0 && m == 1)
                    //  qDebug() << "PosV: " << i << v_sensores[m][n].sended_mapa[i].position;

                    if (v_sensores[m][n].sended_mapa[i].p_anterior ==  v_sensores[m][n].sended_mapa[i].position) {
                         v_sensores[m][n].sended_mapa[i].vehicle_stop = true;
                         v_sensores[m][n].stop_sended = true;
                    }
                    else
                         v_sensores[m][n].sended_mapa[i].vehicle_stop = false;

                     v_sensores[m][n].sended_mapa[i].p_anterior =  v_sensores[m][n].sended_mapa[i].position;
                }


                // if (n == 0 && m == 1)
                //   qDebug() << "PosV: " << 0 << v_sensores[m][n].sended_mapa[0].position;

                // if (n == 0 && m == 1) {
                // qDebug() << "Pos sensor: " << x;
                // qDebug() << "Pos f: " << v_sensores[m][n].sended_mapa.front().position;
                //}


                //int n_tmp = PositionFrontSensor('V', n, m);
                //int aux_x = v_sensores[m][n_tmp].position;//Interseccion y distanica e comienza des de 0

                if (v_sensores[m][n].sended_mapa.front().position < aux_x) {//aux_x es la posicion del sensor de en frente

                    v_sensores[m][n].sended_mapa.erase(v_sensores[m][n].sended_mapa.begin());
                    v_sensores[m][n].sended++;
                }
            }
        }
    }

}


void SensingSelfOrganizingSensor(int n, int m)
{

    int x;
    int y;

    int value;
    int value_temp;
    int id;
    int m_tmp;

    h_sensores[n][m].state_light = h_traffic_light_so[n][m].light;

    m_tmp = PositionFrontSensor('H', n, m);

    /////////////////////////////
    //Adqurir y recibir elementos

    x = GetPositionSensor('H', n, m);
    y = n;

    if (frand() <= error_sensor) {
        value = GetValueCellStreet('H', y, x);
        h_sensores[n][m].my_stop = GetVehiclesStopped('H', y, x);

    }
    else {
        value = 0;
        h_sensores[n][m].my_stop = false;
    }


    if (value == 1) {

        id = GetIDCellStreet('H', y, x);

        if (h_sensores[n][m].before_id != id && (h_sensores[n][m].received_mapa.size() < (d_side_block - distance_e))) {

            VirtualVehicle veh;
            //   veh.t = 0;
            veh.position = x;
            veh.p_anterior = -1;
            veh.vehicle_stop = false;

            h_sensores[n][m].received_mapa.push_back(veh);
            h_sensores[n][m].before_id = id;

            h_sensores[n][m].received++;
        }
    }
    else {
        h_sensores[n][m].before_id = -1;
    }

    x = GetPositionSensor('H', n, m_tmp);
    y = n;

    //Verifica si esta parado un automovil adelante

    if (frand() <= error_sensor) {
        h_sensores[n][m].stop = GetVehiclesStopped('H', y, x); // falta llenar toda la calle
    }
    else
        h_sensores[n][m].stop = false;



    VirtualVehicle veh_h;
    VirtualVehicle veh_temp;
    float density_local_h = 0.0;

    if (value_temp == 1)
        h_sensores[n][m].counter_stop = 0;


    //Simular received
    SimulacioReceived('H', n, m);

    //Simular sended
    SimulacioSended('H', n, m, x, value);

    Correction('H', n, m, m_tmp);


    int m_aux;
    m_aux = m; //m_tmp

    /*
    if (n == 0 && m_aux == 3) {

        // qDebug() << "Pos T: " << h_traffic_light_so[n][m_aux].position;
        //qDebug() << "Pos T: " << h_traffic_light_so[n][m_aux - 1].position;

        if (!h_sensores[n][m_aux].received_mapa.empty()) {

            qDebug() << "Pos Sen: " << h_sensores[n][m_aux].position;
            qDebug() << "Size R: " << h_sensores[n][m_aux].received_mapa.size();
            veh_h = h_sensores[n][m_aux].received_mapa.back();
            qDebug() << "Pos Last: " << veh_h.position;
            qDebug() << "Last Stop: " << veh_h.vehicle_stop;
        }

        // if (!h_sensores[n][m_aux - 1].sended_mapa.empty())
        qDebug() << "Before Size E: " << h_sensores[n][m_aux - 1].sended_mapa.size();

        qDebug() << "Stop en E: " << h_sensores[n][m_aux - 1].stop_sended;
        qDebug() << "Stop: " << h_sensores[n][m_aux - 1].stop;

        qDebug() << "";

    }*/


    if (h_sensores[n][m].stop == false) {


       // density_local_h = float (h_sensores[n][m].received_mapa.size() +  h_sensores[n][m].sended_mapa.size() + h_sensores[n][m_tmp].received_mapa.size()) /
        //       float (2 * d_side_block - distance_e);
       //if (density > 0.6) {


        bool stop_received_front = false;
        bool stop_sended = false;

        stop_received_front = h_sensores[n][m_tmp].stop_received;
        stop_sended = h_sensores[n][m].stop_sended;

        if (stop_received_front == true || stop_sended == true)
           h_sensores[n][m].stop = true;


        if (GetVehiclesStopped('H', y, x) == true && h_sensores[n][m].stop == false) {
          //  qDebug() << "Error" << error_fp;
            error_fp++;
        }


    } //Esta es la llave del falso detectado...

    //Asignacion auto-organizante
    assigmentSelfOrganization('H', n, m);



    /*
          //  if (GetValueCellStreet('H', y, x) != 0)
            //     test_error_fp++;

          //    satisface = true;

            if (GetValueCellStreet('H', y, x) != 0 && h_satisface == true) {

               // qDebug() << "TT-Correct: " << error_fp << "Error: " << error_fn << density_local_h;
                error_fp++;
            }
            else if (GetValueCellStreet('H', y, x) == 0 && h_satisface == false) {

               // qDebug() << "FF-Correct: " << error_fp << "Error: " << error_fn << density_local_h;
                error_fp++;
            }
            else if ((GetValueCellStreet('H', y, x) != 0 && h_satisface == false)) {

                qDebug() << "Correct: " << error_fp << "TF-Error: " << error_fn << density_local_h;
                error_fn++;

            }
            else if (GetValueCellStreet('H', y, x) == 0 && h_satisface == true) {

                qDebug() << "Correct: " << error_fp << "FT-Error: " << error_fn << density_local_h;
                error_fn++;
            }

        */


    /////////////////////////////////////////////////////////////////////
    //Vertical ////////////////////

    int n_tmp;

    v_sensores[m][n].state_light = v_traffic_light_so[m][n].light;

    n_tmp = PositionFrontSensor('V', n, m);

    /////////////////////////////
    x = GetPositionSensor('V', n, m);
    y = m;


    if (frand() <= error_sensor)

    if (frand() <= error_sensor) {
        value = GetValueCellStreet('V', y, x);
        v_sensores[m][n].my_stop = GetVehiclesStopped('V', y, x);

    }
    else {

        value = 0;
        v_sensores[m][n].my_stop = false;
    }

    if (value == 1) {

        id = GetIDCellStreet('V', y, x);

        if (v_sensores[m][n].before_id != id  && (v_sensores[m][n].received_mapa.size() < (d_side_block - distance_e))) {

            VirtualVehicle veh;
            // veh.t = 0;
            veh.position = x;
            veh.p_anterior = -1;
            veh.vehicle_stop = false;

            v_sensores[m][n].received_mapa.push_back(veh);
            v_sensores[m][n].before_id = id;

            v_sensores[m][n].received++;

        }
    }
    else {
        v_sensores[m][n].before_id = -1;
    }

    x = GetPositionSensor('V', n_tmp, m);
    y = m;


    if (frand() <= error_sensor)
      v_sensores[m][n].stop = GetVehiclesStopped('V', y, x);
    else //provocar error
      v_sensores[m][n].stop = false;



    VirtualVehicle veh_v;
    float density_local_v = 0.0;


    //Received
    SimulacioReceived('V', n, m);

    //Sended V
    SimulacioSended('V', n, m, x, value);

    //Correction
    Correction('V', n, m, n_tmp);



    if (v_sensores[m][n].stop == false) {

        float v_density_aux = 0.0;
        int v_distance_e_local = 0;
        int v_number_veh = 0;
        bool v_satisface = false;
        bool v_stop_tmp = false;



        // density_local_v = float (v_sensores[m][n].received_mapa.size() +  v_sensores[m][n].sended_mapa.size() + v_sensores[m][n_tmp].received_mapa.size()) /
        //       float (2 * d_side_block - distance_e);

        //if (density > 0.6) {




        bool stop_received_front = false;
        bool stop_sended = false;

        stop_received_front = v_sensores[m][n_tmp].stop_received;
        stop_sended = v_sensores[m][n].stop_sended;

        if (stop_received_front == true || stop_sended == true)
            h_sensores[n][m].stop = true;


    } //Esta es la llave que esta cerrando e false


    assigmentSelfOrganization('V', n, m);


#if 0

    int n_t = 1;
    int m_t = 0;


    if (n == n_t && m == m_t) {


        qDebug() << "HORIZONTALES";

        qDebug() << "epsilon: " << h_sensores[n][m].epsilon;
        qDebug() << "Numero de autos R: " << h_sensores[n][m].received_mapa.size();
        qDebug() << "Numero de autos E: " << h_sensores[n][m].sended_mapa.size();

        qDebug() << "Numero de autos Front Rp: " << h_sensores[n][m_tmp].received_mapa.size();
        qDebug() <<  "STOP: " << h_sensores[n][m].stop;

        /*
        qDebug() << "Enviados: " <<    h_sensores[n][m].sended;
        qDebug() << "Recibidos: " << h_sensores[n][m_tmp].received;
        qDebug() <<  "STOP: " << h_sensores[n][m].stop;*/

        /*
        qDebug() << "N: " << h_traffic_light_so[n][m].n_vehicles;
        qDebug() << "M: " << h_traffic_light_so[n][m].m_vehicles;
        qDebug() << "stop: " << h_traffic_light_so[n][m].vehicle_stop;
        qDebug() << "time: " << h_traffic_light_so[n][m].time_u;*/

        //qDebug() << endl;


        /*
        ///Verticales /////////////////////////////////////////////////////

        qDebug() << "VERTICALES";

       qDebug() << "epsilon: " << v_sensores[m][n].epsilon;
        qDebug() << "Numero de autos R: " << v_sensores[m][n].received_mapa.size();
        qDebug() << "Numero de autos E: " << v_sensores[m][n].sended_mapa.size();
        qDebug() << "Enviados: " <<    v_sensores[m][n].sended;
        qDebug() << "Recibidos: " << v_sensores[m][n_tmp].received;
        qDebug() <<  "STOP: " << v_sensores[m][n].stop;


        qDebug() << "N: " << v_traffic_light_so[m][n].n_vehicles;
        qDebug() << "M: " << v_traffic_light_so[m][n].m_vehicles;
        qDebug() << "stop: " << v_traffic_light_so[m][n].vehicle_stop;
        qDebug() << "time: " << v_traffic_light_so[m][n].time_u;*/

        qDebug() << endl;


    }

#endif


    #if PRINT
    if (n == n_t && m == m_t) {

        qDebug() << "Hor n: " << h_traffic_light_so[n][m].n_vehicles;
        qDebug() << "Hor m: " << h_traffic_light_so[n][m].m_vehicles;

       /* qDebug() << "Ver n: " << v_traffic_light_so[m][n].n_vehicles;
        qDebug() << "Ver m: " << v_traffic_light_so[m][n].m_vehicles;
*/
        qDebug() << endl;
    }
#endif


}

#endif






/*
SPosition pos = GetPositionVehicle(id);
int direction = GetDirectionVehicle(id);
char type_street = GetTypeStreetVehicle(id);
bool visible = GetVisibleVehicle(id);

//qDebug() << "tssss: " << visible;


if (metodo_sensado == 1) {


    pos_t = GetSensoresTraditional(type_street, pos.x, direction);


    if (pos_t == -1) {//no esta en zona de sensado por lo que el estado por omision es true
        pw_vehicles[id].visible = true;
    }
    else if (visible == false) {

        pw_vehicles[id].visible = false;
    }
    else {

        if (frand() <= error_sensor) {

            pw_vehicles[id].visible = true;

        }
        else {


            pw_vehicles[id].visible = false;

        }

    }
}
else {


    pos_t = GetSensoresRegion(type_street, pos.x, direction);

    if (pos_t == -1) {//no esta en zona de sensado por lo que el estado por omision es true
        pw_vehicles[id].visible = true;
    }
    else if (visible == false) {

        pw_vehicles[id].visible = false;
    }
    else {

        if (frand() <= error_sensor)
            pw_vehicles[id].visible = true;
        else {

            //pr_vehicles[id].visible = false;//TODO aqui hay que ver, es que tiene que reaccionar al momento
            pw_vehicles[id].visible = false;

          //qDebug() << "Toosssss" << id << pos_t;
        }

    }

}*/




/*
void SensarBefore(int n, int m)
{

    int x;
    int y;

    /////////////////////////////
    //Adqurir elementos

    x = GetPositionSensor('H', n, m);
    y = n;

    //h_sensores[n][m].value_vehicle = 0;
    h_sensores[n][m].id = -1;
    h_sensores[n][m].my_stop = false;


    if (GetVisibleCellStreet('H', n, x) == true) {

       // h_sensores[n][m].value_vehicle = GetValueCellStreet('H', y, x);
        h_sensores[n][m].id = GetIDCellStreet('H', y, x);
        h_sensores[n][m].my_stop = GetVehiclesStopped('H', y, x);
    }



    //////////////////////////////////////////////////////////////////////////////////////////

    x = GetPositionSensor('V', n, m);
    y = m;

    //v_sensores[m][n].value_vehicle = 0;
    v_sensores[m][n].id = -1;
    v_sensores[m][n].my_stop = false;


    if (GetVisibleCellStreet('V', m, x) == true) {
     //   v_sensores[m][n].value_vehicle = GetValueCellStreet('V', y, x);
        v_sensores[m][n].id = GetIDCellStreet('V', y, x);
        v_sensores[m][n].my_stop = GetVehiclesStopped('V', y, x);

    }


}

*/




/*  if (h_sensores[n][m].state_light == 0 && h_sensores[n][m].stop == true) {

      int x2 = m * d_side_block + m;

      h_sensores[n][m].received_mapa.clear();

      if (n % 2 == 0) {

          for (int k = 0; k < d_side_block - distance_e + 1; k++) { //tengo la duda si es mas 1 d_block - d_e + 1

              VirtualVehicle veh;
              //   veh.t = 0;

              if (x2 == 0) {

                  veh.position = d_hor_street - k - 1;
                  veh.p_anterior = -1;
                  h_sensores[n][m].received_mapa.push_back(veh);

              }
              else {

                  veh.position = x2 - k - 1;
                  veh.p_anterior = -1;
                  h_sensores[n][m].received_mapa.push_back(veh);
              }
          }
      }
      else {

          for (int k = 0; k <  d_side_block - distance_e + 1; k++) {

              VirtualVehicle veh;
              //   veh.t = 0;

              veh.position = x2 + k + 1;
              veh.p_anterior = -1;
              h_sensores[n][m].received_mapa.push_back(veh);

          }
      }


  }
  else {*/

/*if (v_sensores[m][n].state_light == 0 &&  v_sensores[m][n].stop == true) {

    int y2 = n * d_side_block + n;
    v_sensores[m][n].received_mapa.clear();

    if (m % 2 == 0) {

        for (int k = 0; k < d_side_block - distance_e + 1; k++) {

            VirtualVehicle veh;
            //   veh.t = 0;

            if (y2 == 0) {

                veh.position = d_ver_street - k - 1;
                veh.p_anterior = -1;
                v_sensores[m][n].received_mapa.push_back(veh);
            }
            else {

                veh.position = y2 - k - 1;
                veh.p_anterior = -1;
                v_sensores[m][n].received_mapa.push_back(veh);
            }
        }
    }
    else {

        for (int k = 0; k < d_side_block - distance_e + 1; k++) {

            VirtualVehicle veh;
            //   veh.t = 0;

            veh.position = y2 + k + 1;
            veh.p_anterior = -1;
            v_sensores[m][n].received_mapa.push_back(veh);

        }
    }

}
else  {*/

//////////Sensing function/////////////////////

#if 0
    if (h_sensores[n][m].stop == false) {


       // density_local_h = float (h_sensores[n][m].received_mapa.size() +  h_sensores[n][m].sended_mapa.size() + h_sensores[n][m_tmp].received_mapa.size()) /
        //       float (2 * d_side_block - distance_e);
       //if (density > 0.6) {


        bool stop_received_front = false;
        bool stop_sended = false;

        stop_received_front = h_sensores[n][m_tmp].stop_received;
        stop_sended = h_sensores[n][m].stop_sended;

        if (stop_sended == true) {
           h_sensores[n][m].stop = true;
           h_sensores[n][m].counter_stop = 0;
           h_sensores[n][m].relax = false;

        }
        else if (stop_received_front == true) {

            h_sensores[n][m].stop = true;

            if (h_sensores[n][m].relax == false)
                h_sensores[n][m].counter_stop++;


            if (h_sensores[n][m].counter_stop >= umbral_stop) {
                h_sensores[n][m].stop = false;
                h_sensores[n][m].relax = true;
                h_sensores[n][m].counter_stop = 0;
            }


        }
        else {
            h_sensores[n][m].counter_stop = 0;
            h_sensores[n][m].relax = false;
        }



        /*
        else if (h_sensores[n][m].counter_stop >= umbral_stop) {
           h_sensores[n][m].stop = true;
           h_sensores[n][m].counter_stop = 0;
        }
        else if (stop_received_front == true && stop_sended == false) {
           h_sensores[n][m].counter_stop++;
        }*/




        m_tmp = PositionFrontSensor('H', n, m);
        int x = GetPositionSensor('H', n, m_tmp);
        int y = n;

        /*
        if (GetVehiclesStopped('H', y, x) == true && h_sensores[n][m].stop == false) {
            qDebug() << "Error" << error_fp << error_fn;
            error_fp++;
        }
        else if (GetVehiclesStopped('H', y, x) == false && h_sensores[n][m].stop == true) {
            qDebug() << "Error" << error_fp << error_fn;
            error_fn++;
        }*/


    }
    else {
        h_sensores[n][m].counter_stop = 0;
        h_sensores[n][m].relax = false;
    }

#endif

#if 0
    if (v_sensores[m][n].stop == false) {


        // density_local_v = float (v_sensores[m][n].received_mapa.size() +  v_sensores[m][n].sended_mapa.size() + v_sensores[m][n_tmp].received_mapa.size()) /
        //       float (2 * d_side_block - distance_e);

        //if (density > 0.6) {


        bool stop_received_front = false;
        bool stop_sended = false;

        stop_received_front = v_sensores[m][n_tmp].stop_received;
        stop_sended = v_sensores[m][n].stop_sended;

        if (stop_sended == true) {
           v_sensores[m][n].stop = true;
           v_sensores[m][n].counter_stop = 0;
           v_sensores[m][n].relax = false;
        }
        else if (stop_received_front == true) {

            v_sensores[m][n].stop = true;

            if (v_sensores[m][n].relax == false)
                v_sensores[m][n].counter_stop++;


           /* if (v_sensores[m][n].counter_stop >= umbral_stop) {
                v_sensores[m][n].stop = false;
                v_sensores[m][n].relax = true;
                v_sensores[m][n].counter_stop = 0;
            }*/


        }
        else {
            v_sensores[m][n].counter_stop = 0;
            v_sensores[m][n].relax = false;
        }


        /*else if (v_sensores[m][n].counter_stop >= umbral_stop) {
           v_sensores[m][n].stop = true;
           v_sensores[m][n].counter_stop = 0;
        }
        else if (stop_received_front == true && stop_sended == false) {
           v_sensores[m][n].counter_stop++;
        }*/


    }
    else {
        v_sensores[m][n].counter_stop = 0;
        v_sensores[m][n].relax = false;
    }

#endif




        /*
        void Correction(char type_street, int n, int m, int s_f)
        {

            if (type_street == 'H') {


                int m_tmp = s_f;

                int correction;

                //int sended = 0;
                //  bool cambio = false;
                // int recibidos_h = h_sensores[n][m_tmp].received;
                // int enviados_h = h_sensores[n][m].sended;

                //h_sensores[n][m].epsilon = 0;

                //Correction in the count of the senden and received
                if (h_sensores[n][m].state_light == 1 &&  h_traffic_light_so[n][m].changed == true) {

                    correction =  h_sensores[n][m_tmp].received - h_sensores[n][m].sended;

                    if (correction < 0)
                        correction = -1 * correction;

                    if  (correction != 0) //if  (correction >= umbral || correction < 0)
                        h_sensores[n][m].epsilon = correction; //umbral = 2
                    else
                        h_sensores[n][m].epsilon = 0;

                    h_sensores[n][m].sended = 0;
                    h_sensores[n][m_tmp].received = 0;
                }
            }
            else {

                int n_tmp = s_f;

                int correction;
                // int recibidos_v = v_sensores[m][n_tmp].received;
                // int enviados_v = v_sensores[m][n].sended;
                //  v_sensores[m][n].epsilon = 0;

                //Correction in the count of the senden and received
                if (v_sensores[m][n].state_light == 1 &&  v_traffic_light_so[m][n].changed == true) {

                    correction = v_sensores[m][n_tmp].received - v_sensores[m][n_tmp].sended;

                    if (correction < 0)
                        correction = -1 * correction;

                    if  (correction != 0) //if  (correction >= umbral || correction < 0)
                        v_sensores[m][n].epsilon = correction; //umbral = 2
                    else
                        v_sensores[m][n].epsilon = 0;

                    v_sensores[m][n].sended = 0;
                    v_sensores[m][n_tmp].received = 0;

                }
            }

        }
        */




#if 0
    if (n == 0 && m == 1) {


        qDebug() << "HORIZONTALES";


        qDebug() << "N: " << h_traffic_light_so[n][m].n_vehicles;
        qDebug() << "M: " << h_traffic_light_so[n][m].m_vehicles;
        qDebug() << "stop: " << h_traffic_light_so[n][m].vehicle_stop;
        qDebug() << "time: " << h_traffic_light_so[n][m].time_u;

        //qDebug() << endl;


        ///Verticales /////////////////////////////////////////////////////

        qDebug() << "VERTICALES";


        qDebug() << "N: " << v_traffic_light_so[m][n].n_vehicles;
        qDebug() << "M: " << v_traffic_light_so[m][n].m_vehicles;
        qDebug() << "stop: " << v_traffic_light_so[m][n].vehicle_stop;
        qDebug() << "time: " << v_traffic_light_so[m][n].time_u;

        qDebug() << endl;


    }

#endif

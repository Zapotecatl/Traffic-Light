#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QXmlStreamReader>

#include <QPainter>
#include <QPaintDevice>
#include <QtCore>

#include <QDateTime>
#include <QTime>
#include <QtGui>


#include <QSize>

using namespace std;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QLabel *labelCity;
    QTimer *t_timer;
    QImage *qi_city;
    QPixmap imgpaint;
    QPen p;
    QPixmap img_scaled;
    QSize size_s;

    QString str_info;

    int resolution_w;
    int resolution_h;
    int key;

    int **tmp_screen;
    int **screen;

private:
    Ui::MainWindow *ui;
    QXmlStreamReader m_xmlReader;

private:

    void readScene(QString filename);
    void processReadXML(QByteArray data);
    void PrintStreetCity(QPainter *paint, QPen &p);

private slots:

      void on_Experiment_clicked();
      void on_pBInicia_clicked();
      void PaintCity();

protected:
    void keyPressEvent(QKeyEvent* event);

};

#endif // MAINWINDOW_H

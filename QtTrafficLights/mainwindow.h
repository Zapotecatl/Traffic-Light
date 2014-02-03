#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

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


class MyLabel : public QLabel
{
    Q_OBJECT

public:
    MyLabel(QWidget *parent = 0);
    virtual ~MyLabel();
    int var_x;
    int var_y;

signals:
        void clicked();
};

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



    int resolution_w;
    int resolution_h;

private:
    Ui::MainWindow *ui;

private slots:


      void on_Experiment_clicked();
      void on_NameMethod_itemSelectionChanged();

      void on_pBInicia_clicked();
      void PaintCity();
      void PrintStreetCity(QPainter *paint, QPen &p);



};

#endif // MAINWINDOW_H

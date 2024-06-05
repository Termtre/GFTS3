#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <QtGraphs>
#include <QThread>
#include "../include/Task_manager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class Calculator;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_start_clicked();

    void on_end_clicked(const Numerical_method method);

    void on_comboBox_2_currentIndexChanged(int index);

    void on_editRows_editingFinished();

    void on_editColumns_editingFinished();

    void on_checkBox_clicked(bool checked);

    void on_comboBox_currentIndexChanged(int index);

    void on_epsEdit_editingFinished();

    void on_NmaxEdit_editingFinished();

    void on_comboBox_3_currentIndexChanged(int index);

    void on_checkBox_2_clicked(bool checked);

private:
    bool flagColumns = true, flagRows = true, flagNmax = true, flagEps = true;

    int rows = 11;
    int columns = 11;
    int NMax = 10000;
    double Eps = 0.0001;

    double aX = 0.;
    double aY = 0.;
    double bX = 1.;
    double bY = 1.;

    Task_manager manager;

    Q3DSurface* surfaceGraph = nullptr;
    QSurface3DSeries* surfaces[5] = { nullptr, nullptr, nullptr, nullptr, nullptr };

    Numerical_method curTest; // текущий метод для тестовой
    Numerical_method curMain; // текущий метод для основной
    Numerical_method curMain2;// текущий метод для большей размерности

private:
    void taskTest(std::vector<QSurfaceDataArray>&);
    void taskMain(std::vector<QSurfaceDataArray>&);
//    void task(std::vector<QSurfaceDataArray>&);

private:
    Ui::MainWindow *ui;
};

class Calculator : public QObject
{
    Q_OBJECT
public:
    const Numerical_method method;
    Task_manager & manager;
public:
    Calculator(const Numerical_method _method,Task_manager & _manager):
    method(_method), manager(_manager)
    {};
    void task()
    {
        manager.wait_solution(method);
        emit Complited(method);
    };
signals:
    void Complited(const Numerical_method method);
};
#endif // MAINWINDOW_H

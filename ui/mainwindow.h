#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <QtGraphs>

using Matrix = std::vector<std::vector<float>>;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_start_clicked();

    void on_comboBox_2_currentIndexChanged(int index);

    void on_editRows_editingFinished();

    void on_editColumns_editingFinished();

    void on_checkBox_clicked(bool checked);

    void on_comboBox_currentIndexChanged(int index);

private:
    bool flagColumns = true, flagRows = true;

    int rows = 11;
    int columns = 11;
    Matrix matrix;

    Q3DSurface* surfaceGraph = nullptr;
    QSurface3DSeries *series = nullptr;

private:
    void fillMatrix();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

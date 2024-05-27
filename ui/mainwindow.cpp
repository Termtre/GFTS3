#include "mainwindow.h"
#include <utility>
#include "ui_mainwindow.h"
#include <QMessageBox>
//#include <QFuture>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->reset();
    this->setWindowTitle("GFTS3");

    surfaceGraph = new Q3DSurface();

    ui->gridLayout_7->addWidget(surfaceGraph);

    ui->comboBox->setCurrentIndex(0);
    ui->comboBox_2->setCurrentIndex(0);
    ui->comboBox_3->setCurrentIndex(0);
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget_2->setCurrentIndex(0);

    ui->textBrowser->setSource(QUrl("qrc:/htmls/urls/testtask.html"), QTextDocument::HtmlResource);

    surfaceGraph->setCameraZoomLevel(85.f);
    surfaceGraph->setCameraPreset(QAbstract3DGraph::CameraPreset::IsometricRight);
    surfaceGraph->activeTheme()->setType(Q3DTheme::Theme::PrimaryColors);

    surfaceGraph->setAxisX(new QValue3DAxis);
    surfaceGraph->setAxisY(new QValue3DAxis);
    surfaceGraph->setAxisZ(new QValue3DAxis);

    surfaceGraph->axisX()->setLabelFormat("%.4f");
    surfaceGraph->axisY()->setLabelFormat("%.4f");
    surfaceGraph->axisZ()->setLabelFormat("%.4f");

    surfaceGraph->axisX()->setLabelAutoRotation(30.f);
    surfaceGraph->axisY()->setLabelAutoRotation(90.f);
    surfaceGraph->axisZ()->setLabelAutoRotation(30.f);

    for (int i = 0; i < 5; i++)
    {
        surfaces[i] = new QSurface3DSeries();
        surfaceGraph->addSeries(surfaces[i]);
    }

    ui->checkBox->setChecked(true);
    ui->checkBox_2->setChecked(true);

    ui->groupBox_N2->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_start_clicked()
{
    ui->progressBar->reset();
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum((rows - 1) * (columns - 1));

    ui->tableWidget->setRowCount(rows);
    ui->tableWidget->setColumnCount(columns);

    ui->tableWidget_2->setRowCount(rows);
    ui->tableWidget_2->setColumnCount(columns);

    ui->tableWidget_3->setRowCount(rows);
    ui->tableWidget_3->setColumnCount(columns);

    QStringList horizontalLabels;
    QStringList verticalLabels;

    for (int i = 0; i < rows; i++)
    {
        verticalLabels << QString::number(i);
    }

    for (int j = 0; j < columns; j++)
    {
        horizontalLabels << QString::number(j);
    }

    ui->tableWidget->setHorizontalHeaderLabels(horizontalLabels);
    ui->tableWidget->setVerticalHeaderLabels(verticalLabels);

    ui->tableWidget_2->setHorizontalHeaderLabels(horizontalLabels);
    ui->tableWidget_2->setVerticalHeaderLabels(verticalLabels);

    ui->tableWidget_3->setHorizontalHeaderLabels(horizontalLabels);
    ui->tableWidget_3->setVerticalHeaderLabels(verticalLabels);

    QThread * future = new QThread;
    Calculator * calc;

    if (ui->comboBox_2->currentIndex() == 0)
    {
         manager.createTask(rows - 1, columns - 1, Numerical_method::MWR_TEST, Eps, NMax);
         calc = new Calculator(Numerical_method::MWR_TEST,manager);
    }
    else
    {
        manager.createTask(rows - 1, columns - 1, Numerical_method::MWR_MAIN, Eps, NMax);
        manager.createTask(rows - 1, columns - 1, Numerical_method::MWR_BIGGER, Eps, 2 * NMax);
        calc = new Calculator(Numerical_method::MWR_BIGGER,manager);
    }

    // int numberOfGraphics = (ui->comboBox_2->currentIndex() == 0) ? 4 : 5;
    //
    // std::vector<QSurfaceDataArray> vecArray(numberOfGraphics);
    //
    // for (auto it = vecArray.begin(); it != vecArray.end(); ++it)
    // {
    //     it->reserve(rows);
    // }
    //
    // if (ui->comboBox_2->currentIndex() == 0)
    // {
    //     taskTest(vecArray);
    // }
    // else
    // {
    //     taskMain(vecArray);
    // }
    //
    // for (int i = 0; i < numberOfGraphics; i++)
    // {
    //     surfaces[i]->dataProxy()->resetArray(vecArray[i]);
    // }
    //
    // for (int i = 0; i < numberOfGraphics; i++)
    //     surfaces[i]->setVisible(false);
    //
    // surfaces[ui->comboBox_3->currentIndex()]->setVisible(true);
    //

    calc->moveToThread(future);

    connect (future, &QThread::started, calc, &Calculator::task);

    connect (calc, &Calculator::Complited, this, &MainWindow::on_end_clicked);
    connect (calc, &Calculator::Complited, future, &QThread::quit);
    connect (calc, &Calculator::Complited, calc, &Calculator::deleteLater);
    connect (future, &QThread::finished, future, &QThread::deleteLater);

    future->start();

}


 void MainWindow::on_end_clicked(const Numerical_method method)
{
    int numberOfGraphics = (method == Numerical_method::MWR_TEST) ? 4 : 5;

    std::vector<QSurfaceDataArray> vecArray(numberOfGraphics);

    for (auto it = vecArray.begin(); it != vecArray.end(); ++it)
    {
        it->reserve(rows);
    }

    if (method == Numerical_method::MWR_TEST)
    {
        taskTest(vecArray);
    }
    else
    {
        taskMain(vecArray);
    }

    for (int i = 0; i < numberOfGraphics; i++)
    {
        surfaces[i]->dataProxy()->resetArray(vecArray[i]);
    }

    for (int i = 0; i < numberOfGraphics; i++)
        surfaces[i]->setVisible(false);

    surfaces[ui->comboBox_3->currentIndex()]->setVisible(true);
}

void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
    QUrl url1("qrc:/htmls/urls/testtask.html");
    QUrl url2("qrc:/htmls/urls/maintask.html");

    if (index == 0)
    {
        ui->textBrowser->setSource(url1, QTextDocument::HtmlResource);
        ui->labelTypeTask->setText("Тестовая задача");
        ui->tabWidget_2->setTabText(0, "Точное решение");
        ui->tabWidget_2->setTabText(1, "Численное решение");
        ui->tabWidget_2->setTabText(2, "Разность точного и численного решений");

        if (ui->comboBox_3->count() == 5) ui->comboBox_3->removeItem(4);
        ui->comboBox_3->setItemText(0, "Точное решение");
        ui->comboBox_3->setItemText(1, "Начальное приближение");
        ui->comboBox_3->setItemText(2, "Численное решение");
        ui->comboBox_3->setItemText(3, "Разность точного и численного решений");
    }
    else
    {
        ui->textBrowser->setSource(url2, QTextDocument::HtmlResource);
        ui->labelTypeTask->setText("Основная задача");
        ui->tabWidget_2->setTabText(0, "Численное решение ");
        ui->tabWidget_2->setTabText(1, "Численное решение с половинным шагом");
        ui->tabWidget_2->setTabText(2, "Разность численных решений");

        ui->comboBox_3->setItemText(0, "Начальное приближение");
        ui->comboBox_3->setItemText(1, "Начальное приближение на сетке с половинным шагом");
        ui->comboBox_3->setItemText(2, "Численное решение");
        ui->comboBox_3->setItemText(3, "Численное решение на сетке с половинным шагом");
        ui->comboBox_3->addItem("Разность численных решений");
    }
}

void MainWindow::on_editRows_editingFinished()
{
    bool ok;
    ui->editRows->text().toInt(&ok);

    if (ok == false)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Ошибка");
        msgBox.setText("Недопустимые символы");
        msgBox.exec();

        ui->start->setEnabled(false);
        flagRows = false;

        return;
    }

    rows = ui->editRows->text().toInt();

    if (rows < 0)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Ошибка");
        msgBox.setText("Количество разбиений по y не может быть меньше 0");
        msgBox.exec();

        flagRows = false;

        ui->start->setEnabled(false);

        return;
    }

    rows++;

    flagRows = true;

    if (flagRows && flagColumns && flagNmax && flagEps) ui->start->setEnabled(true);
}


void MainWindow::on_editColumns_editingFinished()
{
    bool ok;
    ui->editColumns->text().toInt(&ok);

    if (ok == false)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Ошибка");
        msgBox.setText("Недопустимые символы");
        msgBox.exec();

        ui->start->setEnabled(false);

        flagColumns = false;

        return;
    }

    columns = ui->editColumns->text().toInt();

    if (columns < 0)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Ошибка");
        msgBox.setText("Количество разбиений по x не может быть меньше 0");
        msgBox.exec();

        flagColumns = false;

        ui->start->setEnabled(false);

        return;
    }

    columns++;

    flagColumns = true;

    if (flagRows && flagColumns && flagNmax && flagEps) ui->start->setEnabled(true);
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    switch(index) //TODO MAKE ENUM, pls
    {
    case 0:
        ui->labelMethod->setText("Метод верхней релаксации");
        ui->labelMethodParametr->setText("С параметром ω = ");
        break;
    case 1:
        ui->labelMethod->setText("Метод простой итерации");
        ui->labelMethodParametr->setText("С параметром τ = ");
        break;
    case 2:
        ui->labelMethod->setText("Метод минимальных невязок");
        ui->labelMethodParametr->setText("С числом K = ");
        break;
    case 3:
        ui->labelMethod->setText("Метод с чебышевским набором параметров");
        ui->labelMethodParametr->setText("С числом K = ");
        break;
    case 4:
        ui->labelMethod->setText("Метод сопряженных градиентов");
        ui->labelMethodParametr->setText("С параметром ω = ");
        break;
    default:
        break;
    }
}

void MainWindow::on_epsEdit_editingFinished()
{
    bool ok;
    ui->epsEdit->text().toDouble(&ok);

    if (ok == false)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Ошибка");
        msgBox.setText("Недопустимые символы");
        msgBox.exec();

        ui->start->setEnabled(false);

        flagEps = false;

        return;
    }

    Eps = ui->epsEdit->text().toDouble();

    if (Eps < 0.)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Ошибка");
        msgBox.setText("Точность не может быть отрицательной");
        msgBox.exec();

        flagEps = false;

        ui->start->setEnabled(false);

        return;
    }

    flagEps = true;

    if (flagRows && flagColumns && flagNmax && flagEps) ui->start->setEnabled(true);
}


void MainWindow::on_NmaxEdit_editingFinished()
{
    bool ok;
    ui->NmaxEdit->text().toInt(&ok);

    if (ok == false)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Ошибка");
        msgBox.setText("Недопустимые символы");
        msgBox.exec();

        ui->start->setEnabled(false);

        flagNmax = false;

        return;
    }

    NMax = ui->NmaxEdit->text().toInt();

    if (NMax < 0)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Ошибка");
        msgBox.setText("Максимальное количество итераций не может быть меньше 0");
        msgBox.exec();

        flagNmax = false;

        ui->start->setEnabled(false);

        return;
    }

    flagNmax = true;

    if (flagRows && flagColumns && flagNmax && flagEps) ui->start->setEnabled(true);
}


void MainWindow::on_comboBox_3_currentIndexChanged(int index)
{
    if(!surfaces[0]) return;

    for (int i = 0; i < (ui->comboBox_2->currentIndex() == 0 ? 4 : 5); i++)
        surfaces[i]->setVisible(false);

    switch (index) // TODO MAKE ENUM, pls
    {
    case 0:
    {
        surfaces[0]->setVisible(true);
        break;
    }
    case 1:
    {
        surfaces[1]->setVisible(true);
        break;
    }
    case 2:
    {
        surfaces[2]->setVisible(true);
        break;
    }
    case 3:
    {
        surfaces[3]->setVisible(true);
        break;
    }
    case 4:
    {
        surfaces[4]->setVisible(true);
        break;
    }
    }
}

void MainWindow::on_checkBox_clicked(bool checked)
{
    for (int i = 0; i < 5; i++)
    {
        if (checked && ui->checkBox_2->isChecked())
            surfaces[i]->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
        else if (checked && !ui->checkBox_2->isChecked())
            surfaces[i]->setDrawMode(QSurface3DSeries::DrawWireframe);
        else if (!checked && ui->checkBox_2->isChecked())
            surfaces[i]->setDrawMode(QSurface3DSeries::DrawSurface);
    }
}

void MainWindow::on_checkBox_2_clicked(bool checked)
{
    for (int i = 0; i < 5; i++)
    {
        if (checked && ui->checkBox->isChecked())
            surfaces[i]->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
        else if (checked && !ui->checkBox->isChecked())
            surfaces[i]->setDrawMode(QSurface3DSeries::DrawSurface);
        else if (!checked && ui->checkBox->isChecked())
            surfaces[i]->setDrawMode(QSurface3DSeries::DrawWireframe);
    }
}


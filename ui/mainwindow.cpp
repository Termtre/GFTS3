#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

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

    ui->tableWidget_2->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_start_clicked()
{
    if (series) surfaceGraph->removeSeries(series);
    series = nullptr;

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
    // test
    auto dataArray = QSurfaceDataArray();

    for (int i = 0; i < rows; i++)
    {
        horizontalLabels << QString::number(i);
        verticalLabels << QString::number(i);
        // test
        auto newRow = QSurfaceDataRow();

        for (int j = 0; j < columns; j++)
        {
            ui->tableWidget->setItem(i, j, new QTableWidgetItem(QString::number(i * j)));
            ui->progressBar->setValue(i * columns + j);
            newRow << QSurfaceDataItem(float(i), float(i * j), float(i));
        }

        dataArray << newRow;
    }

    series = new QSurface3DSeries();
    series->dataProxy()->resetArray(dataArray);

    surfaceGraph->addSeries(series);

    ui->tableWidget->setHorizontalHeaderLabels(horizontalLabels);
    ui->tableWidget->setVerticalHeaderLabels(verticalLabels);
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

void MainWindow::on_checkBox_clicked(bool checked)
{
    if (!series) return;
    if (checked) series->setWireframeColor(QColor(0, 0, 0, 255));
    else series->setWireframeColor(QColor(0, 0, 0, 0));
}


void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    switch(index)
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

    Eps = ui->epsEdit->text().toInt();

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
    if(!series) return;

    switch (index)
    {
    case 0:
    {
        series->setVisible(true);
        break;
    }
    case 1:
    {
        break;
    }
    case 2:
    {
        break;
    }
    case 3:
    {
        break;
    }
    case 4:
    {
        break;
    }
    }
}


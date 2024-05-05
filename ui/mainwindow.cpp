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
    matrix.resize(rows);

    for (auto it = matrix.begin(); it != matrix.end(); ++it)
    {
        it->resize(columns);
    }

    fillMatrix();

    ui->progressBar->reset();
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum((rows - 1) * (columns - 1));
    ui->tableWidget->setRowCount(rows);
    ui->tableWidget->setColumnCount(columns);

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
            ui->tableWidget->setItem(i, j, new QTableWidgetItem(QString::number(matrix[i][j])));
            ui->progressBar->setValue(i * columns + j);
            newRow << QSurfaceDataItem(matrix[i][j], float(j), float(i));
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

    if (flagRows && flagColumns) ui->start->setEnabled(true);
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

    if (flagRows && flagColumns) ui->start->setEnabled(true);
}

void MainWindow::fillMatrix()
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            matrix[i][j] = i * j;
        }
    }
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


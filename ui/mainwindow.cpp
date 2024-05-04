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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_start_clicked()
{
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

    for (int i = 0; i < columns; i++)
    {
        horizontalLabels << QString::number(i);
    }

    for (int j = 0; j < rows; j++)
    {
        verticalLabels << QString::number(j);
    }

    ui->tableWidget->setHorizontalHeaderLabels(horizontalLabels);
    ui->tableWidget->setVerticalHeaderLabels(verticalLabels);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            ui->tableWidget->setItem(i, j, new QTableWidgetItem(QString::number(matrix[i][j])));
            ui->progressBar->setValue(i * columns + j);
        }
    }
}

void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
    QUrl url1("testtask.html");
    QUrl url2("maintask.html");

    if (index == 0)
    {
        ui->textBrowser->setSource(url1, QTextDocument::HtmlResource);
    }
    else
    {
        ui->textBrowser->setSource(url2, QTextDocument::HtmlResource);
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


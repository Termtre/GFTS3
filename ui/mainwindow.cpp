#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->reset();
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

        return;
    }

    rows = ui->editRows->text().toInt() + 1;

    if (rows < 0)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Ошибка");
        msgBox.setText("Количество строк не может быть меньше 0");
        msgBox.exec();

        ui->start->setEnabled(false);
    }

    return;
}


void MainWindow::on_editColumns_editingFinished()
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

        return;
    }

    columns = ui->editRows->text().toInt() + 1;

    if (columns < 0)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Ошибка");
        msgBox.setText("Количество столбцов не может быть меньше 0");
        msgBox.exec();

        ui->start->setEnabled(false);
    }

    return;
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


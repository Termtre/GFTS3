#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFuture>

void MainWindow::taskTest(std::vector<QSurfaceDataArray>& array)
{
    ui->groupBox_N2->setVisible(false);

    const size_t n = rows - 1;
    const size_t m = columns - 1;

    size_t count = 0;

    double epsMax = 0.0;
    double err;
    double xMax = 0., yMax = 0.;
    double x, y;

    std::unique_ptr<numeric_method::Matrix_solver> sN_test = std::make_unique<numeric_method::SOR_COL>(n,m,numeric_method::test{});

    const double stepX = (bX - aX) / static_cast<double>(n);
    const double stepY = (bY - aY) / static_cast<double>(m);

    for(size_t i = 0; i < n + 1; ++i)
    {
        QSurfaceDataRow newRow;

        newRow.reserve(m+1);

        x = qMin(bX, i * stepX);

        for(size_t j = 0; j < m + 1; ++j)
        {
            ui->progressBar->setValue(i * columns + j);

            y = qMin(bY, j * stepY);

            newRow.append(QSurfaceDataItem(y, sN_test->v[i][j], x));


        }
        array[1] << newRow;
    }

    ui->edit_R0->setText(QString::number(sN_test->calculate_residual()));
    //printf("%f\n", sN_test->calculate_residual());

    sN_test = manager.returnTask(count, Numerical_method::SOR_TEST);

    ui->edit_RN->setText(QString::number(sN_test->calculate_residual()));
    ui->edit_EpsN->setText(QString::number(sN_test->precision));
    ui->edit_parametr->setText(QString::number(dynamic_cast<numeric_method::SOR_COL *>(&(*sN_test))->w));

    //printf("%f\n", sN_test->calculate_residual());
    //printf("%f\n", sN_test->precision);

    for(size_t i = 0; i < n + 1; ++i)
    {
        QSurfaceDataRow newRow[3];

        for (int i = 0; i < 3; i++)
        {
            newRow[i].reserve(m + 1);
        }

        x = qMin(bX, i * stepX);

        for(size_t j = 0; j < m + 1; ++j)
        {
            y = qMin(bY, j * stepY);

            double tmp = std::sin(numeric_method::pi * i * j * stepX * stepY);

            tmp = std::exp(tmp * tmp);

            err = sN_test->v[i][j] - tmp;
            ui->tableWidget->setItem(i, j, new QTableWidgetItem(QString::number(tmp)));
            ui->tableWidget_2->setItem(i, j, new QTableWidgetItem(QString::number(sN_test->v[i][j])));
            ui->tableWidget_3->setItem(i, j, new QTableWidgetItem(QString::number(err)));

            newRow[0].append(QSurfaceDataItem(y, tmp, x));
            newRow[1].append(QSurfaceDataItem(y, sN_test->v[i][j], x));
            newRow[2].append(QSurfaceDataItem(y, abs(err), x));

            if (epsMax < abs(err))
            {
                epsMax = abs(err);

                xMax = x;
                yMax = y;
            }
        }

        array[0] << newRow[0];
        array[2] << newRow[1];
        array[3] << newRow[2];
    }

    ui->edit_n->setText(QString::number(n));
    ui->edit_m->setText(QString::number(m));

    ui->edit_epsmet->setText(QString::number(Eps, 'g', 10));
    ui->edit_Nmax->setText(QString::number(NMax));

    ui->edit_N->setText(QString::number(count));
    ui->edit_Eps1->setText(QString::number(epsMax));

    ui->edit_MaxX->setText(QString::number(xMax));
    ui->edit_MaxY->setText(QString::number(yMax));

    ui->lineEdit_12->setText("Линейная интерполяция вдоль оси x");

    ui->label_8->setText("Тестовая задача должна быть решена с погрешностью не более ε = 0.5⋅10<sup>–6</sup>");
    ui->label_11->setText("Максимальное отклонение точного и численного решений наблюдается в узле:");
}

void MainWindow::taskMain(std::vector<QSurfaceDataArray>& array)
{
    ui->groupBox_N2->setVisible(true);

    const size_t n = rows - 1;
    const size_t m = columns - 1;

    size_t count = 0, count1 = 0;

    double epsMax = 0.0;
    double err;
    double xMax = 0., yMax = 0.;
    double x, y;

    std::unique_ptr<numeric_method::Matrix_solver> sN = std::make_unique<numeric_method::SOR_COL>(n,m);
    std::unique_ptr<numeric_method::Matrix_solver> s2N = std::make_unique<numeric_method::SOR_COL>(2 * n,2 * m);
    const double stepX = (bX - aX) / static_cast<double>(n);
    const double stepY = (bY - aY) / static_cast<double>(m);

    for(size_t i = 0; i < n + 1; ++i)
    {
        QSurfaceDataRow newRow[2];

        for (int i = 0; i < 2; i++)
        {
            newRow[i].reserve(m + 1);
        }

        x = qMin(bX, static_cast<double>(i) * stepX);

        for(size_t j = 0; j < m + 1; ++j)
        {
            y = qMin(bY, static_cast<double>(j) * stepY);

            newRow[0].append(QSurfaceDataItem(y, sN->v[i][j], x));
            newRow[1].append(QSurfaceDataItem(y, s2N->v[2 * i][2 * j], x));

        }

        for (int i = 0; i < 2; i++)
        {
            array[i] << newRow[i];
        }
    }

    ui->edit_R0->setText(QString::number(sN->calculate_residual()));


    //printf("%f\n", sN->calculate_residual());
    //printf("%f\n", s2N->calculate_residual());

    sN = manager.returnTask(count, Numerical_method::SOR_MAIN);
    s2N = manager.returnTask(count1, Numerical_method::SOR_BIGGER);

    ui->edit_RN->setText(QString::number(sN->calculate_residual()));
    ui->edit_EpsN->setText(QString::number(sN->precision));
    ui->edit_parametr->setText(QString::number(dynamic_cast<numeric_method::SOR_COL *>(&(*sN))->w));

    ui->lineEdit_endNevN2->setText(QString::number(s2N->calculate_residual()));
    ui->lineEdit_endEpsN2->setText(QString::number(s2N->precision));
    ui->lineEdit_parametrN2->setText(QString::number(dynamic_cast<numeric_method::SOR_COL *>(&(*s2N))->w));

    //printf("%f\n", sN->calculate_residual());
    //printf("%f\n", s2N->calculate_residual());
    //printf("%f\n", sN->precision);
   // printf("%f\n", s2N->precision);

    for(size_t i = 0; i < n + 1; ++i)
    {
        QSurfaceDataRow newRow[3];

        for (int i = 0; i < 3; i++)
        {
            newRow[i].reserve(m + 1);
        }

        x = qMin(bX, static_cast<double>(i) * stepX);

        for(size_t j = 0; j < m + 1; ++j)
        {
            ui->progressBar->setValue(i * columns + j);

            y = qMin(bY, static_cast<double>(j) * stepY);

            err = sN->v[i][j] - s2N->v[i * 2][j * 2];

            ui->tableWidget->setItem(i, j, new QTableWidgetItem(QString::number(sN->v[i][j])));

            ui->tableWidget_2->setItem(i, j, new QTableWidgetItem(QString::number(s2N->v[2 * i][2 * j])));

            ui->tableWidget_3->setItem(i, j, new QTableWidgetItem(QString::number(err)));

            newRow[0].append(QSurfaceDataItem(y, sN->v[i][j], x));
            newRow[1].append(QSurfaceDataItem(y, s2N->v[i * 2][j * 2], x));
            newRow[2].append(QSurfaceDataItem(y, err, x));

            if (epsMax < abs(err))
            {
                epsMax = abs(err);

                xMax = x;
                yMax = y;
            }
        }

        for (int i = 0; i < 3; i++)
        {
            array[i + 2] << newRow[i];
        }
    }

    ui->edit_n->setText(QString::number(n));
    ui->edit_m->setText(QString::number(m));

    ui->edit_epsmet->setText(QString::number(Eps, 'g', 10));
    ui->edit_Nmax->setText(QString::number(NMax));

    ui->lineEdit_epsN2->setText(QString::number(Eps, 'g', 10));
    ui->lineEdit_maxN2->setText(QString::number(2 * NMax));
    ui->lineEdit_endN2->setText(QString::number(count1));

    ui->edit_N->setText(QString::number(count));
    ui->edit_Eps1->setText(QString::number(epsMax));

    ui->edit_MaxX->setText(QString::number(xMax));
    ui->edit_MaxY->setText(QString::number(yMax));

    ui->lineEdit_12->setText("Линейная интерполяция вдоль оси y");

    ui->label_8->setText("Основная задача должна быть решена с погрешностью не более ε = 0.5⋅10<sup>–6</sup>");
    ui->label_11->setText("Максимальное отклонение численных решений на основной сетке и\nсетке с половинным шагом в узле:");
}

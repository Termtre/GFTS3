#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../include/Task_manager.h"

void MainWindow::uiMWR(std::vector<QSurfaceDataArray>& array)
{
    Task_manager manager(4);

    size_t n = rows - 1;
    size_t m = columns - 1;

    size_t count = 0, count1 = 0;

    //std::cout << dynamic_cast<numeric_method::MWR *>(&(*sN))->w << std::endl; ///< How read variables from ingeritance

    double epsMax = 0.0;
    double err;
    double xMax = 0., yMax = 0.;
    double x, y;

   if (ui->comboBox_2->currentIndex())
   {
        manager.createTask(n, m, Numerical_method::MWR_MAIN, Eps, NMax);
        manager.createTask(n, m, Numerical_method::MWR_BIGGER, Eps, NMax);
        std::unique_ptr<numeric_method::Matrix_solver> s2N = manager.returnTask(count1, Numerical_method::MWR_BIGGER);
        std::unique_ptr<numeric_method::Matrix_solver> sN = manager.returnTask(count, Numerical_method::MWR_MAIN);

       for(size_t i = 0; i < n + 1; ++i)
       {
            QSurfaceDataRow newRow[5];
            for(size_t j = 0; j < m + 1; ++j)
            {
               ui->progressBar->setValue(i * columns + j);

               x = static_cast<double>(i) / static_cast<double>(n);
               y = static_cast<double>(j) / static_cast<double>(m);

               err = sN->v[i][j] - s2N->v[2 * i][2 * j];
               ui->tableWidget->setItem(i, j, new QTableWidgetItem(QString::number(sN->v[i][j])));
               ui->tableWidget_2->setItem(i, j, new QTableWidgetItem(QString::number(s2N->v[2 * i][2 * j])));
               ui->tableWidget_3->setItem(i, j, new QTableWidgetItem(QString::number(err)));

               if (epsMax < abs(err))
               {
                   epsMax = abs(err);

                   xMax = x;
                   yMax = y;
               }

               newRow[0] << QSurfaceDataItem(x, y, 0.);
               newRow[1] << QSurfaceDataItem(x, y, 0.);
               newRow[2] << QSurfaceDataItem(x, y, sN->v[i][j]);
               newRow[3] << QSurfaceDataItem(x, y, s2N->v[i][j]);
               newRow[3] << QSurfaceDataItem(x, y, err);

           }

            for (int i = 0; i < 5; i++)
           {
               array[i] << newRow[i];
           }

       }
   }
   else
   {
       manager.createTask(n, m, Numerical_method::MWR_TEST, Eps, NMax);
       std::unique_ptr<numeric_method::Matrix_solver> sN_test = manager.returnTask(count1, Numerical_method::MWR_TEST);

       for(size_t i = 0; i < n + 1; ++i)
       {
           QSurfaceDataRow newRow[4];

           for(size_t j = 0; j < m + 1; ++j)
           {
               ui->progressBar->setValue(i * columns + j);

               x = static_cast<double>(i) / static_cast<double>(n);
               y = static_cast<double>(j) / static_cast<double>(m);

               double tmp = std::sin(numeric_method::pi * static_cast<double>(i * j) / static_cast<double>(n * m));
               tmp = std::exp(tmp * tmp);

               err = sN_test->v[i][j] - tmp;
               ui->tableWidget->setItem(i, j, new QTableWidgetItem(QString::number(tmp)));
               ui->tableWidget_2->setItem(i, j, new QTableWidgetItem(QString::number(sN_test->v[i][j])));
               ui->tableWidget_3->setItem(i, j, new QTableWidgetItem(QString::number(err)));

               newRow[0] << QSurfaceDataItem(x, y, tmp);
               newRow[1] << QSurfaceDataItem(x, y, 0.);
               newRow[2] << QSurfaceDataItem(x, y, sN_test->v[i][j]);
               newRow[3] << QSurfaceDataItem(x, y, err);

               if (epsMax < abs(err))
               {
                   epsMax = abs(err);

                   xMax = x;
                   yMax = y;
               }
           }

           for (int i = 0; i < 4; i++)
           {
               array[i] << newRow[i];
           }
       }
   }

   ui->edit_n->setText(QString::number(n));
   ui->edit_m->setText(QString::number(n));

   ui->edit_epsmet->setText(QString::number(Eps));
   ui->edit_Nmax->setText(QString::number(NMax));

   ui->edit_N->setText(QString::number(count1));
   ui->edit_Eps1->setText(QString::number(epsMax));

   ui->edit_MaxX->setText(QString::number(xMax));
   ui->edit_MaxY->setText(QString::number(yMax));
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "oscillator.h"
#include <insert.h>
#include <QMessageBox>
#include <QFileDialog>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Отключение вопросительных знаков во всём приложении
    QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);
    this->setFixedSize(this->width(),this->height());
    QLabel *lbl;
    lbl=new QLabel(this);
    oscadd conn;
    if(!conn.connOpen())
        lbl->setText("Fail to open");
    QSqlQueryModel * modal=new QSqlQueryModel();

    conn.connOpen();
    QSqlQuery* qry=new QSqlQuery(conn.mydb);
    qry->prepare("SELECT model FROM osc");
    qry->exec();
    modal->setQuery(*qry);
    ui->listView->setModel(modal);
    conn.connClose();
    qDebug() <<(modal->rowCount());
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_pushButton_7_clicked()
{
    oscadd add;
    add.setModal(true);
    add.exec();
    this->close(); // 1
}

void MainWindow::on_pushButton_8_clicked()
{
    Insert ins;
    ins.setModal(true);
    ins.exec();
    this->close(); // 1.1
}

void MainWindow::on_pushButton_9_clicked()
{
    oscadd conn;
    // записываем в виде строки выделенный текст
    QModelIndex index = ui->listView->currentIndex();
    QString itemText = index.data(Qt::DisplayRole).toString();
    conn.connOpen();
    QSqlQuery qry;
    qry.prepare("DELETE FROM osc WHERE model='"+itemText+"'");
    if (itemText!=0){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Удалить", "Вы уверены что хотите удалить профиль "+itemText+"?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            if (qry.exec())
            {
                QMessageBox::information(this,tr("Удалено"),tr("Удалено"));
                conn.connOpen();
                QSqlQueryModel * modal=new QSqlQueryModel();
                QSqlQuery* qry=new QSqlQuery(conn.mydb);
                qry->prepare("SELECT model FROM osc");
                qry->exec();
                modal->setQuery(*qry);
                ui->listView->setModel(modal);
                conn.connClose();
                qDebug() <<(modal->rowCount());
            }else{
                //
            }
        } else{
            //
        }
    }else{
        QMessageBox::information(this,tr("Удалить"),tr("Вы не выбрали профиль"));
    }
}

void MainWindow::on_pushButton_10_clicked()
{
    {
        QModelIndex index = ui->listView->currentIndex();
        QString itemText = index.data(Qt::DisplayRole).toString();
        QString xgens,ygens;
        xgens=ui->xgen_3->text();
        ygens=ui->ygen_3->text();
        int xgen = xgens.toInt();
        int ygen = ygens.toInt();
        QString format = "ddMMyy"; // часы hh минуты mm
        QString time = QDateTime::currentDateTime().toString(format);
        QString sumText = itemText + "_" + "X" + xgens + "Y" + ygens + "_" + time;
        int sumgen = xgen + ygen;
        if (sumgen !=0 and itemText!=0){
            QString fileName = QFileDialog::getSaveFileName(this, tr(""), sumText,
            tr("G-code (*.g);;Текстовый (.txt)"));
            if (fileName != "") {
                QFile file(fileName);
                if (!file.open(QFile::WriteOnly | QFile::Text)){
                    //error
                } else{
                    // нужно достать значения из sqlite и записать ниже
                    oscadd conn;
                    conn.connOpen();
                    QSqlQuery qry;
                    qry.exec("SELECT model, xpt, ypt, zpt, xs, ys, zs, xk, yk, lenta, lentasd FROM osc WHERE model='"+itemText+"'");
                    while (qry.next()) {
                        QString model = qry.value(0).toString();
                        double xpt = qry.value(1).toDouble();
                        double ypt = qry.value(2).toDouble();
                        double zpt = qry.value(3).toDouble();
                        double xs = qry.value(4).toDouble();
                        double ys = qry.value(5).toDouble();
                        double zs = qry.value(6).toDouble();
                        double xk = qry.value(7).toDouble();
                        double yk = qry.value(8).toDouble();
                        /*double lenta = qry.value(9).toDouble();
                         * Параметры пока что не используются.
                        double lentasd = qry.value(10).toDouble();*/
                        QTextStream stream(&file);
                        //нагрев 1 обкладки штампа
                        stream << "M109 S170\n";
                        //нагрев 2 обкладки штампа
                        stream << "M190 S170\n";
                        //включение шаговых двигателей
                        stream << "M17\n";
                        //установка метрической системы координат
                        stream << "G21\n";
                        //установка абсолютной системы координат
                        stream << "G90\n";
                        //установка абсолютной системы координат податчика ленты
                        stream << "M82\n";
                        //включение вакуумного пинцета
                        stream << "M107\n";
                        //автопарковка по xy
                        stream << "G28 X0 Y0\n";
                        //автопарковка по z
                        stream << "M28 Z0\n";
                        //звуковой сигнал начала основной программы
                        stream << "M300 S5000 P280\n";
                        double X_Rise=0;
                        double Y_Rise=0;
                        double X_Step=0;
                        //конст высоты перемещения
                        double Z_Track = 100;
                        double Y_Step=0;
                        for (int i=0; i<ygen;i++)
                        {
                            Y_Rise=ypt+Y_Step;
                            X_Step=0;
                            for (int j=0; j<xgen;j++)
                            {
                                X_Rise=xpt+X_Step;
                                stream << "G0 X" <<X_Rise<<" Y"<<Y_Rise<<"\n";
                                stream << "G0 Z" <<zpt<<"\n";
                                stream << "M106\n";
                                stream << "G0 Z" <<Z_Track<<"\n";
                                stream << "G0 X" <<xs<<" Y"<<ys<<"\n";
                                stream << "G0 Z" <<zs<<"\n";
                                stream << "M107\n";
                                stream << "G0 Z" <<Z_Track<<"\n";
                                X_Step=X_Step+xk;
                            }
                            Y_Step=Y_Step+yk;
                        }
                        //выключение нагрева 1 обкладки штампа
                        stream << "M104 S0\n";
                        //выключение нагрева 2 обкладки штампа
                        stream << "M140 S0\n";
                        //отключение шаговых двигателей
                        stream << "M18\n";
                        //автопарковка по z
                        stream << "G28 Z0\n";
                        //автопарковка по xy
                        stream << "G28 X0 Y0\n";
                        //звуковой сигнал окончания основной программы
                        stream << "M300 S5000 P280\n";
                        /*stream << "model, mm: " << model << "\n";
                        stream << "XPT, mm: " << xpt << "\n";
                        stream << "YPT, mm: " << ypt << "\n";
                        stream << "ZPT, mm: " << zpt << "\n";
                        stream << "XS, mm: " << xs << "\n";
                        stream << "YS, mm: " << ys << "\n";
                        stream << "ZS, mm: " << zs << "\n";
                        stream << "XK, mm: " << xk << "\n";
                        stream << "YK, mm: " << yk << "\n";
                        stream << "lenta, mm: " << lenta << "\n";
                        stream << "lentasd, mm: " << lentasd << "\n";
                        stream.setIntegerBase(10);*/
                        stream.flush();
                        file.close();
                        conn.connClose();
                    }
                }
            }
        }else{
            QMessageBox::information(this,tr("G-code"),tr("Не выбран профиль или не указано количество"));
        }
    }
}

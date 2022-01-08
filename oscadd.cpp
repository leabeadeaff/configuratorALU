#include "oscadd.h"
#include "ui_oscadd.h"
#include "QLabel"
#include "QMessageBox"
#include "QSql"
#include <oscillator.h>
#include<QDebug>
#include <mainwindow.h>
#include <QCloseEvent>
#include <QLocale>
oscadd::oscadd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::oscadd)
{
    ui->setupUi(this);
    // Фиксирует размер окна
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    // Разрешим ввод только цифр (+ точки для разделения)"
    QLocale locale(QLocale::English);
    QDoubleValidator *ipValidator = new QDoubleValidator(0,100,2,this);
    ipValidator->setLocale(locale);
    ui->linexpt->setValidator(ipValidator);
    ui->lineypt->setValidator(ipValidator);
    ui->linezpt->setValidator(ipValidator);
    ui->linexs->setValidator(ipValidator);
    ui->lineys->setValidator(ipValidator);
    ui->linezs->setValidator(ipValidator);
    ui->linexk->setValidator(ipValidator);
    ui->lineyk->setValidator(ipValidator);
    ui->linelenta->setValidator(ipValidator);
    ui->linelentasd->setValidator(ipValidator);
    QLabel *lbl;
    lbl=new QLabel(this);
    if(!connOpen())
        lbl->setText("Fail to open");
    // создание таблицы
    QString query = "CREATE TABLE osc ("
                    "model VARCHAR(20) UNIQUE,"
                    "xpt double,"
                    "ypt double,"
                    "zpt double,"
                    "xs double,"
                    "ys double,"
                    "zs double,"
                    "xk double,"
                    "yk double,"
                    "lenta double,"
                    "lentasd double);";
    QSqlQuery qry;
    if(!qry.exec(query))
    {
        qDebug()<<"Таблица уже создана";
    }
}

oscadd::~oscadd()
{
    delete ui;
}

//append to existing QString List
void oscadd::on_pushButton_clicked()
{
    //Все веденные значения в окне записываются
    QString model,xpt,ypt,zpt,xs,ys,zs,xk,yk,lenta,lentasd;
    model=ui->linemodel->text();
    xpt=ui->linexpt->text();
    ypt=ui->lineypt->text();
    zpt=ui->linezpt->text();
    xs=ui->linexs->text();
    ys=ui->lineys->text();
    zs=ui->linezs->text();
    xk=ui->linexk->text();
    yk=ui->lineyk->text();
    lenta=ui->linelenta->text();
    lentasd=ui->linelentasd->text();


    if(!connOpen()){

        qDebug()<<"Failed to open the database";
        return;
    }
    if (model == "") {
        connClose();
    } else{
        connOpen();
    }
    QSqlQuery qry;
    if (qry.exec("INSERT INTO osc (model, xpt, ypt, zpt, xs, ys, zs, xk, yk, lenta, lentasd) VALUES ('" + model + "','" + xpt + "','" + ypt + "','" + zpt + "','" + xs + "','" + ys + "','" + zs + "','" + xk + "','" + yk + "','" + lenta + "','" + lentasd + "')"))
    {
        QMessageBox::information(this,tr("Сохранено"),tr("Сохранено"));
        MainWindow *w = new MainWindow();
        w->setAttribute(Qt::WA_DeleteOnClose);
        w->show(); // 2
        this->accept();
    }
    else
    {
        QMessageBox::critical(this,tr("Не сохранено"),tr("Наименование уже занято либо не указано"));
    }
    connClose();
}

void QDialog::reject(){
    MainWindow *w = new MainWindow();
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
    this->accept();
}

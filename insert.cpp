#include "insert.h"
#include "ui_insert.h"
#include "oscadd.h"
#include "QMessageBox"
#include "oscillator.h"
#include "mainwindow.h"

Insert::Insert(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Insert),
    _dialog(nullptr)
{
    ui->setupUi(this);
    // Фиксирует размер окна
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    // Разрешим ввод только цифр (+ точки для разделения)"
    QLocale locale(QLocale::English);
    QDoubleValidator *ipValidator = new QDoubleValidator(0,100,2,this);
    ipValidator->setLocale(locale);
    ui->linexptt->setValidator(ipValidator);
    ui->lineyptt->setValidator(ipValidator);
    ui->linezptt->setValidator(ipValidator);
    ui->linexss->setValidator(ipValidator);
    ui->lineyss->setValidator(ipValidator);
    ui->linezss->setValidator(ipValidator);
    ui->linexkk->setValidator(ipValidator);
    ui->lineykk->setValidator(ipValidator);
    ui->linelentaa->setValidator(ipValidator);
    ui->linelentasdd->setValidator(ipValidator);
    oscadd conn;
    QSqlQueryModel * modal=new QSqlQueryModel();
    conn.connOpen();
    QSqlQuery* qry=new QSqlQuery(conn.mydb);
    qry->prepare("SELECT model FROM osc"); // WHERE model='"+global+"'
    qry->exec();
    modal->setQuery(*qry);
    ui->comboBox->setModel(modal);
    conn.connClose();
    qDebug() <<(modal->rowCount());
}
Insert::~Insert()
{
    delete ui;
}

void Insert::on_comboBox_currentTextChanged(const QString &arg1)
{
    QString itemText = ui->comboBox->currentText();
    oscadd conn;
    conn.connOpen();
    QSqlQuery qry;
    qry.prepare("SELECT model, xpt, ypt, zpt, xs, ys, zs, xk, yk, lenta, lentasd FROM osc WHERE model='"+itemText+"'");
    qry.exec();
    while(qry.next()) {
        QString xpt = qry.value(1).toString();
        QString ypt = qry.value(2).toString();
        QString zpt = qry.value(3).toString();
        QString xs = qry.value(4).toString();
        QString ys = qry.value(5).toString();
        QString zs = qry.value(6).toString();
        QString xk = qry.value(7).toString();
        QString yk = qry.value(8).toString();
        QString lenta = qry.value(9).toString();
        QString lentasd = qry.value(10).toString();
        ui->linexptt->setText(xpt);
        ui->lineyptt->setText(ypt);
        ui->linezptt->setText(zpt);
        ui->linexss->setText(xs);
        ui->lineyss->setText(ys);
        ui->linezss->setText(zs);
        ui->linexkk->setText(xk);
        ui->lineykk->setText(yk);
        ui->linelentaa->setText(lenta);
        ui->linelentasdd->setText(lentasd);
    }
}

void Insert::on_pushButton_clicked()
{
    oscadd conn;
    QString model,xpt,ypt,zpt,xs,ys,zs,xk,yk,lenta,lentasd;
    model= ui->comboBox->currentText();
    xpt=ui->linexptt->text();
    ypt=ui->lineyptt->text();
    zpt=ui->linezptt->text();
    xs=ui->linexss->text();
    ys=ui->lineyss->text();
    zs=ui->linezss->text();
    xk=ui->linexkk->text();
    yk=ui->lineykk->text();
    lenta=ui->linelentaa->text();
    lentasd=ui->linelentasdd->text();


    if(!conn.connOpen()){

        qDebug()<<"Failed to open the database";
        return;
    }
    conn.connOpen();
    QSqlQuery qry;
    if (qry.exec("UPDATE osc SET xpt = '" + xpt + "', ypt = '" + ypt + "', zpt = '" + zpt + "', xs = '" + xs + "', ys = '" + ys + "', zs = '" + zs + "', xk = '" + xk + "', yk = '" + yk + "', lenta = '" + lenta + "', lentasd = '" + lentasd + "' WHERE model='"+model+"'"))
    {
        QMessageBox::information(this,tr("Изменено"),tr("Изменено"));
    }
    else
    {
        QMessageBox::critical(this,tr("Не Изменено"),qry.lastError().text());
    }
    this->close();
    conn.connClose();

}

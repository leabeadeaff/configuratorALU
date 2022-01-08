#ifndef OSCADD_H
#define OSCADD_H

#include <QDialog>
#include <QtSql>
#include <QDebug>
#include <QFileInfo>
#include <QLabel>

namespace Ui {
class oscadd;
}
class oscadd : public QDialog
{
    Q_OBJECT
public:
    QSqlDatabase mydb;
    void connClose()
    {
        mydb.close();
        mydb.removeDatabase(QSqlDatabase::defaultConnection);
    }
    bool connOpen()
    {
        mydb=QSqlDatabase::addDatabase("QSQLITE");
        mydb.setDatabaseName("osc.db");

        if(!mydb.open()){
            qDebug()<<("Failed to open the database");
            return false;
        }
        else{
            qDebug()<<("Connected...");
            return true;
        }
    }
public:
    explicit oscadd(QWidget *parent = nullptr);
    ~oscadd();

private slots:
    void on_pushButton_clicked();

private:
    Ui::oscadd *ui;
};

#endif // OSCADD_H

#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <oscadd.h>

namespace Ui {
class oscillator;
}
class oscillator : public QDialog
{
    Q_OBJECT

public:
    QSqlDatabase db;
    explicit oscillator(QWidget *parent = nullptr);
    ~oscillator();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_listView_doubleClicked(const QModelIndex &index);

    void on_pushButton_2_clicked();

private:
    Ui::oscillator *ui;
};

#endif // OSCILLATOR_H

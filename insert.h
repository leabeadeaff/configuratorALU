#ifndef INSERT_H
#define INSERT_H

#include <QDialog>
#include "mainwindow.h"
namespace Ui {
class Insert;
}
class Insert : public QDialog
{
    Q_OBJECT

public:
    explicit Insert(QWidget *parent = nullptr);
    ~Insert();
private slots:
    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_pushButton_clicked();

private:
    Ui::Insert *ui;
    MainWindow* _dialog;
};

#endif // INSERT_H

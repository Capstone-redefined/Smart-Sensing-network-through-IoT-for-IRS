#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void updateTemperature(QString);
    void updatePersons(QString);
    void updateHours(QString);





private:
    Ui::Dialog *ui;
};
#endif // DIALOG_H
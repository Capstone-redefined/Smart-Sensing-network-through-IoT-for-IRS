#include "dialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.setFixedSize(443,322);
    w.setWindowTitle("Capstone Display");
    w.show();
    return a.exec();
}

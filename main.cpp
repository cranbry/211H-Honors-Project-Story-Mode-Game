#include "mainwindow.h"
#include <QApplication>
#include "quiz.h"

// int main(int argc, char *argv[])
// {
//     QApplication a(argc, argv);
//     Quiz quiz;
//     quiz.show();
//     return a.exec();
// }

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

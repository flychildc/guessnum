#include "guessnum.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    guessnum w;
    w.show();

    return a.exec();
}

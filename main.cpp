#include <QApplication>
#include "Sapper.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Sapper sapper;
    sapper.show();

    return app.exec();
}

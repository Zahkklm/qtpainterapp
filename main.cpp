// ---------- main.cpp ----------

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ArtWindow window;
    window.show();

    return app.exec();
}

// ---------- END main.cpp ----------





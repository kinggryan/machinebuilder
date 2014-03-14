#define dDOUBLE // for ode

#include "mainwindow.h"
#include <QApplication>
#include "glheader.h"
#include <ode/ode.h>
#include "gldisplay.h"
#include <QGLFormat>

int main(int argc, char *argv[])
{
    dInitODE();

    QGLFormat glFormat;
    glFormat.setVersion( 3, 2 );
    glFormat.setProfile( QGLFormat::CoreProfile ); // Requires >=Qt-4.8.0
    glFormat.setSampleBuffers( true );

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

#define dDOUBLE // for ode

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QBoxLayout>
#include "gldisplay.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
#if 0
    QGLFormat fmt;
    fmt.setProfile(QGLFormat::CoreProfile);
    fmt.setVersion(3,2);
    QGLFormat::setDefaultFormat(fmt);
#endif

    QWidget *central = new QWidget;
    setCentralWidget(central);
    central->setMinimumHeight(500);
    central->setMinimumWidth(500);
    QHBoxLayout *mainLayout = new QHBoxLayout();
    central->setLayout(mainLayout);
    mGLDisplay = new gldisplay(this);
    mGLDisplay->setFixedSize(500, 500);
    mainLayout->addWidget(mGLDisplay, 1);

    QString buttonText("Push This");
    pushy = new QPushButton(buttonText, this);
    mainLayout->addWidget(pushy,1);
}

MainWindow::~MainWindow()
{
    delete mGLDisplay;
}

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
    QVBoxLayout *buttonLayout = new QVBoxLayout();
    mainLayout->addLayout(buttonLayout);

    QString buttonText("Add Wheel");
    pushy = new QPushButton(buttonText, this);
    buttonLayout->addWidget(pushy,1);
    connect(pushy, SIGNAL(clicked()), this, SLOT(addWheelPieceButtonClicked()));

    QString buttonText2("Turn On Gravity");
    gravityButton = new QPushButton(buttonText2, this);
    buttonLayout->addWidget(gravityButton,1);
    connect(gravityButton, SIGNAL(clicked()), mGLDisplay, SLOT(activateGravity()));

    QString buttonText3("Activate Piece");
    activateButton = new QPushButton(buttonText3, this);
    buttonLayout->addWidget(activateButton,1);
    connect(activateButton, SIGNAL(clicked()), mGLDisplay, SLOT(activatePieceMode()));

    QString buttonText4("Change Activation Direction");
    changeActivationDirectionButton = new QPushButton(buttonText4, this);
    buttonLayout->addWidget(changeActivationDirectionButton,1);
    connect(changeActivationDirectionButton, SIGNAL(clicked()), mGLDisplay, SLOT(changeActivationDirectionMode()));
}

MainWindow::~MainWindow()
{
    delete mGLDisplay;
}

void MainWindow::addWheelPieceButtonClicked()
{
    mGLDisplay->setAddPieceMode(wheelPiece);
}

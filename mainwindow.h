#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gldisplay.h"
#include <QPushButton>
#include "MachinePieces/piecetypes.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    gldisplay* mGLDisplay;
    QPushButton* pushy;
    QPushButton* gravityButton;
    QPushButton* activateButton;
    QPushButton* changeActivationDirectionButton;

public slots:
    void addWheelPieceButtonClicked();
};

#endif // MAINWINDOW_H

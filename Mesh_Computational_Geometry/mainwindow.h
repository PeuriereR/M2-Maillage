#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void tetraedra();
    void pyramid();
    void offFile();
    void switchDisplay();
    void launchCirculator();

    void box2D();
    void switchSplit();
    void addConvexHull();
    void loadBox2File();
    void delauniser();
    void switchIncrLawson();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

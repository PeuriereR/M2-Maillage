#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->tetra,SIGNAL(released()),this,SLOT(tetraedra()));
    connect(ui->pyra,SIGNAL(released()),this,SLOT(pyramid()));
    connect(ui->offfile,SIGNAL(released()),this,SLOT(offFile()));
    connect(ui->wire,SIGNAL(released()),this,SLOT(switchDisplay()));
    connect(ui->circu,SIGNAL(released()),this,SLOT(launchCirculator()));
    connect(ui->delaunay,SIGNAL(released()),this,SLOT(delauniser()));
    connect(ui->box2,SIGNAL(released()),this,SLOT(box2D()));
    connect(ui->convexhull,SIGNAL(released()),this,SLOT(addConvexHull()));
    connect(ui->loadxyfile,SIGNAL(released()),this,SLOT(loadBox2File()));
    connect(ui->split,SIGNAL(released()),this,SLOT(switchSplit()));
    connect(ui->incrlawson,SIGNAL(released()),this,SLOT(switchIncrLawson()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::tetraedra(){
    ui->widget->draw(TETRAEDRA);
}

void MainWindow::pyramid(){
    ui->widget->draw(PYRAMID);
}

void MainWindow::offFile(){
    ui->widget->draw(QUEEN);
}

void MainWindow::switchDisplay(){
    ui->widget->switchDisplay();
}

void MainWindow::launchCirculator(){
    ui->widget->launchCirculator();
}

void MainWindow::delauniser(){
    ui->widget->delauniser();
}

void MainWindow::box2D(){
    ui->widget->draw(BOX2D);
}

void MainWindow::switchSplit(){
    ui->widget->switchSplit();
}

void MainWindow::addConvexHull(){
    ui->widget->addConvexHull();
}

void MainWindow::loadBox2File(){
    ui->widget->loadBox2File();
}

void MainWindow::switchIncrLawson(){
    ui->widget->switchIncrLawson();
}

#include "mainwindowcontrols.h"
#include "./ui_mainwindowcontrols.h"

#include "simulation.h"

MainWindowControls::MainWindowControls(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowControls)
{
    ui->setupUi(this);
    setWindowTitle("Simulation");
}

MainWindowControls::~MainWindowControls()
{
    delete ui;
}


void MainWindowControls::on_pushButtonStart_clicked()
{
    runSimulation();
}


void MainWindowControls::on_pushButtonStop_clicked()
{

}


void MainWindowControls::on_pustButtonMapBrowse_clicked()
{

}


void MainWindowControls::on_pushButtonInstanceBrowse_clicked()
{

}


void MainWindowControls::on_pushButtonOutputBrowse_clicked()
{

}


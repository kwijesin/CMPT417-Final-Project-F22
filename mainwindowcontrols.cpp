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

    const QString MAP_NAME = "";
    const QString INSTANCE_NAME = "instancename.txt";
    const QString OUTPUT_NAME = "output.csv";
    const int POP_COUNT = 100;
    const int NUM_GENERATIONS = 50;
    const int TEST_SIZE = 50;
    const float MUTATION_FACTOR = 1.0;
    const int CULL_RATE = 4;	//	only the top (POP/RATE) agents are kept each generation

    int ret = runSimulation(MAP_NAME, INSTANCE_NAME, OUTPUT_NAME, POP_COUNT, NUM_GENERATIONS, TEST_SIZE, CULL_RATE, MUTATION_FACTOR);
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


#include "mainwindowcontrols.h"
#include "./ui_mainwindowcontrols.h"

#include <QFileDialog>

#include "simulation.h"

#define MAP_IND 0
#define INSTANCE_IND 1
#define OUTPUT_IND 2
#define POP_IND 3
#define GENERATIONS_IND 4
#define TESTSIZE_IND 5
#define CULL_IND 6
#define MUTATION_IND 7

MainWindowControls::MainWindowControls(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowControls)
{
    ui->setupUi(this);
    setWindowTitle("Simulation");

    ui->progressBar->hide();
}

MainWindowControls::~MainWindowControls()
{
    delete ui;
}


void MainWindowControls::onSimulationComplete()
{
    ui->labelCurrentCommand->clear();
    on_pushButtonStart_clicked();
}


void MainWindowControls::onSimulationReport(int generation)
{
    ui->progressBar->setValue(generation);
}


void MainWindowControls::on_pushButtonStart_clicked()
{
    if(ui->listWidgetCommandList->count() <= 0)
        return;

    //Runs the first command in it's own thread
    mainSimulationThread = new SimulationThread();
    QString command = ui->listWidgetCommandList->takeItem(0)->text();
    ui->labelCurrentCommand->setText(command);
    QStringList argList = command.split(" ");
    mainSimulationThread->mapName = argList.at(MAP_IND);
    mainSimulationThread->instanceName = argList.at(INSTANCE_IND);
    mainSimulationThread->outputName = argList.at(OUTPUT_IND);
    mainSimulationThread->pop = argList.at(POP_IND).toInt();
    mainSimulationThread->generations = argList.at(GENERATIONS_IND).toInt();
    mainSimulationThread->testCount = argList.at(TESTSIZE_IND).toInt();
    mainSimulationThread->cullRate = argList.at(CULL_IND).toInt();
    mainSimulationThread->mutation = argList.at(MUTATION_IND).toFloat();

    //set up UI connections and start thread
    ui->progressBar->show();
    ui->progressBar->setMaximum(argList.at(GENERATIONS_IND).toInt());
    ui->progressBar->setValue(0);
    QObject::connect(mainSimulationThread, SIGNAL(reportProgress(int)), this, SLOT(onSimulationReport(int)));
    QObject::connect(mainSimulationThread, SIGNAL(finished()), this, SLOT(onSimulationComplete()));
    QObject::connect(mainSimulationThread, SIGNAL(finished()), mainSimulationThread, SLOT(deleteLater()));
    mainSimulationThread->start();
}


void MainWindowControls::on_pushButtonStop_clicked()
{
    if(mainSimulationThread != NULL && mainSimulationThread->isRunning()){
        QObject::disconnect(mainSimulationThread, SIGNAL(finished()), this, SLOT(onSimulationComplete()));
        mainSimulationThread->quit();
        ui->progressBar->hide();
        ui->labelCurrentCommand->clear();
    }
}


void MainWindowControls::on_pushButtonBrowseCommand_clicked()
{
    //browse for a file
    //place each line in the file on a seperate item in the command list
    QString filename = QFileDialog::getOpenFileName(this, "Open Command file", "", "*.txt");
    QFile file(filename);
    QString line;

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    while(!file.atEnd()){
        line = file.readLine();
        if(line.endsWith("\n"))
            line.chop(1);
        ui->listWidgetCommandList->addItem(line);
    }

    file.close();
}


void MainWindowControls::on_pushButtonReset_clicked()
{
    ui->listWidgetCommandList->clear();
}


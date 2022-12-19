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
#define K_IND 7
#define MUTATION_IND 8

MainWindowControls::MainWindowControls(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowControls)
{
    ui->setupUi(this);
    setWindowTitle("Simulation");

    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(0);
    ui->progressBarTest->setMaximum(100);
    ui->progressBarTest->setValue(0);
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
    currentGen = generation;
    ui->progressBar->setValue(currentGen);
    ui->progressBarTest->setValue(0);
    ui->progressBar->setFormat("Generation " + QString::number(currentGen) + "/" + QString::number(genMax) + " (%p%)");
    ui->labelMap->setHeuristic(currentMap, mainSimulationThread->population.getBest());
}

void MainWindowControls::onSimulationReportTest(int test)
{
    currentTest = test++;
    ui->progressBarTest->setValue(currentTest);
    ui->progressBarTest->setFormat("Test " + QString::number(currentTest) + "/" + QString::number(testMax) + " (%p%)");
}


void MainWindowControls::on_pushButtonStart_clicked()
{
    if(ui->listWidgetCommandList->count() <= 0)
        return;

    QString command = ui->listWidgetCommandList->takeItem(0)->text();
    QStringList argList = command.split(" ");
    QString currentPath = QCoreApplication::applicationDirPath() + "/";
    QString instanceName = argList.at(INSTANCE_IND);

    if(!QFile::exists(currentPath + instanceName) && instanceName != "random")
        return;
    if(!QFile::exists(currentPath + argList.at(MAP_IND)))
        return;

    mainSimulationThread = new SimulationThread();
    mainSimulationThread->mapName = currentPath + argList.at(MAP_IND);
    if(instanceName == "random")
        mainSimulationThread->instanceName = instanceName;
    else
        mainSimulationThread->instanceName = currentPath + instanceName;
    mainSimulationThread->outputName = currentPath + argList.at(OUTPUT_IND);
    mainSimulationThread->pop = argList.at(POP_IND).toInt();
    mainSimulationThread->generations = argList.at(GENERATIONS_IND).toInt();
    mainSimulationThread->testCount = argList.at(TESTSIZE_IND).toInt();
    mainSimulationThread->cullRate = argList.at(CULL_IND).toInt();
    mainSimulationThread->k = argList.at(K_IND).toInt();
    mainSimulationThread->mutation = argList.at(MUTATION_IND).toFloat();

    //set up UI connections and start thread
    ui->labelCurrentCommand->setText(command);
    genMax = argList.at(GENERATIONS_IND).toInt();
    testMax = argList.at(POP_IND).toInt();
    ui->progressBar->setMaximum(genMax);
    ui->progressBar->setValue(0);
    ui->progressBarTest->setMaximum(testMax);
    ui->progressBarTest->setValue(0);
    currentMapFilename = currentPath + argList.at(MAP_IND);
    ui->labelMap->setMap(currentMapFilename);
    currentMap = Map(currentMapFilename);
    QObject::connect(mainSimulationThread, SIGNAL(reportProgress(int)), this, SLOT(onSimulationReport(int)));
    QObject::connect(mainSimulationThread, SIGNAL(reportProgressTest(int)), this, SLOT(onSimulationReportTest(int)));
    QObject::connect(mainSimulationThread, SIGNAL(finished()), this, SLOT(onSimulationComplete()));
    QObject::connect(mainSimulationThread, SIGNAL(finished()), mainSimulationThread, SLOT(deleteLater()));
    mainSimulationThread->start();
}


void MainWindowControls::on_pushButtonStop_clicked()
{
    if(mainSimulationThread != NULL && mainSimulationThread->isRunning()){
        QObject::disconnect(mainSimulationThread, SIGNAL(finished()), this, SLOT(onSimulationComplete()));
        mainSimulationThread->quit();
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

        QStringList argList = line.split(" ");
        QString currentPath = QCoreApplication::applicationDirPath() + "/";
        bool mapExists = QFile::exists(currentPath+argList.at(MAP_IND));
        bool instanceExists = QFile::exists(currentPath+argList.at(INSTANCE_IND)) || argList.at(INSTANCE_IND) == "random";

        QListWidgetItem* newItem = new QListWidgetItem(line);
        if(!mapExists || !instanceExists)
            newItem->setForeground(QBrush(Qt::red));
        ui->listWidgetCommandList->addItem(newItem);
    }

    if(ui->listWidgetCommandList->count() > 0){
        QString command = ui->listWidgetCommandList->item(0)->text();
        QStringList argList = command.split(" ");
        QString currentPath = QCoreApplication::applicationDirPath() + "/";
        ui->labelMap->setMap(currentPath + argList.at(MAP_IND));
    }

    file.close();
}


void MainWindowControls::on_pushButtonReset_clicked()
{
    ui->listWidgetCommandList->clear();
}


void MainWindowControls::on_pushButtonAlgoHeuristic_clicked()
{
    AlgorithmicCanonicalTDH TDH;
    TDH.setK(ui->spinBoxK->value());
    TDH.calculateTDH(currentMap);
    ui->labelMap->setHeuristic(currentMap, TDH);
}


void MainWindowControls::on_pushButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open map", "", "*.map");
    if(!QFile::exists(filename))
        return;

    currentMapFilename = filename;
    ui->pushButton->setText("Current Map:" + filename);
    currentMap = Map(currentMapFilename);
    ui->labelMap->setMap(currentMap);
}


void MainWindowControls::on_pushButtonRandHeuristic_clicked()
{
    CanonicalTDH TDH;
    TDH.setK(ui->spinBoxK->value());
    TDH.randomizeNodes(currentMap);
    TDH.calculateTDH(currentMap);
    ui->labelMap->setHeuristic(currentMap, TDH);
}


#ifndef MAINWINDOWCONTROLS_H
#define MAINWINDOWCONTROLS_H

#include <QMainWindow>
#include "simulation.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindowControls; }
QT_END_NAMESPACE

class MainWindowControls : public QMainWindow
{
    Q_OBJECT

public:
    MainWindowControls(QWidget *parent = nullptr);
    ~MainWindowControls();

private slots:
    void onSimulationComplete();
    void onSimulationReport(int generation);

    void on_pushButtonStart_clicked();

    void on_pushButtonStop_clicked();

    void on_pushButtonBrowseCommand_clicked();

    void on_pushButtonReset_clicked();

    void on_pushButtonAlgoHeuristic_clicked();

    void on_pushButton_clicked();

    void on_pushButtonRandHeuristic_clicked();

private:
    Ui::MainWindowControls *ui;
    SimulationThread* mainSimulationThread;

    QString currentMapFilename;
    Map currentMap;
};
#endif // MAINWINDOWCONTROLS_H

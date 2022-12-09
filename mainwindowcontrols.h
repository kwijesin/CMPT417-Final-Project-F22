#ifndef MAINWINDOWCONTROLS_H
#define MAINWINDOWCONTROLS_H

#include <QMainWindow>

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
    void on_pushButtonStart_clicked();

    void on_pushButtonStop_clicked();

    void on_pushButtonBrowseCommand_clicked();

    void on_pushButtonReset_clicked();

private:
    Ui::MainWindowControls *ui;
};
#endif // MAINWINDOWCONTROLS_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ReimpQGraphicsScene.h"
#include <QMainWindow>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void SeekForWinningState();

private slots:
    void on_newGameButton_clicked();

private:
    Ui::MainWindow *ui;
    ReimpQGraphicsScene *scene;
};
#endif // MAINWINDOW_H

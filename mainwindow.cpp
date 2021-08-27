#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("TicTacToe");
    //ui->graphicsView->scale(2.6,2.6);
    scene = new ReimpQGraphicsScene();
    ui->graphicsView->setScene(scene);
    scene->DrawGrid();
    scene->SetPen();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_newGameButton_clicked()
{
    scene = new ReimpQGraphicsScene;
    ui->graphicsView->setScene(scene);
    scene->DrawGrid();
    scene->SetPen();
}

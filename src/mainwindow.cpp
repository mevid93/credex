#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set some custom signals and slots
    connect(this->ui->action_Exit, &QAction::triggered, this, &MainWindow::closeApplication);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeApplication()
{
    QCoreApplication* app = QApplication::instance();
    app->quit();
}


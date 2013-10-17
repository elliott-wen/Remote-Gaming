#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->loginpushButton,SIGNAL(clicked()),this,SLOT(onLogin()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::onLogin()
{
    if((ui->usernamelineEdit->text()=="test"&&ui->passwordlineEdit_2->text()=="test")||(ui->usernamelineEdit->text()=="test1"&&ui->passwordlineEdit_2->text()=="test1"))
    {
        CloudPortal *portal=new CloudPortal;
        this->hide();
        portal->show();
    }
    else
    {
        QMessageBox::warning(this,"Error","Bad username password");
    }
}

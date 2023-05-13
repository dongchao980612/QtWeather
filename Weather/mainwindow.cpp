#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setMaximumSize(WIN_WEIGHT,WIN_HEIGHT);

    mExitMenu=new QMenu(this);
    mExitAction=new QAction();

    mExitAction->setText("exit");
    mExitAction->setIcon(QIcon(":res/close/ico"));

    mExitMenu->addAction(mExitAction);

    connect(mExitAction,&QAction::triggered,this,[=]{
        qApp->exit(0);
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

//
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    mExitMenu->exec(QCursor::pos());
}


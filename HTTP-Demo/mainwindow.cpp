#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QUrl url = QUrl("http://t.weather.itboy.net/api/weather/city/101010100");
    mNetworkAccessManager = new QNetworkAccessManager(this);
    mNetworkAccessManager->get(QNetworkRequest(url));

    // QNetworkReply* reply = mNetworkAccessManager->get(QNetworkRequest(url));
    connect(mNetworkAccessManager, &QNetworkAccessManager::finished, this, &MainWindow::onReplied);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onReplied(QNetworkReply *reply)
{
    //状态码
    int status_code=reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug()<<"请求方式:"<<reply->operation()<<endl;
    qDebug()<<"状态码:"<<status_code<<endl;
    qDebug()<<"url:"<<reply->url()<<endl;
    qDebug()<<"请求头:"<<reply->rawHeaderList()<<endl;

    //判断是否请求成功
    if(reply->error()!=QNetworkReply::NoError || status_code != 200)
    {
        QMessageBox::warning(this, "天气", reply->errorString(), QMessageBox::Ok);
        // QMessageBox::warning(this,"提示","请求失败",QMessageBox::Ok);
    }else{
        QByteArray replaydata = reply->readAll();
        QByteArray byteArray=QString(replaydata).toUtf8();
        qDebug()<<"read all:"<<byteArray.data();
    }
    reply->deleteLater();
}


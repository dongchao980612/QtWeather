# QT_weather
基于QT的天气预报软件设计

```tex
http://t.weather.itboy.net/api/weather/city/101010100
```

## Json-Demo

### 用到的库
``` c++
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
```
## Http-Demo

```c++
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
```
> cannot find -lGL 问题

```bash
#查找 libGL 所在位置
locate libGL
/usr/lib/x86_64-linux-gnu/libGL.so.1
/usr/lib/x86_64-linux-gnu/libGL.so.1.0.0
/usr/lib/x86_64-linux-gnu/libGLESv2.so.2
/usr/lib/x86_64-linux-gnu/libGLESv2.so.2.0.0
/usr/lib/x86_64-linux-gnu/libGLU.so.1
/usr/lib/x86_64-linux-gnu/libGLU.so.1.3.1
/usr/lib/x86_64-linux-gnu/libGLX.so.0
/usr/lib/x86_64-linux-gnu/libGLX.so.0.0.0
/usr/lib/x86_64-linux-gnu/libGLX_indirect.so.0
/usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0
/usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0
/usr/lib/x86_64-linux-gnu/libGLdispatch.so.0
/usr/lib/x86_64-linux-gnu/libGLdispatch.so.0.0.0
/usr/share/code/libGLESv2.so
/usr/share/code/swiftshader/libGLESv2.so

#创建链接
ln -s /usr/lib/x86_64-linux-gnu/libGL.so.1 /usr/lib/libGL.so
```

```c++
QNetworkAccessManager* mNetworkAccessManager;
mNetworkAccessManager = new QNetworkAccessManager(this);

QUrl url = QUrl("http://t.weather.itboy.net/api/weather/city/101010100");
mNetworkAccessManager->get(QNetworkRequest(url));


connect(mNetworkAccessManager, &QNetworkAccessManager::finished, this, &MainWindow::onReplied);


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
```
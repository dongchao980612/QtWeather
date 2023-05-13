# QT_weather
基于QT的天气预报软件设计

```tex
http://t.weather.itboy.net/api/weather/city/101010100
```


## 整体效果、技术点
在右上角输入要查询的城市，然后点击查询按钮就会发送 http请求给服务器，请求回来的天气数据为JSON格式，通过解析 JSON可以获取以下信息
- 今天的信息 
  温度、湿度、风向、风力、天气类型（晴、多云、小雨等）、 PM2 .5、温馨提示、感冒指数、日出日落
- 未来15天的信息 
  日期、星期、天气类型（晴、多云、小雨等）、PM25、最高温、最低温
  
### 样式表的设置
合理的使用样式表，可以使界面更加美观，样式表如下
- 背景图片
  为整个窗体设置一张背景图片

- 背景色
  设置空间背景透明，或者设置一个透明度

- 圆角
为控件设置圆角

- 字体颜色和大小
  为控件设置合适的字体颜色和字体大小  

### json数据格式
http服务端返回的天气数据是json格式，使用QT提供的解析json相关的类可以很方便的解析出其中的字段

### http请求
根据http服务端提供的将诶口，发送http请求，获取天气数据

###  事件
为了实现将诶面美观，将窗口设置为无标题，这样就无法通过右上角的【关闭】按钮关闭程序，因此需要添加右键菜单退出功能，还重写了鼠标事件，让窗口可以跟随鼠标移动

###  绘图
绘制高低温曲线图，根据每天高低温数据，绘制一个曲线图，更直观的展示温度的变化

###  资源文件
根据不同的天气类型，可以用不同的图标进行展示，更加直观，而这些图表通常放在资源文件里，这样他们可以一同被打包进QT可执行程序中

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
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDebug>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include "weatherTool.h"
#include <QPainter>
#include <QKeyEvent>

#include <QNetworkInterface>
#include <QHostInfo>

#include <QTimer>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);//设置窗口无边框
    this->setFixedSize(this->width(),this->height());//设置固定窗口大小
//************************
    //构建右键菜单
    m_ExitMenu=new QMenu(this);
    m_ExitAction= new QAction(this);

    //设置菜单项的名字和图标
    m_ExitAction->setText("退出");
    m_ExitAction->setIcon(QIcon(":/res/close.png"));

    m_ExitMenu->addAction(m_ExitAction);//添加到菜单上

    //连接槽函数
    connect(m_ExitAction,&QAction::triggered,this,[=]{
        qApp->exit();

    });

//************************
    mNetworkAccessManager = new QNetworkAccessManager(this);
    connect(mNetworkAccessManager, &QNetworkAccessManager::finished, this, &MainWindow::onReplied);

    //将UI控件放到数组里边,方便使用循环进行处理
    m_DateList << ui->lblDate0 << ui->lblDate1 << ui->lblDate2 << ui->lblDate3 << ui->lblDate4 << ui->lblDate5;
    m_WeekList << ui->lblWeek0 << ui->lblWeek1 << ui->lblWeek2 << ui->lblWeek3 << ui->lblWeek4 << ui->lblWeek5;
    m_QulityList << ui->lblQuality0 << ui->lblQuality1 << ui->lblQuality2 << ui->lblQuality3 << ui->lblQuality4 << ui->lblQuality5;
    m_TypeList << ui->lblType0 << ui->lblType1 << ui->lblType2 << ui->lblType3 << ui->lblType4 << ui->lblType5;
    m_TypeIconList << ui->lblTypeIcon0 << ui->lblTypeIcon1 << ui->lblTypeIcon2 << ui->lblTypeIcon3 << ui->lblTypeIcon4 << ui->lblTypeIcon5;
    m_FxList << ui->lblFx0 << ui->lblFx1 << ui->lblFx2 << ui->lblFx3 << ui->lblFx4 << ui->lblFx5;
    m_FlList << ui->lblFl0 << ui->lblFl1 << ui->lblFl2 << ui->lblFl3 << ui->lblFl4 << ui->lblFl5;

    //根据keys,设置icon的路径
    m_TypeMap.insert("暴雪",":/res/type/BaoXue.png");
    m_TypeMap.insert("暴雨",":/res/type/BaoYu. png");
    m_TypeMap.insert("暴雨到大暴雨",":/res/type/BaoYuDaoDaBaoYu.png");
    m_TypeMap.insert("大暴雨",":/res/type/DaBaoYu.png");
    m_TypeMap.insert("大暴雨到特大暴雨",":/res/type/DaBaoYuDaoTeDaBaoYu.png");
    m_TypeMap.insert("大到暴雪",":/res/type/DaDaoBaoXue.png");
    m_TypeMap.insert("大雪",":/res/type/DaXue.png");
    m_TypeMap.insert("大雨",":/res/type/DaYu.png");
    m_TypeMap.insert("冻雨",":/res/type/DongYu.png");
    m_TypeMap.insert("多云",":/res/type/DuoYun.png");
    m_TypeMap.insert("浮沉",":/res/type/FuChen.png");
    m_TypeMap.insert("雷阵雨",":/res/type/LeiZhenYu.png");
    m_TypeMap.insert("雷阵雨伴有冰雹",":/res/type/LeiZhenYuBanYouBingBao.png");
    m_TypeMap.insert("霾",":/res/type/Mai.png");
    m_TypeMap.insert("强沙尘暴",":/res/type/QiangShaChenBao.png");
    m_TypeMap.insert("晴",":/res/type/Qing.png");
    m_TypeMap.insert("沙尘暴",":/res/type/ShaChenBao.png");
    m_TypeMap.insert("特大暴雨",":/res/type/TeDaBaoYu.png");
    m_TypeMap.insert("undefined",":/res/type/undefined.png");
    m_TypeMap.insert("雾",":/res/type/Wu.png");
    m_TypeMap.insert("小到中雪",":/res/type/XiaoDaoZhongXue.png");
    m_TypeMap.insert("小到中雨",":/res/type/XiaoDaoZhongYu.png");
    m_TypeMap.insert("小雪",":/res/type/XiaoXue.png");
    m_TypeMap.insert("小雨",":/res/type/XiaoYu.png");
    m_TypeMap.insert("雪",":/res/type/Xue.png");
    m_TypeMap.insert("扬沙",":/res/type/YangSha.png");
    m_TypeMap.insert("阴",":/res/type/Yin.png");
    m_TypeMap.insert("雨",":/res/type/Yu.png");
    m_TypeMap.insert("雨夹雪",":/res/type/YuJiaXue.png");
    m_TypeMap.insert("阵雪",":/res/type/ZhenXue.png");
    m_TypeMap.insert("阵雨",":/res/type/ZhenYu.png");
    m_TypeMap.insert("中到大雪",":/res/type/ZhongDaoDaXue.png");
    m_TypeMap.insert("中到大雨",":/res/type/ZhongDaoDaYu.png");
    m_TypeMap.insert("中雪",":/res/type/ZhongXue.png");
    m_TypeMap.insert("中雨",":/res/type/ZhongYu.png");

    // http://whois.pconline.com.cn/ipJson.jsp?ip=58.57.27.252&json=true
    // getlocal();
    getWeatherInfo(getlocal()); //请求数据
    connect(ui->btnSearch, &QPushButton::clicked, this, &MainWindow::onBtnSerchClicked);

    //安装事件过滤器,用于拦截框架发送的事件QEvent::Point事件(标签调用update函数后会自动发送的),拦截后就可以区分是哪一个,进行绘制曲线
    ui->lblHighCurve->installEventFilter(this);
    ui->lblLowCurve->installEventFilter(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

//重写父类的虚函数,因为父类的实现默认忽略点击事件
void MainWindow::contextMenuEvent(QContextMenuEvent *e)
{
    // 需要Menu和Action
    // 弹出右键菜单
    m_ExitMenu->exec(QCursor::pos());

    // 事件已经处理,不需要向上传
    // e->accept();

}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    m_Offest=event->globalPos() - this->pos();
    // this->pos()左上角的位置
    // globalPos 鼠标点击的位置相对于左上角的位置
    // qDebug() << event->globalPos() << this->pos()<<m_Offest;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    // windows平台
    this->move(event->globalPos() - m_Offest);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->lblHighCurve && event->type() == QEvent::Paint) {
        //是否是lblHighCurve且事件的类型为Paint
        paintHigtCurve();
    }
    if (watched == ui->lblLowCurve && event->type() == QEvent::Paint) {
        //是否是lblLowCurve且事件的类型为Paint
        paintLowCurve();
    }
    //执行完我们的操作后,继续父类的操作
    return QWidget::eventFilter(watched, event);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return )   {
        // qDebug()<<"按下了回车键";
        // 是否按下enter键
        QString cityName = ui->leCity->text();
        // qDebug() << cityName;
        getWeatherInfo(cityName);
    }
    else QWidget::keyPressEvent(event);

}

void MainWindow::paintHigtCurve()
{
    //qDebug() << "paintHigtCurve";
    //设置painter
    QPainter painter(ui->lblHighCurve);
    //设置抗锯齿
    painter.setRenderHint(QPainter::Antialiasing, true);
    //构造画笔
    QPen pen = painter.pen();
    //设置画笔的宽度
    pen.setWidth(1);
    //设置画笔的颜色
    pen.setColor(QColor(255, 170, 0));
    //设置画笔
    painter.setPen(pen);
    //设置画刷
    painter.setBrush(QColor(255, 170, 0));
    //保存
    painter.save();


    //计算x y的坐标
    //x的坐标: 上面标签的x+标签的宽度/2
    //y的坐标: 先计算全部数据的平均值,将平均值设置在当前label高度/2,计算公式为(温度 - 平均值) * 要移动的像素点
    QPoint pos[6] = {};
    //高温和
    int tempSum = 0;
    //平均高温
    int tempAverage = 0;
    //中心点
    float center = ui->lblHighCurve->height() / 2;
    for (int i = 0; i < 6; ++i) {
        //计算高温和
        tempSum += m_Day[i].high;
    }
    //计算平均值
    tempAverage = tempSum / 6;
    for (int i = 0; i < 6; ++i) {
        //计算往上移动还是往下移动
        int offset = (m_Day[i].high - tempAverage) * INCREMENT;
        //x
        pos[i].setX(m_WeekList[i]->pos().x() + m_WeekList[i]->width() / 2);
        //y
        pos[i].setY(center - offset);
        //qDebug() << i << QPoint(pos[i].x() - TEXT_OFFSET_X, pos[i].y() - TEXT_OFFSET_Y);
        //绘制点
        painter.drawEllipse(pos[i], PAINT_RADIUS, PAINT_RADIUS);
        //绘制文字
        painter.drawText(QPoint(pos[i].x() - TEXT_OFFSET_X, pos[i].y() - TEXT_OFFSET_Y), QString::number(m_Day[i].high) + "°");
    }
    //只需要5段
    for (int i = 0; i < 5; ++i) {
        //绘制曲线,昨天为虚线,其余的为实线
        if (i == 0) {
            pen.setStyle(Qt::DotLine);
            painter.setPen(pen);
        }else{
            pen.setStyle(Qt::SolidLine);
            painter.setPen(pen);
        }
        //绘制线
        painter.drawLine(pos[i].x(), pos[i].y(), pos[i + 1].x(), pos[i + 1].y());
    }
    //恢复
    painter.restore();
}

void MainWindow::paintLowCurve()
{
    //qDebug() << "paintLowCurve";
    //设置painter
    QPainter painter(ui->lblLowCurve);
    //设置抗锯齿
    painter.setRenderHint(QPainter::Antialiasing, true);
    //构造画笔
    QPen pen = painter.pen();
    //设置画笔的宽度
    pen.setWidth(1);
    //设置画笔的颜色
    pen.setColor(QColor(0, 255, 255));
    //设置画笔
    painter.setPen(pen);
    //设置画刷
    painter.setBrush(QColor(0, 255, 255));
    //保存
    painter.save();


    //计算x y的坐标
    //x的坐标: 上面标签的x+标签的宽度/2
    //y的坐标: 先计算全部数据的平均值,将平均值设置在当前label高度/2,计算公式为(温度 - 平均值) * 要移动的像素点
    QPoint pos[6] = {};
    //高温和
    int tempSum = 0;
    //平均高温
    int tempAverage = 0;
    //中心点
    float center = ui->lblLowCurve->height() / 2;
    for (int i = 0; i < 6; ++i) {
        //计算高温和
        tempSum += m_Day[i].low;
    }
    //计算平均值
    tempAverage = tempSum / 6;
    for (int i = 0; i < 6; ++i) {
        //计算往上移动还是往下移动
        int offset = (m_Day[i].low - tempAverage) * INCREMENT;
        //x
        pos[i].setX(m_WeekList[i]->pos().x() + m_WeekList[i]->width() / 2);
        //y
        pos[i].setY(center - offset);
        //qDebug() << i << QPoint(pos[i].x() - TEXT_OFFSET_X, pos[i].y() - TEXT_OFFSET_Y);
        //绘制点
        painter.drawEllipse(pos[i], PAINT_RADIUS, PAINT_RADIUS);
        //绘制文字
        painter.drawText(QPoint(pos[i].x() - TEXT_OFFSET_X, pos[i].y() - TEXT_OFFSET_Y), QString::number(m_Day[i].low) + "°");
    }
    //只需要5段
    for (int i = 0; i < 5; ++i) {
        //绘制曲线,昨天为虚线,其余的为实线
        if (i == 0) {
            pen.setStyle(Qt::DotLine);
            painter.setPen(pen);
        }else{
            pen.setStyle(Qt::SolidLine);
            painter.setPen(pen);
        }
        //绘制线
        painter.drawLine(pos[i].x(), pos[i].y(), pos[i + 1].x(), pos[i + 1].y());
    }
    //恢复
    painter.restore();
}

void MainWindow::getWeatherInfo(QString cityName)
{
    QString cityCode = weatherTool::getCityCode(cityName);
    // qDebug()<<cityCode;
    // 检查返回的是否为空值
    if (cityCode.isEmpty()) {
       QMessageBox::information(this, "提示", "请检查输入的是否正确!", QMessageBox::Ok);
       return;
    }
    QUrl url = QUrl("http://t.weather.itboy.net/api/weather/city/" + cityCode);
    mNetworkAccessManager->get(QNetworkRequest(url));
}

void MainWindow::parseJson(QByteArray data)
{
    //解析错误时会触发的
    QJsonParseError err;
    //创建QJsonDocument
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError) {
        qDebug("%s(%d): %s", __FUNCTION__, __LINE__, err.errorString().toLatin1().data());
        return;
    }

    if(!doc.isObject())
    {
        qDebug("%s(%d): %s", __FUNCTION__, __LINE__,"not obj");
        return;
    }

    //Json对象,包括了请求的全部数据
    QJsonObject rootObj = doc.object();
    // qDebug() << rootObj.value("message").toString();

    //1.解析日期和城市
    m_Today.date = rootObj.value("date").toString();
    m_Today.city = rootObj.value("cityInfo").toObject().value("city").toString();

    //2.解析yesterday
    QJsonObject dataObj = rootObj.value("data").toObject();
    QJsonObject yesterdayObj = dataObj.value("yesterday").toObject();

    m_Day[0].date = yesterdayObj.value("date").toString();//日期
    m_Day[0].aqi = yesterdayObj.value("aqi").toDouble();//空气质量,toDouble包括整数型
    m_Day[0].type = yesterdayObj.value("type").toString();//天气类型
    m_Day[0].week = yesterdayObj.value("week").toString();//星期
    m_Day[0].ymd = yesterdayObj.value("ymd").toString();//完整年月日
    m_Day[0].low = parseString(yesterdayObj.value("low").toString());//最低温度
    m_Day[0].high = parseString(yesterdayObj.value("high").toString());//最高温度
    m_Day[0].fx = yesterdayObj.value("fx").toString();//风向
    m_Day[0].fl = yesterdayObj.value("fl").toString();//风力
#if 0
    qDebug()<<"date\t"<<m_Today.date<< "cityInfo\t"<< m_Today.city<<endl<<
              "yesterday"<<m_Day[0].date<<m_Day[0].aqi<<m_Day[0].type<<m_Day[0].week<<m_Day[0].ymd<<m_Day[0].low<<m_Day[0].high<<m_Day[0].fx<<m_Day[0].fl<<endl;
#endif
    //3.解析forecast中的6天数据
    //Json数组,未来几天的数据
    QJsonArray forecastArray = dataObj.value("forecast").toArray();
    // qDebug() << forecastArray.size(); //15
    for (int i = 1; i < DAYNUM; ++i) {         //跳过0,因为0已经赋值给昨天的数据了
        QJsonObject forecastData = forecastArray[i].toObject();
        m_Day[i].date = forecastData.value("date").toString();//日期
        m_Day[i].aqi = forecastData.value("aqi").toDouble();//空气质量,toDouble包括整数型
        m_Day[i].type = forecastData.value("type").toString();//天气类型
        m_Day[i].week = forecastData.value("week").toString();//星期
        m_Day[i].low = parseString(forecastData.value("low").toString());//最低温度
        m_Day[i].high = parseString(forecastData.value("high").toString());//最高温度
        m_Day[i].fx = forecastData.value("fx").toString();//风向
        m_Day[i].fl = forecastData.value("fl").toString();//风力
        m_Day[i].ymd = forecastData.value("ymd").toString();//完整年月日
    }

#if 0
    for (int i=0;i<DAYNUM ;i++ ) {
        qDebug()<<m_Day[i].date<<m_Day[i].aqi<<m_Day[i].type<<m_Day[i].week<<m_Day[i].ymd<<m_Day[i].low<<m_Day[i].high<<m_Day[i].fx<<m_Day[i].fl;
    }
#endif

    //4.解析今天的数据
    m_Today.shidu = dataObj.value("shidu").toString();
    m_Today.quality = dataObj.value("quality").toString();
    m_Today.ganmao = dataObj.value("ganmao").toString();
    m_Today.pm25 = dataObj.value("pm25").toDouble();
    m_Today.wendu = dataObj.value("wendu").toString().toInt();
    //注意:forecast中的第2个元素也是今天的数据
    m_Today.type = m_Day[1].type;
    m_Today.low = m_Day[1].low;
    m_Today.high = m_Day[1].high;
    m_Today.fx = m_Day[1].fx;
    m_Today.fl = m_Day[1].fl;
    // qDebug() << m_Today.shidu<<m_Today.quality<<m_Today.ganmao<<m_Today.pm25<<m_Today.wendu <<m_Today.type << m_Today.low << m_Today.high << m_Today.fx << m_Today.fl;

    //5.更新UI
    updateUI();

    ui->lblHighCurve->update();
    ui->lblLowCurve->update();


}

void MainWindow::onReplied(QNetworkReply *reply)
{
    //状态码
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    //判断是否请求成功
    if (reply->error() != QNetworkReply::NoError || statusCode != 200) {
        QMessageBox::warning(this, "天气", reply->errorString(), QMessageBox::Ok);
    }else{
        QByteArray requestData = reply->readAll();
        // qDebug() << requestData.data();
        parseJson(requestData);//解析json数据,创建2个类,ToDay和Day(数组)

    }
    //释放内存
    reply->deleteLater();
}

int MainWindow::parseString(QString str)
{
    str = str.split(" ").at(1);//以空格为分割符,去除前面的文字,取出第二个X℃,
    str = str.left(str.length() - 1);//去除后面的摄氏度符号
    return str.toInt();
}

QString MainWindow::getlocal()
{
    QString ip;
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl("http://www.net.cn/static/customercare/yourip.asp")));

    QEventLoop eventLoop;
    QTimer m_timer;
    connect(manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    connect(&m_timer, SIGNAL(timeout()), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    m_timer.start(5000);
    if(m_timer.isActive())
    {
        m_timer.stop();
        QString data = reply->readAll();
        int len1 = data.indexOf("<h2>");
        int len2 = data.indexOf("</h2>");
        ip = data.mid(len1+4,len2-len1-4);
    }


    // qDebug()<<ip;
    // "http://opendata.baidu.com/api.php?query="+ip+"&co=&resource_id=6006&oe=utf8"
    reply =manager->get(QNetworkRequest(QUrl("http://opendata.baidu.com/api.php?query="+ip+"&co=&resource_id=6006&oe=utf8")));
    connect(manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));

    connect(&m_timer, SIGNAL(timeout()), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    m_timer.start(5000);
    QByteArray byteArray;
    if(m_timer.isActive())
    {
        m_timer.stop();
        QByteArray data = reply->readAll();
        byteArray=QString(data).toUtf8();
        // qDebug()<<"read all:"<<byteArray.data();
    }
    QJsonDocument doc = QJsonDocument::fromJson(byteArray);
    QJsonObject obj = doc.object();
    QStringList keys = obj.keys();

    QJsonValue value = obj.value("data");
    // qDebug()<<value[0]["location"].toString().split(" ")[0].right(3).left(2);

    // qDebug()<<value;

    eventLoop.deleteLater();
    m_timer.deleteLater();
    delete manager;
    return value[0]["location"].toString().split(" ")[0].right(3).left(2) ;
}

//更新UI控件
void MainWindow::updateUI()
{
    //更新今天的数据
    QString changeStr = QDateTime::fromString(m_Today.date, "yyyyMMdd").toString("yyyy/MM/dd");
    ui->lblDate->setText(changeStr + " " + m_Day[1].week);
    ui->lblCity->setText(m_Today.city);
    ui->lblPM25->setText(QString::number(m_Today.pm25));
    ui->lblLowHigh->setText(QString::number(m_Today.low) + "℃" + "~" + QString::number(m_Today.high) + "℃" );
    ui->lblGanMao->setText(m_Today.ganmao);
    ui->lblShiDu->setText(m_Today.shidu);
    ui->lblQuality ->setText(QString::number(m_Day[1].aqi));
    ui->lblWindFx->setText(m_Today.fx);
    ui->lblWindFl->setText(m_Today.fl);
    ui->lblTemp->setText(QString::number(m_Today.wendu) + "℃");
    //更新icon
    ui->lblTypeIcon->setPixmap(m_TypeMap[m_Today.type]);
    // qDebug() << m_Today.type;

    //更新未来几天的数据
        for (int i = 0; i < DAYNUM; ++i) {
            //星期
            m_WeekList[i]->setText("周" + m_Day[i].week.right(1));
            m_WeekList[0]->setText("昨天");
            m_WeekList[1]->setText("今天");
            m_WeekList[2]->setText("明天");

            //日期
            QStringList ymdList = m_Day[i].ymd.split("-");
            m_DateList[i]->setText(ymdList.at(1) + "-" + ymdList.at(2));

            //空气质量
            switch (checkedQulity(m_Day[i].aqi)) {
            case 1:
                m_QulityList[i]->setText("优");
                m_QulityList[i]->setStyleSheet("background-color: rgb(121,184,0);");
                break;
            case 2:
                m_QulityList[i]->setText("良");
                m_QulityList[i]->setStyleSheet("background-color: rgb(255,187,23);");
                break;
            case 3:
                m_QulityList[i]->setText("轻度");
                m_QulityList[i]->setStyleSheet("background-color: rgb(255,87,97);");
                break;
            case 4:
                m_QulityList[i]->setText("中度");
                m_QulityList[i]->setStyleSheet("background-color: rgb(235,17,27);");
                break;
            case 5:
                m_QulityList[i]->setText("重度");
                m_QulityList[i]->setStyleSheet("background-color: rgb(170,0,0);");
                break;
            case 6:
                m_QulityList[i]->setText("严重");
                m_QulityList[i]->setStyleSheet("background-color: rgb(110,0,0);");
                break;
            }
            //风向,风力
            m_FxList[i]->setText(m_Day[i].fx);
            m_FlList[i]->setText(m_Day[i].fl);

            //天气类型和天气类型的Icon
            m_TypeIconList[i]->setPixmap(m_TypeMap[m_Day[i].type]);
            m_TypeList[i]->setText(m_Day[i].type);
        }
}

//检查空气质量
int MainWindow::checkedQulity(int aqi)
{
    if (aqi >= 0 && aqi <= 50) {
        return 1;
    }else if(aqi > 50 && aqi <= 100){
        return 2;
    }else if(aqi > 100 && aqi <= 150){
        return 3;
    }else if(aqi > 150 && aqi <= 200){
        return 4;
    }else if(aqi > 200 && aqi <= 250){
        return 5;
    }else{
        return 6;
    }
    return -1;
}

//根据lineEdit的内容搜索
void MainWindow::onBtnSerchClicked()
{
    QString cityName = ui->leCity->text();
    // qDebug() << cityName;
    getWeatherInfo(cityName);
}
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QLabel>
#include <QNetworkAccessManager>
#include "weatherdata.h"

#include <QMenu> //菜单
#include <QContextMenuEvent>
#include <QMouseEvent> //鼠标事件

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include <QMap>
#include <QList>
#include <QLabel>
#include "weatherdata.h"
#include <QKeyEvent>

#define DAYNUM 6
#define INCREMENT 1.5       //温度升高/降低1度 y轴移动的像素点增量
#define PAINT_RADIUS 3      //曲线描点的大小
#define TEXT_OFFSET_X 12    //温度文本X偏移
#define TEXT_OFFSET_Y 12    //温度文本Y偏移

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QMenu* m_ExitMenu;  //退出菜单
    QAction* m_ExitAction; //退出行为
    QPoint m_Offest; //鼠标点击的pos距离左上角的pos的偏移

    QNetworkAccessManager* mNetworkAccessManager; //网络请求

protected:
    void contextMenuEvent(QContextMenuEvent *e);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    //事件过滤器,默认无,需要重写
    bool eventFilter(QObject *watched, QEvent *event);

    void keyPressEvent(QKeyEvent *event);
    //高温曲线
    void paintHigtCurve();
    //低温曲线
    void paintLowCurve();

    void getWeatherInfo(QString cityCode);

    void parseJson(QByteArray data); //解析json数据
    int parseString(QString str);
    QString getlocal();
    void updateUI();

    Today m_Today; //今天的数据
    Day m_Day[DAYNUM]; //未来几天的数据
    QMap<QString, QString> m_TypeMap;
    //UI的List 用于循环赋值
    QList<QLabel*> m_DateList;
    QList<QLabel*> m_WeekList;
    QList<QLabel*> m_QulityList;
    QList<QLabel*> m_TypeList;
    QList<QLabel*> m_TypeIconList;
    QList<QLabel*> m_FxList;
    QList<QLabel*> m_FlList;

    int checkedQulity(int aqi);
public slots:
    void onReplied(QNetworkReply* reply);
    //搜索按钮
    void onBtnSerchClicked();
};


#endif // MAINWINDOW_H

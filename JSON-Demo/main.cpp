#include <QCoreApplication>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include <QFile>
#include <QDebug>

void writejson()
{
    //新建Json对象
    QJsonObject rootObj;


    //插入 name
    rootObj.insert("name", "China");

    //插入 info
    QJsonObject infoObj;
    infoObj.insert("capital", "beijing");
    infoObj.insert("asian", true);
    infoObj.insert("founded", 1949);

    rootObj.insert("info", infoObj);

    //插入 provinces
    QJsonArray provinceArray;

    QJsonObject provinceSdObj;
    provinceSdObj.insert("name", "shandong");
    provinceSdObj.insert("capital", "beijing");

    QJsonObject provinceZjObj;
    provinceZjObj.insert("name", "zhejiang");
    provinceZjObj.insert("capital", "hangzhou");

    provinceArray.append(provinceSdObj);
    provinceArray.append(provinceZjObj);

    rootObj.insert("provinces", provinceArray);

    //转换成字符串
    QJsonDocument doc(rootObj);
    // QJsonDocument({"info":{"asian":true,"capital":"beijing","founded":1949},"name":"China","provinces":[{"capital":"beijing","name":"shandong"},{"capital":"hangzhou","name":"zhejiang"}]})

    QByteArray json = doc.toJson();

    // 打印输出
    // qDebug() << json.data() << endl;
    qDebug() << QString(json).toUtf8().data() << endl;

    // 写入文件
    QFile file("China.json");
    file.open(QFile::WriteOnly);
    file.write(json);
    file.close();
}

void readjson()
{
    QFile file("China.json");
    file.open(QFile::ReadOnly);
    QByteArray json = file.readAll();// 字节数组
    file.close();//

    QJsonDocument doc = QJsonDocument::fromJson(json);// 转化为QJsonDocument
//    QJsonDocument doc;
//    doc.fromJson(json);

    if(!doc.isObject())
    {
        qDebug() << "not obj" << endl;
        return;
    }

    QJsonObject obj = doc.object();
    QStringList keys = obj.keys();
    // qDebug() << keys; // ("info", "name", "provinces")

    for (int i = 0; i < keys.length(); i++)
    {
        //获取 k v
        QString key = keys[i];
        QJsonValue value = obj.value(key);
        // qDebug() << key << value;

        if(value.isBool())
        {
            qDebug() << key << ":" << value.toBool() << endl;
        }
        else if(value.isString())
        {

            qDebug() << key << ":" << value.toString() << endl;
        }
        else if (value.isDouble())
        {
            qDebug() << key << ":" << value.toInt() << endl;
        }
        else if (value.isObject())
        {
            qDebug() << key << ":";

            QJsonObject infoObj = value.toObject();
            QString capital = infoObj["capital"].toString();
            bool asian = infoObj["asian"].toBool();
            int founded = infoObj["founded"].toInt();

            qDebug() << "\t" << "capital" << ":" << capital << endl;
            qDebug() << "\t" << "asian" << ":" << asian << endl;
            qDebug() << "\t" << "founded" << ":" << founded << endl;

        }
        else if (value.isArray())
        {
            qDebug() << key << endl;
            QJsonArray provinceArray = value.toArray();
            for(int i = 0; i < provinceArray.size(); i++)
            {
                QJsonObject provinceObj = provinceArray[i].toObject();
                QString name = provinceObj["name"].toString();
                QString capital = provinceObj["capital"].toString();
                qDebug() << "\t" << "name" << name << ":" << "capital" << capital << endl;
            }
        }
    }

    file.close();
}
int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);
    // writejson();
    readjson();
    return a.exec();
}

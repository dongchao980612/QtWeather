# QT_weather
基于QT的天气预报软件设计

```tex
http://t.weather.itboy.net/api/weather/city/101010100
```

## QJson

> 所需要的头文件 
>
> - QJsonArray 
>
> - QJsonObject 
>
> - QJsonDocument



```c++
// writejson()
//新建Json对象

QJsonObject rootObj;

rootObj.insert("name", "China");//插入 name


QJsonObject infoObj;
infoObj.insert("capital", "beijing");
infoObj.insert("asian", true);
infoObj.insert("founded", 1949);

rootObj.insert("info", infoObj);//插入 info


QJsonArray provinceArray;

QJsonObject provinceSdObj;
provinceSdObj.insert("name", "shandong");
provinceSdObj.insert("capital", "beijing");

QJsonObject provinceZjObj;
provinceZjObj.insert("name", "zhejiang");
provinceZjObj.insert("capital", "hangzhou");

provinceArray.append(provinceSdObj);
provinceArray.append(provinceZjObj);

rootObj.insert("provinces", provinceArray);//插入 provinces


//转换成字符串
QJsonDocument doc(rootObj);
QByteArray json = doc.toJson();

// 写入文件
QFile file("China.json");
file.open(QFile::WriteOnly);
file.write(json);
file.close();
```


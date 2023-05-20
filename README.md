# QT_weather
基于QT的天气预报软件设计

```tex
http://t.weather.itboy.net/api/weather/city/101010100
```


## 1. 整体效果、技术点
在右上角输入要查询的城市，然后点击查询按钮就会发送 http请求给服务器，请求回来的天气数据为JSON格式，通过解析 JSON可以获取以下信息:
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

### JSON数据格式
<font color='red'> HTTP </font>服务端返回的天气数据是<font color='red'> JSON </font>格式，使用QT提供的解析<font color='red'> JSON </font>相关的类可以很方便的解析出其中的字段

### HTTP请求
根据<font color='red'> HTTP </font>服务端提供的将诶口，发送<font color='red'> HTTP </font>请求，获取天气数据

###  事件
为了实现将界面美观，将窗口设置为无标题，这样就无法通过右上角的【关闭】按钮关闭程序，因此需要添加右键菜单退出功能，还重写了鼠标事件，让窗口可以跟随鼠标移动

###  绘图
绘制高低温曲线图，根据每天高低温数据，绘制一个曲线图，更直观的展示温度的变化

###  资源文件
根据不同的天气类型，可以用不同的图标进行展示，更加直观，而这些图表通常放在资源文件里，这样他们可以一同被打包进QT可执行程序中

## 2. JSON格式解析

### 2.1 什么是JSON
<font color='red'> JSON </font>对象中，还可以嵌套<font color='red'> JSON </font>对象和<font color='red'> JSON </font>数组
```Json
{
  "name":"China",
  "info":{
    "capital":"beijing",
    "asian":true,
    "founded":1949
  },
  "provices":
  [
    {
      "name":"shandong",
      "capital":"jinan"
    },
    {
    "name":"guangdong",
    "capital":"shenzhen"
    }
  ]
}
```

### 2.2 JSON的两种数据格式

<font color='red'> JSON </font>有两种数据格式
 - <font color='red'> JSON </font>对象
 - <font color='red'> JSON </font>数组
  > 被大括号包裹的是<font color='red'> JSON </font>对象，中括号包裹的是<font color='red'> JSON </font>数组

<font color='red'> JSON </font>数组之间的元素用逗号分隔，元素可以是不同类型。
  <font color='red'> JSON </font>对象内部使用键值对的方式进行组织，键和值之间使用冒号分隔，多个键值对之间使用逗号分隔。
>   <font color='red'> JSON </font>对象中还可以嵌套<font color='red'> JSON </font>对象和<font color='red'> JSON </font>数组。

### 2.3 JSON在线解析
> https://www.json.cn/#

### 2.4 Qt中使用JSON

#### 2.4.1 JSON相关类
Qt提供的SJON相关类有四个：

- QJsonObject
- QJsonArray
- QJsonValue
- QJsonDocument

####  （1) QJsonObject
<font color='red'>QJsonObject</font>封装了<font color='red'>JSON</font>里面的对象，可以存储多个键值对，其中键为字符串类型，值为<font color='red'>QJsonValue</font>类型。

- 创建一个<font color='red'>QJsonObject</font>对象
  ```c++
  QJsonObject::QJsonObject();
  ```

- 将键值对添加到<font color='red'>QJsonObject</font>对象中
  ```c++
  QJsonObject::iterator insert(const QString &Key, const QJSonValue &value);
  ```

- 获取<font color='red'>QJsonObject</font>对象中键值对的个数
  ```c++
  int QJsonObject::count() const;
  int QJsonObject::size() const;
  int QJsonObject::length() const;
  ```

- 通过<font  color="red">key</font>得到 <font  color="red">value</font>
  ```c++
  QJSonValue QJsonObject::value(const QString &key) const;
  QJSonValue QJsonObject::operator[](const QString &key) const;
  ```

- 遍历<font  color="red">key</font>
   ```c++
  QStringList QJsonObject::keys() const ;
  ```
####  （2) QJsonArray
<font color='red'>QJsonArray</font>封装了<font color='red'>JSON</font>中的数组，数组中每一个元素类型统一为<font color='red'>QJsonValue</font>类型。

- 创建一个<font color='red'>QJsonArray</font>
  ```c++
  QJsonArray::QJsonArray();
  ```
- 添加数组元素
  ```c++
  // 添加到头部和尾部
  void QJsonArray::append(const QJsonValue &value);
  void QJsonArray::prepend(const QJsonValue &value);

  //插入到i的位置之前
  void QJsonArray::insert(int i, const QJsonValue &value);

  // 添加到头部和尾部
  void QJsonArray::push_back(const QJsonValue &value);
  void QJsonArray::push_front(const QJsonValue &value);
  ```
-  获取<font color='red'>QJsonArray</font>对象中元素的个数
    ```c++
    int QJsonObject::count() const;
    int QJsonObject::size() const;
    ```
    
  -  获取<font color='red'>QJsonArray</font>中元素值
      ``` c++
      //获取头部和尾部
      QJsonValue QJsonArray::first() const;
      QJsonValue QJsonArray::last() const;

      //获取指定位置
      QJsonValue QJsonValue::at(int i) const;
      QJsonValueRef QJsonValue::operator[](int i);
      ``` 

- 删除元素
  ```c++
  //删除头部和尾部
  void QJsonArray::pop_back() ;
  void QJsonArray::pop_front() ;

  void QJsonArray::removeFirst() ;
  void QJsonArray::removeLast() ;

  //删除指定位置
  QJsonValue QJsonValue::at(int i) const;
  QJsonValueRef QJsonValue::operator[](int i);
  ```
  
####  （3) QJsonValue

####  （4) QJsonDocument

#### 2.4.1 构建JSON字符串
使用<font color='red'>Qt</font>构建的工具类，生成如下格式的<font color='red'>JSON</font>字符串
```Json
{
  "name":"China",
  "info":{
    "capital":"beijing",
    "asian":true,
    "founded":1949
  },
  "provices":
  [
    {
      "name":"shandong",
      "capital":"jinan"
    },
    {
    "name":"guangdong",
    "capital":"shenzhen"
    }
  ]
}
```

代码：
```c++
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
```
#### 2.4.1 解析JSON字符串
在接收完毕之后，需要解析其中的每一个字段，根据每个字段的值作出相应的显示或者其他处理，接下来使用<font color='red'>Qt</font>提供的工具类，读取<font color='red'>JSON</font>字符串，把其中的字段解析出来。

```c++


```
## 3. HTTP通信

### 3.4 Qt实现HTTP请求


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

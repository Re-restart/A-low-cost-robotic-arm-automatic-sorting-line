#include <WiFi.h>
#include <WebServer.h>
#include <Arduino.h>
//访问服务器的设备和服务器本身需要连接同一局域网
// 替换以下变量为你的Wi-Fi网络信息
const char* ssid = "mmp";
const char* password = "1395st2020";

// 创建WebServer对象，端口号为80
WebServer server(80);

//PROGMEM关键字将数据存储在flash中，而不是RAM，因为回调函数中不能有参数，所以还是用常规的字符数组方式，而非string
const char indexHtml[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8", meta name="viewport" 
     content="width=device-width, initial-scale=1.0, maximum-scale=1.0, 
     user-scalable=0;"/>
    <title>Automatic sorting line</title>
    <h3>rot1----->belt1&nbsp;&nbsp;&nbsp;&nbsp;rot2------>belt2   </h3>
    <style>
        h3 {
            color: black;
            background-color:#0066CC;
        }    
        div {
            width: 300px;
            border: 10px solid green;
            padding: 30px;
            font-size: 18px;
            margin: 20px;
        }
        body {
            background-color:#F0F0F0;
            display: flex;
            flex-wrap: wrap;
            justify-content: center;
            padding: 15px;
        }
        form {
            margin: 15px;
        }
        input[type="submit"] {
            background-color:#00FFFF;
            color: white
            width: 400px;
            height: 40px;
            font-size: 16px;
            margin: 10px;
            transition: background-color 0.8s;  <!--按钮背景颜色有0.8s渐变-->
            cursor: pointer;
        }
        input[type="submit"]:hover {
            background-color: dodgerblue;
        }
    </style>
</head>
<body>
    <form action="/HIGH" method="POST"><input type="submit" value="HIGH"></form>
    <form action="/LOW" method="POST"><input type="submit" value="LOW"></form>
    <form action="/FAR" method="POST"><input type="submit" value="FAR"></form>
    <form action="/NEAR" method="POST"><input type="submit" value="NEAR"></form>
    <form action="/ROT1" method="POST"><input type="submit" value="ROT1"></form>
    <form action="/ROT2" method="POST"><input type="submit" value="ROT2"></form>
    <form action="/STOP" method="POST"><input type="submit" value="STOP"></form>
    <form action="/PUT" method="POST"><input type="submit" value="PUT"></form>
    <form action="/TOUCH" method="POST"><input type="submit" value="TOUCH"></form>
    <form action="/BELT" method="POST"><input type="submit" value="BELT"></form>
    <div>欢迎来到分拣控制系统</div>    
</body>
</html>
)rawliteral";

// 设置处理404情况的函数'handleNotFound'
void handleNotFound() {
  server.send(404, "text/plain", "没有找到界面");  // 发送 HTTP 状态 404 (未找到页面) 并向浏览器发送文字 "404: Not found"
}

void high() {
  Serial2.print(":moter3left.");
  Serial.print(":moter3left.");
  server.sendHeader("Location", "/");  // 跳转回页面根目录
  server.send(303);                    // 发送Http相应代码303 跳转
}

void low() {
  Serial2.print(":moter3right.");
  Serial.print(":moter3right.");
  server.sendHeader("Location", "/");  // 跳转回页面根目录
  server.send(303);                    // 发送Http相应代码303 跳转
}

void far() {
  Serial2.print(":moter1left.");
  Serial.print(":moter1right.");
  server.sendHeader("Location", "/");  // 跳转回页面根目录
  server.send(303);                    // 发送Http相应代码303 跳转
}

void near() {
  Serial2.print(":moter1right.");
  Serial.print(":moter1left.");
  server.sendHeader("Location", "/");  // 跳转回页面根目录
  server.send(303);                    // 发送Http相应代码303 跳转
}

void rot1() {
  Serial2.print(":moter2left.");
  Serial.print(":moter2left.");
  server.sendHeader("Location", "/");  // 跳转回页面根目录
  server.send(303);                    // 发送Http相应代码303 跳转
}

void rot2() {
  Serial2.print(":moter2right.");
  Serial.print(":moter2right.");
  server.sendHeader("Location", "/");  // 跳转回页面根目录
  server.send(303);                    // 发送Http相应代码303 跳转
}

void all_stop() {
  Serial2.print(":togetherstop.");
  Serial.print(":togetherstop.");
  server.sendHeader("Location", "/");  // 跳转回页面根目录
  server.send(303);                    // 发送Http相应代码303 跳转
}

void put() {
  Serial2.print(":littlemotor_put.");
  Serial.print(":littlemotor_put.");
  server.sendHeader("Location", "/");  // 跳转回页面根目录
  server.send(303);                    // 发送Http相应代码303 跳转
}

void touch() {
  Serial2.print(":littlemotor_touch.");
  Serial.print(":littlemotor_touch.");
  server.sendHeader("Location", "/");  // 跳转回页面根目录
  server.send(303);                    // 发送Http相应代码303 跳转
}

void belt_right() {
  Serial2.print(":belt_right.");
  Serial.print(":belt_right.");
  server.sendHeader("Location", "/");  // 跳转回页面根目录
  server.send(303);                    // 发送Http相应代码303 跳转
}

void belt_left() {
  Serial2.print(":belt_left.");
  Serial.print(":belt_left.");
  server.sendHeader("Location", "/");  // 跳转回页面根目录
  server.send(303);                    // 发送Http相应代码303 跳转
}

void connect_wifi() {
  Serial.begin(115200);                       // 开启串行通讯，波特率115200
  Serial2.begin(115200, SERIAL_8N1, 16, 17);  // 开启串行通讯，波特率115200, 16为RX，17为TX
  Serial2.setDebugOutput(true);               //设置核心调试级别是不够的，必须在配置上启用debug日志
  WiFi.begin(ssid, password);                 // 连接Wi-Fi

  // 等待Wi-Fi连接
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Wi-Fi连接成功，打印ESP32的IP地址
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void call_back() {
  Serial.println("User requested");
  server.send(200, "text/html", indexHtml);  // 响应请求
}

void web_server() {
  // 路由设置，当访问根目录时调用handleRoot函数
  server.on("/", HTTP_GET, call_back);
  // 开始监听HTTP请求
  server.begin();
  Serial.println("HTTP server started");

  server.on("/HIGH", HTTP_POST, high);
  server.on("/LOW", HTTP_POST, low);
  server.on("/FAR", HTTP_POST, far);
  server.on("/NEAR", HTTP_POST, near);
  server.on("/ROT1", HTTP_POST, rot1);
  server.on("/ROT2", HTTP_POST, rot2);
  server.on("/STOP", HTTP_POST, all_stop);
  server.on("/PUT", HTTP_POST, put);
  server.on("/TOUCH", HTTP_POST, touch);
  server.on("/BELT", HTTP_POST, belt_left);
}
void setup() {
  connect_wifi();
  web_server();
}

void loop() {
  // 处理任何收到的HTTP请求
  server.handleClient();
}
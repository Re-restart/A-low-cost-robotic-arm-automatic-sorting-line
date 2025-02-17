# A-low-cost-robotic-arm-automatic-sorting-line
这是一个低成本的自动分拣流水线装置<br>


下位机和部分上位机代码在master分支里；<br>
全部代码在temp分支里；<br>
主分支只负责介绍具体功能<br>

在这里切换分支<br>
![image](https://github.com/user-attachments/assets/85c8d61b-fd74-425b-80cc-19683939f73d)


放一下演示视频链接和实物图片：
https://www.bilibili.com/video/BV1F9Y3eGE9t/<br>
https://www.bilibili.com/video/BV1eogkecEhx/<br>

![037840a41b97a3a7861dca1b568372a](https://github.com/user-attachments/assets/935b91e6-2aa5-4981-9273-800f2bc1a76a)



介绍一下：<br>
结合生产线的实际工况，设计开发一种基于三自由度机械臂的自动分拣流水线，作为类似流水线系统的调试模型。主要从机器人控制系统硬件平台搭建，软件系统设计，速度规划，轨迹规划与直线插补，结构构建与设计等方面进行研究。本文以三自由度机器人为研究对象，结合传送带协同控制实现物块的自动分拣为研究目的，主要进行以下研究工作：<br>
（1）设计分拣系统的结构方案。建立三自由度机器人和流水线，遥控器的简化模型，完成UG上的虚拟装配设计，机械结构模型展示。完成机械臂几何逆运动学的解算，构建闭环矢量方程，并结合机构的自身约束条件，完善底层控制板的逆解计算代码。 <br>
（2）基于自动分拣流水线系统的总体功能需求，搭建系统硬件总体结构框架，并基于Cortex-M3系列STM32F103ZET6单片机和通讯单片机ESP32-WROOM共同构建运动控制系统的硬件平台，多电源完成整个分拣系统的供电。摄像头，串口1&串口4，通讯单片机通过USB-HUB连接到上位机PC端。<br>
（3）考虑设计要求，使用RTOS系统搭建出整个底层软件框架。机械臂的电机采用梯型加减速控制方案，控制三个步进电机并完成初始化，逆解计算，空间定位工作。使用两个继电器完成传送带直流电机的正反转控制，另有语音播报，OLED状态显示，串口协同调试，逆解初始化与按钮控制自动分拣等功能。末端抓取使用28BYJ-48型步进电机，ULN2003芯片驱动完成夹爪的开关控制。<br>
（4）使用网页端上位机完成对分拣系统具体动作的控制。网页端上位机使用ESP32中的STA工作站模式，控制时连接局域网热点，打开相关网页即可，板内互相通讯选择串口3<br>
（5）融合硬件测试，测试开发的一些理念，对已经完成的自动分拣实物进行不同方面的测试，联调与改进。共包括机械结构的维护和固定，单开关电源供电方案，下位机控制程序，上位机辅助程序四个部分。<br>


串口1可以通过"a;b;c;a角度;b角度;c角度."这种字符串完成控制，<br>
也可以输入":moter1right."这样的字符串完成逆运动学的解算<br>
调试信息显示在串口1&串口4<br>


本项目下载master分支源码的方式：<br>
git clone -b master https://github.com/Re-restart/A-low-cost-robotic-arm-automatic-sorting-line.git<br>
temp分支同理：<br>
git clone -b temp https://github.com/Re-restart/A-low-cost-robotic-arm-automatic-sorting-line.git<br>
想下载全部分支源码：<br>
git clone https://github.com/Re-restart/A-low-cost-robotic-arm-automatic-sorting-line.git<br>
在新的文件夹里：<br>
git branch -a<br>
出现类似图片里的内容，这就是本项目的所有分支<br>
![image](https://github.com/user-attachments/assets/5ab90019-b7a3-4664-b7af-dab719baf687)<br>
想要某个分支内的文件，git checkout 某分支就可以了<br>
![image](https://github.com/user-attachments/assets/c4c75149-ff5f-4c32-b16f-76560fd60778)<br>

课程很多，比较难熬的研一第一学期已经结束，目前开始忙着选题写论文

会有时间重置这个项目的，当时很多方法都是能跑就行，没有经过很细致的推敲


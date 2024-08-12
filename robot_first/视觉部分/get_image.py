import cv2
import time
import threading
import os

#使用多线程来处理摄像头读取和图像保存的操作。
#这可以确保主线程持续更新显示窗口，而另一个线程处理图像的保存。

#目前摁g退出和frame卡住的问题已解决，也能成功保存图片，但gray页面还在卡，
#下一阶段目标是保证gray有实时frame，且可以保存gray图像，有多个文件夹储存文件/0001变0010.。。

save_images=False
capture_thread = None

class start_detection():
    def __init__(self):
        global save_images
        self.save_images=save_images
        self.cap = cv2.VideoCapture(1)
        if not self.cap.isOpened():
            print("Cannot open camera")
            exit()

        self.running = True
        capture_thread = threading.Thread(target=self.capture_frames)
        capture_thread.start()

        while self.running:
            self.show_frame()
            key = cv2.waitKey(1) & 0xFF
            if key == ord('q'):
                self.running = False
            elif key == ord('g'):
                self.gray_image()
            elif key == 27:  # ESC 退出
                self.save_images = True
           # self.code_listening()   真正的问题是，code_listening函数被页面进程获取函数capture_frames取代了，所以代码需要改动

        self.cap.release()
        cv2.destroyAllWindows()#不应该在while循环里

    def capture_frames(self):#用于监视键盘状态
        while self.running:
            ret, frame = self.cap.read()
            if not ret:
                print("目前已经无法获取图像，正在退出，，，")
                self.running = False
                break
            self.frame = frame
            if self.save_images:
                self.save_image()

    def show_frame(self):
        if hasattr(self, 'frame'):
            cv2.imshow('Original', self.frame)

    def save_image(self):
        i=1
        while i<11 and self.save_images:
            pre_number = "000"
            detection_number = pre_number + str(i)
            detection_name ="detection"+"_"+detection_number+".png"
            cv2.imwrite(detection_name,self.frame)
            time.sleep(0.5)
            self.ret, self.frame = self.cap.read()
            cv2.imwrite(detection_name, self.frame)#必须有一次重新读帧，再次写入的过程，否则保存的图像都一样
            i = i + 1
            if (i == 10):
                print("获取了全部图像")

    def gray_image(self):
        if hasattr(self, 'frame'):#用frame画框形式体现，就不会卡了
            gray = cv2.cvtColor(self.frame, cv2.COLOR_BGR2GRAY)
            cv2.imshow('Gray', gray)


if __name__ == "__main__":
    start_detection()


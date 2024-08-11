import serial
import cv2

send_message = False

class start_detection():
    def __init__(self):
        self.cap = cv2.VideoCapture(1)
        if not self.cap.isOpened():
            print("Cannot open camera")
            exit()
        while True:
            self.ret, self.frame=self.cap.read()
            self.show_frame()
            key = cv2.waitKey(1) & 0xFF
            if key == ord('q'):
                break

    def show_frame(self):
        if hasattr(self, 'frame'):
            cv2.imshow('Original', self.frame)
            self.white_detection()
            self.do_something()

    def white_detection(self):
        gray_image = cv2.cvtColor(self.frame, cv2.COLOR_BGR2GRAY)
        _, self.threshold_image = cv2.threshold(gray_image, 200, 255, cv2.THRESH_BINARY)
        contours, _ = cv2.findContours(self.threshold_image, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        x0 = 0
        y0 = 0
        for contour in contours:#画框后返回x,y的值
            x, y, w, h = cv2.boundingRect(contour)
            if cv2.contourArea(contour) > 500:  # 设置最小区域面积以排除噪声
                #print(x + w / 2, y + h / 2)
                x0 = x + w / 2
                y0 = y + h / 2
        return x0, y0

    def do_something(self):
        x,y = self.white_detection()
        cv2.line(self.threshold_image, (0, 200), (611, 200), (255, 0, 0), 5)
        if y>200:
            ser = serial.Serial('COM9', 115200)
            # 发送数据到串口
            ser.close()  # 关闭串口连接
        cv2.imshow('thr_img', self.threshold_image)


if __name__ == "__main__":
    start_detection()


import yaml
import serial
import time
import logging
from serial.tools.list_ports import comports

logging.basicConfig(level=logging.DEBUG,
                    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s')

class port_RX_TX():
    def __init__(self):
        # 读取 YAML 配置文件
        with open('arm_device.yaml', 'rb') as file:
            config = yaml.safe_load(file)
            self.serial_config = config['serial']

    def get_serial_port(self,config):
        available_ports = [port.device for port in comports()]
        if config['port'] not in available_ports:
            raise serial.SerialException(f"Port {config['port']} not found.")
        try:
            # 配置串口
            self.ser = serial.Serial(
                port=self.serial_config['port'],
                baudrate=self.serial_config['baudrate'],
                bytesize=self.serial_config['bytesize'],
                parity=self.serial_config['parity'],
                stopbits=self.serial_config['stopbits'],
                timeout=self.serial_config['timeout']
            )
            logging.info(f"串口 {config['port']} 成功打开")
        except serial.SerialException as e:
            raise serial.SerialException(f"Error: {e}")

    def message_TX_RX(self, read_size=100):
        logging.debug(f"已发送消息")
        time.sleep(0.5)
        self.ser.close()
        logging.info(f"串口 {self.serial_config['port']} 已关闭")

def begin():
    port_communicator = port_RX_TX()
    response = port_communicator.message_TX_RX()
    logging.info(f"响应内容: {response}")


if __name__ == '__main__':
    begin()



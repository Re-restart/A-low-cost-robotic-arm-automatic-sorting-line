import serial
ser = serial.Serial('COM9',115200)
ser.write(b':togetherstop.')
ser.close()
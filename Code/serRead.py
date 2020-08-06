import serial
import re

ser = serial.Serial("COM5", 115200)
ser.flush()

while True:
     cc=str(ser.readline())
     # print(cc)
     cc = cc[2:][:-5]
     # print(cc)
     a = re.findall(r'\d+', cc)
     b = float(a[0])/100
     c = float(a[1])/100
     print(str(b) + ',' + str(c))

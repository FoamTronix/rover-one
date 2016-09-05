import serial

ser = serial.Serial('/dev/ttyAMA0', 9600)
ser.write('L01')

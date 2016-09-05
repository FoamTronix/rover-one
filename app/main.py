from flask import Flask
from flask import render_template, redirect

import serial
import time
import RPi.GPIO as GPIO

ser = serial.Serial('/dev/ttyUSB0', 9600)

# pin = 13 # GPIO 27  # This is the board number for which GPIO pin will be used
# delay = 0.5  # This is the amount of delay (in seconds) between blinks
# GPIO.setmode(GPIO.BOARD)
# GPIO.setup(pin, GPIO.OUT)

app = Flask(__name__)

@app.route('/')
@app.route('/<msg>')
def index(msg=None):
    return render_template('index.html', msg = msg)

@app.route('/ledon')
def ledon():
    # GPIO.output(pin, GPIO.HIGH)
    ser.write('L01')
    ser.write('L11')
    return redirect('http://192.168.0.17:5000/ON')

@app.route('/ledoff')
def ledoff():
    # GPIO.output(pin, GPIO.LOW)
    ser.write('L00')
    ser.write('L10')
    return redirect('http://192.168.0.17:5000/OFF')

if __name__ == "__main__":
    app.run(host='192.168.0.17')

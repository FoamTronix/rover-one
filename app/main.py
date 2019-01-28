from flask import Flask, request, render_template, redirect, jsonify, Response
from camera import VideoCamera

import json
import serial
import time
import RPi.GPIO as GPIO

# ser = serial.Serial('/dev/ttyACM0', 9600)
# ser = serial.Serial('/dev/ttyAMA0', 9600)
ser = serial.Serial('/dev/ttyUSB0', 9600)

GPIO.setmode(GPIO.BOARD)
# GPIO.setup(pin, GPIO.OUT)

app = Flask(__name__)

@app.route('/')
def index():
  return render_template('index.html')

# Movements
# -------------------------------------------------
@app.route('/move/<state>')
def move(state):
  if state.lower() == 'forward':
    ser.write("MoveForward\n")
    return jsonify({ 'move': 'forward' })
  elif state.lower() == 'backward':
    ser.write("MoveBackward\n")
    return jsonify({ 'move': 'backward' })
  else:
    ser.write("MoveStop\n")
    return jsonify({ 'move': 'stop' })

@app.route('/turn/<state>')
def turn(state):
  if state.lower() == 'left':
    ser.write("TurnLeft\n")
    return jsonify({ 'turn': 'left' })
  elif state.lower() == 'right':
    ser.write("TurnRight\n")
    return jsonify({ 'turn': 'right' })
  else:
    ser.write("TurnCenter\n")
    return jsonify({ 'turn': 'center' })
# -------------------------------------------------

# Lights
# -------------------------------------------------
@app.route('/lights/<state>')
def lights(state):
  if state.lower() == 'on':
    ser.write("LightsOn\n")
    return jsonify({ 'lights': 'on' })
  else:
    ser.write("LightsOff\n")
    return jsonify({ 'lights': 'off' })

@app.route('/infrared/<state>')
def infrared(state):
  if state.lower() == 'on':
    ser.write("InfraredOn\n")
    return jsonify({ 'infrared': 'on' })
  else:
    ser.write("InfraredOff\n")
    return jsonify({ 'infrared': 'off' })
# -------------------------------------------------

# Sensor Readings
# -------------------------------------------------
@app.route('/sensors')
def sensors():
  ser.write("Sensors\n")
  data = ser.readline().strip()
  data = "{\"sensors\": %s}" % data
  data = json.loads(data)
  data = json.dumps(data) # cleans up float values, and makes sure values are correct
  return data, 200, {'Content-Type': 'application/json'} # cannot use jsonify, so do it manually
# -------------------------------------------------

# Camera
# -------------------------------------------------
@app.route('/video_feed')
def video_feed():
  return Response(gen(VideoCamera()),
                  mimetype='multipart/x-mixed-replace; boundary=frame')

def gen(camera):
  while True:
    frame = camera.get_frame()
    yield (b'--frame\r\n'
           b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n\r\n')
# -------------------------------------------------

if __name__ == "__main__":
  app.run(host='192.168.1.79', threaded=True, debug=True)
  # app.run(host='0.0.0.0', threaded=True)

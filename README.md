# rover-one
RoverOne is small ROV (Remotely Operated Vehicle)

Note::
I did not use TimerOne.h because Servo.h takes over the time1 interrupt, which caused issues with TimerOne.h.  It was a tough choice, but I decided to go doing my own timer function to for the blinking signal light.  It will not be blinking all the time, so the timer interrupt running for it was not needed as badly.

https://blog.miguelgrinberg.com/post/video-streaming-with-flask

https://github.com/miguelgrinberg/flask-video-streaming/tree/v1

https://learn.adafruit.com/adafruit-arduino-lesson-14-servo-motors/if-the-servo-misbehaves

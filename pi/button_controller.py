#!/usr/bin/env python3
from i2c_master import Arduino, registers
import RPi.GPIO as GPIO
import time

SPEED_PIN = 17
ROTATE_LEFT_PIN = 27

def rotate_left(channel):
    while GPIO.input(channel) is False:
        if GPIO.input(SPEED_PIN) is False:
            ard.send('255', register=registers['RIGHT_THRUSTER'])
            ard.send('-255', register=registers['LEFT_THRUSTER'])
        else:
            ard.send('10', register=registers['RIGHT_THRUSTER'])
            ard.send('-10', register=registers['LEFT_THURSTER'])
        time.sleep(0.1)
    ard.send('0', register=registers['RIGHT_THRUSTER']) #IS THIS CLEARING BUFFER?
    ard.send('0', register=registers['LEFT_THRUSTER'])

if __name__ == '__main__':
    ard = Arduino()
    GPIO.setmode(GPIO.BCM)

    GPIO.setup(SPEED_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)
    GPIO.setup(ROTATE_LEFT_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)
    GPIO.add_event_detect(ROTATE_LEFT_PIN, GPIO.FALLING,
                          callback=rotate_left, bouncetime=200)
    while True:
        input_state = GPIO.input(SPEED_PIN)
        if input_state == False:
            print('Button Pressed')
        time.sleep(1)

# Script for 6.S06Scream micropython
# Authors: Mitchell Hwang & Xin Wen
# This script is intended to be in the esp8266's main.py file
# It will run after boot, and after a prompt from an arduino,
# it will record the maximum noise level the mic picks up and 
# send that to the arduino to be placed on the leaderboard.
import machine

# Interface with the arduino
# Most cases only pins 0, 2, 4, 5, 12, 13, 14, 15, and 16 can be used
# Enable pin to record from microphone
rec = machine.Pin(0, Pin.IN)
# Data pins to relay max volume 
data = [machine.Pin(2, Pin.OUT), machine.Pin(4, Pin.OUT),
 machine.Pin(5, Pin.OUT), machine.Pin(12, Pin.OUT),
 machine.Pin(13, Pin.OUT), machine.Pin(14, Pin.OUT),
 machine.Pin(15, Pin.OUT), machine.Pin(16, Pin.OUT)] 


# Interface with the electret microphone
adc = machine.ADC(0)

try:
	while(True):
		db_max = 0
		while(rec.read()):
			if ()
		if ()
		print(adc.read())
		print(rec.read())
except KeyboardInterrupt:
	pass
import time
import wiringpi2 as wiringpi
import RPi.GPIO as gpio

wiringpi.wiringPiSetupGpio()
wiringpi.pinMode(13, 1)
wiringpi.pinMode(16, 0)

wiringpi.pinMode(27, 1)
wiringpi.pinMode(17, 0)

wiringpi.pinMode(21, 1)
wiringpi.pinMode(18, 0)
wiringpi.pinMode(19, 1)
wiringpi.pinMode(26, 1)
wiringpi.pinMode(23, 1)
wiringpi.pinMode(24, 1)
wiringpi.pinMode(25, 1)
wiringpi.pinMode(12, 1)
wiringpi.pinMode(20, 1)


print "Press the button"

try :
	wiringpi.digitalWrite(13, 0)
	wiringpi.digitalWrite(27, 0)
	wiringpi.digitalWrite(21, 0)
	wiringpi.digitalWrite(19, 0)
	wiringpi.digitalWrite(26, 0)
	wiringpi.digitalWrite(23, 0)
	wiringpi.digitalWrite(24, 0)
	wiringpi.digitalWrite(25, 0)
	wiringpi.digitalWrite(12, 0)
	wiringpi.digitalWrite(20, 0)

	while True:
		if (wiringpi.digitalRead(16) == 1):
			if (wiringpi.digitalRead(13) == 0):
				wiringpi.digitalWrite(13, 1)
			else :
				wiringpi.digitalWrite(13, 0)
			print "LED2 Button pressed!"
			time.sleep(0.5)
			print "Press the button (CTRL-C) to exit"

		elif (wiringpi.digitalRead(17) == 1):
			if (wiringpi.digitalRead(27) == 0):
				wiringpi.digitalWrite(27, 1)
			else :
				wiringpi.digitalWrite(27, 0)
			print "LED1 Button pressed!"
			time.sleep(0.5)
			print "Press the button (CTRL-C) to exit"

		elif (wiringpi.digitalRead(18) == 1):
			if (wiringpi.digitalRead(21) == 0):
				wiringpi.digitalWrite(21, 1)
				wiringpi.digitalWrite(19, 1)
				wiringpi.digitalWrite(26, 0)
				wiringpi.digitalWrite(23, 1)
				wiringpi.digitalWrite(24, 1)
				wiringpi.digitalWrite(25, 1)
				wiringpi.digitalWrite(12, 1)
				wiringpi.digitalWrite(20, 1)
			else :
				wiringpi.digitalWrite(21, 0)
				wiringpi.digitalWrite(19, 0)
				wiringpi.digitalWrite(26, 0)
				wiringpi.digitalWrite(23, 0)
				wiringpi.digitalWrite(24, 0)
				wiringpi.digitalWrite(25, 0)
				wiringpi.digitalWrite(12, 0)
				wiringpi.digitalWrite(20, 0)
			print "TV Button pressed!"
			time.sleep(0.5)
			print "Press the button (CTRL-C) to exit"		


except KeyboardInterrupt:
	wiringpi.digitalWrite(13, 0)
	wiringpi.digitalWrite(17, 0)
	wiringpi.digitalWrite(21, 0)
	wiringpi.digitalWrite(19, 0)
	wiringpi.digitalWrite(26, 0)
	wiringpi.digitalWrite(23, 0)
	wiringpi.digitalWrite(24, 0)
	wiringpi.digitalWrite(25, 0)
	wiringpi.digitalWrite(12, 0)
	wiringpi.digitalWrite(20, 0)
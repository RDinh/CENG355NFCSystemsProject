from Adafruit_CharLCD import Adafruit_CharLCD
import Adafruit_GPIO.PCF8574 as PCF
import wiringpi as wiringpi
import time
from time import sleep
import sys
import os
from OrderInfo import getItems


#Initialization of the Wiring Pi Library
wiringpi.wiringPiSetup()
wiringpi.wiringPiSetupGpio()
wiringpi.wiringPiSetupPhys()
wiringpi.pinMode(7, 1)


#Getting the I2C address of the LCD
GPIO = PCF.PCF8574(address=0x38)

#Define PCF pins connected to the LCD
lcd_rs		= 4
lcd_en		= 6
d4,d5,d6,d7	= 0,1,2,3
cols,lines	= 16,2

#instantiate LCD display
lcd = Adafruit_CharLCD(lcd_rs, lcd_en, d4, d5, d6, d7,
	cols, lines, gpio=GPIO)
lcd.clear()

test = ' '.join(sys.argv[1:])

#os.system('python OrderInfo.py '+sys.argv[1])

#if len(test)>16:
#	test = test[0:15] + "\n" + test[15:]

itemArray = getItems(str(sys.argv[1]))

wiringpi.digitalWrite(7,1)
i = 0
while i < len(itemArray):
    msg = ""
    if (i + 1 < len(itemArray)):
        msg = itemArray[i]+"\n"+ itemArray[i+1]
    else:
        msg = itemArray[i]
    lcd.message(str(msg))
    sleep(2)   
    lcd.clear()
    i+=2

#Display the Contents to the LCD. Turn on Backlight and turn off.
print str(test)
print "LCD ON"

#lcd.message(str(test))
sleep(5)
wiringpi.digitalWrite(7,0)
print "LCD OFF"


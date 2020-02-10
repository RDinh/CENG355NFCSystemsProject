from Adafruit_CharLCD import Adafruit_CharLCD
import Adafruit_GPIO.PCF8574 as PCF
import time
import sys

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

if len(test)>16:
	test = test[0:15] + "\n" + test[15:]

print str(test)
print "LCD ON"
lcd.message(str(test))

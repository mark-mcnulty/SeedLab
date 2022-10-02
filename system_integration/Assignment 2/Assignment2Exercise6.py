import smbus
import time
import board
import adafruit_character_lcd.character_lcd_rgb_i2c as character_lcd
# for RPI version 1, use “bus = smbus.SMBus(0)”
bus = smbus.SMBus(1)

# This is the address we setup in the Arduino Program
address = 0x04

lcd_columns = 16
lcd_rows = 2

i2c = board.I2C()

lcd = character_lcd.Character_LCD_RGB_I2C(i2c, lcd_columns, lcd_rows)
lcd.clear()

lcd.color = [50,0,50]

def writeNumber(value,offset):
    #bus.write_byte(address, value)
    bus.write_byte_data(address, 0, value)
    #lcd.message = "Sent: " + (str(value))
    time.sleep(5)
    return -1

def readNumber():
    number = bus.read_byte(address)
    #number = bus.read_byte_data(address, offset)
    #lcd.message = "\nGot: " + (str(number))
    #time.sleep(5)
    return number

while True:
    time.sleep(2)
    lcd.clear()
    number = 5*readNumber()/256

    # sleep one second
    time.sleep(1)

    number = str(number)
    print ("Arduino: Hey RPI, the voltage is: ", number)
    lcd.message = (number + "V")


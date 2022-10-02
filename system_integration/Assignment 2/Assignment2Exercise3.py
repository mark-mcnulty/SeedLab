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

lcd.color = [100,0,0]

def writeNumber(value,offset):
    #bus.write_byte(address, value)
    bus.write_byte_data(address, offset, value)
    lcd.message = "Sent: " + (str(value))
    time.sleep(5)
    return -1

def readNumber():
    #number = bus.read_byte(address)
    number = bus.read_byte_data(address, offset)
    lcd.message = "\nGot: " + (str(number))
    time.sleep(5)
    return number

while True:
    lcd.clear()
    var = int(input("Enter 1 – 9: "))
    offset = int(input("Enter the offset: "))
    if not var:
        continue

    writeNumber(var,offset)
    print ("RPI: Hi Arduino, I sent you ", var, " and offset ", offset)
    # sleep one second
    time.sleep(1)

    number = readNumber()
    print ("Arduino: Hey RPI, I received a digit and changed it using the offset given: ", number)
    print


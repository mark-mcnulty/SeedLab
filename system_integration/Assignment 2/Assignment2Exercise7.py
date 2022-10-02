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

time.sleep(1)

bus = smbus.SMBus(1)

def writeNumber(value):
    #bus.write_byte(address, value)
    #bus.write_byte_data(address, offset, value)
    try :
        bus.write_i2c_block_data(address, 0, value)
    except IOError :
        print("I2C Error")
        lcd.message = "I2C Error"
    
    return -1

def readNumber():
    #number = bus.read_byte(address)
    #number = bus.read_byte_data(address, offset)
    number = [0]
    try :
        number = bus.read_i2c_block_data(address, 0, value)
    except IOError :
        print("I2C Error")
        lcd.message = "I2C Error"
    return number

while True:
    string = str(input("Please enter a string: "))
    numList = []
    for char in string :
        numList.extend(ord(num) for num in char)
    if not string :
        continue
    dataLen = len(numList)
    writeNumber(numList)
    print("RPI: Hi Arduino, I sent you ", str(numList))
    
    # sleep one second
    time.sleep(1)

    number = readNumber()
    
    invStr = ""
    for i in range(len(number)) :
        invStr += chr(number[i])
    print ("Arduino: Hey RPI, I received a string and inverted it: ", invStr)
    print




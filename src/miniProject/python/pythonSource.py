import time
import camera
import i2c
import board
import digitalio
import adafruit_character_lcd.character_lcd_rgb_i2c as character_lcd

# for RPI version 1, use “bus = smbus.SMBus(0)”
bus = smbus.SMBus(1)

# This is the address we setup in the Arduino Program
# this is the slave address
address = 0x04

def writeNumber(value,offset):
    #bus.write_byte(address, value)
    bus.write_byte_data(address, offset, value)
    return -1

def readNumber():
    #number = bus.read_byte(address)
    number = bus.read_byte_data(address, offset)
    return number


if __name__ == "__main__":
    # make the camera object
    cam = camera.arducam()

    # setup the camera
    cam.setup()

    # setup the bus connection 
    device = i2c.Device(address, 1)

    # setup the lcd
    lcd_rs = digitalio.DigitalInOut(board.D7)
    lcd_en = digitalio.DigitalInOut(board.D8)
    lcd_d7 = digitalio.DigitalInOut(board.D12)
    lcd_d6 = digitalio.DigitalInOut(board.D11)
    lcd_d5 = digitalio.DigitalInOut(board.D10)
    lcd_d4 = digitalio.DigitalInOut(board.D9)
    lcd_backlight = digitalio.DigitalInOut(board.D13)
    lcd_columns = 16
    lcd_rows = 2 
    lcd = character_lcd.Character_LCD_Mono(lcd_rs, lcd_en, lcd_d4, lcd_d5, lcd_d6, lcd_d7, lcd_columns, lcd_rows, lcd_backlight)
    lcd.message = "hello\nWorld"

    # continuously capture images
    while True:
        try:
            # capture the image
            cam.capture()

            # display the image
            cam.display()

            # detect the quadrant
            quadrant = cam.detect_quadrant()

            # if the quadrant is not none
            if quadrant != None:
                # send the quadrant to the arduino
                device.write8(0, quadrant)

        except KeyboardInterrupt:
            print("Exiting")
            break
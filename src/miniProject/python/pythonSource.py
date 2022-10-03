import time
import camera
import i2c
import board
import digitalio
import adafruit_character_lcd.character_lcd_rgb_i2c as character_lcd

# for RPI version 1, use “bus = smbus.SMBus(0)”
# bus = smbus.SMBus(1)
# This is the address we setup in the Arduino Program
# this is the slave address
address = 0x04

# def writeNumber(value,offset):
#     #bus.write_byte(address, value)
#     bus.write_byte_data(address, offset, value)
#     return -1

# def readNumber():
#     #number = bus.read_byte(address)
#     number = bus.read_byte_data(address, offset)
#     return number


if __name__ == "__main__":
    lcd_columns = 16
    lcd_rows = 2

    i2c = board.I2C()

    lcd = character_lcd.Character_LCD_RGB_I2C(i2c, lcd_columns, lcd_rows)
    lcd.clear()
    lcd.color = [100,0,0]

    # make the camera object
    cam = camera.arducam()

    # setup the camera
    cam.setup()

    # setup the bus connection 
    device = i2c.Device(address, 1)

    # setup the lcd
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
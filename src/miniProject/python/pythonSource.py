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
# adding comments la la la
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
    # define variables
    lcd_update = False 
    old_Quadrant = None
    quadrant = None

    lcd_columns = 16
    lcd_rows = 2

    i2c_local = board.I2C()

    lcd = character_lcd.Character_LCD_RGB_I2C(i2c_local, lcd_columns, lcd_rows)
    lcd.clear()
    lcd.color = [100,0,0]

    # make the camera object
    cam = camera.arducam()

    # setup the camera
    cam.setup()

    # setup the bus connection 
    device = i2c.Device(address, 1)

    # setup the lcd
    lcd.message = str(1)

    # send the inital 1 to the arduino
    device.write8(0, 1)

    # continuously capture images
    while True:
        try:
            # capture the image
            cam.capture()

            # detect the quadrant
            quadrant = cam.detect_quadrant()

            # if the quadrant is not none
            if quadrant != None and quadrant != old_Quadrant:
                # display the image
                cam.display()
                # send the quadrant to the arduino
                lcd.message = str(quadrant)
                device.write8(0, quadrant)
                old_Quadrant = quadrant

        except KeyboardInterrupt:
            print("Exiting")
            break
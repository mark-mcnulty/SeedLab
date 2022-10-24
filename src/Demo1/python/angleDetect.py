from camera import arducam
import time

# for the lcd screen
import board
import busio
import adafruit_character_lcd.character_lcd_rgb_i2c as character_lcd

# constants
address = 0x04
lcd_columns = 16
lcd_rows = 2

if __name__ == '__main__':
    # initialize variables
    i2c = busio.I2C(board.SCL, board.SDA)
    lcd = character_lcd.Character_LCD_RGB_I2C(i2c, lcd_columns, lcd_rows)
    oldAngle = 0

    # Initialize the camera
    cam = arducam.Arducam()

    try: 
        while True:
            # capture image
            cam.capture()

            # detect angle
            angle = cam.get_marker_location()

            # if there is actually an angle
            if angle is not None:
                print(angle)
                # if the angle is different than the last angle
                if angle != oldAngle:
                    # Set LCD color to blue
                    lcd.color = [0, 100, 0]
                    lcd.clear()
                    lcd.message = "Angle:\n" + str(angle)
                    time.sleep(1)
                # update the old angle
                oldAngle = angle
    except KeyboardInterrupt:
        print("KeyboardInterrupt")
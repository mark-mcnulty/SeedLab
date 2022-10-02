import smbus
import time
import camera

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
                writeNumber(quadrant, 0)

        except KeyboardInterrupt:
            print("Exiting")
            break
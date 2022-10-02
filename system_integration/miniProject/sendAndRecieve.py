import smbus
import time
# for RPI version 1, use “bus = smbus.SMBus(0)”
bus = smbus.SMBus(1)

# This is the address we setup in the Arduino Program
# this is the slave address
address = 0x04

def writeNumber(value, offset):
    #bus.write_byte(address, value)
    bus.write_byte_data(address, value, offset)
    return -1

def readNumber():
    #number = bus.read_byte(address)
    number = bus.read_byte_data(address, 32)
    return number

if __name__ == "__main__":
    while True:
        var = int(input("Enter 1 – 9: "))
        if not var:
            continue

        writeNumber(var, 0)
        print ("RPI: Hi Arduino, I sent you ", var, " and offset ")
        # sleep one second
        time.sleep(1)

        number = readNumber()
        print ("Arduino: Hey RPI, I received a digit ", number)
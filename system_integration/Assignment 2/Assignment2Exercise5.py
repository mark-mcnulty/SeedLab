import smbus
import time
# for RPI version 1, use “bus = smbus.SMBus(0)”
bus = smbus.SMBus(1)

# This is the address we setup in the Arduino Program
address = 0x04

def writeNumber(value):
    #bus.write_byte(address, value)
    #bus.write_byte_data(address, offset, value)
    bus.write_i2c_block_data(address,0,value)
    return -1

def readNumber():
    #number = bus.read_byte(address)
    #number = bus.read_byte_data(address, offset)
    number = bus.read_i2c_block_data(address,0,dataLen)
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


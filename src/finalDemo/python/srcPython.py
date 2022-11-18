import camera
import smbus
import time

def send(bus, angle, distance):
    try:
        distance = round(distance, 2)
        angle = round(angle, 2)
        temp = str(distance) + " " + str(angle)
        byteValue = StringToBytes(temp)
        bus.write_i2c_block_data(0x04, 0, byteValue)
        print(str(angle) + " " + str(distance))
        time.sleep(0.5)
    except:
        print("Error")

def StringToBytes(val):
    retVal = []
    for c in val:
        retVal.append(ord(c))
    return retVal


if __name__ == "__main__":
    print("main")